package edu.visutr.opencctv_android_library.notification.utils;

import android.content.Context;
import android.content.SharedPreferences;

public class OpenCCTVUnreadNotificationCounter {
    private static OpenCCTVUnreadNotificationCounter instance;

    private Context context;
    private SharedPreferences sharedPreferences;
    private SharedPreferences.Editor editor;

    private OpenCCTVUnreadNotificationCounter(Context context) {
        sharedPreferences = context.getSharedPreferences("opencctv_sharedpreferences", Context.MODE_PRIVATE);
        editor = sharedPreferences.edit();
    }

    public static OpenCCTVUnreadNotificationCounter getInstance(Context context) {
        if (instance == null)
            instance = new OpenCCTVUnreadNotificationCounter(context);
        return instance;
    }

    public void increment() {
        int count = sharedPreferences.getInt("notification_count", 0) + 1;
        editor.putInt("notification_count", count);
        editor.apply();
    }

    public int count() {
        return sharedPreferences.getInt("notification_count", 0);
    }

    public void reset() {
        editor.putInt("notification_count", 0);
        editor.apply();
    }
}
