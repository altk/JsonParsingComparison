package com.example.jsonparsingcomparison;

import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;

final class Executor
{
    public static void Execute()
    {
        try
        {
            PerformComputations();
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }

    private static void PerformComputations() throws Exception
    {
        final String jsonSource = Data.getBigJson();

        final long start = System.currentTimeMillis();

        final ArrayList<SampleModel> result = parseArray(jsonSource);

        final long time = System.currentTimeMillis() - start;

        Log.d("[Executor]", String.valueOf(time));
        Log.d("[Executor]", String.valueOf(result.size()));
    }

    private static ArrayList<SampleModel> parseArray(String jsonSource) throws Exception
    {
        ArrayList<SampleModel> result = new ArrayList<>();

        final JSONArray jsonArray = new JSONArray(jsonSource);

        for (int i = 0; i < jsonArray.length(); i++)
        {
            result.add(parseModel(jsonArray.getJSONObject(i)));
        }

        return result;
    }

    private static SampleModel parseModel(String jsonSource) throws Exception
    {
        return parseModel(new JSONObject(jsonSource));
    }

    private static SampleModel parseModel(final JSONObject jsonObject) throws JSONException
    {
        String id = jsonObject.getString("_id");
        int index = jsonObject.getInt("index");
        String guid = jsonObject.getString("guid");
        double balance = jsonObject.getDouble("balance");
        String picture = jsonObject.getString("picture");
        int age = jsonObject.getInt("age");
        String name = jsonObject.getString("name");
        String gender = jsonObject.getString("gender");
        String company = jsonObject.getString("company");
        String email = jsonObject.getString("email");
        String phone = jsonObject.getString("phone");
        String address = jsonObject.getString("address");
        String about = jsonObject.getString("about");
        double latitude = jsonObject.getDouble("latitude");
        double longitude = jsonObject.getDouble("longitude");
        String greeting = jsonObject.getString("greeting");
        boolean isActive = jsonObject.getBoolean("isActive");
        JSONArray tagsArray = jsonObject.getJSONArray("tags");

        ArrayList<String> tags = new ArrayList<>();
        for (int i = 0; i < tagsArray.length(); i++)
        {
            tags.add(tagsArray.getString(i));
        }

        return new SampleModel(id,
                               index,
                               guid,
                               balance,
                               picture,
                               age,
                               name,
                               gender,
                               company,
                               email,
                               phone,
                               address,
                               about,
                               latitude,
                               longitude,
                               tags,
                               greeting,
                               isActive);
    }
}

