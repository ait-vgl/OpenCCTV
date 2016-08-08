package edu.visutr.opencctv_android.activityties;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;

import edu.visutr.opencctv_android.R;
import edu.visutr.opencctv_android.adapters.LiveStreamsAdapter;
import edu.visutr.opencctv_android_library.authentication.OpenCCTVAuthentication;
import edu.visutr.opencctv_android_library.video_streaming.OpenCCTVLiveStreaming;

public class MainActivity extends AppCompatActivity {
    private OpenCCTVLiveStreaming openCCTVLiveStreaming;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initUiComponents();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.option, menu);
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.menu_signout:
                OpenCCTVAuthentication openCCTVAuthentication = new OpenCCTVAuthentication(this);
                openCCTVAuthentication.setOnSignedOutListener(new OpenCCTVAuthentication.OnSignedOutListener() {

                    @Override
                    public void onSignedOut() {
                        SharedPreferences sharedPreferences = getSharedPreferences("application_info", Context.MODE_PRIVATE);
                        SharedPreferences.Editor editor = sharedPreferences.edit();
                        editor.putBoolean("isLoggedIn", false);
                        editor.apply();
                        Intent intent = new Intent(MainActivity.this, LogInActivity.class);
                        startActivity(intent);
                        finish();
                    }
                });

                openCCTVAuthentication.signOut();
                break;

            case R.id.menu_view_event:
                Intent intent = new Intent(this, EventViewActivity.class);
                startActivity(intent);

            default:
                break;
        }
        return super.onOptionsItemSelected(item);
    }

    private void initUiComponents() {
        // Init ListView
        ListView liveStreamsList = (ListView) findViewById(R.id.mainAct_liveStreamsList);

        openCCTVLiveStreaming = new OpenCCTVLiveStreaming(this);

        final LiveStreamsAdapter adapter = new LiveStreamsAdapter(this, android.R.layout.simple_list_item_1, openCCTVLiveStreaming);
        adapter.prepareDataSet();

        liveStreamsList.setAdapter(adapter);
        liveStreamsList.setOnItemClickListener(new AdapterView.OnItemClickListener() {

            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                String url = adapter.getUrls().get(position);
                openCCTVLiveStreaming.startStreaming(url);
            }
        });
    }
}
