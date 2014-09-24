package webworks.ui.AboutScreen;

import net.rim.device.api.script.Scriptable;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.system.Characters;
import net.rim.device.api.ui.Graphics;
import net.rim.device.api.ui.UiApplication;
import net.rim.device.api.ui.component.LabelField;
import net.rim.device.api.ui.component.SeparatorField;
import net.rim.device.api.ui.container.HorizontalFieldManager;
import net.rim.device.api.ui.container.PopupScreen;
import net.rim.device.api.ui.container.VerticalFieldManager;

public class ShowAboutScreen extends ScriptableFunction
{
	String title = "";
	String version = "";
	String intro = "";
	String author = "";
	String email = "";
	String weibo = "";
	String facebook = "";
	String twitter = "";
	String alipay = "";

	public Object invoke(Object obj, Object[] args) throws Exception
    {
		if (args.length == 1)
		{
			Scriptable config = (Scriptable) args[0];

			if (config != null)
			{
				if (config.getField("title") instanceof String) {title = config.getField("title").toString();}
				if (config.getField("version") instanceof String) {version = config.getField("version").toString();}
				if (config.getField("intro") instanceof String) {intro = config.getField("intro").toString();}
				if (config.getField("author") instanceof String) {author = config.getField("author").toString();}
				if (config.getField("email") instanceof String) {email = config.getField("email").toString();}
				if (config.getField("weibo") instanceof String) {weibo = config.getField("weibo").toString();}
				if (config.getField("facebook") instanceof String) {facebook = config.getField("facebook").toString();}
				if (config.getField("twitter") instanceof String) {twitter = config.getField("twitter").toString();}
				if (config.getField("alipay") instanceof String) {alipay = config.getField("alipay").toString();}

				UiApplication.getUiApplication().invokeLater(new Runnable()
				{
					public void run()
					{
						UiApplication.getUiApplication().pushScreen(new AboutScreen(title, version, intro, author, email, weibo, facebook, twitter, alipay));
					}
				});
			}
        }

        return UNDEFINED;
    }

	class AboutScreen extends PopupScreen
	{
		LabelField _labelEmail;
		LabelField _labelWeibo;
		LabelField _labelFacebook;
		LabelField _labelTwitter;
		LabelField _labelAlipay;

