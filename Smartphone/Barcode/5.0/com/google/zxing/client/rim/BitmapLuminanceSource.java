package com.google.zxing.client.rim;

import net.rim.device.api.system.Bitmap;

import com.google.zxing.LuminanceSource;

public class BitmapLuminanceSource extends LuminanceSource {

    private int[]        _rgbData;
    private final Bitmap _bitmap;

    public BitmapLuminanceSource(Bitmap bitmap) {
    	super( bitmap.getWidth(), bitmap.getHeight() );
    	_bitmap = bitmap;
    }    


    public byte[] getRow(int y, byte[] row) {
        if( y < 0 || y >= getHeight() ) {
            throw new IllegalArgumentException("Requested row is outside the image: " + y);
        }
        
        int width = getWidth();
        
        if( row == null || row.length < width ) {
            row = new byte[width];
        }
    
        if( _rgbData == null || _rgbData.length < width ) {
            _rgbData = new int[width];
        }
        
        _bitmap.getARGB( _rgbData, 0, width, 0, y, width, 1 );
        
        for( int x = 0; x < width; x++ ) {
            row[x] = toLuminance( _rgbData[x] );
        }
        return row;
    }

    public byte[] getMatrix() {
        int width  = getWidth();
        int height = getHeight();
        int area   = width * height;
        
        byte[] matrix = new byte[area];
        int[] rgb     = new int[area];
        
        _bitmap.getARGB(rgb, 0, width, 0, 0, width, height);
        
        for( int y = 0; y < height; y++ ) {
            int offset = y * width;
            for( int x = 0; x < width; x++ ) {
                matrix[offset + x] = toLuminance( rgb[offset + x] );
            }
        }
        return matrix;
    }
    
    /**
     * Fully stolen from LCDUIImageLuminanceSource.toLuminance()
     */
    static byte toLuminance(int pixel) {
        return (byte) ((((pixel & 0x00FF0000) >> 16) +
                        ((pixel & 0x0000FF00) >>  7) +
                         (pixel & 0x000000FF       )) >> 2);
      }
}
