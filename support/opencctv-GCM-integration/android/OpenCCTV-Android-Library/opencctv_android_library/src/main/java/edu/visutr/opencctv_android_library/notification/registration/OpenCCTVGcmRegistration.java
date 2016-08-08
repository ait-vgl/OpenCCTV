package edu.visutr.opencctv_android_library.notification.registration;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.util.Log;

import com.google.android.gms.gcm.GoogleCloudMessaging;
import com.google.android.gms.iid.InstanceID;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.net.URL;

import edu.visutr.opencctv_android_library.R;
import edu.visutr.opencctv_android_library.authentication.OpenCCTVCookie;
import edu.visutr.opencctv_android_library.utils.OpenCCTVProjectInfo;

public class OpenCCTVGcmRegistration {
    private final String TAG = "OpenCCTVGcmRegistration";
    private Context context;

    private SharedPreferences sharedPreferences;
    private SharedPreferences.Editor editor;

    private String googleProjectNumber;
    private String regToken;
    private int regTokenID;
    private String address;
    private int port = -1;

    //    private String email;
    public enum Operation {
        REGISTER,
        UNREGISTER
    }

    public enum Status {
        ALREADY_REGISTERED,
        SUCCESS,
        CONFLICT,
        UNAUTHORIZED,
        NETWORK_ERROR,
        SERVER_ERROR,
        GENERIC_ERROR
    }

    public OpenCCTVGcmRegistration(Context context) {
        this.context = context;
        initGcmPreferences();
    }

    public void setServerAddress(String address) {
        this.address = address;
    }

    public void setPortNumber(int portNumber) {
        this.port = portNumber;
    }

    public void setGoogleProjectNumber(String googleProjectNumber) {
        this.googleProjectNumber = googleProjectNumber;
    }

    public void register() {
        GcmRegistrationTask task = new GcmRegistrationTask();
        task.execute(Operation.REGISTER);
    }

    public void unregister() {
        GcmRegistrationTask task = new GcmRegistrationTask();
        task.execute(Operation.UNREGISTER);
    }

    private void initGcmPreferences() {
        sharedPreferences = context.getSharedPreferences("opencctv_sharedpreferences", Context.MODE_PRIVATE);
        editor = sharedPreferences.edit();
    }

    String getRegistrationToken() {
        return sharedPreferences.getString("gcm_regToken", "");
    }

    void setRegistrationToken() {
        editor.putString("gcm_regToken", regToken);
    }

    int getRegistrationTokenID() {
        return sharedPreferences.getInt("gcm_regTokenID", -1);
    }

    void setRegistrationTokenID() {
        editor.putInt("gcm_regTokenID", regTokenID);
    }

    private boolean isRegistered() {
        return sharedPreferences.getBoolean("gcm_registered", false);
    }

    void setRegistrationState(boolean registered) {
        editor.putBoolean("gcm_registered", registered);
    }

    void cleanPreferences() {
        editor.clear();
    }

    void savePreferences() {
        editor.apply();
    }

    private void retry(long timeInSec, Operation operation) {
        AlarmManager alarmManager = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);
        Intent intent = new Intent(context, OpenCCTVGcmRegistrationAlarmReceiver.class);
        intent.putExtra("operation", operation);
        PendingIntent pendingIntent = PendingIntent.getBroadcast(context, 0, intent, 0);
        alarmManager.set(AlarmManager.ELAPSED_REALTIME_WAKEUP, timeInSec * 1000, pendingIntent);

