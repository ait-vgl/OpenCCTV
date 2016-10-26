package edu.visutr.opencctv_android_library.notification;

import android.content.Context;
import android.os.AsyncTask;
import android.support.annotation.MainThread;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

import edu.visutr.opencctv_android_library.R;
import edu.visutr.opencctv_android_library.authentication.OpenCCTVCookie;
import edu.visutr.opencctv_android_library.db.OpenCCTVEventsDb;
import edu.visutr.opencctv_android_library.utils.OpenCCTVProjectInfo;

public class OpenCCTVUnnotifiedMessageFetcher {
    private final String TAG = "UnnotifiedMsgFetcher";

    private Context applicationContext;

    private String googleProjectNumber;

    private OnUnnotifiedMessagesFetched onUnnotifiedMessagesFetched;

    private enum Status {
        SUCCESS,
        UNAUTHORIZED,
        NETWORK_ERROR,
        SERVER_ERROR,
        UNKNOWN_ERROR
    }

    public OpenCCTVUnnotifiedMessageFetcher(Context applicationContext) {
        this.applicationContext = applicationContext;
    }

    @MainThread
    public void fetch() {
        googleProjectNumber = OpenCCTVProjectInfo.getInstance(applicationContext).getGoogleProjectNumber();

        UnnotifiedMessageFetchTask fetchTask = new UnnotifiedMessageFetchTask();
        fetchTask.execute();

        UnnotifiedMessageDeleteTask deleteTask = new UnnotifiedMessageDeleteTask();
        deleteTask.execute();
    }

    public void setOnUnnotifiedMessagesFetched(OnUnnotifiedMessagesFetched onUnnotifiedMessagesFetched) {
        this.onUnnotifiedMessagesFetched = onUnnotifiedMessagesFetched;
    }

    public String getString(int resId) {
        return applicationContext.getString(resId);
    }

    private class UnnotifiedMessageFetchTask extends AsyncTask<Void, Void, Status> {
        JSONArray unnotifiedMessages;

        @Override
        protected OpenCCTVUnnotifiedMessageFetcher.Status doInBackground(Void... params) {
            String cookie = OpenCCTVCookie.getInstance(applicationContext).load();

            try {
                HttpURLConnection conn = (HttpURLConnection) generateURL().openConnection();
                conn.setRequestMethod("GET");
                conn.setRequestProperty("Cookie", cookie);
                conn.setDoInput(true);

                int responseCode = conn.getResponseCode();

                if (responseCode == 200) {
                    BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));
                    StringBuffer buffer = new StringBuffer();
                    String line;

                    while ((line = reader.readLine()) != null)
                        buffer.append(line);

                    String response = buffer.toString();

                    if (response.contains("Forgot your password?")) {
                        return OpenCCTVUnnotifiedMessageFetcher.Status.UNAUTHORIZED;

                    } else {
                        unnotifiedMessages = new JSONArray(response);
                        return OpenCCTVUnnotifiedMessageFetcher.Status.SUCCESS;
                    }
                }


            } catch (IOException e) {
                Log.e(TAG, e.getMessage());
                return OpenCCTVUnnotifiedMessageFetcher.Status.NETWORK_ERROR;

            } catch (JSONException e) {
                Log.e(TAG, e.getMessage());
            }
            return OpenCCTVUnnotifiedMessageFetcher.Status.UNKNOWN_ERROR;
        }

        @Override
        protected void onPostExecute(OpenCCTVUnnotifiedMessageFetcher.Status status) {
            switch (status) {
                case SUCCESS:
                    String[] messages = new String[unnotifiedMessages.length()];
                    OpenCCTVEventsDb eventsDb = new OpenCCTVEventsDb(applicationContext);
                    String notificationMessage = "";
                    for (int i = 0; i < unnotifiedMessages.length(); i++) {
                        try {
                            JSONObject unnotifiedMessage = unnotifiedMessages.getJSONObject(i);

                            notificationMessage = unnotifiedMessage.getString("notification_message");
                            int analyticInstanceId = unnotifiedMessage.getInt("analytic_instance_id");
                            String timestamp = unnotifiedMessage.getString("timestamp");
                            eventsDb.insertEvent(analyticInstanceId, notificationMessage, timestamp);

                            messages[i] = notificationMessage;
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                    }

                    if (onUnnotifiedMessagesFetched != null)
                        onUnnotifiedMessagesFetched.onUnnotifiedMessagesFetched(messages);
                    break;

                default:
                    break;
            }

            super.onPostExecute(status);
        }

        private URL generateURL() throws MalformedURLException {
            String url = getString(R.string.url_opencctv_unnotified_messages);
            url = url.replace("[address]", OpenCCTVProjectInfo.getInstance(applicationContext).getServerAddress());
            url = url.replace("[port]", String.valueOf(OpenCCTVProjectInfo.getInstance(applicationContext).getPortNumber()));
            url = url.replace(":google_project_number", googleProjectNumber);
            return new URL(url);
        }

    }

    private class UnnotifiedMessageDeleteTask extends AsyncTask<Void, Void, Status> {

        @Override
        protected OpenCCTVUnnotifiedMessageFetcher.Status doInBackground(Void... params) {
            String cookie = OpenCCTVCookie.getInstance(applicationContext).load();
            try {
                HttpURLConnection conn = (HttpURLConnection) generateURL().openConnection();
                conn.setRequestMethod("DELETE");
                conn.setRequestProperty("Cookie", cookie);
                conn.setDoOutput(true);
                conn.setDoInput(true);

                int responseCode = conn.getResponseCode();

                if (responseCode == 200) {
                    return OpenCCTVUnnotifiedMessageFetcher.Status.SUCCESS;

                } else if (responseCode == 500) {
                    return OpenCCTVUnnotifiedMessageFetcher.Status.SERVER_ERROR;
                }

            } catch (IOException e) {
                e.printStackTrace();
                return OpenCCTVUnnotifiedMessageFetcher.Status.NETWORK_ERROR;
            }
            return null;
        }

        private URL generateURL() throws MalformedURLException {
            String url = getString(R.string.url_opencctv_unnotified_messages_destroy);
            url = url.replace("[address]", OpenCCTVProjectInfo.getInstance(applicationContext).getServerAddress());
            url = url.replace("[port]", String.valueOf(OpenCCTVProjectInfo.getInstance(applicationContext).getPortNumber()));
            url = url.replace(":google_project_number", googleProjectNumber);
            return new URL(url);
        }
    }

    public static abstract class OnUnnotifiedMessagesFetched {

        public OnUnnotifiedMessagesFetched() {
        }

        public abstract void onUnnotifiedMessagesFetched(String[] messages);
    }
}
