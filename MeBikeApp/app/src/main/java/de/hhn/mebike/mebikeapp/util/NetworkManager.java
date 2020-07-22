package de.hhn.mebike.mebikeapp.util;

import android.content.res.Resources;
import android.util.Log;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

import de.hhn.mebike.mebikeapp.R;

public class NetworkManager {

    private static NetworkManager instance = new NetworkManager();

    private NetworkManager(){}

    public static NetworkManager getInstance(){
        return instance;
    }


    public void get(final String uri, final NetworkResponse response){
        new Thread(() -> {
            String urlString ="http://207.180.247.98:8080" + uri;
            HttpURLConnection urlConnection = null;
            JSONObject result = null;
            Exception exception = null;
            boolean success = false;
            try {
                URL url = new URL(urlString);
                urlConnection = connect(url, Method.GET);
                result = readResultJson(urlConnection);
                Log.d("GET RESULT:", "Received: " + result);
                success = true;
            } catch (IOException | JSONException e) {
                e.printStackTrace();
                exception = e;
            }finally {
                if(urlConnection != null)
                    urlConnection.disconnect();
                if(success)
                    response.onSuccess(result);
                else
                    response.onError(exception);
            }
        }).start();
    }

    public void post(final JSONObject body, final String uri, final NetworkResponse response) {
        new Thread(() -> {
            String urlString = "http://207.180.247.98:8080" + uri;
            HttpURLConnection urlConnection = null;
            JSONObject result = null;
            Exception exception = null;
            boolean success = false;
            try {
                URL url = new URL(urlString);
                urlConnection = connect(url, Method.POST);
                sendJsonParams(urlConnection, body);
                result = readResultJson(urlConnection);
                Log.d("POST RESULT:", "Received: " + result);
                success = true;
            } catch (IOException | JSONException e) {
                e.printStackTrace();
                exception = e;
            }finally {
                if(urlConnection != null)
                    urlConnection.disconnect();
                if(success)
                    response.onSuccess(result);
                else
                    response.onError(exception);
            }
        }).start();
    }

    private void sendJsonParams(HttpURLConnection connection, JSONObject params) throws IOException{
        if(params == null)
            return;
        DataOutputStream outStream = new DataOutputStream(new BufferedOutputStream(connection.getOutputStream()));
        outStream.writeBytes(params.toString());
        outStream.flush();
        outStream.close();
    }

    private String readResultString(HttpURLConnection connection) throws IOException {
        BufferedReader reader = new BufferedReader(new InputStreamReader(connection.getInputStream()));
        StringBuilder sb = new StringBuilder();
        String line = null;
        while((line = reader.readLine()) != null){
            sb.append(line);
        }
        return sb.toString();
    }

    private JSONObject readResultJson(HttpURLConnection connection) throws IOException, JSONException {
        return new JSONObject(readResultString(connection));
    }

    private HttpURLConnection connect(URL url, Method method) throws IOException{
        HttpURLConnection connection = (HttpURLConnection) url.openConnection();

        connection.setRequestProperty("Content-Type", "application/json");
        connection.setConnectTimeout(3000);
        connection.setReadTimeout(3000);
        switch (method) {
            case GET:
                connection.setRequestMethod("GET");
                connection.setDoOutput(false);
                break;
            case POST:
                connection.setRequestMethod("POST");
                connection.setDoOutput(true);
                break;
        }
        connection.setDoInput(true);
        connection.connect();
        return connection;
    }


    enum Method{
        POST,
        GET
    }

}
