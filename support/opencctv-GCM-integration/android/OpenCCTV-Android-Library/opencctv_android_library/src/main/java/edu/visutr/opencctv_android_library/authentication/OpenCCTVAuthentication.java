package edu.visutr.opencctv_android_library.authentication;

import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.os.AsyncTask;
import android.util.Log;

import org.apache.http.Header;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.protocol.BasicHttpContext;
import org.apache.http.protocol.HttpContext;

import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

import edu.visutr.opencctv_android_library.R;
import edu.visutr.opencctv_android_library.db.OpenCCTVEventsDb;
import edu.visutr.opencctv_android_library.notification.registration.OpenCCTVGcmRegistration;
import edu.visutr.opencctv_android_library.utils.OpenCCTVProjectInfo;
import edu.visutr.opencctv_android_library.utils.OpenCCTVSharedPreferences;

public class OpenCCTVAuthentication {
    private final String TAG = "OpenCCTVAuth";

    private Context context;

    private OnSignedInListener onSignedInListener;
    private OnSignedOutListener onSignedOutListener;

    private enum Operation {
        SIGN_IN,
        SIGN_UP,
        SIGN_OUT
    }

    public enum Status {
        SUCCESS,
        CREDENTIAL_ERROR,
        CONNECTION_ERROR,
        SERVER_ERROR,
        GENERIC_ERROR
    }

    private String address = null;
    private int port = -1;

    private String email;
    private String password;

    public OpenCCTVAuthentication(Context context) {
        this.context = context;
    }

    public void setServerAddress(String host, int port) {
        this.address = host;
        this.port = port;
    }

    public void signIn(String email, String password) {
        this.email = email;
        this.password = password;

        AuthenticationTask task = new AuthenticationTask();
        task.execute(Operation.SIGN_IN);

        OpenCCTVGcmRegistration gcmRegistration = new OpenCCTVGcmRegistration(context);
        gcmRegistration.register();
    }

    public void signUp(String email, String password) {
        this.email = email;
        this.password = password;

        AuthenticationTask task = new AuthenticationTask();
        task.execute(Operation.SIGN_UP);
    }

    public void signOut() {
        OpenCCTVGcmRegistration gcmRegistration = new OpenCCTVGcmRegistration(context);
        gcmRegistration.unregister();

        AuthenticationTask task = new AuthenticationTask();
        task.execute(Operation.SIGN_OUT);
    }

    public void setOnSignedInListener(OnSignedInListener onSignedInListener) {
        this.onSignedInListener = onSignedInListener;
    }

    public void setOnSignedOutListener(OnSignedOutListener onSignedOutListener) {
        this.onSignedOutListener = onSignedOutListener;
    }

    private class AuthenticationTask extends AsyncTask<Operation, Void, OpenCCTVAuthentication.Status> {
        ProgressDialog pDialog;
        Operation operation;

        @Override
        protected OpenCCTVAuthentication.Status doInBackground(Operation... params) {
            operation = params[0];

            switch (operation) {
                case SIGN_IN:

                    // SIGN IN

                    // NOTE: HttpURLConnection cannot properly receive cookie from Devise,
                    // legacy HttpClient is used instead.

                    if (address == null)
                        return null;

                    try {
                        HttpClient httpClient = new DefaultHttpClient();

                        HttpPost httpPost = new HttpPost(generateURL(Operation.SIGN_IN).toString());
                        List<NameValuePair> pairList = new ArrayList<NameValuePair>();
                        pairList.add(new BasicNameValuePair("authenticity_token", Character.toString('\u2713')));
                        pairList.add(new BasicNameValuePair("user[email]", email));
                        pairList.add(new BasicNameValuePair("user[password]", password));
                        pairList.add(new BasicNameValuePair("user[remember_me]", "1"));
                        pairList.add(new BasicNameValuePair("commit", "Sign in"));

                        UrlEncodedFormEntity entity;
                        entity = new UrlEncodedFormEntity(pairList);
                        httpPost.setEntity(entity);

                        HttpContext httpContext = new BasicHttpContext();
                        HttpResponse httpResponse = httpClient.execute(httpPost, httpContext);
                        InputStream inputStream = httpResponse.getEntity().getContent();
                        Scanner in = new Scanner(inputStream);
                        in.useDelimiter("\\A");

                        StringBuffer buffer = new StringBuffer();

                        while (in.hasNext())
                            buffer.append(in.next());
                        inputStream.close();

                        String response = buffer.toString();

                        if (response.contains("Invalid email or password")) {
                            Log.i(TAG, "Invalid credentials");
                            return OpenCCTVAuthentication.Status.CREDENTIAL_ERROR;
                        } else {
                            StringBuffer cookieBuffer = new StringBuffer();

                            for (Header h : httpResponse.getAllHeaders())
                                if (h.getName().equals("Set-Cookie"))
                                    cookieBuffer.append(h.getValue());

                            if (cookieBuffer.toString().equals(""))
                                return OpenCCTVAuthentication.Status.GENERIC_ERROR;

                            String cookie = cookieBuffer.toString();
                            OpenCCTVCookie.getInstance(context).save(cookie);
                            Log.i(TAG, "Logged in successfully");
                            return OpenCCTVAuthentication.Status.SUCCESS;
                        }

                    } catch (MalformedURLException e) {
                        e.printStackTrace();
                    } catch (UnsupportedEncodingException e) {
                        e.printStackTrace();
                    } catch (ClientProtocolException e) {
                        e.printStackTrace();
                    } catch (IOException e) {
                        Log.e(TAG, e.getMessage());
                        return OpenCCTVAuthentication.Status.CONNECTION_ERROR;
                    }

                    return OpenCCTVAuthentication.Status.GENERIC_ERROR;

                case SIGN_OUT:
                    try {
                        HttpURLConnection conn = (HttpURLConnection) generateURL(Operation.SIGN_OUT).openConnection();
                        conn.setRequestMethod("DELETE");
                        conn.setRequestProperty("Cookie", OpenCCTVCookie.getInstance(context).load());

                        int responseCode = conn.getResponseCode();

                        if (responseCode == 200) {
                            Log.i(TAG, "Logged out successfully");
                        }

                        OpenCCTVCookie.getInstance(context).delete();
                        conn.disconnect();
                        return OpenCCTVAuthentication.Status.SUCCESS;

                    } catch (MalformedURLException e) {
                        e.printStackTrace();
                    } catch (IOException e) {
                        Log.e(TAG, e.getMessage());
                        return OpenCCTVAuthentication.Status.CONNECTION_ERROR;
                    }

                    return OpenCCTVAuthentication.Status.GENERIC_ERROR;

                case SIGN_UP:
                    // TODO: Sign up (to be implemented)
                    break;

                default:
                    break;
            }

            return OpenCCTVAuthentication.Status.GENERIC_ERROR;
        }

