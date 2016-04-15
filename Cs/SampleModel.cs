using System;
using System.Collections.Generic;

namespace Cs
{
    public enum Gender
    {
        Male,
        Female
    }

    public class SampleModel
    {
        public String About { get; }

        public String Address { get; }

        public UInt16 Age { get; }

        public Double Balance { get; }

        public String Company { get; }

        public String Email { get; }

        public Gender Gender { get; }

        public String Greeting { get; }

        public String Guid { get; }

        public String Id { get; }

        public UInt16 Index { get; }

        public Boolean IsActive { get; }

        public Double Latitude { get; }

        public Double Longitude { get; }

        public String Name { get; }

        public String Phone { get; }

        public String Picture { get; }

        public IReadOnlyList<String> Tags { get; }

        public SampleModel(String id,
                           UInt16 index,
                           String guid,
                           Double balance,
                           String picture,
                           UInt16 age,
                           String name,
                           Gender gender,
                           String company,
                           String email,
                           String phone,
                           String address,
                           String about,
                           Double latitude,
                           Double longitude,
                           IReadOnlyList<String> tags,
                           String greeting,
                           Boolean isActive)
        {
            Id = id;
            Index = index;
            Guid = guid;
            Balance = balance;
            Picture = picture;
            Age = age;
            Name = name;
            Greeting = greeting;
            Company = company;
            Email = email;
            Phone = phone;
            Address = address;
            About = about;
            Latitude = latitude;
            Longitude = longitude;
            Tags = tags;
            IsActive = isActive;
            Gender = gender;
        }
    }
}