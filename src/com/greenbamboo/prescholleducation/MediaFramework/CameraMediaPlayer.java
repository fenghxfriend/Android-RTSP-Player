package com.greenbamboo.prescholleducation.MediaFramework;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.os.Message;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.ViewGroup;

import com.greenbamboo.prescholleducation.MediaFramework.GMediaFramework.MessageListener;
import com.video.draw.PlayerEGLConfigChooser;
import com.video.draw.PlayerEGLContextFactory;
import com.video.draw.PlayerEGLWindowSurfaceFactory;
import com.video.draw.PlayerRenderer;
import com.video.draw.PlayerView;

public class CameraMediaPlayer implements MessageListener {

	static {
		System.loadLibrary("GMediaFramework");
	}

	private static final String TAG = CameraMediaPlayer.class.getSimpleName();

	public static final int AUDIO = 1;
	public static final int VIDEO = 2;
	public static final int BOTHAV = 3;

	public static final int STOPED = 1;
	public static final int PLAYING = 2;

	private GLSurfaceView mGLSurface = null;
	private PlayerRenderer mRenderer = null;
	private DisplayMetrics outMetrics;
	private Activity mActivity;
	private String mAddress = "";
	private String mUserName = "";
	private String mPassword = "";
	private int mode = VIDEO;
	private int mStatus;
	private boolean mNeedLayout = true;

	public static final String url = "rtsp://192.168.42.1/live";

	public CameraMediaPlayer(Activity activity) {
		this.mActivity = activity;
		this.outMetrics = new DisplayMetrics();
		this.mActivity.getWindowManager().getDefaultDisplay()
				.getMetrics(this.outMetrics);
	}

	public void setGLSurface(GLSurfaceView glSurface) {
		this.mGLSurface = glSurface;
		initialize();
	}

	private void initialize() {
		GMediaFramework.sharedInstance().install();
		GMediaFramework.sharedInstance().setMessageListener(this);
		this.mGLSurface.getHolder().setFormat(PixelFormat.OPAQUE);
		this.mGLSurface.setEGLContextClientVersion(2);
		this.mGLSurface.setEGLConfigChooser(new PlayerEGLConfigChooser(8, 8, 8,
				8, 0, 0));
		this.mGLSurface
				.setEGLWindowSurfaceFactory(new PlayerEGLWindowSurfaceFactory());
		this.mGLSurface.setEGLContextFactory(new PlayerEGLContextFactory());
		this.mGLSurface.setEGLContextClientVersion(2);
		this.mGLSurface.setDebugFlags(GLSurfaceView.DEBUG_CHECK_GL_ERROR
				| GLSurfaceView.DEBUG_LOG_GL_CALLS);

		mRenderer = new PlayerRenderer();
		mRenderer.setScale(1.0f, 1.0f, 1.0f);
		mRenderer.setListener(new PlayerRenderer.FrameListener() {

			public void onNewFrame() {
				mGLSurface.requestRender();
			}
		});
		mGLSurface.setRenderer(mRenderer);

		mGLSurface.setDebugFlags(GLSurfaceView.DEBUG_CHECK_GL_ERROR
				| GLSurfaceView.DEBUG_LOG_GL_CALLS);
		mGLSurface.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

		mStatus = STOPED;

	}

	public void destroy() {
		stop();
		GMediaFramework.sharedInstance().unInstall();
		if (mGLSurface != null) {
			mGLSurface.queueEvent(new Runnable() {

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

	public void play() {
		if (mStatus != PLAYING) {
			Play(url);
		}
	}

	public void Play(String address) {
		GMediaFramework.sharedInstance().bindRenderer(mRenderer);
		boolean changeed = false;
		if (!mAddress.equals(address)) {
			changeed = true;
			mAddress = address;
		}

		if (changeed) {
			switch (mStatus) {
			case STOPED:
				GMediaFramework.sharedInstance().mediaPlay(mAddress, mUserName,
						mPassword);
				break;
			case PLAYING:
				if (changeed) {
					GMediaFramework.sharedInstance().mediaStop();
					mHandler.postDelayed(new Runnable() {
						@Override
						public void run() {
							GMediaFramework.sharedInstance().mediaPlay(
									mAddress, mUserName, mPassword);
						}
					}, 1000);

				}
				break;

			default:
				break;
			}
			mStatus = PLAYING;
		}
	}

	public void stop() {
		if (mStatus == STOPED) {
			return;
		} else {
			GMediaFramework.sharedInstance().mediaStop();
			mStatus = STOPED;
		}
	}

	public void onPause() {
		if (mGLSurface != null) {
			mGLSurface.onPause();
		}

	}

	public void onResume() {
		if (mGLSurface != null) {
			mGLSurface.onResume();
		}
	}

	@SuppressLint("HandlerLeak")
	private Handler mHandler = new Handler() {

		@Override
		public void handleMessage(Message msg) {

			int id = msg.what;
			String content = (String) msg.obj;
			Log.d(TAG, "Message: id@" + id + ", content@" + content);

			boolean fIs555 = (((id >> 30) & 1) > 0);
			int f555Value = ((id << 2) >> 2);
			boolean fVideo = (((id >> 24) & 1) > 0);
			boolean fOpen = (((id >> 16) & 1) > 0);
			boolean fSuccess = (((id >> 8) & 1) > 0);
			boolean fFirstFrame = (((id >> 31) & 1) > 0);
			int fWidth = (id << 2) >> 17;
			int fHeight = (id << 17) >> 17;

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
					setSurfaceSize(fWidth, fHeight);
				}
			} else /* Live555 Event */
			{
				Log.d(TAG, "OnLive555 Event, Code@ " + f555Value);
			}
		}

		private void setSurfaceSize(int fWidth, int fHeight) {
			ViewGroup.LayoutParams param = (ViewGroup.LayoutParams) mGLSurface
					.getLayoutParams();
			float scaleX = 1.0f;
			float scaleY = (float)((double)fHeight / fWidth)
					/ ((float) param.height / param.width);
			Log.d(TAG, "scale: XYZ: " + scaleX + " / " + scaleY + " / " + 1.0f);

			mRenderer.setScale(scaleX, scaleY, 1.0f);
		}
	};

	@Override
	public void onMessage(int id, String content) {
		Message msg = new Message();
		msg.what = id;
		msg.obj = content;
		mHandler.sendMessage(msg);
	}

}
