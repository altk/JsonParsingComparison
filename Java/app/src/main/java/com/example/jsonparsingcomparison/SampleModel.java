package com.example.jsonparsingcomparison;

import java.util.ArrayList;

final class SampleModel
{
    private final String about;

    private final String address;

    private final int age;

    private final double balance;

    private final String company;

    private final String email;

    private final String gender;

    private final String greeting;

    private final String guid;

    private final String id;

    private final int index;

    private final boolean isActive;

    private final double latitude;

    private final double longitude;

    private final String name;

    private final String phone;

    private final String picture;

    private final ArrayList<String> tags;

    public SampleModel(String id,
                       int index,
                       String guid,
                       Double balance,
                       String picture,
                       int age,
                       String name,
                       String gender,
                       String company,
                       String email,
                       String phone,
                       String address,
                       String about,
                       Double latitude,
                       Double longitude,
                       ArrayList<String> tags,
                       String greeting,
                       Boolean isActive)
    {

        this.about = about;
        this.address = address;
        this.age = age;
        this.balance = balance;
        this.company = company;
        this.email = email;
        this.gender = gender;
        this.greeting = greeting;
        this.guid = guid;
        this.id = id;
        this.index = index;
        this.isActive = isActive;
        this.latitude = latitude;
        this.longitude = longitude;
        this.name = name;
        this.phone = phone;
        this.picture = picture;
        this.tags = tags;
    }

    public String getAbout()
    {
        return about;
    }

    public String getAddress()
    {
        return address;
    }

    public int getAge()
    {
        return age;
    }

    public double getBalance()
    {
        return balance;
    }

    public String getCompany()
    {
        return company;
    }

    public String getEmail()
    {
        return email;
    }

    public String getGreeting()
    {
        return greeting;
    }

    public String getGuid()
    {
        return guid;
    }

    public String getId()
    {
        return id;
    }

    public int getIndex()
    {
        return index;
    }

    public boolean isActive()
    {
        return isActive;
    }

    public double getLatitude()
    {
        return latitude;
    }

    public double getLongitude()
    {
        return longitude;
    }

    public String getName()
    {
        return name;
    }

    public String getPhone()
    {
        return phone;
    }

    public String getPicture()
    {
        return picture;
    }

    public ArrayList<String> getTags()
    {
        return tags;
    }
}
