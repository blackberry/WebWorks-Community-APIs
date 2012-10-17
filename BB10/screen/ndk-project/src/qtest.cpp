/*
 *  Copyright 2012 Research In Motion Limited.
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

#include "../public/plugin.h"
#include <bb/platform/HomeScreen>
#include "../public/json/reader.h"

class QTest: public JSExt {
public:

	QTest(const std::string& id) :
			m_id(id) {
	}
	virtual ~QTest() {
	}

	virtual std::string InvokeMethod(const std::string& command) {
		//parse the command
		int index = command.find_first_of(" ");
		string strCommand = command.substr(0, index);
		string jsonObject = command.substr(index + 1, command.length());

		//call the method with args
		if (strCommand == "setWallpaper") {
			return setAsWallpaper(jsonObject);
		}
		else
			return "No command found";
	}

	virtual bool CanDelete() {
		return true;
	}

private:
	void NotifyEvent(const char* szEvent) { }

	std::string m_id;

	string setAsWallpaper(const std::string& jsonObject) {
		// Parse the JSON
		Json::Reader reader;
		Json::Value obj;
		bool parse = reader.parse(jsonObject, obj);

		QString qstr = QString::fromStdString(obj["file"].asString());

		//set as home screen
		bb::platform::HomeScreen homescreen;
		QUrl url(qstr);
		homescreen.setWallpaper(url);
		return "Success";
	}

};


char* onGetObjList() {
	static char name[] = "QTest";
	return name;
}


JSExt* onCreateObject(const std::string& className, const std::string& id) {
	if (className != "QTest")
		return 0;

	return new QTest(id);
}

