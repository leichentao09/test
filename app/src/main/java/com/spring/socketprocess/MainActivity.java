package com.spring.socketprocess;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    private Button btnKillProcess;
    private String TAG = MainActivity.class.getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Intent intent = new Intent(this, ProcessService.class);
        startService(intent);
        init();
    }

    private void init() {
        btnKillProcess = (Button) findViewById(R.id.btn_kill_process);
        btnKillProcess.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.i(TAG, "服务sss杀死进程====success");
                android.os.Process.killProcess(android.os.Process.myPid());
            }
        });
    }
}
