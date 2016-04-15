#include "pch.h"
#include "Executor.h"
#include "Data.h"
#include "SampleModel.h"
#include "LocalSettings.h"
#include <MTL.h>
#include <windows.data.json.h>

using namespace CppRapidJson;

std::vector<SampleModel> rapidJson(wchar_t *source) noexcept
{
    using namespace std;
    using namespace rapidjson;

    GenericDocument<UTF16LE<>> document;
    document.ParseInsitu(source);
    vector<SampleModel> result;
    result.reserve(document.Size());

    auto endModels = document.End();
    for (auto beginModels = document.Begin(); beginModels != endModels; ++beginModels)
    {
        auto &beginRef = *beginModels;

        auto &id = beginRef[L"_id"];
        auto &index = beginRef[L"index"];
        auto &guid = beginRef[L"guid"];
        auto &balance = beginRef[L"balance"];
        auto &picture = beginRef[L"picture"];
        auto &age = beginRef[L"age"];
        auto &name = beginRef[L"name"];
        auto &gender = beginRef[L"gender"];
        auto &company = beginRef[L"company"];
        auto &email = beginRef[L"email"];
        auto &phone = beginRef[L"phone"];
        auto &address = beginRef[L"address"];
        auto &about = beginRef[L"about"];
        auto &latitude = beginRef[L"latitude"];
        auto &longitude = beginRef[L"longitude"];
        auto &tags = beginRef[L"tags"];
        auto &greeting = beginRef[L"greeting"];
        auto &isActive = beginRef[L"isActive"];

        vector<wstring> tagModels;
        auto endTags = tags.End();
        for (auto beginTags = tags.Begin(); beginTags != endTags; ++beginTags)
        {
            tagModels.emplace_back(beginTags->GetString(), beginTags->GetStringLength());
        }

        result.emplace_back(wstring(id.GetString(), id.GetStringLength()),
                            uint32_t(index.GetUint()),
                            wstring(guid.GetString(), guid.GetStringLength()),
                            double_t(balance.GetDouble()),
                            wstring(picture.GetString(), picture.GetStringLength()),
                            uint8_t(age.GetUint()),
                            wstring(name.GetString(), name.GetStringLength()),
                            Gender(wcscmp(L"female", gender.GetString()) == 0 ? Gender::Female : Gender::Male),
                            wstring(company.GetString(), company.GetStringLength()),
                            wstring(email.GetString(), email.GetStringLength()),
                            wstring(phone.GetString(), phone.GetStringLength()),
                            wstring(address.GetString(), address.GetStringLength()),
                            wstring(about.GetString(), about.GetStringLength()),
                            double_t(latitude.GetDouble()),
                            double_t(longitude.GetDouble()),
                            vector<wstring>(move(tagModels)),
                            wstring(greeting.GetString(), greeting.GetStringLength()),
                            bool(isActive.GetBool()));
    }

    return result;
}

