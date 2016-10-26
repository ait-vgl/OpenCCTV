package edu.visutr.opencctv_android.services;

import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.media.RingtoneManager;
import android.net.Uri;
import android.os.IBinder;
import android.support.annotation.Nullable;
import android.support.v4.app.NotificationCompat;

import edu.visutr.opencctv_android.R;
import edu.visutr.opencctv_android.activityties.EventViewActivity;
import edu.visutr.opencctv_android_library.notification.OpenCCTVUnnotifiedMessageFetcher;

public class MissedNotificationFetcherService extends Service {

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        OpenCCTVUnnotifiedMessageFetcher unnotifiedMessageFetcher = new OpenCCTVUnnotifiedMessageFetcher(this);
        unnotifiedMessageFetcher.fetch();
        unnotifiedMessageFetcher.setOnUnnotifiedMessagesFetched(new OpenCCTVUnnotifiedMessageFetcher.OnUnnotifiedMessagesFetched() {

            @Override
            public void onUnnotifiedMessagesFetched(String[] messages) {
                int length = messages.length;
                String message = messages[length - 1];

                NotificationCompat.Builder builder =
                        new NotificationCompat.Builder(getApplicationContext());

                Uri defaultSound = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);

                Intent intent = new Intent(MissedNotificationFetcherService.this, EventViewActivity.class);
                PendingIntent pendingIntent = PendingIntent.getActivity(MissedNotificationFetcherService.this, 0, intent, 0);

                builder.setSmallIcon(R.mipmap.ic_launcher)
                        .setContentTitle(getString(R.string.app_name))
                        .setContentText(message)
                        .setStyle(new NotificationCompat.BigTextStyle().bigText(message))
                        .setSound(defaultSound)
                        .setAutoCancel(true)
                        .setNumber(length)
                        .setContentIntent(pendingIntent);

                NotificationManager notificationManager =
                        (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
                notificationManager.notify(0, builder.build());
            }
        });
        return null;
    }

}
