package com.greenbamboo.prescholleducation.activity;

import com.greenbamboo.prescholleducation.MediaFramework.GMediaPlayer;

import android.app.Activity;

import android.os.Bundle;
import android.util.Log;

public class TestGMediaPlayer extends Activity {
	// private String mRTSPAddress = "rtsp://124.232.150.3:9001/27_0";
	// private String mRTSPAddress =
	// "rtsp://stream1.gzcbn.tv:1935/app_2/ls_5.stream?domain=gztv";
	// private String mRTSPAddress =
	// "rtsp://220.250.64.228:1935/app_2/ls_5.stream?domain=gztv";
	private final String TAG = "TESTPLAYER";

	private String mRTSPAddress = "rtsp://192.168.42.1/live";
	private String mUserName = "";
	private String mPassword = "";

	/*
	 * private int mVideoWidth = 704; private int mVideoHeight = 576;
	 */

	private GMediaPlayer mMediaPlayer = null;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		Log.v(TAG, "Java >> onCreate ");
		super.onCreate(savedInstanceState);
		mMediaPlayer = new GMediaPlayer(this);
		setContentView(mMediaPlayer);
		mMediaPlayer.init();
	}

	@Override
	protected void onRestart() {
		// TODO Auto-generated method stub
		Log.v(TAG, "Java >> onRestart ");
		if (mMediaPlayer != null) {
			mMediaPlayer.Play(mRTSPAddress, mUserName, mPassword);
		}
		super.onRestart();
	}

	@Override
	protected void onStart() {
		Log.v(TAG, "Java >> onStart ");
		if (mMediaPlayer != null) {
			mMediaPlayer.Play(mRTSPAddress, mUserName, mPassword);
		}
		super.onStart();
	}

	@Override
	protected void onStop() {
		Log.v(TAG, "Java >> onStop ");
		if (mMediaPlayer != null) {
			mMediaPlayer.Stop();
		}
		super.onStop();
	}

	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		Log.v(TAG, "Java >> onPause ");
		if (mMediaPlayer != null) {
			mMediaPlayer.Pause();
		}
		super.onPause();
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		Log.v(TAG, "Java >> onResume");
		if (mMediaPlayer != null) {
			mMediaPlayer.Resume();
		}
		super.onResume();
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		Log.v(TAG, "Java >> onDestroy");
		if (mMediaPlayer != null) {
			mMediaPlayer.destroy();
		}
		super.onDestroy();
	}
}
