package edu.visutr.opencctv_android_library.video_streaming.activity;

import android.app.NotificationManager;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;

import edu.visutr.opencctv_android_library.R;
import edu.visutr.opencctv_android_library.notification.utils.OpenCCTVUnreadNotificationCounter;
import edu.visutr.opencctv_android_library.video_streaming.OpenCCTVArchiveStreaming;
import edu.visutr.opencctv_android_library.video_streaming.OpenCCTVLiveStreaming;

public class ArchiveStreamingHelper extends AppCompatActivity {
    private static final String TAG = "StreamingHelper";

    private int analyticInstanceId;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.blank);

        Intent intent = getIntent();
        analyticInstanceId = intent.getIntExtra("analytic_instance_id", 0);
        String appName = intent.getStringExtra("app_name");

        NotificationManager notificationManager =
                (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
        notificationManager.cancel(appName, 4455);

        OpenCCTVUnreadNotificationCounter.getInstance(this).reset();
    }

    @Override
    protected void onStart() {
        super.onStart();

        if (analyticInstanceId == 0) {
            Log.e(TAG, "Invalid analytic instance id");
            return;
        }

        OpenCCTVArchiveStreaming streaming = new OpenCCTVArchiveStreaming(this);
        streaming.startStreaming(analyticInstanceId);
    }

    protected void onStop() {
        super.onStop();
        finish();
    }
}
