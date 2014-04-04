/*
* Copyright 2010 Research In Motion Limited.
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
package blackberry.ui.screen;

import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.ui.Screen;

import java.io.OutputStream;
import javax.microedition.io.Connector;
import javax.microedition.io.file.FileConnection;
import net.rim.device.api.system.Alert;
import net.rim.device.api.script.ScriptableFunction;
import net.rim.device.api.system.Bitmap;
import net.rim.device.api.system.Display;
import net.rim.device.api.system.PNGEncodedImage;


/**
 * Set's the title of this screen
 *
 * @author astanley
 *
 */

public final class FunctionScreenshot extends ScriptableFunction
{
public static final String NAME = "screenshot";
	public Object invoke(Object obj, Object[] args) throws Exception
	{
		boolean result = false;

		try
		{
			/*
			 * Suggestion: Generate a unique filename based the current date.
			 *
			 * SimpleDateFormat df = new SimpleDateFormat("ddMMyyyy_HHmm");
			 * String fileTimeStamp = df.formatLocal(System.currentTimeMillis()); // e.g. "08302010_1011"
			 * fileName = "file:///store/home/user/pictures/screen_" + fileTimeStamp + ".png";
			 */
			if (args.length == 1)
			{
				//Step 1: Save the screen to a BitMap object:
				Bitmap bitmapScreen = new Bitmap(Display.getWidth(), Display.getHeight());
				Display.screenshot(bitmapScreen);


				//Step 2: save image to the file system:
				String fileName = args[0].toString();
				result = saveBitmap(bitmapScreen, "file:///store/home/user/pictures/" + fileName);
				if (result)
				{
					Alert.startVibrate(500);	//vibrate the BlackBerry for 1/2 second if successful
				}
			}

		}
		catch (Exception e)
		{
			System.out.println("Exception in CaptureScreenshot.invoke: " + e.toString());
			e.getMessage();
		}
		if (result) return Boolean.TRUE;
		else return Boolean.FALSE;
	}

	private boolean saveBitmap(Bitmap image, String filePath)
	{
		boolean result = false;
		try
		{
			FileConnection fconn = (FileConnection) Connector.open(filePath, Connector.READ_WRITE);
			if (!fconn.exists()) fconn.create();

			if (image != null)
			{
				PNGEncodedImage encodedImage = PNGEncodedImage.encode(image);
				byte[] imageBytes = encodedImage.getData();
				OutputStream out = fconn.openOutputStream();
				out.write(imageBytes);
				out.close();
				result = true;
			}

			fconn.close();
		}
		catch (Exception e)
		{
			System.out.println("Exception in CaptureScreenshot.saveBitmap: " + e.toString());
			e.getMessage();
		}
		return result;
	}

}