std::vector<SampleModel> systemApi(wchar_t *source) noexcept
{
    using namespace std;
    using namespace MTL;
    using namespace ABI::Windows::Data::Json;
    using namespace ABI::Windows::Foundation::Collections;

    vector<SampleModel> result;

    ComPtr<IJsonArrayStatics> jsonArrayStatics;
    if (SUCCEEDED(GetActivationFactory(HStringReference(RuntimeClass_Windows_Data_Json_JsonArray).Get(), &jsonArrayStatics)))
    {
        ComPtr<IJsonArray> rootJsonArray;
        if (SUCCEEDED(jsonArrayStatics->Parse(HStringReference(source).Get(), &rootJsonArray)))
        {
            ComPtr<IVector<IJsonValue*>> iterable;
            if (SUCCEEDED(rootJsonArray.As(&iterable)))
            {
                unsigned size;
                iterable->get_Size(&size);
                result.reserve(size);

                for (auto jsonValue : iterable.Get())
                {
                    ComPtr<IJsonObject> jsonObject;
                    if (SUCCEEDED(jsonValue->GetObject(&jsonObject)))
                    {
                        HString id;
                        double_t index;
                        HString guid;
                        double_t balance;
                        HString picture;
                        double_t age;
                        HString name;
                        HString gender;
                        HString company;
                        HString email;
                        HString phone;
                        HString address;
                        HString about;
                        double_t latitude;
                        double_t longitude;
                        vector<wstring> tags;
                        HString greeting;
                        boolean isActive;

                        jsonObject->GetNamedString(HStringReference(L"_id").Get(), &id);
                        jsonObject->GetNamedNumber(HStringReference(L"index").Get(), &index);
                        jsonObject->GetNamedString(HStringReference(L"guid").Get(), &guid);
                        jsonObject->GetNamedNumber(HStringReference(L"balance").Get(), &balance);
                        jsonObject->GetNamedString(HStringReference(L"picture").Get(), &picture);
                        jsonObject->GetNamedNumber(HStringReference(L"age").Get(), &age);
                        jsonObject->GetNamedString(HStringReference(L"name").Get(), &name);
                        jsonObject->GetNamedString(HStringReference(L"gender").Get(), &gender);
                        jsonObject->GetNamedString(HStringReference(L"company").Get(), &company);
                        jsonObject->GetNamedString(HStringReference(L"email").Get(), &email);
                        jsonObject->GetNamedString(HStringReference(L"phone").Get(), &phone);
                        jsonObject->GetNamedString(HStringReference(L"address").Get(), &address);
                        jsonObject->GetNamedString(HStringReference(L"about").Get(), &about);
                        jsonObject->GetNamedNumber(HStringReference(L"latitude").Get(), &latitude);
                        jsonObject->GetNamedNumber(HStringReference(L"longitude").Get(), &longitude);
                        jsonObject->GetNamedString(HStringReference(L"greeting").Get(), &greeting);
                        jsonObject->GetNamedBoolean(HStringReference(L"isActive").Get(), &isActive);

                        ComPtr<IJsonArray> tagsJsonArray;
                        if (SUCCEEDED(jsonObject->GetNamedArray(HStringReference(L"tags").Get(), &tagsJsonArray)))
                        {
                            ComPtr<IIterable<IJsonValue*>> tagsIterable;
                            if (SUCCEEDED(tagsJsonArray.As(&tagsIterable)))
                            {
                                for (auto tagJsonValue : tagsIterable.Get())
                                {
                                    HString tag;
                                    if (SUCCEEDED(tagJsonValue->GetString(&tag)))
                                    {
                                        tags.emplace_back(tag.GetRawBuffer(), tag.Size());
                                    }
                                }
                            }
                        }

                        result.emplace_back(wstring(id.GetRawBuffer(), id.Size()),
                                            uint32_t(index),
                                            wstring(guid.GetRawBuffer(), guid.Size()),
                                            double_t(balance),
                                            wstring(picture.GetRawBuffer(), picture.Size()),
                                            uint8_t(age),
                                            wstring(name.GetRawBuffer(), name.Size()),
                                            Gender(wcscmp(L"female", gender.GetRawBuffer()) == 0 ? Gender::Female : Gender::Male),
                                            wstring(company.GetRawBuffer(), company.Size()),
                                            wstring(email.GetRawBuffer(), email.Size()),
                                            wstring(phone.GetRawBuffer(), phone.Size()),
                                            wstring(address.GetRawBuffer(), address.Size()),
                                            wstring(about.GetRawBuffer(), about.Size()),
                                            double_t(latitude),
                                            double_t(longitude),
                                            vector<wstring>(move(tags)),
                                            wstring(greeting.GetRawBuffer(), greeting.Size()),
                                            bool(isActive));
                    }
                }
            }
        }
    }

    return result;
}

std::vector<SampleModel> systemApiProjected(wchar_t *source) noexcept
{
    
}

void Executor::Execute() noexcept
{
    if (IsDebuggerPresent())
    {
        ShowResult();
    }
    else
    {
        PerformComputations();
    }
}

void Executor::PerformComputations() noexcept
{
    using namespace ABI::Windows::Foundation;
    using namespace MTL;
    using namespace std::chrono;
    using namespace std;

    auto &dataSource = Data::GetJsonSource();

    auto beginTime = high_resolution_clock::now();

    auto result = systemApi(const_cast<wchar_t*>(dataSource.data()));

    auto endTime = high_resolution_clock::now();
    auto ms = duration_cast<milliseconds>(endTime - beginTime).count();

    ComPtr<IPropertyValueStatics> propertyValueStatics;
    ComPtr<IPropertyValue> propertyValue;

    if (SUCCEEDED(GetActivationFactory(HStringReference(RuntimeClass_Windows_Foundation_PropertyValue).Get(), &propertyValueStatics)))
    {
        if (SUCCEEDED( propertyValueStatics->CreateInt64(ms, &propertyValue)))
        {
            LocalSettings::SetValue(HStringReference(L"milliseconds").Get(), propertyValue.Get());
        }

        propertyValue.Release();

        if (SUCCEEDED(propertyValueStatics->CreateInt64(result.size(), &propertyValue)))
        {
            LocalSettings::SetValue(HStringReference(L"counts").Get(), propertyValue.Get());
        }
    }

    exit(0);
}

void Executor::ShowResult() noexcept
{
    using namespace ABI::Windows::Foundation;
    using namespace MTL;
    using namespace std;

    auto millisecondsInspectable = CreateComPtr(LocalSettings::GetValue(HStringReference(L"milliseconds").Get()));
    if (millisecondsInspectable)
    {
        ComPtr<IPropertyValue> propertyValue;
        if (SUCCEEDED(millisecondsInspectable.As(&propertyValue)))
        {
            int64_t milliseconds;
            if (SUCCEEDED( propertyValue->GetInt64(&milliseconds)))
            {
                OutputDebugStringW(to_wstring(milliseconds).data());
                OutputDebugStringW(L"\r\n");
            }
        }
    }

    auto countsInspectable = CreateComPtr(LocalSettings::GetValue(HStringReference(L"counts").Get()));
    if (countsInspectable)
    {
        ComPtr<IPropertyValue> propertyValue;
        if (SUCCEEDED(countsInspectable.As(&propertyValue)))
        {
            int64_t counts;
            if (SUCCEEDED(propertyValue->GetInt64(&counts)))
            {
                OutputDebugStringW(to_wstring(counts).data());
                OutputDebugStringW(L"\r\n");
            }
        }
    }

    __debugbreak();
}
