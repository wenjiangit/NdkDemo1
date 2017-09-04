package com.douliu.ndkdemo1;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;

import java.io.File;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class MainActivity extends AppCompatActivity {

    private static final String SD_CARD_PATH = Environment.getExternalStorageDirectory().getAbsolutePath();
    private static final String TAG = "MainActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void diff(View view) {
        ExecutorService service = Executors.newSingleThreadExecutor();
        service.execute(new Runnable() {
            @Override
            public void run() {
                Log.i(TAG, "diff: start");
                String path = SD_CARD_PATH + File.separatorChar + "woyaoni.mp3";
                Log.i(TAG, "path: " + path);
                String pattern_path = SD_CARD_PATH + File.separatorChar + "woyaoni_%d.mp3";
                Log.i(TAG, "pattern_path: " + pattern_path);
                FileUtils.diff(path, pattern_path, 4);
                Log.i(TAG, "diff: end");
            }
        });
    }


    public void patch(View view) {
        Log.i(TAG, "patch: start");
        String path = SD_CARD_PATH + File.separatorChar + "woyaoni_merge.mp3";
        Log.i(TAG, "path: " + path);
        String pattern_path = SD_CARD_PATH + File.separatorChar + "woyaoni_%d.mp3";
        Log.i(TAG, "pattern_path: " + pattern_path);
        FileUtils.patch(path, pattern_path, 4);
        Log.i(TAG, "patch: end");
    }
}
