package com.chyapp.tpj_pro_android;

import android.content.Intent;
import android.net.Uri;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.SeekBar;
import android.widget.TextView;
import android.view.View;
import android.widget.Switch;
import android.widget.CompoundButton;

import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;

import android.util.Log;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.widget.Toast;


import com.google.android.gms.appindexing.Action;
import com.google.android.gms.appindexing.AppIndex;
import com.google.android.gms.appindexing.Thing;
import com.google.android.gms.common.api.GoogleApiClient;

public class MainActivity extends AppCompatActivity {

    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    // Intent request codes
    private static final int REQUEST_CONNECT_DEVICE_SECURE = 1;
    private static final int REQUEST_CONNECT_DEVICE_INSECURE = 2;
    private static final int REQUEST_ENABLE_BT = 3;

    private static final String BLUETOOTH_DEVICE_ID = "98:D3:31:30:4B:E3";

    private GoogleApiClient client;

    // bt status
    public Button bt_connect;
    public TextView lb_status;

    // current value
    public TextView lb_hum;
    public TextView lb_temp;
    public TextView lb_brt;

    // controls
    public Switch sw_cover;
    public Switch sw_fan;
    public Switch sw_heater;
    public Switch sw_wpump;


    //seekbar
    public SeekBar sb_hum;
    public SeekBar sb_brt;
    public SeekBar sb_temp;

    public TextView lb_set_hum;
    public TextView lb_set_brt;
    public TextView lb_set_temp;


    private String mConnectedDeviceName = null;

    /**
     * Array adapter for the conversation thread
     */
    private ArrayAdapter<String> mConversationArrayAdapter;

    /**
     * String buffer for outgoing messages
     */
    private StringBuffer mOutStringBuffer;

    /**
     * Local Bluetooth adapter
     */
    private BluetoothAdapter mBluetoothAdapter = null;

    /**
     * Member object for the chat services
     */
    private BluetoothChatService mChatService = null;



