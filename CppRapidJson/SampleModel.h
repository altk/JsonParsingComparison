#pragma once
#include <cinttypes>
#include <string>
#include <vector>
#include <ctime>

namespace std
{
    enum class Gender
    {
        Male,
        Female
    };

    enum class Color : std::uint32_t
    {
        Green = 0xFF00FF00,
        Red = 0xFFFF0000,
        Blue = 0xFF0000FF,
    };

    enum class Fruit
    {
        Apple,
        Strawberry,
    };
}

namespace CppRapidJson
{
    enum class Tag
    {

    };

    class SampleModel final
    {
    private:
        std::string _id;
        std::uint16_t _index;
        std::string _guid;
        std::double_t _balance;
        std::string _picture;
        std::uint8_t _age;
        std::Color _eyeColor;
        std::string _name;
        std::Gender _gender;
        std::string _company;
        std::string _email;
        std::string _phone;
        std::string _address;
        std::string _about;
        std::time_t _registered;
        std::double_t _latitude;
        std::double_t _longitude;
        std::vector<Tag> _tags;
        std::string _greeting;
        std::Fruit _favoriteFruit;
        bool _isActive;
    };
}
