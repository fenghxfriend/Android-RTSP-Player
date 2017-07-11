package com.video.draw;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.MotionEvent;

public class PlayerView extends GLSurfaceView {
	public PlayerView(Context context) {
		super(context);
		setEGLConfigChooser(new PlayerEGLConfigChooser(8, 8, 8, 8, 0, 0));
		setEGLWindowSurfaceFactory(new PlayerEGLWindowSurfaceFactory());
		setEGLContextFactory(new PlayerEGLContextFactory());
		setEGLContextClientVersion(2);
		setDebugFlags(GLSurfaceView.DEBUG_CHECK_GL_ERROR
				| GLSurfaceView.DEBUG_LOG_GL_CALLS);
	}

	public PlayerView(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub
		return super.onTouchEvent(event);

	}

}