    private final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
//            FragmentActivity activity = getActivity();
            switch (msg.what) {
                case Constants.MESSAGE_STATE_CHANGE:
                    switch (msg.arg1) {
                        case BluetoothChatService.STATE_CONNECTED:
                        case BluetoothChatService.STATE_CONNECTING:
                        case BluetoothChatService.STATE_LISTEN:
                        case BluetoothChatService.STATE_NONE:
                            break;
                    }
                    break;
                case Constants.MESSAGE_WRITE:
                    byte[] writeBuf = (byte[]) msg.obj;
                    // construct a string from the buffer
                    String writeMessage = new String(writeBuf);
//                    mConversationArrayAdapter.add("Me:  " + writeMessage);
                    break;
                case Constants.MESSAGE_READ:
                    byte[] readBuf = (byte[]) msg.obj;

                    // construct a string from the valid bytes in the buffer
                    String readMessage = new String(readBuf, 0, msg.arg1);
                    Log.d("MA", "Read MSG = " + readMessage);

                    String[] items = readMessage.split(",");

                    if(items[0].equals("HUM"))
                        lb_hum.setText(items[1]);
                    else if(items[0].equals("BRT"))
                        lb_brt.setText(items[1]);
                    else if(items[0].equals("TMP"))
                        lb_temp.setText("    "+ items[1] + "    ");
                    else if(items[0].equals("FAN")) {

                        boolean isChecked = items[1].equals("1");
                        sw_fan.setChecked(isChecked);
                    }
                    else if(items[0].equals("HEATER")) {
                        boolean isChecked = items[1].equals("1");
                        sw_heater.setChecked(isChecked);
                    }
                    else if(items[0].equals("WPUMP")) {
                        boolean isChecked = items[1].equals("1");
                        sw_wpump.setChecked(isChecked);
                    }
                    else if(items[0].equals("COVER")) {
                        boolean isChecked = items[1].equals("1");
                        sw_cover.setChecked(isChecked);
                    }

                    break;
                case Constants.MESSAGE_DEVICE_NAME:
                    break;
                case Constants.MESSAGE_TOAST:
                    break;
            }
        }
    };

    private void sendMessage(String message) {
        // Check that we're actually connected before trying anything
        if (mChatService.getState() != BluetoothChatService.STATE_CONNECTED) {
//            Toast.makeText(getActivity(), R.string.not_connected, Toast.LENGTH_SHORT).show();
            return;
        }

        // Check that there's actually something to send
        if (message.length() > 0) {
            // Get the message bytes and tell the BluetoothChatService to write
            byte[] send = message.getBytes();
            mChatService.write(send);
        }
    }


    public void init_seekbar()
    {
        sb_hum = (SeekBar) findViewById(R.id.sb_hum);
        sb_brt = (SeekBar) findViewById(R.id.sb_brt);
        sb_temp = (SeekBar) findViewById(R.id.sb_temp);

        sb_hum.setMax(100);
        sb_brt.setMax(100);
        sb_temp.setMax(40);

        sb_hum.setProgress(80);
        sb_brt.setProgress(60);
        sb_temp.setProgress(30);

        sb_hum.setOnSeekBarChangeListener(
                new SeekBar.OnSeekBarChangeListener() {

                    int progress_value = 0;
                    @Override
                    public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                        progress_value = i;

//                        Log.d("MA", "progress_value = " + progress_value);
                        String showedValue = String.valueOf(progress_value);
                        lb_set_hum.setText(showedValue + "%");
                    }

                    @Override
                    public void onStartTrackingTouch(SeekBar seekBar) {
                    }

                    @Override
                    public void onStopTrackingTouch(SeekBar seekBar) {
                        Log.d("sb_hum", String.valueOf(seekBar.getProgress()));
                        sendMessage("S");   // Soil Moisture
                        sendMessage(String.valueOf(Character.toChars(seekBar.getProgress())));   // Value
                    }
                }
        );

        sb_brt.setOnSeekBarChangeListener(
                new SeekBar.OnSeekBarChangeListener() {

                    int progress_value = 0;
                    @Override
                    public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                        progress_value = i;

//                        Log.d("MA", "progress_value = " + progress_value);

                        String showedValue = String.valueOf(progress_value);
                        lb_set_brt.setText(showedValue + "%");
                    }

                    @Override
                    public void onStartTrackingTouch(SeekBar seekBar) {
                    }

                    @Override
                    public void onStopTrackingTouch(SeekBar seekBar) {
                        sendMessage("B");   // Soil Moisture
                        sendMessage(String.valueOf(Character.toChars(seekBar.getProgress())));   // Value
                    }
                }
        );

        sb_temp.setOnSeekBarChangeListener(
                new SeekBar.OnSeekBarChangeListener() {

                    int progress_value = 0;
                    @Override
                    public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                        progress_value = i;

//                        Log.d("MA", "progress_value = " + progress_value);
                        String showedValue = String.valueOf(progress_value);
                        lb_set_temp.setText(showedValue);
                    }

                    @Override
                    public void onStartTrackingTouch(SeekBar seekBar) {
                    }

                    @Override
                    public void onStopTrackingTouch(SeekBar seekBar) {


                        int value = seekBar.getProgress();
                        if(value > 35)
                        {
                            sendMessage("f");
                            for(int i = 0; i < 256; i++)
                                ;
                            sendMessage("H");
                        }
                        else if(value < 16)
                        {
                            sendMessage("h");

                            for(int i = 0; i < 256; i++)
                                ;

                            sendMessage("F");
                        }
                        else
                        {
                            sendMessage("f");
                            for(int i = 0; i < 256; i++)
                                ;
                            sendMessage("h");
                            for(int i = 0; i < 256; i++)
                                ;
                        }

                        // sendMessage("T");
                        // sendMessage(String.valueOf(Character.toChars(seekBar.getProgress() / 3)));   // Value
                    }
                }
        );

    }

    public void init_switch()
    {
        sw_cover = (Switch) findViewById(R.id.swh_cover);

        sw_cover.setChecked(true);

        sw_cover.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked == true) {
                    Log.d("MA", "Cover Switch ON");
                    sendMessage("C");
                }
                else
                {
                    Log.d("MA", "Cover Switch OFF");
                    sendMessage("c");
                }
            }

        });

        sw_fan = (Switch) findViewById(R.id.swh_fan);
        sw_fan.setChecked(false);
        sw_fan.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked == true)
                {
                    Log.d("MA", "Fan Switch ON");
                    sendMessage("F");
                }
                else
                {
                    Log.d("MA", "Fan Switch OFF");
                    sendMessage("f");
                }
            }

        });

        sw_heater = (Switch) findViewById(R.id.swh_heater);

        sw_heater.setChecked(false);
        sw_heater.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked == true) {
                    Log.d("MA", "Heater Switch ON");
                    sendMessage("H");
                }
                else
                {
                    Log.d("MA", "Heater Switch OFF");
                    sendMessage("h");
                }
            }

        });

        sw_wpump = (Switch) findViewById(R.id.swh_wpump);

        sw_wpump.setChecked(true);
        sw_wpump.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked == true) {
                    Log.d("MA", "Water Pump Switch ON");
                    sendMessage("W");
                }
                else
                {
                    Log.d("MA", "Water Pump Switch OFF");
                    sendMessage("w");
                }
            }

        });
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client = new GoogleApiClient.Builder(this).addApi(AppIndex.API).build();


        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        // Initialize the BluetoothChatService to perform bluetooth connections
        mChatService = new BluetoothChatService(MainActivity.this, mHandler);

        // init bluetooth label
        lb_status = (TextView) findViewById(R.id.lb_bt_status);
        bt_connect = (Button) findViewById(R.id.bt_bt_connect);

        lb_hum  = (TextView) findViewById(R.id.lb_Humidity);
        lb_temp = (TextView) findViewById(R.id.lb_temperaure);
        lb_brt  = (TextView) findViewById(R.id.lb_brightness);

        lb_set_hum = (TextView) findViewById(R.id.lb_set_hum);
        lb_set_brt = (TextView) findViewById(R.id.lb_set_brt);
        lb_set_temp = (TextView) findViewById(R.id.lb_set_temp);

        lb_set_hum.setText("80%");
        lb_set_temp.setText("30");
        lb_set_brt.setText("60%");

        init_seekbar();

        init_switch();


    }

    public void onClickBtn(View view) {

        lb_status.setText("Connected");
        lb_hum.setText("99");
        lb_temp.setText("66");
        lb_brt.setText("33");

        lb_set_hum.setText("33");
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

    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.bluetooth_chat, menu);

        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.secure_connect_scan: {
                return true;
            }
            case R.id.insecure_connect_scan: {
                return true;
            }
            case R.id.discoverable: {
                BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(BLUETOOTH_DEVICE_ID);

                // TRUE for Secure Connection
                mChatService.connect(device, true);
                return true;
            }
        }
        return false;
    }
}
