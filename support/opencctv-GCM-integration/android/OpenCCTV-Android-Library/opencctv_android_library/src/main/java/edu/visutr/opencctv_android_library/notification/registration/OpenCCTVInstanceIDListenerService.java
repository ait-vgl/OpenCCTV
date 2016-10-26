package edu.visutr.opencctv_android_library.notification.registration;

import android.content.Intent;

import com.google.android.gms.iid.InstanceIDListenerService;

public class OpenCCTVInstanceIDListenerService extends InstanceIDListenerService {

    @Override
    public void onTokenRefresh() {
        Intent gcmUpdateService = new Intent(this, OpenCCTVGcmUpdateService.class);
        startService(gcmUpdateService);
    }
}
