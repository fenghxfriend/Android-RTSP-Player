package com.greenbamboo.prescholleducation.MediaFramework;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.greenbamboo.prescholleducation.MediaFramework.GMediaFramework.MessageListener;
import com.video.draw.PlayerView;
import com.video.draw.PlayerRenderer;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import com.video.draw.PlayerEGLConfigChooser;
import com.video.draw.PlayerEGLContextFactory;
import com.video.draw.PlayerEGLWindowSurfaceFactory;
import android.os.Handler;
import android.os.Message;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.FrameLayout;

public class GMediaPlayer extends FrameLayout implements MessageListener {
	static {
		System.loadLibrary("GMediaFramework");
	}

	private static final String TAG = "GMediaPlayer";

	public static final int AUDIO = 1;
	public static final int VIDEO = 2;
	public static final int BOTHAV = 3;

	public static final int STOPED = 1;
	public static final int PLAYING = 2;

	private PlayerView mPlayerView = null;
	private PlayerRenderer mRenderer = null;
	private String mAddress = "";
	private String mUserName = "";
	private String mPassword = "";

	private int mode = VIDEO;
	private int screenWidth;
	private int mStatus;

	public GMediaPlayer(Context context) {
		super(context);
		init(context);
	}

	public GMediaPlayer(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init(context);
	}

	public GMediaPlayer(Context context, AttributeSet attrs) {
		super(context, attrs);
		init(context);
	}

	private void init(Context context) {
		Log.d(TAG, "init");
		WindowManager wm = (WindowManager) getContext().getSystemService(
				Context.WINDOW_SERVICE);
		screenWidth = wm.getDefaultDisplay().getWidth();

		mPlayerView = new PlayerView(context);
		addView(mPlayerView);
		mPlayerView.getHolder().setFormat(PixelFormat.OPAQUE);

		mRenderer = new PlayerRenderer();
		mRenderer.setScale(1.0f,1.0f,1.0f);
		mRenderer.setListener(new PlayerRenderer.FrameListener() {

			public void onNewFrame() {
				mPlayerView.requestRender();
			}
		});
		mPlayerView.setRenderer(mRenderer);
		mPlayerView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

		mStatus = STOPED;

	}

	public void init() {
		// TODO Auto-generated method stub
		GMediaFramework.sharedInstance().install();
		GMediaFramework.sharedInstance().setMessageListener(this);
	}

	public void destroy() {
		// TODO Auto-generated method stub
		Log.d(TAG, "destroy");
		GMediaFramework.sharedInstance().unInstall();
		if (mPlayerView != null) {
			mPlayerView.queueEvent(new Runnable() {

				@Override
				public void run() {
					if (mRenderer != null) {
						// TODO Auto-generated method stub
						mRenderer.destroy();
						mRenderer = null;
					}
				}
			});
		}

	}

	public void Play(String address, String name, String password) {
		GMediaFramework.sharedInstance().bindRenderer(mRenderer);
		boolean changeed = false;
		if (address == null)
			return;

		String tUserNmae = "";
		String tPassword = "";
		if (name != null)
			tUserNmae = name;
		if (password != null)
			tPassword = password;

		if (!mUserName.equals(tUserNmae)) {
			changeed = true;
			mUserName = tUserNmae;
		}
		if (!mPassword.equals(tPassword)) {
			changeed = true;
			mPassword = tPassword;
		}
		if (!mAddress.equals(address)) {
			changeed = true;
			mAddress = address;
		}
		switch (mStatus) {
		case STOPED:
			GMediaFramework.sharedInstance().mediaPlay(mAddress, mUserName,
					mPassword);
			break;
		case PLAYING:
			if (changeed) {
				Log.d(TAG,
						"Play, but the address or username or password is changed!");
				GMediaFramework.sharedInstance().mediaStop();
				mHandler.postDelayed(new Runnable() {
					@Override
					public void run() {
						// TODO Auto-generated method stub
						GMediaFramework.sharedInstance().mediaPlay(mAddress,
								mUserName, mPassword);
					}
				}, 2000);

			}
			break;

		default:
			break;
		}
		mStatus = PLAYING;
	}

	public void Stop() {
		if (mStatus == STOPED) {
			Log.d(TAG, "already stoped");
			return;
		} else {
			GMediaFramework.sharedInstance().mediaStop();
			mStatus = STOPED;
		}
	}

	public void Pause() {
		if (mPlayerView != null) {
			mPlayerView.onPause();
		}

	}

	public void Resume() {
		if (mPlayerView != null) {
			mPlayerView.onResume();
		}
	}

	@SuppressLint("HandlerLeak")
	private Handler mHandler = new Handler() {

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub

			int id = msg.what;
			String content = (String) msg.obj;
			Log.d(TAG, "Message: id@" + id + ", content@" + content);

			boolean fIs555 = (((id >> 30) & 1) > 0);
			int f555Value = ((id << 2) >> 2);
			boolean fVideo = (((id >> 24) & 1) > 0);
			boolean fOpen = (((id >> 16) & 1) > 0);
			boolean fSuccess = (((id >> 8) & 1) > 0);
			boolean fFirstFrame = (((id >> 31) & 1) > 0);
			int fWidth = (fFirstFrame ? ((id << 2) >> 17) : 0);
			int fHeight = (fFirstFrame ? ((id << 17) >> 17) : 0);

			if (!fIs555) {
				if (!fFirstFrame) {

					if (fVideo && fOpen && fSuccess) {
						switch (mode) {
						case AUDIO:
							Log.d(TAG, "just audio!");
							break;
						case VIDEO:
							Log.d(TAG, "just video!");
							break;
						case BOTHAV:
							Log.d(TAG, "both video and audio!");
							break;

						default:
							break;
						}
					} else if (fVideo && !fOpen && fSuccess) {
						Log.e(TAG, "the video can not open!");
					}
					Log.d(TAG, (fOpen ? "open" : "close") + " "
							+ (fVideo ? "video" : "audio") + " "
							+ (fSuccess ? "success" : "failed"));
					if (!fSuccess) {
						Log.e(TAG, "ERROR >> "
								+ (content != null ? content : "null"));
						mStatus = STOPED;
					}
				} else /* 第一帧图像 */
				{
					Log.d(TAG, "First video frame:  " + fWidth + " * "
							+ fHeight);
					ViewGroup.LayoutParams param = (ViewGroup.LayoutParams) mPlayerView
							.getLayoutParams();
					param.width = screenWidth;
					param.height = screenWidth * fHeight / fWidth;
					mPlayerView.setLayoutParams(param);
				}
			} else /* Live555 Event */
			{
				Log.d(TAG, "OnLive555 Event, Code@ " + f555Value);
				mStatus = STOPED;
				// GMediaFramework.sharedInstance().mediaPlay(mAddress,
				// mUserName,
				// mPassword);

			}

		}
	};

	@Override
	public void onMessage(int id, String content) {
		// TODO Auto-generated method stub
		Message msg = new Message();
		msg.what = id;
		msg.obj = content;
		mHandler.sendMessage(msg);
	}
}
