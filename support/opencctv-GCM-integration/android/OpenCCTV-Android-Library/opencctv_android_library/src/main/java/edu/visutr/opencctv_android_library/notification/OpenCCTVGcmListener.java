package edu.visutr.opencctv_android_library.notification;

import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.media.RingtoneManager;
import android.os.Bundle;
import android.support.v7.app.NotificationCompat;

import com.google.android.gms.gcm.GcmListenerService;

import edu.visutr.opencctv_android_library.R;
import edu.visutr.opencctv_android_library.db.OpenCCTVEventsDb;
import edu.visutr.opencctv_android_library.notification.utils.OpenCCTVUnreadNotificationCounter;
import edu.visutr.opencctv_android_library.video_streaming.activity.ArchiveStreamingHelper;
import edu.visutr.opencctv_android_library.video_streaming.activity.LiveStreamingHelper;

public abstract class OpenCCTVGcmListener extends GcmListenerService {
    private final String TAG = "OpenCCTVGcmListener";

    private String appName;
    private int icon;

    @Override
    public void onMessageReceived(String s, Bundle data) {
        String analyticInstanceId = data.getString("analytic_instance_id");
        String notificationMessage = data.getString("notification_message");
        String timestamp = data.getString("timestamp");

        OpenCCTVEventsDb eventDb = new OpenCCTVEventsDb(this);
        eventDb.insertEvent(Integer.parseInt(analyticInstanceId), notificationMessage, timestamp);

        setApplicationInfo();
        sendNotification(notificationMessage, Integer.parseInt(analyticInstanceId));
    }

    public void setApplicationInfo() {
        this.appName = "OpenCCTV";
        this.icon = android.R.mipmap.sym_def_app_icon;
    }

    public void setApplicationInfo(String appName, int icon) {
        this.appName = appName;
        this.icon = icon;
    }

    public void sendNotification(String notificationMessage, int analyticInstanceId) {
        NotificationCompat.Builder builder = new NotificationCompat.Builder(getApplicationContext());
        builder.setSmallIcon(icon);
        builder.setContentTitle(appName);
        builder.setContentText(notificationMessage);
        builder.setStyle(new NotificationCompat.BigTextStyle().bigText(notificationMessage));
        builder.setSound(RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION));

        PendingIntent pendingIntent = createPendingIntent();
        if (pendingIntent != null) {
            builder.setContentIntent(pendingIntent);
            builder.setAutoCancel(true);
        }

        Intent liveIntent = new Intent(this, LiveStreamingHelper.class);
        liveIntent.putExtra("analytic_instance_id", analyticInstanceId);
        liveIntent.putExtra("app_name", appName);

        Intent archiveIntent = new Intent(this, ArchiveStreamingHelper.class);
        archiveIntent.putExtra("analytic_instance_id", analyticInstanceId);
        archiveIntent.putExtra("app_name", appName);

        PendingIntent live =
                PendingIntent.getActivity(this, 0, liveIntent, PendingIntent.FLAG_UPDATE_CURRENT);

        PendingIntent archive =
                PendingIntent.getActivity(this, 0, archiveIntent, PendingIntent.FLAG_UPDATE_CURRENT);

        builder.addAction(0, getString(R.string.common_view_live_streaming), live);
        builder.addAction(0, getString(R.string.common_view_recorded_video), archive);

        OpenCCTVUnreadNotificationCounter unreadCounter =
                OpenCCTVUnreadNotificationCounter.getInstance(this);

        unreadCounter.increment();
        int unreadCount = unreadCounter.count();
        if (unreadCount > 1)
            builder.setNumber(unreadCount);

        NotificationManager notificationManager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
        notificationManager.notify(appName, 4455, builder.build());
    }

    private PendingIntent createPendingIntent() {
        return onCreatePendingIntent();
    }

    public abstract PendingIntent onCreatePendingIntent();

}
