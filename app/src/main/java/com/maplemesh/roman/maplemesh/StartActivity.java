package com.maplemesh.roman.maplemesh;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.content.Context;

public class StartActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //setContentView(R.layout.activity_start);

        Intent intent = new Intent (StartActivity.this, LoginActivity.class);
        startActivity(intent);


    }

    @Override
    protected void onResume() {
        super.onResume();

    }
}
