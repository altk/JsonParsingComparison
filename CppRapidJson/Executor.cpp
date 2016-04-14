#include "pch.h"
#include "Executor.h"
#include "Data.h"
#include "SampleModel.h"
#include "LocalSettings.h"
#include <MTL.h>

using namespace CppRapidJson;

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
    using namespace rapidjson;

    GenericDocument<UTF16LE<>> document;

    auto &dataSource = Data::GetJsonSource();

    auto beginTime = high_resolution_clock::now();

    document.Parse(dataSource.data());

    vector<SampleModel> result;

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
