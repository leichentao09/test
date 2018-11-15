package com.spring.socketprocess;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.Process;
import android.util.Log;

import java.util.Timer;
import java.util.TimerTask;

public class ProcessService extends Service {
    private String TAG = ProcessService.class.getSimpleName();
    private int i = 0;

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Watcher watcher = new Watcher();
        watcher.createWatcher(String.valueOf(Process.myUid()));
        watcher.connectMonitor();
        Timer timer = new Timer();
        //定时器
        timer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                Log.i(TAG, "服务开启中" + i);
                i++;
            }
        }, 0, 1000 * 3);
    }
}
