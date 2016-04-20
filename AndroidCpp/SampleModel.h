#pragma once
#include <vector>
#include <string>

namespace AndroidCpp
{
    enum class Gender
    {
        Male,
        Female
    };
    
    class SampleModel final
    {
    public:
        SampleModel(std::wstring id,
                    uint32_t index,
                    std::wstring guid,
                    double balance,
                    std::wstring picture,
                    uint8_t age,
                    std::wstring name,
                    Gender gender,
                    std::wstring company,
                    std::wstring email,
                    std::wstring phone,
                    std::wstring address,
                    std::wstring about,
                    double latitude,
                    double longitude,
                    std::vector<std::wstring> tags,
                    std::wstring greeting,
                    bool isActive) noexcept
            : _id(move(id)),
              _index(index),
              _guid(move(guid)),
              _balance(balance),
              _picture(move(picture)),
              _age(age),
              _name(move(name)),
              _gender(gender),
              _company(move(company)),
              _email(move(email)),
              _phone(move(phone)),
              _address(move(address)),
              _about(move(about)),
              _latitude(latitude),
              _longitude(longitude),
              _tags(move(tags)),
              _greeting(move(greeting)),
              _isActive(isActive) {}

        SampleModel(const SampleModel &other) noexcept
            : _id(other._id),
              _index(other._index),
              _guid(other._guid),
              _balance(other._balance),
              _picture(other._picture),
              _age(other._age),
              _name(other._name),
              _gender(other._gender),
              _company(other._company),
              _email(other._email),
              _phone(other._phone),
              _address(other._address),
              _about(other._about),
              _latitude(other._latitude),
              _longitude(other._longitude),
              _tags(other._tags),
              _greeting(other._greeting),
              _isActive(other._isActive) {}

        SampleModel(SampleModel &&other) noexcept
            : _id(move(other._id)),
              _index(other._index),
              _guid(move(other._guid)),
              _balance(other._balance),
              _picture(move(other._picture)),
              _age(other._age),
              _name(move(other._name)),
              _gender(other._gender),
              _company(move(other._company)),
              _email(move(other._email)),
              _phone(move(other._phone)),
              _address(move(other._address)),
              _about(move(other._about)),
              _latitude(other._latitude),
              _longitude(other._longitude),
              _tags(move(other._tags)),
              _greeting(move(other._greeting)),
              _isActive(other._isActive) {}

        SampleModel& operator=(const SampleModel &other)
        {
            if (this == &other)
            {
                return *this;
            }

            _id = other._id;
            _index = other._index;
            _guid = other._guid;
            _balance = other._balance;
            _picture = other._picture;
            _age = other._age;
            _name = other._name;
            _gender = other._gender;
            _company = other._company;
            _email = other._email;
            _phone = other._phone;
            _address = other._address;
            _about = other._about;
            _latitude = other._latitude;
            _longitude = other._longitude;
            _tags = other._tags;
            _greeting = other._greeting;
            _isActive = other._isActive;
            return *this;
        }

        SampleModel& operator=(SampleModel &&other)
        {
            if (this == &other)
            {
                return *this;
            }

            _id = move(other._id);
            _index = other._index;
            _guid = move(other._guid);
            _balance = other._balance;
            _picture = move(other._picture);
            _age = other._age;
            _name = move(other._name);
            _gender = other._gender;
            _company = move(other._company);
            _email = move(other._email);
            _phone = move(other._phone);
            _address = move(other._address);
            _about = move(other._about);
            _latitude = other._latitude;
            _longitude = other._longitude;
            _tags = move(other._tags);
            _greeting = move(other._greeting);
            _isActive = other._isActive;
            return *this;
        }

        const std::wstring& getId() const
        {
            return _id;
        }

        const uint32_t& getIndex() const
        {
            return _index;
        }

        const std::wstring& getGuid() const
        {
            return _guid;
        }

        const double& getBalance() const
        {
            return _balance;
        }

        const std::wstring& getPicture() const
        {
            return _picture;
        }

        const uint8_t& getAge() const
        {
            return _age;
        }

        const std::wstring& getName() const
        {
            return _name;
        }

        const Gender& getGender() const
        {
            return _gender;
        }

        const std::wstring& getCompany() const
        {
            return _company;
        }

        const std::wstring& getEmail() const
        {
            return _email;
        }

        const std::wstring& getPhone() const
        {
            return _phone;
        }

        const std::wstring& getAddress() const
        {
            return _address;
        }

        const std::wstring& getAbout() const
        {
            return _about;
        }

        const double& getLatitude() const
        {
            return _latitude;
        }

        const double& getLongitude() const
        {
            return _longitude;
        }

        const std::vector<std::wstring>& getTags() const
        {
            return _tags;
        }

        const std::wstring& getGreeting() const
        {
            return _greeting;
        }

        const bool& getIsActive() const
        {
            return _isActive;
        }

    private:
        std::wstring _id;
        uint32_t _index;
        std::wstring _guid;
        double _balance;
        std::wstring _picture;
        uint8_t _age;
        std::wstring _name;
        Gender _gender;
        std::wstring _company;
        std::wstring _email;
        std::wstring _phone;
        std::wstring _address;
        std::wstring _about;
        double _latitude;
        double _longitude;
        std::vector<std::wstring> _tags;
        std::wstring _greeting;
        bool _isActive;
    };
}
