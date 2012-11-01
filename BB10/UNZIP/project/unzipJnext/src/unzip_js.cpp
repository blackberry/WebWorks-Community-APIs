/*
* Copyright 2012 Research In Motion Limited.
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

#include <string>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "unzip_js.hpp"

using namespace std;

/**
 * Default constructor.
 */
Memory::Memory(const std::string& id) : m_id(id) {
    m_thread = 0;
}

/**
 * Memory destructor.
 */
Memory::~Memory() {
}

/**
 * This method returns the list of objects implemented by this native
 * extension.
 */
char* onGetObjList() {
    static char name[] = "Memory";
    return name;
}

/**
 * This method is used by JNext to instantiate the Memory object when
 * an object is created on the JavaScript server side.
 */
JSExt* onCreateObject(const string& className, const string& id) {
    if (className == "Memory") {
        return new Memory(id);
    }

    return NULL;
}

/**
 * Method used by JNext to determine if the object can be deleted.
 */
bool Memory::CanDelete() {
    return true;
}

/**
 * It will be called from JNext JavaScript side with passed string.
 * This method implements the interface for the JavaScript to native binding
 * for invoking native code. This method is triggered when JNext.invoke is
 * called on the JavaScript side with this native objects id.
 */
string Memory::InvokeMethod(const string& command) {
    // Determine which function should be executed
    if (command == "getMemoryNative") {
        return convertLongToString(getMemory());
    } else if (command == "monitorMemoryNative") {
        return MonitorMemoryNative();
    } else {
        return "Unsupported Method";
    }
}

/**
 * Thread that retrieves the current amount of free memory every second and
 * sends it to the JavaScript side using an event. The thread shall continue
 * to retrieve the memory usage until the native object is destroyed on the
 * JavaScript side.
 */
void* MemoryThread(void* parent) {
    Memory *pParent = static_cast<Memory *>(parent);

    // Endless loop that collect memory information and send it to JNext JavaScript side
    while (true) {
        pParent->SendMemoryInfo();
        sleep(1);
    }

    return NULL;
}

/**
 * Method responsible for starting the thread to get memory usage. Only one
 * thread can be created per native JavaScript instance. This method returns
 * true if the thread was created successfully and false othrewise.
 */
bool Memory::StartThread() {
    if (!m_thread) {
        pthread_attr_t thread_attr;
        pthread_attr_init(&thread_attr);
        pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);

        pthread_create(&m_thread, &thread_attr, MemoryThread,
                static_cast<void *>(this));
        pthread_attr_destroy(&thread_attr);
        return true;
    } else {
        return false;
    }
}

/**
 * Method used to start the get memory usage thread. The method shall return a
 * string to the JavaScript side indicating whether or not the memory
 * monitoring was initialized.
 */
string Memory::MonitorMemoryNative() {
    if (StartThread()) {
        return "Memory monitored";
    } else {
        return "Memory already being monitored";
    }
}

/**
 * Method used by the getMemoryUsage thread to pass the amount of free memory
 * on the JavaScript side by firing an event.
 */
void Memory::SendMemoryInfo() {
    std::string eventString = "FreeMemory " + convertLongToString(getMemory());
    NotifyEvent(eventString);
}

// Notifies JavaScript of an event
void Memory::NotifyEvent(const std::string& event) {
    std::string eventString = m_id + " ";
    eventString.append(event);
    SendPluginEvent(eventString.c_str(), m_pContext);
}

/**
 * Method that retreives the current free memory of OS.
 */
long Memory::getMemory() {
    struct stat statbuf;
    paddr_t freemem;

    stat("/proc", &statbuf);
    freemem = (paddr_t) statbuf.st_size;

    return freemem;
}

/**
 * Utility function to convert a long into a string.
 */
string Memory::convertLongToString(long l) {
    stringstream ss;
    ss << l;
    return ss.str();
}
