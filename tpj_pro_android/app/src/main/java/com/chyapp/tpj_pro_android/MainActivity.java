package com.chyapp.tpj_pro_android;

import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.view.View;
import android.widget.Switch;
import android.widget.CompoundButton;

import com.google.android.gms.appindexing.Action;
import com.google.android.gms.appindexing.AppIndex;
import com.google.android.gms.appindexing.Thing;
import com.google.android.gms.common.api.GoogleApiClient;

public class MainActivity extends AppCompatActivity {

    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    private GoogleApiClient client;

    // bt status
    public Button bt_connect;
    public TextView lb_status;

    // current value
    public TextView lb_hum;
    public TextView lb_temp;
    public TextView lb_brt;

    // progress bar
    public ProgressBar pgb_hum;
    public ProgressBar pgb_brt;
    public ProgressBar pgb_temp;

    // controls
    public Switch sw_cover;
    public Switch sw_fan;
    public Switch sw_heater;
    public Switch sw_wpump;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client = new GoogleApiClient.Builder(this).addApi(AppIndex.API).build();


        // init
        lb_status = (TextView) findViewById(R.id.lb_bt_status);

        lb_hum  = (TextView) findViewById(R.id.lb_Humidity);
        lb_temp = (TextView) findViewById(R.id.lb_temperaure);
        lb_brt  = (TextView) findViewById(R.id.lb_brightness);

        pgb_hum = (ProgressBar) findViewById(R.id.pbar_humidity);
        pgb_brt = (ProgressBar) findViewById(R.id.pbar_brightness);
        pgb_temp = (ProgressBar) findViewById(R.id.pbar_temperature);

        sw_cover = (Switch) findViewById(R.id.swh_cover);
        sw_cover.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
//                Log.v("Switch State=", ""+isChecked);

                //send msg
            }

        });

        sw_fan = (Switch) findViewById(R.id.swh_fan);
        sw_fan.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
//                Log.v("Switch State=", ""+isChecked);

                if(isChecked) // on
                    ;
                else          // off
                    ;
                //send msg
            }

        });

        sw_heater = (Switch) findViewById(R.id.swh_heater);
        sw_heater.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
//                Log.v("Switch State=", ""+isChecked);

                //send msg
            }

        });

        sw_wpump = (Switch) findViewById(R.id.swh_wpump);
        sw_wpump.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
//                Log.v("Switch State=", ""+isChecked);

                //send msg
            }

        });


        pgb_hum.setMax(1024);
        pgb_hum.setProgress(60);
        pgb_hum.setVisibility(View.VISIBLE);


        pgb_brt.setMax(1024);
        pgb_brt.setProgress(60);
        pgb_brt.setVisibility(View.VISIBLE);

        pgb_temp.setMax(1024);
        pgb_temp.setProgress(100);
        pgb_temp.setVisibility(View.VISIBLE);


    }


    public void onClickBtn(View view) {


//        Button button = (Button) findViewById(R.id.bt_bt_connect);


//        button.setText("Connected");

        lb_status.setText("Connected");
        lb_hum.setText("99");
        lb_temp.setText("66");
        lb_brt.setText("33");

    }

    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    public Action getIndexApiAction() {
        Thing object = new Thing.Builder()
                .setName("Main Page") // TODO: Define a title for the content shown.
                // TODO: Make sure this auto-generated URL is correct.
                .setUrl(Uri.parse("http://[ENTER-YOUR-URL-HERE]"))
                .build();
        return new Action.Builder(Action.TYPE_VIEW)
                .setObject(object)
                .setActionStatus(Action.STATUS_TYPE_COMPLETED)
                .build();
    }

    @Override
    public void onStart() {
        super.onStart();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client.connect();
        AppIndex.AppIndexApi.start(client, getIndexApiAction());
    }

    @Override
    public void onStop() {
        super.onStop();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        AppIndex.AppIndexApi.end(client, getIndexApiAction());
        client.disconnect();
    }
}
