package edu.visutr.opencctv_android_library.db.utils;

public class OpenCCTVEventStruct {
    private int analyticInstanceId;
    private String notificationMessage;
    private String timestamp;

    public OpenCCTVEventStruct() {
    }

    public OpenCCTVEventStruct(int analyticInstanceId, String notificationMessage, String timestamp) {
        this.analyticInstanceId = analyticInstanceId;
        this.notificationMessage = notificationMessage;
        this.timestamp = timestamp;
    }

    public int getAnalyticInstanceId() {
        return analyticInstanceId;
    }

    public String getNotificationMessage() {
        return notificationMessage;
    }

    public String getTimestamp() {
        return timestamp;
    }
}
