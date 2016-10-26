package edu.visutr.opencctv_android_library.video_streaming;

import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.AsyncTask;
import android.support.v7.app.AlertDialog;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.LinearLayout;
import android.widget.ListView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

import edu.visutr.opencctv_android_library.R;
import edu.visutr.opencctv_android_library.authentication.OpenCCTVCookie;
import edu.visutr.opencctv_android_library.utils.OpenCCTVProjectInfo;

public class OpenCCTVArchiveStreaming {
    private final String TAG = "OpenCCTVArchStreaming";

    private Context context;

    private OnStreamFetchedListener onStreamFetchedListener;

    private String email;
    private int analyticInstanceId;

    private enum Operation {
        FETCH_ALL_BY_EMAIL,
        FETCH_BY_ANALYTIC_INSTANCE_ID
    }

    private enum Status {
        SUCCESS,
        UNAUTHORIZED,
        VMS_NOT_SUPORTED,
        GENERIC_ERROR,
        NETWORK_ERROR,
        SERVER_ERROR,
        UNKNOWN_ERROR
    }

    public OpenCCTVArchiveStreaming(Context context) {
        this.context = context;
    }

    public void fetchAvailableStreamsForUser(String email) {
        this.email = email;
        FetchTask task = new FetchTask();
        task.execute(Operation.FETCH_ALL_BY_EMAIL);
    }

    public void startStreaming(int analyticInstanceId) {
        this.analyticInstanceId = analyticInstanceId;
        FetchTask task = new FetchTask();
        task.execute(Operation.FETCH_BY_ANALYTIC_INSTANCE_ID);
    }

    public void startStreaming(String streamingURL) {
        Intent intent = new Intent(Intent.ACTION_VIEW);
        intent.setData(Uri.parse(streamingURL));
        context.startActivity(intent);
    }

    public void setOnStreamFetchedListener(OnStreamFetchedListener onStreamFetchedListener) {
        this.onStreamFetchedListener = onStreamFetchedListener;
    }

    public static abstract class OnStreamFetchedListener {

        public OnStreamFetchedListener() {
        }

        public abstract void onStreamFetched(JSONArray streamInfo);
    }

    private class FetchTask extends AsyncTask<Operation, Void, Status> {
        private Operation operation;
        private JSONArray streamInfo;

        private ProgressDialog pDialog;

        @Override
        protected OpenCCTVArchiveStreaming.Status doInBackground(Operation... params) {
            operation = params[0];

            URL url = null;

            try {
                switch (operation) {
                    case FETCH_ALL_BY_EMAIL:
                        url = generateURL(getString(R.string.url_opencctv_archive_streams));
                        break;

                    case FETCH_BY_ANALYTIC_INSTANCE_ID:
                        url = generateURL(getString(R.string.url_opencctv_archive_streams_show));
                        break;

                    default:
                        break;
                }

            } catch (MalformedURLException e) {
                Log.e(TAG, e.getMessage());
                return OpenCCTVArchiveStreaming.Status.GENERIC_ERROR;
            }

            if (url == null)
                return OpenCCTVArchiveStreaming.Status.GENERIC_ERROR;

            try {
                HttpURLConnection conn = (HttpURLConnection) url.openConnection();
                conn.setRequestMethod("GET");
                conn.setRequestProperty("Cookie", OpenCCTVCookie.getInstance(context).load());
                conn.setDoInput(true);

                int responseCode = conn.getResponseCode();

                if (responseCode == 200) {
                    BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));
                    StringBuffer buffer = new StringBuffer();
                    String line;

                    while ((line = reader.readLine()) != null)
                        buffer.append(line);

                    String response = buffer.toString();

                    if (response.contains(getString(R.string.common_forgot_password)))
                        return OpenCCTVArchiveStreaming.Status.UNAUTHORIZED;

                    streamInfo = new JSONArray(response);

                    conn.disconnect();
                    return OpenCCTVArchiveStreaming.Status.SUCCESS;
                }

            } catch (IOException e) {
                e.printStackTrace();
                return OpenCCTVArchiveStreaming.Status.NETWORK_ERROR;
            } catch (JSONException e) {
                e.printStackTrace();
            }

