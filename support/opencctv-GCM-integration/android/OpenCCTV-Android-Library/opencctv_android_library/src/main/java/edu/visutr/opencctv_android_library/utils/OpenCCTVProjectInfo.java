package edu.visutr.opencctv_android_library.utils;

import android.content.Context;
import android.content.SharedPreferences;

public class OpenCCTVProjectInfo {
    private static OpenCCTVProjectInfo instance;

    private SharedPreferences projectInfo;
    private SharedPreferences.Editor editor;

    private OpenCCTVProjectInfo(Context context) {
        projectInfo = context.getSharedPreferences("project_info", Context.MODE_PRIVATE);
        editor = projectInfo.edit();
    }

    public static OpenCCTVProjectInfo getInstance(Context context) {
        if (instance == null) {
            instance = new OpenCCTVProjectInfo(context);
        }
        return instance;
    }

    public void setGoogleProjectNumber(String googleProjectNumber) {
        editor.putString("google_project_number", googleProjectNumber);
    }

    public String getGoogleProjectNumber() {
        return projectInfo.getString("google_project_number", "");
    }

    public void setServerAddress(String serverAddress) {
        editor.putString("server_address", serverAddress);
    }

    public String getServerAddress() {
        return projectInfo.getString("server_address", "");
    }

    public void setPortNumber(int portNumber) {
        editor.putInt("port_number", portNumber);
    }

    public int getPortNumber() {
        return projectInfo.getInt("port_number", 3000);
    }

    public void apply() {
        editor.apply();
    }
}
