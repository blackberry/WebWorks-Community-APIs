/*
 * Copyright 2013 Research In Motion Limited.
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
#include "../public/tokenizer.h"
#include "template_js.hpp"
#include "template_ndk.hpp"

using namespace std;

/**
 * Default constructor.
 */
TemplateJS::TemplateJS(const std::string& id) :
		m_id(id) {
	m_pTemplateController = new webworks::DeviceInfo(this);
}

/**
 * TemplateJS destructor.
 */
TemplateJS::~TemplateJS() {
	if (m_pTemplateController)
		delete m_pTemplateController;
}

/**
 * This method returns the list of objects implemented by this native
 * extension.
 */
char* onGetObjList() {
	static char name[] = "TemplateJS";
	return name;
}

/**
 * This method is used by JNext to instantiate the TemplateJS object when
 * an object is created on the JavaScript server side.
 */
JSExt* onCreateObject(const string& className, const string& id) {
	if (className == "TemplateJS") {
		return new TemplateJS(id);
	}

	return NULL;
}

/**
 * Method used by JNext to determine if the object can be deleted.
 */
bool TemplateJS::CanDelete() {
	return true;
}

/**
 * It will be called from JNext JavaScript side with passed string.
 * This method implements the interface for the JavaScript to native binding
 * for invoking native code. This method is triggered when JNext.invoke is
 * called on the JavaScript side with this native objects id.
 */
string TemplateJS::InvokeMethod(const string& command) {
	// command appears with parameters following after a space
	int index = command.find_first_of(" ");
	std::string strCommand = command.substr(0, index);
	std::string arg = command.substr(index + 1, command.length());

	if (strCommand == "getModelNumber")
		return m_pTemplateController->getModelNumber();
	else if (strCommand == "getMCC")
		return m_pTemplateController->getMCC();
	else if (strCommand == "getRoamingStatus")
		return m_pTemplateController->getRoamingStatus();
	else if (strCommand == "getMNC")
			return m_pTemplateController->getMNC();
	else if (strCommand == "isSimulator")
			return m_pTemplateController->isSimulator();
	else if (strCommand == "getNetwork")
		return m_pTemplateController->getNetwork();
	else if (strCommand == "hasPhysicalKeyboard")
		return m_pTemplateController->hasPhysicalKeyboard();

	return "Unknown C++ method";
}