            return OpenCCTVArchiveStreaming.Status.UNKNOWN_ERROR;
        }

        @Override
        protected void onPreExecute() {
            super.onPreExecute();

            pDialog = new ProgressDialog(context);
            pDialog.setTitle(getString(R.string.common_please_wait));
            pDialog.setMessage(getString(R.string.progress_dialog_getting_streaming_information));
            pDialog.show();
        }

        @Override
        protected void onPostExecute(OpenCCTVArchiveStreaming.Status status) {
            if (pDialog.isShowing())
                pDialog.dismiss();

            if (status == OpenCCTVArchiveStreaming.Status.SUCCESS) {

                switch (operation) {
                    case FETCH_ALL_BY_EMAIL:
                        if (onStreamFetchedListener != null) {
                            onStreamFetchedListener.onStreamFetched(streamInfo);
                        }
                        break;

                    case FETCH_BY_ANALYTIC_INSTANCE_ID:
                        if (streamInfo.length() > 1) {
                            showSelectTrackDialog();
                        } else if (streamInfo.length() == 1) {
                            try {
                                String url = streamInfo.getJSONObject(0).getString("url");
                                startStreaming(url);

                            } catch (JSONException e) {
                                e.printStackTrace();
                            }
                        } else {
                            AlertDialog.Builder builder = new AlertDialog.Builder(context);
                            builder.setTitle(getString(R.string.common_error));
                            builder.setMessage(getString(R.string.no_avaliable_stream));
                            builder.setPositiveButton(getString(android.R.string.ok), null);

                            builder.create().show();
                        }
                        break;

                    default:
                        break;
                }

            } else {
                AlertDialog.Builder builder = new AlertDialog.Builder(context);
                builder.setTitle(getString(R.string.common_error));

                switch (status) {
                    case NETWORK_ERROR:
                        builder.setMessage(getString(R.string.common_connection_error_msg));
                        break;

                    case VMS_NOT_SUPORTED:
                        builder.setMessage(getString(R.string.dialog_vms_not_supported_message));
                        break;

                    default:
                        builder.setMessage(getString(R.string.common_unknown_error));
                        break;
                }

                builder.setPositiveButton(getString(android.R.string.ok), null);

                AlertDialog dialog = builder.create();
                dialog.show();
            }

            super.onPostExecute(status);
        }

        private URL generateURL(String url) throws MalformedURLException {
            url = url.replace("[address]", OpenCCTVProjectInfo.getInstance(context).getServerAddress());
            url = url.replace("[port]", String.valueOf(OpenCCTVProjectInfo.getInstance(context).getPortNumber()));

            if (url.contains(":email")) {
                url = url.replace(":email", email);

            } else if (url.contains(":aiid")) {
                url = url.replace(":aiid", String.valueOf(analyticInstanceId));
            }
            return new URL(url);
        }

        private void showSelectTrackDialog() {
            LinearLayout layout = (LinearLayout)
                    LayoutInflater
                            .from(context)
                            .inflate(R.layout.dialog_select_camera, null);
            ListView listView = (ListView) layout.findViewById(R.id.dialog_listView);

            ArrayAdapter<String> adapter = new ArrayAdapter<String>(context, R.layout.item_select_camera);

            for (int i = 0; i < streamInfo.length(); i++) {
                try {
                    JSONObject stream = streamInfo.getJSONObject(i);
                    adapter.add(stream.getString("track_name"));

                } catch (JSONException e) {
                    e.printStackTrace();
                }
            }

            listView.setAdapter(adapter);

            AlertDialog.Builder builder = new AlertDialog.Builder(context);
            builder.setTitle(getString(R.string.dialog_select_track));
            builder.setView(layout);

            final AlertDialog dialog = builder.create();
            listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {

                @Override
                public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                    try {
                        String url = streamInfo.getJSONObject(position).getString("url");
                        startStreaming(url);
                        dialog.dismiss();

                    } catch (JSONException e) {
                        e.printStackTrace();
                    }
                }
            });

            dialog.show();
        }
    }

    private String getString(int resId) {
        return context.getString(resId);
    }
}
