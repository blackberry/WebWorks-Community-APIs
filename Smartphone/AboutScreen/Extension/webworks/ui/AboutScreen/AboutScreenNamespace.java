/*
* Copyright (c) 2014 BlackBerry Limited.
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

package webworks.ui.AboutScreen;

import net.rim.device.api.script.Scriptable;

public final class AboutScreenNamespace extends Scriptable 
{
	public static final String FIELD_ABOUTSCREEN_SHOW = "show";

    public AboutScreenNamespace() {}

    public Object getField(String name) throws Exception
    {
    	if(name.equals(FIELD_ABOUTSCREEN_SHOW))
        {
            return new ShowAboutScreen();
        }

        return super.getField(name);
    }
}
