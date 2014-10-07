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

import java.util.Hashtable;

import net.rim.device.api.i18n.Locale;

public class Languages
{
	Hashtable language_zh = new Hashtable();
	public String _about = "";
	public String _version = "";
	public String _contact = "";
	public String _email = "";
	public String _weibo = "";
	public String _facebook = "";
	public String _twitter = "";
	public String _alipay = "";
	public String _shortcutkeys = "";
	public String _writereview = "";

	public Languages()
	{
		String defaultLanguage = Locale.getDefaultForSystem().getLanguage().toLowerCase();

		if (defaultLanguage.equalsIgnoreCase("zh"))
		{
			_about = "\u5173\u4E8E";
			_version = "\u7248\u672C\uFF1A";
			_contact = "\u8054\u7CFB\u65B9\u5F0F";
			_email = "\u7535\u5B50\u90AE\u4EF6\uFF1A";
			_weibo = "\u65B0\u6D6A\u5FAE\u535A\uFF1A";
			_facebook = "Facebook\uFF1A";
			_twitter = "Twitter\uFF1A";
			_alipay = "\u652F\u4ED8\u5B9D\uFF1A";
			_shortcutkeys = "\u5FEB\u6377\u952E";
			_writereview = "\u7F16\u5199\u8BC4\u8BBA";
		} else {
			_about = "About";
			_version = "ver: ";
			_contact = "Contact";
			_email = "E-mail: ";
			_weibo = "Weibo: ";
			_facebook = "Facebook: ";
			_twitter = "Twitter: ";
			_alipay = "Alipay: ";
			_shortcutkeys = "Shortcut Keys";
			_writereview = "Write a review";
		}
	}

	public String getAbout() {return _about;}

	public String getVersion() {return _version;}

	public String getContact() {return _contact;}

	public String getEmail() {return _email;}

	public String getWeibo() {return _weibo;}

	public String getFacebook() {return _facebook;}

	public String getTwitter() {return _twitter;}

	public String getAlipay() {return _alipay;}

	public String getShortcutKeys() {return _shortcutkeys;}

	public String getWriteReview() {return _writereview;}
}
