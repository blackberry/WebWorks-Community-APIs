/*
* Copyright 2010-2011 Research In Motion Limited.
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


package io.fileTransfer;

import net.rim.device.api.script.Scriptable;

/**
 * @author kwallis
 *
 */
public class FileTransferNamespace extends Scriptable {

	public static final String NAME = "webworks.io.fileTransfer";


	// This function is accessed whenever our namespace is accessed using the
	// dot operator.

	// The name sent to function is the text after the dot operator.

	// Here we check of the call to our Add function and return a new
	// FunctionAdd , however this could

	// also be a static reference or literal data in the case of FIELD_MESSAGE.

	public Object getField(String name) throws Exception {

		if (name.equals(FunctionFileTransferUpload.NAME)) {

			return new FunctionFileTransferUpload();

		} else if (name.equals(FunctionFileTransferDownload.NAME)) {

			return new FunctionFileTransferDownload();

		}

		return super.getField(name);

	}
}
