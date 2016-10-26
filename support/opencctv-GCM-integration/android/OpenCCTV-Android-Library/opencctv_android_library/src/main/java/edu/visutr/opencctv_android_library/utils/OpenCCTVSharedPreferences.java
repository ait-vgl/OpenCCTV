package edu.visutr.opencctv_android_library.utils;

import android.content.Context;
import android.content.SharedPreferences;

public class OpenCCTVSharedPreferences {
    private static OpenCCTVSharedPreferences instance;
    private Context context;

    private OpenCCTVSharedPreferences(Context context) {
        this.context = context;
    }

    public static OpenCCTVSharedPreferences getInstance(Context context) {
        if (instance == null)
            instance = new OpenCCTVSharedPreferences(context);
        return instance;
    }

    public boolean clear() {
        SharedPreferences sharedPreferences = (SharedPreferences)
                context.getSharedPreferences("opencctv_sharedpreferences", Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.clear();
        return editor.commit();
    }
}
