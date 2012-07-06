/*
 * Copyright 2010 Nuffer Brothers Software, LLC
 * All Rights Reserved
 */
package com.nufferbrotherssoftware.RedneckRacerLite;

// See http://android-developers.blogspot.com/2009/04/introducing-glsurfaceview.html

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;
///// MADD /////
import android.widget.Toast;
import android.content.ActivityNotFoundException;
import java.text.MessageFormat;
import android.os.Build;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.IOException;
import android.net.Uri;


public class StartActivity extends Activity {
    ///// MADD /////
    
    static boolean tryEmailAuthor(Context c, boolean isCrash, String body)
	{
		String addr = c.getString(R.string.author_email);
		Intent i = new Intent(Intent.ACTION_SEND);
		String modVer = "";
		try {
			Process p = Runtime.getRuntime().exec(new String[]{"getprop","ro.modversion"});
			modVer = readAllOf(p.getInputStream()).trim();
		} catch (Exception e) {}
		if (modVer.length() == 0) modVer = "original";
		// second empty address because of http://code.google.com/p/k9mail/issues/detail?id=589
		i.putExtra(Intent.EXTRA_EMAIL, new String[]{addr, ""});
		i.putExtra(Intent.EXTRA_SUBJECT, MessageFormat.format(c.getString(
                                                                          isCrash ? R.string.crash_subject : R.string.email_subject),
                                                              getVersion(c), Build.MODEL, modVer, Build.FINGERPRINT));
		i.setType("message/rfc822");
		i.putExtra(Intent.EXTRA_TEXT, body!=null ? body : "");
		try {
			c.startActivity(i);
			return true;
		} catch (ActivityNotFoundException e) {
			try {
				// Get the OS to present a nicely formatted, translated error
				c.startActivity(Intent.createChooser(i,null));
			} catch (Exception e2) {
				e2.printStackTrace();
				Toast.makeText(c, e2.toString(), Toast.LENGTH_LONG).show();
			}
			return false;
		}
	}

    static String getVersion(Context c)
	{
		try {
			return c.getPackageManager().getPackageInfo(c.getPackageName(),0).versionName;
		} catch(Exception e) { return c.getString(R.string.unknown_version); }
	}
    
    static String readAllOf(InputStream s) throws IOException
	{
		BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(s),8096);
		String line;
		StringBuilder log = new StringBuilder();
		while ((line = bufferedReader.readLine()) != null) {
			log.append(line);
			log.append("\n");
		}
		return log.toString();
	}
    
    
    
    
    private SensorManager mSensorManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        setFullscreen();
        mGLView = new StartGLSurfaceView(this);
        setContentView(mGLView);
    }

	private void setFullscreen()
	{
		requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
	}

    @Override
    protected void onPause() {
        super.onPause();
        mGLView.onPause();
        
        // We want the application to exit completely, so these two calls seem to do the trick.
        finish();
        android.os.Process.killProcess(android.os.Process.myPid());
    }

    @Override
    protected void onResume() {
        super.onResume();
        mSensorManager.registerListener((SensorEventListener)mGLView,
        	mSensorManager.getDefaultSensor(Sensor.TYPE_ORIENTATION),            
            SensorManager.SENSOR_DELAY_FASTEST);
        mGLView.onResume();
    }

    @Override
    protected void onStop() {
        mSensorManager.unregisterListener((SensorEventListener)mGLView);
        super.onStop();
    }

    private StartGLSurfaceView mGLView;

    static {
		System.loadLibrary("openal");
        System.loadLibrary("redneckracer");
    }
    
}

class StartGLSurfaceView extends GLSurfaceView implements SensorEventListener {
    public StartGLSurfaceView(Context context) {
        super(context);
        mRenderer = new StartRenderer(context);
        setRenderer(mRenderer);
        setFocusableInTouchMode(true);
    }

    public boolean onTouchEvent(final MotionEvent event) {
    	queueEvent(new Runnable(){
            public void run() {
                mRenderer.onTouchEvent(event);
            }});
    	return true;
    }
    
    public boolean onTrackballEvent(final MotionEvent event) {
    	queueEvent(new Runnable(){
            public void run() {
                mRenderer.onTrackballEvent(event);
            }});
    	return true;
    }

    StartRenderer mRenderer;

