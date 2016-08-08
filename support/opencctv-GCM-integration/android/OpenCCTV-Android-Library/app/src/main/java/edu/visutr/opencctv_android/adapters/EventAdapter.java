package edu.visutr.opencctv_android.adapters;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.util.ArrayList;

import edu.visutr.opencctv_android.R;
import edu.visutr.opencctv_android_library.db.OpenCCTVEventsDb;
import edu.visutr.opencctv_android_library.db.utils.OpenCCTVEventStruct;

public class EventAdapter extends BaseAdapter {
    private Context context;

    private OpenCCTVEventsDb db;
    private ArrayList<OpenCCTVEventStruct> events;

    public EventAdapter(Context context) {
        this.context = context;
        db = new OpenCCTVEventsDb(context);
        refresh();
    }

    @Override
    public int getCount() {
        if (events != null)
            return events.size();
        else
            return 0;
    }

    @Override
    public Object getItem(int position) {
        return null;
    }

    @Override
    public long getItemId(int position) {
        return 0;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        LayoutInflater inflater = LayoutInflater.from(context);

        LinearLayout layout =
                (LinearLayout) inflater.inflate(R.layout.item_event_view, parent, false);

        TextView message = (TextView) layout.findViewById(R.id.textView_message);
        TextView analyticInstanceId = (TextView) layout.findViewById(R.id.textView_analyticiid);
        TextView timestamp = (TextView) layout.findViewById(R.id.textView_timestamp);

        OpenCCTVEventStruct event = events.get(position);

        message.setText(event.getNotificationMessage());
        analyticInstanceId.setText("Analytic Instance ID: " + String.valueOf(event.getAnalyticInstanceId()));
        timestamp.setText("Timestamp: " + event.getTimestamp());

        return layout;
    }

    public void refresh() {
        events = db.getAllEvents();
    }
}
