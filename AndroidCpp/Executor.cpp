#include "pch.h"
#include <cstdlib>
#include <chrono>
#include <rapidjson\document.h>
#include "Executor.h"
#include "Data.h"
#include "SampleModel.h"

using namespace std;
using namespace rapidjson;
using namespace AndroidCpp;

SampleModel ParseModel(const GenericValue<UTF16LE<>> &value) noexcept
{
    auto &id = value[L"_id"];
    auto &index = value[L"index"];
    auto &guid = value[L"guid"];
    auto &balance = value[L"balance"];
    auto &picture = value[L"picture"];
    auto &age = value[L"age"];
    auto &name = value[L"name"];
    auto &gender = value[L"gender"];
    auto &company = value[L"company"];
    auto &email = value[L"email"];
    auto &phone = value[L"phone"];
    auto &address = value[L"address"];
    auto &about = value[L"about"];
    auto &latitude = value[L"latitude"];
    auto &longitude = value[L"longitude"];
    auto &tags = value[L"tags"];
    auto &greeting = value[L"greeting"];
    auto &isActive = value[L"isActive"];

    vector<wstring> tagModels;
    auto endTags = tags.End();
    for (auto beginTags = tags.Begin(); beginTags != endTags; ++beginTags)
    {
        tagModels.emplace_back(beginTags->GetString(), beginTags->GetStringLength());
    }

    return SampleModel(wstring(id.GetString(), id.GetStringLength()),
                       uint32_t(index.GetUint()),
                       wstring(guid.GetString(), guid.GetStringLength()),
                       double(balance.GetDouble()),
                       wstring(picture.GetString(), picture.GetStringLength()),
                       uint8_t(age.GetUint()),
                       wstring(name.GetString(), name.GetStringLength()),
                       Gender(wcscmp(L"female", gender.GetString()) == 0 ? Gender::Female : Gender::Male),
                       wstring(company.GetString(), company.GetStringLength()),
                       wstring(email.GetString(), email.GetStringLength()),
                       wstring(phone.GetString(), phone.GetStringLength()),
                       wstring(address.GetString(), address.GetStringLength()),
                       wstring(about.GetString(), about.GetStringLength()),
                       double(latitude.GetDouble()),
                       double(longitude.GetDouble()),
                       vector<wstring>(move(tagModels)),
                       wstring(greeting.GetString(), greeting.GetStringLength()),
                       bool(isActive.GetBool()));
}

SampleModel ParseModel(const wchar_t *source) noexcept
{
    GenericDocument<UTF16LE<>> document;
    document.Parse(source);

    return ParseModel(document);
}

std::vector<SampleModel> ParseArray(const wchar_t *source) noexcept
{
    GenericDocument<UTF16LE<>> document;
    document.Parse(source);

    vector<SampleModel> result;
    result.reserve(document.Size());

    auto endModels = document.End();
    for (auto beginModels = document.Begin(); beginModels != endModels; ++beginModels)
    {
        result.emplace_back(ParseModel(*beginModels));
    }

    return result;
}

jlong Executor::ParseArray() noexcept
{
    using namespace chrono;

    auto jsonSource = Data::GetBigJson();

    auto start = high_resolution_clock::now();

    auto result = ::ParseArray(jsonSource.data());

    auto duration = duration_cast<milliseconds>(high_resolution_clock::now() - start).count();

    __android_log_print(ANDROID_LOG_VERBOSE, "[AndroidCpp::Executor::ParseArray time]", "%lli", duration);
    __android_log_print(ANDROID_LOG_VERBOSE, "[AndroidCpp::Executor::ParseArray count]", "%u", result.size());

    return duration;
}

jlong Executor::ParseModel() noexcept
{
    using namespace chrono;

    auto jsonSource = Data::GetSmallJson();
    auto jsonSourcePtr = jsonSource.data();

    auto start = high_resolution_clock::now();

    vector<SampleModel> result;
    result.reserve(10000);

    for (auto i = 0; i < 10000; ++i)
    {
        result.emplace_back(::ParseModel(jsonSourcePtr));
    }

    auto duration = duration_cast<milliseconds>(high_resolution_clock::now() - start).count();

    __android_log_print(ANDROID_LOG_VERBOSE, "[AndroidCpp::Executor::ParseModel time]", "%lli", duration);
    __android_log_print(ANDROID_LOG_VERBOSE, "[AndroidCpp::Executor::ParseModel count]", "%u", result.size());

    return duration;
}

extern "C"
{
    JNIEXPORT jlong JNICALL
        Java_com_example_jsonparsingcomparison_Executor_rapidJsonParseArray(JNIEnv* env, jclass)
    {
        return Executor::ParseArray();
    }

    JNIEXPORT jlong JNICALL
        Java_com_example_jsonparsingcomparison_Executor_rapidJsonParseModel(JNIEnv* env, jclass)
    {
        return Executor::ParseModel();
    }
}