package com.example.jsonparsingcomparison;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;


final class ExecutorRunnable implements Runnable
{
    @Override
    public void run()
    {
        Executor.execute();
    }
}

public final class MainActivity extends AppCompatActivity
{
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        final Button button = new Button(this.getBaseContext());

        button.setText("Click me");

        button.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                final Thread thread = new Thread(new ExecutorRunnable());

                thread.start();
            }
        });

        this.setContentView(button);
    }
}