	@Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) {}

	@Override
	public void onSensorChanged(SensorEvent SensorEvent)
	{
		final int sensorType = SensorEvent.sensor.getType();
		final float[] values = SensorEvent.values;
		queueEvent(new Runnable(){
			public void run() {
				mRenderer.onSensorChanged(sensorType, values);
			}});
	}
	
	private boolean shouldIgnoreKey(int keyCode)
	{
		if( keyCode == KeyEvent.KEYCODE_HOME || 
			keyCode == KeyEvent.KEYCODE_CALL || 
			keyCode == KeyEvent.KEYCODE_ENDCALL ||
			keyCode == KeyEvent.KEYCODE_POWER ||
			keyCode == KeyEvent.KEYCODE_SYM ||
            keyCode == KeyEvent.KEYCODE_VOLUME_UP ||
            keyCode == KeyEvent.KEYCODE_VOLUME_DOWN)
		{
			return true;
		}
		return false;
	}

	@Override
	public boolean onKeyDown(final int keyCode, final KeyEvent event)
	{
		if( shouldIgnoreKey(keyCode) )
		{
			return super.onKeyDown(keyCode, event);
		}
		
		queueEvent(new Runnable(){
			public void run() {
				mRenderer.onKeyDown(keyCode, event);
			}});
		return true;
	}
	@Override
	public boolean onKeyUp(final int keyCode, final KeyEvent event)
	{
		if( shouldIgnoreKey(keyCode) )
		{
			return super.onKeyDown(keyCode, event);
		}

		queueEvent(new Runnable(){
			public void run() {
				mRenderer.onKeyUp(keyCode, event);
			}});
		return true;
	}

}

class StartRenderer implements GLSurfaceView.Renderer{

    public StartRenderer(Context context) {
        init(context);
    }

    public void showMarketFeedback()
    {
    	Intent goToMarket = new Intent(Intent.ACTION_VIEW);
    	goToMarket.setData(Uri.parse("market://details?id=com.nufferbrotherssoftware.RedneckRacerLite"));
    	//goToMarket.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK
        //    | Intent.FLAG_ACTIVITY_SINGLE_TOP
        //    | Intent.FLAG_ACTIVITY_CLEAR_TOP
        //    | Intent.FLAG_ACTIVITY_NO_HISTORY
        //    | Intent.FLAG_ACTIVITY_CLEAR_WHEN_TASK_RESET);
    	mContext.startActivity(goToMarket);
    }
    
    public void onKeyDown(int keyCode, KeyEvent event)
	{
		nativeOnKeyDown(keyCode, event.getMetaState());
	}
    public void onKeyUp(int keyCode, KeyEvent event)
	{
		nativeOnKeyUp(keyCode, event.getMetaState());
	}

	private void init(Context context) {
        mContext = context;
    }

    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        nativeInit(getAPKFilePath(), mContext.getFilesDir().getAbsolutePath());
    }

	private String getAPKFilePath()
	{
		String apkFilePath = null;
        ApplicationInfo appInfo = null;
        PackageManager packMgmr = mContext.getPackageManager();
        try {
		    appInfo = packMgmr.getApplicationInfo("com.nufferbrotherssoftware.RedneckRacerLite", 0);
		} catch (NameNotFoundException e) {
		    e.printStackTrace();
		    throw new RuntimeException("Unable to locate assets, aborting...");
		}
		apkFilePath = appInfo.sourceDir;
		return apkFilePath;
	}

    public void onSurfaceChanged(GL10 gl, int w, int h) {
        nativeResize(w, h);
    }

    public void onDrawFrame(GL10 gl) {
        nativeRender();
    }

    private Context mContext;

    public boolean onTouchEvent(final MotionEvent event) {
    	nativeOnTouchEvent(event.getDownTime(), event.getEventTime(), event.getAction(), event.getX(), event.getY(), event.getPressure(), event.getSize(), event.getMetaState(), event.getXPrecision(), event.getYPrecision(), event.getDeviceId(), event.getEdgeFlags());
        return true;
    }
    
    public boolean onTrackballEvent(final MotionEvent event) {
    	// event.getX() seems to range from -2/3 to 2/3; Adjusted here to range from -1 to 1.
    	nativeOnTrackballEvent(event.getX() * 3 / 2);
    	return true;
    }
    
	public void onSensorChanged(int sensor, float[] values)
	{
		if (sensor == Sensor.TYPE_ORIENTATION) {
			// negate Z so that tilting the phone left is negative, right positive
			float rollAngle = -values[SensorManager.DATA_Z];
			float pitchAngle = values[SensorManager.DATA_Y];
			float headingAngle = values[SensorManager.DATA_X];
			nativeOnOrientationChanged(rollAngle, pitchAngle, headingAngle);
		}
	}

    ///// MADD /////
    /** A signal handler in native code has been triggered. As our last gasp,
	 * launch the crash handler (in its own process), because when we return
	 * from this function the process will soon exit. */
	void nativeCrashed()
	{
		new RuntimeException("crashed here (native trace should follow after the Java trace)").printStackTrace();
		mContext.startActivity(new Intent(mContext, CrashHandler.class));
	}

    private native void nativeInit(String apkPath, String dataPath);
    private static native void nativeResize(int w, int h);
    private static native void nativeRender();
    private static native void nativeOnTouchEvent(long downTime, long eventTime, int action, float x, float y, float pressure, float size, int metaState, float xPrecision, float yPrecision, int deviceId, int edgeFlags);
    private static native void nativeOnTrackballEvent(float deltaX);
    private static native void nativeOnOrientationChanged(float rollAngle, float pitchAngle, float headingAngle);
    private static native void nativeOnKeyDown(int keyCode, int metastate);
    private static native void nativeOnKeyUp(int keyCode, int metastate);
}
