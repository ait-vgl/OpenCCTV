package edu.visutr.opencctv_android_library.notification.registration;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class OpenCCTVGcmRegistrationAlarmReceiver extends BroadcastReceiver {

    @Override
    public void onReceive(Context context, Intent intent) {
        OpenCCTVGcmRegistration.Operation operation =
                (OpenCCTVGcmRegistration.Operation) intent.getSerializableExtra("operation");
        Intent gcmRegistrationService = new Intent(context, OpenCCTVGcmUpdateService.class);
        gcmRegistrationService.putExtra("operation", operation);
        context.startService(gcmRegistrationService);
    }
}
