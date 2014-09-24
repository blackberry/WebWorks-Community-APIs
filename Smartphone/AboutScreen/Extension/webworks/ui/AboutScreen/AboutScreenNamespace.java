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