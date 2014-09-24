package webworks.ui.AboutScreen;

import net.rim.device.api.ui.Color;
import net.rim.device.api.ui.Font;
import net.rim.device.api.ui.Ui;
import net.rim.device.api.ui.XYEdges;
import net.rim.device.api.ui.decor.Background;
import net.rim.device.api.ui.decor.BackgroundFactory;
import net.rim.device.api.ui.decor.Border;
import net.rim.device.api.ui.decor.BorderFactory;

public class AboutScreenConfig
{
	public static final Background bgColor_Gradient=BackgroundFactory.createLinearGradientBackground(Color.GRAY, Color.GRAY, Color.BLACK, Color.BLACK);

	public static final Border border_popup_Transparent=BorderFactory.createRoundedBorder(new XYEdges(16,16,16,16), Color.BLACK, 200, Border.STYLE_FILLED);
	public static final Background bg_popup_Transparent=BackgroundFactory.createSolidTransparentBackground(Color.BLACK, 200);

	public static final Font FONT_ABOUT_TITLE = Font.getDefault().derive(Font.BOLD, Font.getDefault().getHeight(Ui.UNITS_pt)+2, Ui.UNITS_pt);
	public static final Font FONT_ABOUT_HEADLINE = Font.getDefault().derive(Font.BOLD | Font.ITALIC, Font.getDefault().getHeight(Ui.UNITS_pt), Ui.UNITS_pt);
	public static final Font FONT_ABOUT_SMALL = Font.getDefault().derive(Font.PLAIN, Font.getDefault().getHeight(Ui.UNITS_pt)-1, Ui.UNITS_pt);
	public static final Font FONT_ABOUT_LARGE = Font.getDefault().derive(Font.PLAIN, Font.getDefault().getHeight(Ui.UNITS_pt)+1, Ui.UNITS_pt);
}