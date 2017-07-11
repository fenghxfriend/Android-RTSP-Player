package com.greenbamboo.prescholleducation.MediaFramework;

public class GMediaFramework {
	private native static boolean classInit();
	static {
		
		classInit();
		
	}

	// 变量定义
	static class GMediaFrameworkHolder {
		static GMediaFramework mSharedInstance = new GMediaFramework();
	}

	public static GMediaFramework sharedInstance() {
		return GMediaFrameworkHolder.mSharedInstance;
	}

	// 回调函数
	public interface MessageListener {
		public void onMessage(int id, String message);
	}

	private MessageListener mListener = null;

	public void setMessageListener(MessageListener listener) {
		this.mListener = listener;
	}

	public void resetMessageListener() {
		this.mListener = null;
	}

	protected void fireOnMessage(int id, String message) {
		try {
			if (mListener != null) {
				mListener.onMessage(id, message);
			}
		} catch (Exception ex) {
			// Log.e(TAG, "fire fireOnMessage failed: " + ex.getMessage());
		}
	}
	
	// member accessed by native methods.
	private int mNativeMediaFramework;

	// 接口函数
	public native void install();

	public native void mediaPlay(String address, String name, String password);

	public native void bindRenderer(Object renderer);

	public native void mediaStop();

	public native void unInstall();
}
