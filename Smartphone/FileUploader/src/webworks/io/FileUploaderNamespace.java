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

package webworks.io;

import net.rim.device.api.script.Scriptable;

public class FileUploaderNamespace extends Scriptable {
	
	public static final String NAME = "webworks.io.FileUploader";
	
	private static final String FIELD_UPLOAD = FunctionUpload.NAME;
	
	private FunctionUpload _uploadFunction;
	
	public FileUploaderNamespace() {
		_uploadFunction = new FunctionUpload();
	}

    public Object getField(String name) throws Exception {
        if (name.equals(FIELD_UPLOAD)) {
            return _uploadFunction;
        }
		return super.getField(name);
    }
	
}
