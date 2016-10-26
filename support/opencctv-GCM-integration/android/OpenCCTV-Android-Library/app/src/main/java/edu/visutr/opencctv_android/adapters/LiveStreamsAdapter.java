package edu.visutr.opencctv_android.adapters;

import android.content.Context;
import android.widget.ArrayAdapter;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;

import edu.visutr.opencctv_android_library.video_streaming.OpenCCTVLiveStreaming;

public class LiveStreamsAdapter extends ArrayAdapter<String> {
    private OpenCCTVLiveStreaming openCCTVLiveStreaming;

    private ArrayList<String> cameras;
    private ArrayList<String> urls;

    public LiveStreamsAdapter(Context context, int resource, OpenCCTVLiveStreaming openCCTVLiveStreaming) {
        super(context, resource);
        this.openCCTVLiveStreaming = openCCTVLiveStreaming;
    }

    public void prepareDataSet() {
        cameras = new ArrayList<String>();
        urls = new ArrayList<String>();

        openCCTVLiveStreaming.setOnStreamFetchedListener(new OpenCCTVLiveStreaming.OnStreamFetchedListener() {

            @Override
            public void onStreamFetched(JSONArray streamInfo) {
                for (int i = 0; i < streamInfo.length(); i++) {
                    try {
                        cameras.add(streamInfo.getJSONObject(i).getString("camera_name"));
                        urls.add(streamInfo.getJSONObject(i).getString("url"));

                    } catch (JSONException e) {
                        e.printStackTrace();
                    }

                }

                refreshDataSet();
            }
        });

        openCCTVLiveStreaming.fetchAvailableStreamsForUser();
    }

    public void refreshDataSet() {
        clear();
        addAll(cameras);
        notifyDataSetChanged();
    }

    public ArrayList<String> getUrls() {
        return urls;
    }
}

