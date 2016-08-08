package edu.visutr.opencctv_android.activityties;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.ListView;

import edu.visutr.opencctv_android.R;
import edu.visutr.opencctv_android.adapters.EventAdapter;

public class EventViewActivity extends AppCompatActivity {
    EventAdapter adapter;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_event_view);

        adapter = new EventAdapter(this);

        ListView listView = (ListView) findViewById(R.id.eventList);
        listView.setAdapter(adapter);
    }

    @Override
    protected void onResume() {
        super.onResume();
    }
}
