/*
 * Copyright 2011 Research In Motion Limited.
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

package webworks.media.barcode;

import net.rim.device.api.applicationcontrol.ApplicationPermissions;
import net.rim.device.api.applicationcontrol.ApplicationPermissionsManager;
import net.rim.device.api.browser.field2.BrowserField;
import net.rim.device.api.script.ScriptEngine;
import net.rim.device.api.web.WidgetConfig;
import net.rim.device.api.web.WidgetExtension;

import org.w3c.dom.Document;

public class BarcodeExtension implements WidgetExtension {

	/* @Override */
	public String[] getFeatureList() {
		return new String[]{BarcodeNamespace.FEATURE_NAME};
	}

	/* @Override */
	public void loadFeature(final String feature, final String version, final Document doc, final ScriptEngine scriptEngine) throws Exception {
		if(feature.equals(BarcodeNamespace.FEATURE_NAME)){
			scriptEngine.addExtension(feature, BarcodeNamespace.getInstance());
			;
			ApplicationPermissions missingPermissions = getMissingPermissions();

			if( missingPermissions.getPermissionKeys().length > 0 ) {
				boolean accept = requestPermissions(missingPermissions);
				if( !accept )
					throw new Exception( "Could not load blackberry.media.camera" );                               
			}

			scriptEngine.addExtension(feature, BarcodeNamespace.getInstance());

		}

	}

	private boolean requestPermissions(ApplicationPermissions permissions){
		return  ApplicationPermissionsManager.getInstance().invokePermissionsRequest( permissions );
	}

	private ApplicationPermissions getMissingPermissions(){
		ApplicationPermissionsManager apm = ApplicationPermissionsManager.getInstance();
		ApplicationPermissions permissions = apm.getApplicationPermissions();
		ApplicationPermissions newPermissions = new ApplicationPermissions();

		int permissionKeys[] = new int[] { ApplicationPermissions.PERMISSION_FILE_API, ApplicationPermissions.PERMISSION_INPUT_SIMULATION, ApplicationPermissions.PERMISSION_MEDIA, ApplicationPermissions.PERMISSION_RECORDING };

		for( int i = 0; i < permissionKeys.length; i++ ) {
			int key = permissionKeys[ i ];
			if( permissions.getPermission( key ) != ApplicationPermissions.VALUE_ALLOW ) {
				newPermissions.addPermission( key );
			}
		}

		return newPermissions;
	}

	/* @Override */
	public void register(final WidgetConfig widgetconfig, final BrowserField browserfield) {
	}

	/* @Override */
	public void unloadFeatures(final Document document) {
	}

}
