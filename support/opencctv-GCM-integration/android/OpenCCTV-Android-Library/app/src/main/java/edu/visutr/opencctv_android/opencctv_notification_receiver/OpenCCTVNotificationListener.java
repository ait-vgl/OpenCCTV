package edu.visutr.opencctv_android.opencctv_notification_receiver;

import android.app.PendingIntent;
import android.content.Intent;
import android.os.Bundle;

import edu.visutr.opencctv_android.activityties.EventViewActivity;
import edu.visutr.opencctv_android_library.notification.OpenCCTVGcmListener;

public class OpenCCTVNotificationListener extends OpenCCTVGcmListener {

    @Override
    public void onMessageReceived(String s, Bundle data) {
        super.onMessageReceived(s, data);
    }

    @Override
    public PendingIntent onCreatePendingIntent() {
        Intent intent = new Intent(this, EventViewActivity.class);
        PendingIntent pendingIntent = PendingIntent.getActivity(this, 0, intent, 0);
        return pendingIntent;
    }
}
