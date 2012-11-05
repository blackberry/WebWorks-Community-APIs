package webworks.mail.attachment;

import net.rim.device.api.script.Scriptable;

public final class MailAttachmentNamespace extends Scriptable {
	
	public static final String FIELD_INVOKE_LP = MailAttachmentFunction.NAME;
	public static final String NAME = "webworks.mail.attachment";
	
	private MailAttachmentFunction mailAttachmentFunc;

	public MailAttachmentNamespace() {
		this.mailAttachmentFunc = new MailAttachmentFunction();
	}

	// The getField() function is called when the 
	//  dot '.' extender is used on your JavaScript object.
	public Object getField(String name) throws Exception {
		if (name.equals(FIELD_INVOKE_LP)) {
			return new MailAttachmentFunction();
		} 
		return super.getField(name);
	}
	
	public boolean putField(String field, Object value) throws Exception {
        return super.putField(field, value);
    }
}
