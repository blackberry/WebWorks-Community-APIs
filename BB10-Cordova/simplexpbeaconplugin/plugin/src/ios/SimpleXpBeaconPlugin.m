/*
 * Copyright (c) 2015 BlackBerry Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#import "SimpleXpBeaconPlugin.h"

NSString * const PLUGIN_VERSION = @"1.1.0";

NSString* const ACTION_INITIALISE_BLUETOOTH = @"initialiseBluetooth";
NSString* const ACTION_TERMINATE_BLUETOOTH = @"terminateBluetooth";
NSString* const ACTION_PLUGIN_VERSION = @"pluginVersion";
NSString* const ACTION_START_MONITORING = @"startMonitoring";
NSString* const ACTION_STOP_MONITORING = @"stopMonitoring";

NSString* const JSON_KEY_STATUS = @"status";
NSString* const JSON_KEY_DESCRIPTION = @"desc";
NSString* const JSON_KEY_ERROR_CODE = @"error_code";
NSString* const JSON_KEY_EVENT = @"event";
NSString* const JSON_KEY_PLUGIN_VERSION = @"plugin_version";
NSString* const JSON_KEY_DATA = @"data";
NSString* const JSON_KEY_IBEACON_UUID = @"uuid";
NSString* const JSON_KEY_IBEACON_MAJOR = @"major";
NSString* const JSON_KEY_IBEACON_MINOR = @"minor";
NSString* const JSON_KEY_IBEACON_RSSI = @"rssi";
NSString* const JSON_KEY_IBEACON_TXPOWER = @"txpower";
NSString* const JSON_KEY_IBEACON_PROXIMITY = @"proximity";
NSString* const JSON_KEY_IBEACON_ACCURACY = @"accuracy";

NSString* const JSON_VALUE_ERROR = @"ERROR";
NSString* const JSON_VALUE_OK = @"OK";
NSString* const JSON_VALUE_STARTED = @"STARTED";
NSString* const JSON_VALUE_IBEACON = @"IBEACON";
NSString* const JSON_VALUE_IBEACON_TYPE_PROXIMITY = @"IBEACON_PROXIMITY";
NSString* const JSON_VALUE_PLUGIN_DESCRIPTION = @"Plugin Version";
NSString* const JSON_VALUE_NO_BT_LE_FEATURE = @"This device doesn't have BT LE feature";
NSString* const JSON_VALUE_BT_ALREADY_INITIALISED = @"Bluetooth already initialised";
NSString* const JSON_VALUE_BT_NOT_INITIALISED = @"Bluetooth not initialised";
NSString* const JSON_VALUE_BT_INITIALISED = @"Bluetooth initialised";
NSString* const JSON_VALUE_BT_TERMINATE = @"Bluetooth Terminated";
NSInteger const JSON_VALUE_DEFAULT_ERROR_CODE = -1;
NSString* const JSON_VALUE_ALREADY_MONITORING_FOR_I_BEACONS = @"Already monitoring for iBeacons";
NSString* const JSON_VALUE_FAILED_TO_ENABLE_MONITORING = @"Failed to enable iBeacon monitoring";
NSString* const JSON_VALUE_IBEACON_MONITORING_NOT_AVAILABLE = @"iBeacon monitoring not available for this application";
NSString* const JSON_VALUE_NOT_MONITORING = @"Not monitoring for iBeacons";
NSString* const JSON_VALUE_FAILED_TO_DISABLE_MONITORING = @"Failed to Disable Monitoring";
NSString* const JSON_VALUE_STOPPED_MONITORING = @"Stopped Monitoring for iBeacons OK";
NSString* const JSON_VALUE_REQUESTED_MONITORING = @"Requested iBeacon Monitoring OK";
NSString* const JSON_VALUE_IBEACON_EVENT = @"iBeacon event";
NSString* const JSON_VALUE_IBEACON_PROXIMITY_EVENT = @"iBeacon proximity event";
NSString* const JSON_VALUE_BEACON_REMOVED = @"iBeacon UUID removed sucessfully";
NSString* const JSON_VALUE_NO_MATCH_TO_BEACON_UUID = @"iBeacon UUID did not match any being monitored";
NSString* const JSON_VALUE_IBEACON_ALREADY_BEING_MONITORED = @"iBeacon already being monitored";
NSString* const JSON_VALUE_UUID_ADDED = @"UUID added";
NSString* const JSON_VALUE_UUID_WAS_IMPROPER_FORMAT = @"UUID was improper format";

@interface SimpleXpBeaconPlugin () <CBCentralManagerDelegate, CBPeripheralDelegate, CLLocationManagerDelegate>

@property (nonatomic) NSString* monitoringCallbackId;
@property (nonatomic) NSString* initialiseCallbackId;
@property (nonatomic) BOOL btInitialised;
@property (nonatomic) BOOL monitoring;
@property (nonatomic) BOOL supressMonitorCallback;
@property (nonatomic) BOOL initialised;
@property (nonatomic) CBCentralManager *manager;
@property (nonatomic) CBPeripheral *peripheral;
@property (nonatomic) CLBeaconRegion *beaconRegion;
@property (nonatomic) CLLocationManager *locationManager;
@property (nonatomic) NSMutableDictionary *beaconRegionsToMonitor;

@end

@implementation SimpleXpBeaconPlugin

#pragma mark - initWithWebView

-(CDVPlugin *)initWithWebView:(UIWebView *)theWebView
{
    self = [super initWithWebView:theWebView];
    if (self) {
        [self __init];
    }
    return self;
}

#pragma mark - __init

- (void)__init
{
    if ((self != nil) && !self.initialised) {
        self.initialised = YES;
        self.monitoringCallbackId = nil;
        self.initialiseCallbackId = nil;
        self.btInitialised = NO;
        self.monitoring = NO;
        self.supressMonitorCallback = NO;
        self.manager = nil;
        self.locationManager = nil;
        self.beaconRegionsToMonitor = [[NSMutableDictionary alloc] init];
    }
}

#pragma mark - pluginInitialize

- (void)pluginInitialize
{
    [super pluginInitialize];
    
    NSLog(@"SimpleXpBeaconPlugin - Plugin initialisation");

    self.locationManager = [[CLLocationManager alloc] init];
    self.locationManager.delegate = self;
    
    NSLog(@"SimpleXpBeaconPlugin - auth status %d", CLLocationManager.authorizationStatus);
    if ([self.locationManager respondsToSelector:@selector(requestAlwaysAuthorization)]) {
        NSLog(@"SimpleXpBeaconPlugin - respondsToSelector");
        [self.locationManager requestAlwaysAuthorization];
    } else {
        NSLog(@"SimpleXpBeaconPlugin - does not respondsToSelector");
    }
}

#pragma mark - deviceHasBtLeFeature

- (BOOL) deviceHasBtLeFeature
{
    NSString *state = nil;
    BOOL response = FALSE;

    switch ([self.manager state])
    {
        case CBCentralManagerStateUnsupported:
            state = @"The platform/hardware doesn't support Bluetooth Low Energy.";
            break;
        case CBCentralManagerStateUnauthorized:
            state = @"The app is not authorized to use Bluetooth Low Energy.";
            break;
        case CBCentralManagerStatePoweredOff:
            state = @"Bluetooth is currently powered off.";
            response = TRUE;
            break;
        case CBCentralManagerStatePoweredOn:
            state = @"Bluetooth is currently powered on.";
            response = TRUE;
            break;
        case CBCentralManagerStateUnknown:
            state = @"Bluetooth state unknown.";
        default:
            state = @"State invalid.";
            response = FALSE;
    }
    
    NSLog(@"SimpleXpBeaconPlugin - Central manager state: %@", state);

    return response;
}

#pragma mark - bluetoothSwitchedOn

- (BOOL) bluetoothSwitchedOn
{
    switch ([self.manager state])
    {
        case CBCentralManagerStatePoweredOff:
            break;
        case CBCentralManagerStatePoweredOn:
            return TRUE;
        case CBCentralManagerStateUnknown:
        default:
            return FALSE;
    }
    return FALSE;
}

#pragma mark - pluginVersion

- (void)pluginVersion:(CDVInvokedUrlCommand *) command
{
    NSLog(@"SimpleXpBeaconPlugin - pluginVersion");

    NSString* callbackId = [command callbackId];
    
    [self pluginVersionResponse:callbackId];
}

#pragma mark - addBeaconUuidToMonitor

- (void)addBeaconUuidToMonitor:(CDVInvokedUrlCommand *) command
{
    NSLog(@"SimpleXpBeaconPlugin - addBeaconRegionToMonitor");

    NSString *callbackId = [command callbackId];
    NSString *uuidString = [[command arguments] objectAtIndex:0];

    NSLog(@"SimpleXpBeaconPlugin - addBeaconRegionToMonitor - adding %@", uuidString);

    NSUUID *uuid = [[NSUUID alloc] initWithUUIDString:uuidString];
    
    if (uuid == nil) {
        [self errorResponse:callbackId description:JSON_VALUE_UUID_WAS_IMPROPER_FORMAT];
        return;
    }

    NSString *identifier = [NSString stringWithFormat:@"Beacon Region UUID: %@", uuid.UUIDString];
    
    CLBeaconRegion *beaconRegion = [self.beaconRegionsToMonitor objectForKey:uuid.UUIDString];
    
    if (!beaconRegion) {
        beaconRegion = [[CLBeaconRegion alloc] initWithProximityUUID:uuid identifier:identifier];
        beaconRegion.notifyEntryStateOnDisplay = YES;
        beaconRegion.notifyOnEntry = YES;
        beaconRegion.notifyOnExit = YES;
        
        [self.beaconRegionsToMonitor setObject:beaconRegion forKey:uuid.UUIDString];
        
        if (self.monitoring) {
            [self.locationManager startMonitoringForRegion:beaconRegion];
            [self.locationManager startRangingBeaconsInRegion:beaconRegion];
        }
        [self successResponse:callbackId description:JSON_VALUE_UUID_ADDED];
    } else {
        [self errorResponse:callbackId description:JSON_VALUE_IBEACON_ALREADY_BEING_MONITORED];
    }
}

#pragma mark - removeBeaconUuidToMonitor

- (void)removeBeaconUuidToMonitor:(CDVInvokedUrlCommand *) command
{
    NSLog(@"SimpleXpBeaconPlugin - removeBeaconRegionToMonitor");

    NSString *callbackId = [command callbackId];
    NSString *uuidString = [[command arguments] objectAtIndex:0];

    NSLog(@"SimpleXpBeaconPlugin - addBeaconRegionToMonitor - removing %@", uuidString);
    
    NSUUID *uuid = [[NSUUID alloc] initWithUUIDString:uuidString];

    if (uuid == nil) {
        [self errorResponse:callbackId description:JSON_VALUE_UUID_WAS_IMPROPER_FORMAT];
        return;
    }

    CLBeaconRegion *beaconRegion = [self.beaconRegionsToMonitor objectForKey:uuid.UUIDString];

    if (beaconRegion) {
        if (self.monitoring) {
            [self.locationManager stopMonitoringForRegion:beaconRegion];
            [self.locationManager stopRangingBeaconsInRegion:beaconRegion];
        }
        [self.beaconRegionsToMonitor removeObjectForKey:uuid.UUIDString];
        [self successResponse:callbackId description:JSON_VALUE_BEACON_REMOVED];
    } else {
        [self errorResponse:callbackId description:JSON_VALUE_NO_MATCH_TO_BEACON_UUID];
    }
}

#pragma mark - initialiseBluetooth

- (void) initialiseBluetooth:(CDVInvokedUrlCommand *) command
{
    NSLog(@"SimpleXpBeaconPlugin - initialiseBluetooth");

    NSString* callbackId = [command callbackId];

    if (self.btInitialised) {
        [self errorResponse:callbackId description:JSON_VALUE_BT_ALREADY_INITIALISED];
        return;
    }

    self.initialiseCallbackId = callbackId;
    
    self.manager = [[CBCentralManager alloc]
                    initWithDelegate:self
                    queue:dispatch_get_main_queue()
                    options:@{CBCentralManagerOptionShowPowerAlertKey: @(NO)}];
    
    [self keepCallbackResponse:self.initialiseCallbackId];
}

#pragma mark - centralManagerDidUpdateState:

- (void)centralManagerDidUpdateState:(CBCentralManager *)central
{
    NSLog(@"SimpleXpBeaconPlugin - centralManagerDidUpdateState");

    if (!self.btInitialised && self.initialiseCallbackId != nil) {
        if (![self deviceHasBtLeFeature]) {
            [self errorResponse:self.initialiseCallbackId description:JSON_VALUE_NO_BT_LE_FEATURE];
            return;
        }
        
        if (![self bluetoothSwitchedOn]) {
            [self errorResponse:self.initialiseCallbackId description:JSON_VALUE_BT_NOT_INITIALISED];
            return;
        }
        
        self.btInitialised = YES;
        [self successResponse:self.initialiseCallbackId description:JSON_VALUE_BT_INITIALISED];
    }
}

#pragma mark - locationManager:didRangeBeacons:inRegion:

- (void) locationManager:(CLLocationManager *)manager
         didRangeBeacons:(NSArray *)beacons
                inRegion:(CLBeaconRegion *)region
{
    NSLog(@"SimpleXpBeaconPlugin - clocationManager:didRangeBeacons:inRegion:");

    for (CLBeacon *beacon in beacons) {
        [self monitorResponse:self.monitoringCallbackId beacon:beacon inRegion:region];
    }
}

#pragma mark - locationManager:rangingBeaconsDidFailForRegion:inRegion:withError:

- (void) locationManager:(CLLocationManager *)manager
rangingBeaconsDidFailForRegion:(CLBeaconRegion *)region
               withError:(NSError *)error
{
    NSLog(@"SimpleXpBeaconPlugin - locationManager:rangingBeaconsDidFailForRegion:inRegion:withError:");
}

#pragma mark - locationManager:didEnterRegion:

- (void)locationManager:(CLLocationManager *)manager didEnterRegion:(CLRegion *)region {
    NSLog(@"SimpleXpBeaconPlugin - locationManager:didEnterRegion:");
}

#pragma mark - locationManager:didExitRegion:

-(void)locationManager:(CLLocationManager *)manager didExitRegion:(CLRegion *)region {
    NSLog(@"SimpleXpBeaconPlugin - locationManager:didExitRegion:");
}

#pragma mark - terminateBluetooth

- (void) terminateBluetooth:(CDVInvokedUrlCommand *) command
{
    NSLog(@"SimpleXpBeaconPlugin - terminateBluetooth");

    NSString* callbackId = [command callbackId];

    if (!self.btInitialised) {
        [self errorResponse:callbackId description:JSON_VALUE_BT_NOT_INITIALISED];
        return;
    }

    [self successResponse:callbackId description:JSON_VALUE_BT_TERMINATE];
    self.btInitialised = NO;
    self.initialiseCallbackId = nil;
}

#pragma mark - startMonitoring

- (void) startMonitoring:(CDVInvokedUrlCommand *) command
{
    NSLog(@"SimpleXpBeaconPlugin - startMonitoring");

    NSString* callbackId = [command callbackId];

    if (!self.btInitialised) {
        [self errorResponse:callbackId description:JSON_VALUE_BT_NOT_INITIALISED];
        return;
    }

    if (![CLLocationManager isMonitoringAvailableForClass:[CLBeaconRegion class]]) {
        [self errorResponse:callbackId description:JSON_VALUE_IBEACON_MONITORING_NOT_AVAILABLE];
        return;
    }

    if (self.monitoring) {
        [self errorResponse:callbackId description:JSON_VALUE_ALREADY_MONITORING_FOR_I_BEACONS];
        return;
    }

    self.monitoringCallbackId = callbackId;
    self.monitoring = YES;
    self.supressMonitorCallback = NO;
    
    for (id key in self.beaconRegionsToMonitor) {
        CLBeaconRegion *beaconRegion = [self.beaconRegionsToMonitor objectForKey:key];
        [self.locationManager startMonitoringForRegion:beaconRegion];
        [self.locationManager startRangingBeaconsInRegion:beaconRegion];
    }

    [self monitorSuccessResponse:callbackId];
}

#pragma mark - stopMonitoring

- (void) stopMonitoring:(CDVInvokedUrlCommand *) command
{
    NSLog(@"SimpleXpBeaconPlugin - stopMonitoring");

    NSString* callbackId = [command callbackId];
    
    if (!self.btInitialised) {
        [self errorResponse:callbackId description:JSON_VALUE_BT_NOT_INITIALISED];
        return;
    }

    if (!self.monitoring) {
        [self errorResponse:callbackId description:JSON_VALUE_NOT_MONITORING];
        return;
    }
    
    for (id key in self.beaconRegionsToMonitor) {
        CLBeaconRegion *beaconRegion = [self.beaconRegionsToMonitor objectForKey:key];
        [self.locationManager stopMonitoringForRegion:beaconRegion];
        [self.locationManager stopRangingBeaconsInRegion:beaconRegion];
    }
    
    self.monitoringCallbackId = nil;
    self.monitoring = NO;

    [self successResponse:callbackId description:JSON_VALUE_STOPPED_MONITORING];
}

#pragma mark - monitorResponse

- (void) monitorResponse:(NSString *)callbackId beacon:(CLBeacon *)beacon inRegion:(CLBeaconRegion *)region
{
    CDVPluginResult* result = nil;
    NSString* responseJsonString = nil;
    NSData* responseData = nil;
    NSError* error = nil;
    NSMutableDictionary* responseObject = [[NSMutableDictionary alloc] init];
    NSMutableDictionary* dataObject = [[NSMutableDictionary alloc] init];
    
    NSLog(@"Region %@", region.identifier);
    NSLog(@"UUID %@", beacon.proximityUUID.UUIDString);
    NSLog(@"Major %@", beacon.major);
    NSLog(@"Minor %@", beacon.minor);
    NSLog(@"RSSI %ld", (long)beacon.rssi);
    NSLog(@"Proximity %ld", (long)beacon.proximity);
    NSLog(@"Accuracy %f", beacon.accuracy);

    [dataObject setObject:beacon.proximityUUID.UUIDString
                   forKey:JSON_KEY_IBEACON_UUID];
    [dataObject setObject:beacon.major
                   forKey:JSON_KEY_IBEACON_MAJOR];
    [dataObject setObject:beacon.minor
                   forKey:JSON_KEY_IBEACON_MINOR];
    [dataObject setObject:[NSNumber numberWithInteger:beacon.rssi]
                   forKey:JSON_KEY_IBEACON_RSSI];
    [dataObject setObject:[NSNumber numberWithInteger:beacon.proximity]
                   forKey:JSON_KEY_IBEACON_PROXIMITY];
    [dataObject setObject:[NSNumber numberWithDouble:beacon.accuracy]
                   forKey:JSON_KEY_IBEACON_ACCURACY];
    
    [responseObject setObject:dataObject
                       forKey:JSON_KEY_DATA];
    [responseObject setObject:JSON_VALUE_OK
                       forKey:JSON_KEY_STATUS];
    [responseObject setObject:JSON_VALUE_IBEACON_TYPE_PROXIMITY
                       forKey:JSON_KEY_EVENT];
    [responseObject setObject:JSON_VALUE_IBEACON_PROXIMITY_EVENT
                       forKey:JSON_KEY_DESCRIPTION];
    
    responseData = [NSJSONSerialization dataWithJSONObject:responseObject
                                                   options:0
                                                     error:&error];
    if (!error) {
        responseJsonString = [[NSString alloc] initWithData:responseData
                                                   encoding:NSUTF8StringEncoding];
        result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsString:responseJsonString];
        [result setKeepCallbackAsBool:YES];
        [self.commandDelegate sendPluginResult:result callbackId:self.monitoringCallbackId];
    } else {
        NSLog(@"SimpleXpBeaconPlugin JSONString error: %@", [error localizedDescription]);
    }
}

#pragma mark - errorResponse

- (void) errorResponse:(NSString *)callbackId description:(NSString *)desc
{
    // {"desc":"...","status":"ERROR", "error_code": nnn}
    
    CDVPluginResult* result = nil;
    NSString* responseJsonString = nil;
    NSData* responseData = nil;
    NSError* error = nil;
    NSMutableDictionary* responseObject = [[NSMutableDictionary alloc] init];
    
    [responseObject setObject:desc
                       forKey:JSON_KEY_DESCRIPTION];
    [responseObject setObject:JSON_VALUE_ERROR
                       forKey:JSON_KEY_STATUS];
    [responseObject setObject:[NSNumber numberWithInteger:JSON_VALUE_DEFAULT_ERROR_CODE]
                       forKey:JSON_KEY_ERROR_CODE];
    responseData = [NSJSONSerialization dataWithJSONObject:responseObject
                                                   options:0
                                                     error:&error];
    if (!error) {
        responseJsonString = [[NSString alloc] initWithData:responseData
                                                   encoding:NSUTF8StringEncoding];
        result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsString:responseJsonString];
        [result setKeepCallbackAsBool:NO];
        [self.commandDelegate sendPluginResult:result callbackId:callbackId];
    } else {
        NSLog(@"SimpleXpBeaconPlugin JSONString error: %@", [error localizedDescription]);
    }
}

#pragma mark - successResponse

- (void) successResponse:(NSString *)callbackId description:(NSString *)desc
{
    // {"desc":"...","status":"OK"}
    
    CDVPluginResult* result = nil;
    NSString* responseJsonString = nil;
    NSData* responseData = nil;
    NSError* error = nil;
    NSMutableDictionary* responseObject = [[NSMutableDictionary alloc] init];
    
    [responseObject setObject:desc
                       forKey:JSON_KEY_DESCRIPTION];
    [responseObject setObject:JSON_VALUE_OK
                       forKey:JSON_KEY_STATUS];
    responseData = [NSJSONSerialization dataWithJSONObject:responseObject
                                                   options:0
                                                     error:&error];
    if (!error) {
        responseJsonString = [[NSString alloc] initWithData:responseData
                                                   encoding:NSUTF8StringEncoding];
        result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsString:responseJsonString];
        [result setKeepCallbackAsBool: NO];
        [self.commandDelegate sendPluginResult:result callbackId:callbackId];
    } else {
        NSLog(@"SimpleXpBeaconPlugin JSONString error: %@", [error localizedDescription]);
    }
}

#pragma mark - pluginVersionResponse

- (void) pluginVersionResponse:(NSString *)callbackId
{
    // {"desc":"Plugin Version","plugin_version":"1.0.0","status":"OK"}
    
    CDVPluginResult* result = nil;
    NSString* responseJsonString = nil;
    NSData* responseData = nil;
    NSError* error = nil;
    NSMutableDictionary* responseObject = [[NSMutableDictionary alloc] init];
    
    [responseObject setObject:PLUGIN_VERSION                forKey:JSON_KEY_PLUGIN_VERSION];
    [responseObject setObject:JSON_VALUE_PLUGIN_DESCRIPTION forKey:JSON_KEY_DESCRIPTION];
    [responseObject setObject:JSON_VALUE_OK                 forKey:JSON_KEY_STATUS];
    responseData = [NSJSONSerialization dataWithJSONObject:responseObject
                                                   options:0
                                                     error:&error];
    if (!error) {
        responseJsonString = [[NSString alloc] initWithData:responseData
                                                   encoding:NSUTF8StringEncoding];
        result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsString:responseJsonString];
        [result setKeepCallbackAsBool:NO];
        [self.commandDelegate sendPluginResult:result callbackId:callbackId];
    } else {
        NSLog(@"SimpleXpBeaconPlugin JSONString error: %@", [error localizedDescription]);
    }
}

#pragma mark - keepCallbackResponse

- (void) keepCallbackResponse:(NSString *)callbackId
{
    CDVPluginResult* result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsString:@""];
    [result setKeepCallbackAsBool:YES];
    [self.commandDelegate sendPluginResult:result callbackId:callbackId];
}

#pragma mark - monitorSuccessResponse

- (void) monitorSuccessResponse:(NSString *)callbackId
{
    // {"desc":"...","status":"OK"}

    CDVPluginResult* result = nil;
    NSString* responseJsonString = nil;
    NSData* responseData = nil;
    NSError* error = nil;
    NSMutableDictionary* responseObject = [[NSMutableDictionary alloc] init];
    
    [responseObject setObject:JSON_VALUE_REQUESTED_MONITORING
                       forKey:JSON_KEY_DESCRIPTION];
    [responseObject setObject:JSON_VALUE_OK
                       forKey:JSON_KEY_STATUS];
    [responseObject setObject:JSON_VALUE_STARTED
                       forKey:JSON_KEY_EVENT];
    responseData = [NSJSONSerialization dataWithJSONObject:responseObject
                                                   options:0
                                                     error:&error];
    if (!error) {
        responseJsonString = [[NSString alloc] initWithData:responseData
                                                   encoding:NSUTF8StringEncoding];
        result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsString:responseJsonString];
        [result setKeepCallbackAsBool:YES];
        [self.commandDelegate sendPluginResult:result callbackId:callbackId];
    } else {
        NSLog(@"SimpleXpBeaconPlugin JSONString error: %@", [error localizedDescription]);
    }
}

#pragma mark - monitorFailResponse

- (void) monitorFailResponse:(NSString *)callbackId
{
    // {"desc":"...","status":"ERROR", "error_code": nnn, "event":"STARTED"}

    CDVPluginResult* result = nil;
    NSString* responseJsonString = nil;
    NSData* responseData = nil;
    NSError* error = nil;
    NSMutableDictionary* responseObject = [[NSMutableDictionary alloc] init];
    
    [responseObject setObject:JSON_VALUE_FAILED_TO_ENABLE_MONITORING
                       forKey:JSON_KEY_DESCRIPTION];
    [responseObject setObject:JSON_VALUE_ERROR
                       forKey:JSON_KEY_STATUS];
    [responseObject setObject:[NSNumber numberWithInteger:JSON_VALUE_DEFAULT_ERROR_CODE]
                       forKey:JSON_KEY_ERROR_CODE];
    [responseObject setObject:JSON_VALUE_STARTED
                       forKey:JSON_KEY_EVENT];
    responseData = [NSJSONSerialization dataWithJSONObject:responseObject
                                                   options:0
                                                     error:&error];
    if (!error) {
        responseJsonString = [[NSString alloc] initWithData:responseData
                                                   encoding:NSUTF8StringEncoding];
        result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsString:responseJsonString];
        [result setKeepCallbackAsBool:YES];
        [self.commandDelegate sendPluginResult:result callbackId:callbackId];
    } else {
        NSLog(@"SimpleXpBeaconPlugin JSONString error: %@", [error localizedDescription]);
    }
}

#pragma mark - locationManager:didChangeAuthorizationStatus:

-(void)locationManager:(CLLocationManager *)manager didChangeAuthorizationStatus:(CLAuthorizationStatus)status {
    
    if( ![CLLocationManager locationServicesEnabled] ) {
        NSLog(@"SimpleXpBeaconPlugin Location services are not enabled.");
    } else {
        NSLog(@"SimpleXpBeaconPlugin Location services ARE enabled.");
    }
    
    if( [CLLocationManager authorizationStatus] != kCLAuthorizationStatusAuthorizedWhenInUse ) {
        NSLog(@"SimpleXpBeaconPlugin Location services not authorised when app in use.");
    } else {
        NSLog(@"SimpleXpBeaconPlugin Location services ARE authorised for when app in use.");
    }
    
    if( [CLLocationManager authorizationStatus] != kCLAuthorizationStatusAuthorizedAlways ) {
        NSLog(@"SimpleXpBeaconPlugin Location services not authorised always.");
    } else {
        NSLog(@"SimpleXpBeaconPlugin Location services ARE authorised always.");
    }
}

#pragma mark - onAppTerminate

- (void)onAppTerminate
{
    // override this if you need to do any cleanup on app exit
    
    NSLog(@"SimpleXpBeaconPlugin - onAppTerminate");
    
    [super onAppTerminate];
}

#pragma mark - onMemoryWarning

- (void)onMemoryWarning
{
    // override to remove caches, etc
    
    NSLog(@"SimpleXpBeaconPlugin - onMemoryWarning");
    
    [super onMemoryWarning];
}

#pragma mark - onReset

- (void)onReset
{
    // Override to cancel any long-running requests when the WebView navigates or refreshes.
    
    NSLog(@"SimpleXpBeaconPlugin - onReset");
    
    [super onReset];
}

#pragma mark - dispose

- (void)dispose
{
    NSLog(@"SimpleXpBeaconPlugin - dispose");
    
    [super dispose];
}

@end
