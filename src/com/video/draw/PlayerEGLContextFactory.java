package com.video.draw;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

import android.opengl.GLSurfaceView;
import android.util.Log;

public class PlayerEGLContextFactory implements GLSurfaceView.EGLContextFactory {
	private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;

	public EGLContext createContext(EGL10 egl, EGLDisplay display,
			EGLConfig eglConfig) {
		Log.d("EGLContextFactory", "createContext");
		int[] attrib_list = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
		checkEglError("Before eglCreateContext", egl);
		EGLContext context = egl.eglCreateContext(display, eglConfig,
				EGL10.EGL_NO_CONTEXT, attrib_list);
		checkEglError("After eglCreateContext", egl);
		return context;
	}
    private static void checkEglError(String prompt, EGL10 egl) {
        int error;
        while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS) {
            Log.e("EGLContextFactory", String.format("%s: EGL error: 0x%x", prompt, error));
        }
    }
	public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) {
		Log.d("EGLContextFactory", "destroyContext");
		egl.eglDestroyContext(display, context);
	}
}
