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
