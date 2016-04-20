package com.example.jsonparsingcomparison;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

public final class MainActivity extends AppCompatActivity
{
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        Executor.execute();
    }
}
