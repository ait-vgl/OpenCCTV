package edu.visutr.opencctv_android.activityties;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.IBinder;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.AppCompatEditText;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import edu.visutr.opencctv_android.R;
import edu.visutr.opencctv_android.services.MissedNotificationFetcherService;
import edu.visutr.opencctv_android_library.authentication.OpenCCTVAuthentication;
import edu.visutr.opencctv_android_library.utils.OpenCCTVProjectInfo;

public class LogInActivity extends AppCompatActivity {
    boolean serviceRegistered = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        SharedPreferences sharedPreferences = getSharedPreferences("application_info", Context.MODE_PRIVATE);
        final SharedPreferences.Editor editor = sharedPreferences.edit();

        final Intent intent = new Intent(LogInActivity.this, MainActivity.class);

        boolean isLoggedIn = sharedPreferences.getBoolean("isLoggedIn", false);
        if (isLoggedIn) {
            startActivity(intent);
            finish();
        }

        final AppCompatEditText emailInput = (AppCompatEditText) findViewById(R.id.login_emailInput);
        final AppCompatEditText passwordInput = (AppCompatEditText) findViewById(R.id.login_passwordInput);
        Button loginButton = (Button) findViewById(R.id.login_loginButton);

        OpenCCTVProjectInfo openCCTVProjectInfo = OpenCCTVProjectInfo.getInstance(this);
        openCCTVProjectInfo.setServerAddress("10.0.2.2");
        openCCTVProjectInfo.setPortNumber(3000);
        openCCTVProjectInfo.setGoogleProjectNumber(getString(R.string.google_project_number));
        openCCTVProjectInfo.apply();

        final Intent missedNotificationFetcherService =
                new Intent(this, MissedNotificationFetcherService.class);

        final OpenCCTVAuthentication openCCTVAuthentication = new OpenCCTVAuthentication(this);

        openCCTVAuthentication.setOnSignedInListener(new OpenCCTVAuthentication.OnSignedInListener() {

            @Override
            public void onSignedIn() {
                Toast.makeText(LogInActivity.this, "Signed In", Toast.LENGTH_SHORT).show();
                editor.putBoolean("isLoggedIn", true);
                editor.apply();
                startActivity(intent);
                bindService(missedNotificationFetcherService, serviceConnection, BIND_AUTO_CREATE);
                serviceRegistered = true;
                finish();
            }
        });

        loginButton.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                String email = emailInput.getText().toString();
                String password = passwordInput.getText().toString();
                openCCTVAuthentication.signIn(email, password);
            }
        });
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onStop() {
        super.onStop();
    }

    @Override
    protected void onDestroy() {
        if (serviceRegistered)
            unbindService(serviceConnection);
        super.onDestroy();
    }

    private ServiceConnection serviceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
        }
    };
}