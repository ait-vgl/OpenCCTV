package edu.visutr.opencctv_android_library.authentication;

import android.content.Context;
import android.util.Base64;
import android.util.Log;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class OpenCCTVCookie {
    private final String TAG = "OpenCCTVCookie";

    private static OpenCCTVCookie instance;
    private static Context context;

    private OpenCCTVCookie(Context context) {
        this.context = context;
    }

    public static OpenCCTVCookie getInstance(Context context) {
        if (instance == null) {
            instance = new OpenCCTVCookie(context);
        }
        return instance;
    }

    public int save(String cookie) {
        Log.d(TAG, cookie);
        File file = new File(context.getCacheDir().getAbsolutePath().toString() + "/cookie");
        try {
            String encodedCookie = new String(Base64.encode(cookie.getBytes("UTF-8"), Base64.DEFAULT));
            FileWriter writer = new FileWriter(file);
            writer.write(encodedCookie);
            writer.flush();
            writer.close();

            Log.i(TAG, "Cookie saved");
            return 0;

        } catch (IOException e) {
            Log.e(TAG, e.getMessage());
            return -1;
        }

    }

    public String load() {
        String decodedCookie = "";
        File file = new File(context.getCacheDir().getAbsolutePath().toString() + "/cookie");
        try {
            BufferedReader reader = new BufferedReader(new FileReader(file));
            StringBuffer buffer = new StringBuffer();
            String line;
            while ((line = reader.readLine()) != null)
                buffer.append(line);
            String temp = buffer.toString();
            decodedCookie = new String(Base64.decode(temp.getBytes("UTF-8"), Base64.DEFAULT));

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return decodedCookie;
    }

    public int delete() {
        File file = new File(context.getCacheDir().getAbsolutePath().toString() + "/cookie");
        if (file.delete()) {
            Log.i(TAG, "Cookie deleted");
            return 0;
        }
        return -1;
    }
}