        Log.i(TAG, "Retrying to register in " + timeInSec + " seconds");
    }


    private class GcmRegistrationTask extends AsyncTask<OpenCCTVGcmRegistration.Operation, Integer, OpenCCTVGcmRegistration.Status> {
        OpenCCTVGcmRegistration.Operation operation;

        @Override
        protected OpenCCTVGcmRegistration.Status doInBackground(OpenCCTVGcmRegistration.Operation... params) {
            Log.i(TAG, "Task started");

            operation = params[0];

            try {
                HttpURLConnection conn = null;
                int responseCode = -1;

                switch (operation) {
                    case REGISTER:
                        InstanceID instanceID = InstanceID.getInstance(context);
                        regToken = instanceID.getToken(
                                OpenCCTVProjectInfo.getInstance(context).getGoogleProjectNumber(),
                                GoogleCloudMessaging.INSTANCE_ID_SCOPE
                        );

                        if (regToken == null || regToken.equals(""))
                            return OpenCCTVGcmRegistration.Status.GENERIC_ERROR;
                        conn = (HttpURLConnection) generateURL(operation).openConnection();
                        responseCode = registerWithServer(conn);
                        break;

                    case UNREGISTER:
                        regTokenID = getRegistrationTokenID();
                        Log.d(TAG, "RegTokenID: " + String.valueOf(regTokenID));

                        if (regTokenID == -1)
                            return OpenCCTVGcmRegistration.Status.GENERIC_ERROR;
                        conn = (HttpURLConnection) generateURL(operation).openConnection();
                        responseCode = unregisterFromServer(conn);
                        break;

                    default:
                        break;
                }

                if (responseCode == 200) {
                    BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));
                    StringBuffer buffer = new StringBuffer();
                    String line;
                    String response;

                    while ((line = reader.readLine()) != null)
                        buffer.append(line);

                    response = buffer.toString();

                    if (response.contains(getString(R.string.common_forgot_password))) {
                        return OpenCCTVGcmRegistration.Status.UNAUTHORIZED;

                    } else {
                        regTokenID = Integer.parseInt(response);
                        Log.d(TAG, "RegTokenID: " + String.valueOf(regTokenID));
                        setRegistrationTokenID();
                    }

                    return OpenCCTVGcmRegistration.Status.SUCCESS;


                } else if (responseCode == 204) {
                    setRegistrationState(true);
                    setRegistrationToken();
                    return OpenCCTVGcmRegistration.Status.SUCCESS;

                } else if (responseCode == 401) {
                    return OpenCCTVGcmRegistration.Status.UNAUTHORIZED;

                } else if (responseCode == 409) {
                    return OpenCCTVGcmRegistration.Status.CONFLICT;

                } else if (responseCode == 500) {
                    return OpenCCTVGcmRegistration.Status.SERVER_ERROR;
                }

                conn.disconnect();

            } catch (IOException e) {
                Log.e(TAG, e.getMessage());
                return OpenCCTVGcmRegistration.Status.NETWORK_ERROR;
            }
            return OpenCCTVGcmRegistration.Status.GENERIC_ERROR;
        }

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            Log.i(TAG, "Starting task");

            if (googleProjectNumber == null) {
                googleProjectNumber = OpenCCTVProjectInfo.getInstance(context).getGoogleProjectNumber();
            }

            if (address == null) {
                address = OpenCCTVProjectInfo.getInstance(context).getServerAddress();
            }

            if (port == -1) {
                port = OpenCCTVProjectInfo.getInstance(context).getPortNumber();
            }
        }

        @Override
        protected void onPostExecute(OpenCCTVGcmRegistration.Status status) {

            switch (status) {
                case ALREADY_REGISTERED:
                    Log.i(TAG, "Already registered");
                    break;

                case SUCCESS:
                    Log.i(TAG, "Task completed successfully");
                    break;

                case CONFLICT:
                    Log.i(TAG, "Conflict in registration token");
                    break;

                case UNAUTHORIZED:
                    Log.i(TAG, "Unauthorized");
                    break;

                case NETWORK_ERROR:
                    Log.i(TAG, "Network error, retrying in 1 minute");
                    retry(60000, operation);
                    break;

                case GENERIC_ERROR:
                    Log.i(TAG, "Cannot complete task due to generic error");
                    break;

                default:
                    Log.i(TAG, "Unknown error");
                    break;
            }

            if (operation == OpenCCTVGcmRegistration.Operation.UNREGISTER) {
                cleanPreferences();
            }

            savePreferences();
            super.onPostExecute(status);
        }

        private int registerWithServer(HttpURLConnection conn) {
            try {
                conn.setRequestMethod("POST");
                conn.setRequestProperty("Content-Type", "application/json");
                conn.setRequestProperty("Cookie", OpenCCTVCookie.getInstance(context).load());
                conn.setDoInput(true);
                conn.setDoOutput(true);

                JSONObject googleToken = new JSONObject();
                googleToken.put("google_project_number", googleProjectNumber);
                googleToken.put("reg_token", regToken);

                JSONObject reqBody = new JSONObject();
                reqBody.put("google_token", googleToken);

                OutputStreamWriter writer = new OutputStreamWriter(conn.getOutputStream());
                writer.write(reqBody.toString());
                writer.flush();
                writer.close();

                int responseCode = conn.getResponseCode();
                return responseCode;

            } catch (ProtocolException e) {
                e.printStackTrace();
            } catch (JSONException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return -1;
        }

        private int unregisterFromServer(HttpURLConnection conn) {
            Log.d(TAG, "Unregistering from server");
            try {
                conn.setRequestMethod("DELETE");
                conn.setRequestProperty("Cookie", OpenCCTVCookie.getInstance(context).load());
                conn.setDoInput(true);

                int responseCode = conn.getResponseCode();
                return responseCode;

            } catch (IOException e) {
                e.printStackTrace();
            }

            return -1;
        }

        private URL generateURL(OpenCCTVGcmRegistration.Operation operation) throws MalformedURLException {
            String url = "";

            switch (operation) {
                case REGISTER:
                    url = getString(R.string.url_opencctv_google_tokens_register);
                    break;

                case UNREGISTER:
                    url = getString(R.string.url_opencctv_google_tokens_unregister);
                    url = url.replace(":id", String.valueOf(regTokenID));
                    Log.d(TAG, "GENERATE URL ID = " + String.valueOf(regTokenID));
                    break;

                default:
                    break;
            }

            if (!url.equals("")) {
                url = url.replace("[address]", address);
                url = url.replace("[port]", String.valueOf(port));
            }
            return new URL(url);
        }
    }

    private String getString(int resId) {
        return context.getString(resId);
    }
}
