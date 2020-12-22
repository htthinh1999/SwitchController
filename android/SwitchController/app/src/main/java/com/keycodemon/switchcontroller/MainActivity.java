package com.keycodemon.switchcontroller;

import androidx.appcompat.app.AppCompatActivity;

import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Debug;
import android.os.Handler;
import android.util.Base64;
import android.util.Log;
import android.view.View;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.Switch;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.VolleyLog;
import com.android.volley.toolbox.HttpHeaderParser;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    Switch[] sw = new Switch[4];
    int[] swId = new int[]{ R.id.sw1, R.id.sw2, R.id.sw3, R.id.sw4 };
    boolean[] swData = new boolean[4];

    final String mainServer = "http://switchcontrol.xyz";
    final String mainURI = "/index.php";
    final String dataURI = "/data.json";
    static boolean sendingData = false;

    int delayGetData = 1000;
    RequestQueue getQueue, sendQueue;
    Thread alwaysGetDataThread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        init();
    }

    void init(){
        for(int i=0; i<swId.length; i++){
            sw[i] = findViewById(swId[i]);
            sw[i].setOnClickListener(this);
        }

        getQueue = Volley.newRequestQueue(this);
        sendQueue = Volley.newRequestQueue(this);

        alwaysGetDataWhileNotSendData();
    }

    void alwaysGetDataWhileNotSendData(){
        alwaysGetDataThread = new Thread(){
            @Override
            public void run() {
                try {
                    getDataFromWebsite();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        };
        alwaysGetDataThread.start();
    }

    void getDataFromWebsite() throws InterruptedException {

        do{

            String URL = mainServer + dataURI;
            JsonArrayRequest jsonArrayRequest = new JsonArrayRequest(Request.Method.GET, URL, null, new Response.Listener<JSONArray>() {
                @Override
                public void onResponse(JSONArray response) {
                    for (int i = 0; i < 4; i++) {
                        try {
                            JSONObject jsonObject = response.getJSONObject(i);
                            swData[i] = jsonObject.getString("value").contains("on");
                            sw[i].setChecked(swData[i]);
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }

                    }
                }
            }, new Response.ErrorListener() {
                @Override
                public void onErrorResponse(VolleyError error) {
                    Toast.makeText(getApplicationContext(), "Cannot get data from website!", Toast.LENGTH_LONG).show();
                    Log.d("Error get request", error.toString());
                }
            });

            getQueue.add(jsonArrayRequest);

            Thread.sleep(delayGetData);

        }while(!sendingData);

    }

    void sendDataToWebsite(){
        try {
            String URL = mainServer + mainURI;
            JSONArray jsonArray = new JSONArray();
            for(int i=0; i<4; i++){
                JSONObject jsonObject = new JSONObject();
                jsonObject.put("name", "sw"+(i+1));
                jsonObject.put("value", "sw"+(i+1)+((swData[i])?"on":"off"));

                jsonArray.put(jsonObject);
            }

            final String requestBody = jsonArray.toString();

            StringRequest stringRequest = new StringRequest(Request.Method.POST, URL, new Response.Listener<String>() {
                @Override
                public void onResponse(String response) {
                    Log.i("VOLLEY", response);
                }
            }, new Response.ErrorListener() {
                @Override
                public void onErrorResponse(VolleyError error) {
                    Log.e("VOLLEY", error.toString());
                }
            }) {
                @Override
                public Map<String, String> getHeaders() throws AuthFailureError {
                    Map<String, String> headers = new HashMap<>();
                    // "YWRtaW46YWRtaW4xMjM=" is base 64 encode of username:password "admin:admin123"
                    headers.put("Authorization", "Basic YWRtaW46YWRtaW4xMjM=");
                    return headers;
                }

                @Override
                public String getBodyContentType() {
                    return "application/json; charset=utf-8";
                }

                @Override
                public byte[] getBody() throws AuthFailureError {
                    try {
                        return requestBody == null ? null : requestBody.getBytes("utf-8");
                    } catch (UnsupportedEncodingException uee) {
                        VolleyLog.wtf("Unsupported Encoding while trying to get the bytes of %s using %s", requestBody, "utf-8");
                        return null;
                    }
                }

                @Override
                protected Response<String> parseNetworkResponse(NetworkResponse response) {
                    String responseString = "";
                    if (response != null) {
                        responseString = String.valueOf(response.statusCode);
                    }
                    return Response.success(responseString, HttpHeaderParser.parseCacheHeaders(response));
                }
            };

            sendQueue.add(stringRequest);

        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onClick(View view) {

        sendingData = true;

        switch (view.getId()){
            case R.id.sw1:
                swData[0] = !swData[0];
                sw[0].setChecked(swData[0]);
                break;
            case R.id.sw2:
                swData[1] = !swData[1];
                sw[1].setChecked(swData[1]);
                break;
            case R.id.sw3:
                swData[2] = !swData[2];
                sw[2].setChecked(swData[2]);
                break;
            case R.id.sw4:
                swData[3] = !swData[3];
                sw[3].setChecked(swData[3]);
                break;
            default:
                break;
        }

        sendDataToWebsite();

        sendingData = false;

        alwaysGetDataWhileNotSendData();
    }
}