        @Override
        protected void onPreExecute() {
            super.onPreExecute();

            if (address == null) {
                OpenCCTVProjectInfo openCCTVProjectInfo = OpenCCTVProjectInfo.getInstance(context);
                address = openCCTVProjectInfo.getServerAddress();
            }

            if (port == -1) {
                OpenCCTVProjectInfo openCCTVProjectInfo = OpenCCTVProjectInfo.getInstance(context);
                port = openCCTVProjectInfo.getPortNumber();
            }

            pDialog = new ProgressDialog(context);
            pDialog.setMessage(getString(R.string.common_please_wait));
            pDialog.show();
        }

        @Override
        protected void onPostExecute(OpenCCTVAuthentication.Status status) {
            if (pDialog.isShowing()) {
                pDialog.dismiss();
            }

            if (status != OpenCCTVAuthentication.Status.SUCCESS) {

                AlertDialog.Builder builder = new AlertDialog.Builder(context);
                String dialogMessage = "Unknown error";

                switch (status) {
                    case CONNECTION_ERROR:
                        dialogMessage = getString(R.string.common_connection_error_msg);
                        break;
                    case CREDENTIAL_ERROR:
                        dialogMessage = getString(R.string.common_check_login_credentials);
                        break;
                    default:
                        break;
                }

                builder.setTitle(getString(R.string.common_error));
                builder.setMessage(dialogMessage);
                builder.setPositiveButton(R.string.ok, null);

                AlertDialog dialog = builder.create();
                dialog.show();

            } else {
                if (operation == Operation.SIGN_IN) {
                    if (onSignedInListener != null)
                        onSignedInListener.onSignedIn();

                } else if (operation == Operation.SIGN_OUT) {
                    if (onSignedOutListener != null)
                        onSignedOutListener.onSignedOut();
                    OpenCCTVEventsDb eventsDb = new OpenCCTVEventsDb(context);
                    eventsDb.deleteDatabase();

                    OpenCCTVSharedPreferences.getInstance(context).clear();
                }
            }

            super.onPostExecute(status);
        }

        private URL generateURL(OpenCCTVAuthentication.Operation operation) throws MalformedURLException {
            String url = "";
            if (operation == Operation.SIGN_IN)
                url = getString(R.string.url_opencctv_auth_sign_in);
            else if (operation == Operation.SIGN_OUT)
                url = getString(R.string.url_opencctv_auth_sign_out);

            url = url.replace("[address]", address);
            url = url.replace("[port]", String.valueOf(port));
            return new URL(url);
        }
    }

    public static abstract class OnSignedInListener {

        public OnSignedInListener() {
        }

        public abstract void onSignedIn();
    }

    public static abstract class OnSignedOutListener {

        public OnSignedOutListener() {
        }

        public abstract void onSignedOut();
    }

    // Wrapper method for context.getString(int resId)
    private String getString(int resId) {
        return context.getString(resId);
    }
}
