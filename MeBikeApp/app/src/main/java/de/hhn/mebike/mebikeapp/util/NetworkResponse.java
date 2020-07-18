package de.hhn.mebike.mebikeapp.util;

import org.json.JSONObject;

public interface NetworkResponse {

    void onSuccess(JSONObject result);
    void onError(Exception e);

}
