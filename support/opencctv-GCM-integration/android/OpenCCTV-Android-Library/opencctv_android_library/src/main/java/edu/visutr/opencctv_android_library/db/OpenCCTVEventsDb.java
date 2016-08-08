package edu.visutr.opencctv_android_library.db;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

import java.util.ArrayList;

import edu.visutr.opencctv_android_library.db.utils.OpenCCTVEventStruct;

public class OpenCCTVEventsDb {
    private static Context context;

    private static final int DB_VERSION = 1;
    private static final String DB_NAME = "opencctv_events.db";
    private static final String TABLE_NAME = "events";

    private final String COL_ID = "id";
    private final String COL_ANALYTIC_INSTANCE_ID = "analytic_instance_id";
    private final String COL_NOTIFICATION_MESSAGE = "notification_message";
    private final String COL_TIMESTAMP = "timestamp";

    public OpenCCTVEventsDb(Context context) {
        this.context = context;
    }

    public void insertEvent(int analyticInstanceId, String notificationMessage, String timestamp) {
        DBHelper dbHelper = new DBHelper();
        SQLiteDatabase db = dbHelper.getWritableDatabase();

        ContentValues contentValues = new ContentValues();
        contentValues.put(COL_ANALYTIC_INSTANCE_ID, analyticInstanceId);
        contentValues.put(COL_NOTIFICATION_MESSAGE, notificationMessage);
        contentValues.put(COL_TIMESTAMP, timestamp);

        db.insert(TABLE_NAME, null, contentValues);
        db.close();
    }

    public ArrayList<OpenCCTVEventStruct> getAllEvents() {
        DBHelper dbHelper = new DBHelper();
        SQLiteDatabase db = dbHelper.getReadableDatabase();

        String[] projection = {
                COL_ID,
                COL_ANALYTIC_INSTANCE_ID,
                COL_NOTIFICATION_MESSAGE,
                COL_TIMESTAMP
        };

        String sortOrder = COL_ID + " DESC";

        Cursor cursor = db.query(
                TABLE_NAME,
                projection,
                null,
                null,
                null,
                null,
                sortOrder
        );

        ArrayList<OpenCCTVEventStruct> results = new ArrayList<OpenCCTVEventStruct>();

        cursor.moveToFirst();
        int i = 0;
        while (cursor.moveToNext()) {
            int c_id = cursor.getColumnIndexOrThrow(COL_ID);
            int c_analyticInstanceId = cursor.getColumnIndexOrThrow(COL_ANALYTIC_INSTANCE_ID);
            int c_notificationMessage = cursor.getColumnIndexOrThrow(COL_NOTIFICATION_MESSAGE);
            int c_timestamp = cursor.getColumnIndexOrThrow(COL_TIMESTAMP);

            results.add(
                    new OpenCCTVEventStruct(
                            cursor.getInt(c_analyticInstanceId),
                            cursor.getString(c_notificationMessage),
                            cursor.getString(c_timestamp)
                    )
            );

            i++;
        }
        db.close();
        return results;
    }

    public static void deleteDatabase() {
        context.deleteDatabase(DB_NAME);
    }

    private class DBHelper extends SQLiteOpenHelper {

        public DBHelper() {
            super(context, DB_NAME, null, DB_VERSION);
        }

        @Override
        public void onCreate(SQLiteDatabase db) {
            String sql =
                    "CREATE TABLE IF NOT EXISTS " + TABLE_NAME + " "
                            + "("
                            + COL_ID + " INTEGER PRIMARY KEY" + ", "
                            + COL_ANALYTIC_INSTANCE_ID + " INTEGER" + ", "
                            + COL_NOTIFICATION_MESSAGE + " TEXT" + ", "
                            + COL_TIMESTAMP + " TEXT"
                            + ");";
            db.execSQL(sql);
        }

        @Override
        public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
            String sql =
                    "DROP TABLE " + TABLE_NAME + ";";
            db.execSQL(sql);
            onCreate(db);
        }
    }
}