		public AboutScreen(String title, String version, String intro, String author, String email, String weibo, String facebook, String twitter, String alipay)
		{
			super(new VerticalFieldManager(FOCUSABLE | NO_VERTICAL_SCROLL));

			Languages language = new Languages();

			setBorder(AboutScreenConfig.border_popup_Transparent);
			setBackground(AboutScreenConfig.bg_popup_Transparent);

	    	LabelField labelAbout = new LabelField(language.getAbout(), USE_ALL_WIDTH | LabelField.ELLIPSIS);
	    	labelAbout.setFont(AboutScreenConfig.FONT_ABOUT_TITLE);
	    	labelAbout.setPadding(0, 0, 1, 0);

	    	LabelField labelTitle = new LabelField(title, USE_ALL_WIDTH | LabelField.ELLIPSIS);
	    	labelTitle.setFont(AboutScreenConfig.FONT_ABOUT_LARGE);

	    	LabelField labelVersion = new LabelField(language.getVersion() + version, USE_ALL_WIDTH | LabelField.ELLIPSIS | LabelField.RIGHT);
	    	labelVersion.setFont(AboutScreenConfig.FONT_ABOUT_SMALL);

			VerticalFieldManager vfmTitle = new VerticalFieldManager(FIELD_VCENTER);
	    	vfmTitle.add(labelAbout);
	    	vfmTitle.add(new SeparatorField());
	    	vfmTitle.add(labelTitle);
	    	vfmTitle.add(labelVersion);

			VerticalFieldManager vfmContent = new VerticalFieldManager(VERTICAL_SCROLL);
			HorizontalFieldManager horizontalContactTitle = new HorizontalFieldManager();
			//HorizontalFieldManager horizontalShortcutTitle = new HorizontalFieldManager();
			//VerticalButtonFieldSet vbf = new VerticalButtonFieldSet(USE_ALL_WIDTH);

	    	LabelField labelIntro = new LabelField(intro);

	    	LabelField labelContact = new LabelField(language.getContact(), USE_ALL_WIDTH | LabelField.ELLIPSIS);
	    	labelContact.setFont(AboutScreenConfig.FONT_ABOUT_HEADLINE);

	    	LabelField labelAuthor = addLabel("\u2022 " + author);
	    	if (!email.equalsIgnoreCase("")) {_labelEmail = addLabel("\u2022 " + language.getEmail() + email);}
	    	if (!weibo.equalsIgnoreCase("")) {_labelWeibo = addLabel("\u2022 " + language.getWeibo() + weibo);}
	    	if (!facebook.equalsIgnoreCase("")) {_labelFacebook = addLabel("\u2022 " + language.getFacebook() + facebook);}
	    	if (!twitter.equalsIgnoreCase("")) {_labelTwitter = addLabel("\u2022 " + language.getTwitter() + twitter);}
	    	if (!alipay.equalsIgnoreCase("")) {_labelAlipay = addLabel("\u2022 " + language.getAlipay() + alipay);}

	    	/*
	    	LabelField labelShortcut = new LabelField("快捷键：", USE_ALL_WIDTH | LabelField.ELLIPSIS);
	    	labelShortcut.setFont(AboutScreenConfig.FONT_ABOUT_HEADLINE);

	    	LabelField shortB=addLabel("\u2022 B：列表滚动到底部");
	    	LabelField shortT=addLabel("\u2022 T：列表滚动到顶部");
	    	LabelField shortL=addLabel("\u2022 L：获取最新新闻");
	    	LabelField shortH=addLabel("\u2022 H：获取热门新闻");
	    	LabelField shortS=addLabel("\u2022 S：获取专题分类");
	    	LabelField shortC=addLabel("\u2022 C：选择往期新闻");
	    	LabelField shortN=addLabel("\u2022 N：下一期新闻");
	    	LabelField shortP=addLabel("\u2022 P：上一期新闻");
	    	LabelField shortA=addLabel("\u2022 A：显示本窗口");
	    	LabelField shortX=addLabel("\u2022 X：退出程序");

	    	ButtonField btnWriteAReview = new ButtonField("编写评论", ButtonField.NEVER_DIRTY | ButtonField.CONSUME_CLICK);
	    	btnWriteAReview.setChangeListener(new FieldChangeListener() {
				public void fieldChanged(Field field, int context)
				{
					try
		            {
		                openAppWorld(AboutScreenConfig.BBW_APPID);
		            } catch(final Exception e)
		            {
		                UiApplication.getUiApplication().invokeLater(new Runnable()
		                {
		                    public void run()
		                    {
		                    	if(e instanceof ContentHandlerException)
		                    	{
		                    		Dialog.alert("BlackBerry World is not installed!");
		                    	} else {
		                    		Dialog.alert("Problems opening App World: " + e.getMessage());
		                    	}
		                    }
		                });
		            }
				}
			});

	    	ButtonField btnBrowseOtherApps = new ButtonField("浏览其它软件", ButtonField.NEVER_DIRTY | ButtonField.CONSUME_CLICK);
	    	btnBrowseOtherApps.setChangeListener(new FieldChangeListener() {
				public void fieldChanged(Field field, int context)
				{
					BrowserSession browser=Browser.getDefaultSession();
		    		browser.displayPage("http://appworld.blackberry.com/webstore/vendor/69061");
				}
			});
			*/

	    	horizontalContactTitle.add(labelContact);
	    	horizontalContactTitle.add(new LabelField("", LabelField.FOCUSABLE));
	    	vfmContent.add(labelIntro);
	    	vfmContent.add(new LabelField());
	    	vfmContent.add(horizontalContactTitle);
	    	vfmContent.add(labelAuthor);
	    	if (_labelEmail != null) {vfmContent.add(_labelEmail);}
	    	if (_labelWeibo != null) {vfmContent.add(_labelWeibo);}
	    	if (_labelAlipay != null) {vfmContent.add(_labelAlipay);}
	    	if (_labelFacebook != null) {vfmContent.add(_labelFacebook);}
	    	if (_labelTwitter != null) {vfmContent.add(_labelTwitter);}
	    	vfmContent.add(new LabelField());

			/*
	    	horizontalShortcutTitle.add(labelShortcut);
	    	horizontalShortcutTitle.add(new LabelField("", LabelField.FOCUSABLE));
	    	vfmContent.add(horizontalShortcutTitle);
	    	vfmContent.add(shortB);
	    	vfmContent.add(shortT);
	    	vfmContent.add(shortL);
	    	vfmContent.add(shortH);
	    	vfmContent.add(shortS);
	    	vfmContent.add(shortC);
	    	vfmContent.add(shortN);
	    	vfmContent.add(shortP);
	    	vfmContent.add(shortA);
	    	vfmContent.add(shortX);
	    	vfmContent.add(new LabelField());
	    	vbf.add(btnWriteAReview);
	    	vbf.add(btnBrowseOtherApps);
	    	vfmContent.add(vbf);
			*/
	    	add(vfmTitle);
	    	add(vfmContent);
		}

		private LabelField addLabel(String label)
		{
			return new LabelField(label, USE_ALL_WIDTH | LabelField.ELLIPSIS);
		}

		protected void paintBackground(Graphics g) {}

		protected boolean keyChar(char character, int status, int time)
		{
			switch (character)
			{
				case Characters.ESCAPE:
					UiApplication.getUiApplication().popScreen(this);
					return true;
			}

			return super.keyChar(character, status, time);
		}
	}
}