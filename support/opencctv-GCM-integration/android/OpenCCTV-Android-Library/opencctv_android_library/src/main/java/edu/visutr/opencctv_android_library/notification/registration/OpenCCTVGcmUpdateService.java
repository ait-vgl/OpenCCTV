package edu.visutr.opencctv_android_library.notification.registration;

import android.app.IntentService;
import android.content.Intent;
import android.util.Log;

import com.google.android.gms.gcm.GoogleCloudMessaging;
import com.google.android.gms.iid.InstanceID;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

import edu.visutr.opencctv_android_library.R;
import edu.visutr.opencctv_android_library.authentication.OpenCCTVCookie;
import edu.visutr.opencctv_android_library.utils.OpenCCTVProjectInfo;

public class OpenCCTVGcmUpdateService extends IntentService {
    private static final String TAG = "OpenCCTVGcmUpdateSvc";

    public OpenCCTVGcmUpdateService() {
        super(TAG);
    }

    @Override
    protected void onHandleIntent(Intent intent) {
        try {
            InstanceID instanceID = InstanceID.getInstance(this);
            String token = instanceID.getToken(OpenCCTVProjectInfo.getInstance(this).getGoogleProjectNumber(),
                    GoogleCloudMessaging.INSTANCE_ID_SCOPE);

            OpenCCTVGcmRegistration gcmRegistration = new OpenCCTVGcmRegistration(this);
            String token_old = gcmRegistration.getRegistrationToken();

            HttpURLConnection conn = (HttpURLConnection) generateURL().openConnection();
            conn.setRequestMethod("PUT");
            conn.setRequestProperty("Content-Type", "application/json");
            conn.setRequestProperty("Cookie", OpenCCTVCookie.getInstance(this).load());
            conn.setDoInput(true);
            conn.setDoOutput(true);

            JSONObject google_token = new JSONObject();
            google_token.put("reg_token", token);
            google_token.put("reg_token_old", token_old);

            JSONObject requestBody = new JSONObject();
            requestBody.put("google_token", google_token);

            OutputStreamWriter writer = new OutputStreamWriter(conn.getOutputStream());
            writer.write(requestBody.toString());
            writer.flush();
            writer.close();

            int responseCode = conn.getResponseCode();
            if (responseCode == 200 || responseCode == 204) {
                Log.i(TAG, "Registration token updated");
                gcmRegistration.setRegistrationToken();
                gcmRegistration.savePreferences();
            }

            conn.disconnect();

        } catch (IOException e) {
            e.printStackTrace();
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    private URL generateURL() throws MalformedURLException {
        String url = getString(R.string.url_opencctv_google_tokens_update);
        OpenCCTVProjectInfo openCCTVProjectInfo = OpenCCTVProjectInfo.getInstance(this);
        url = url.replace("[address]", openCCTVProjectInfo.getServerAddress());
        url = url.replace("[port]", String.valueOf(openCCTVProjectInfo.getPortNumber()));
        return new URL(url);
    }
}
