#define DEBUG
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using Windows.ApplicationModel.Core;
using Windows.Data.Json;
using Windows.Storage;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace Cs
{
    internal static class Executor
    {
        public static void Execute()
        {
            if (Debugger.IsAttached)
            {
                ShowResult();
            }
            else
            {
                PerformComputations();
            }
        }

        private static IReadOnlyList<SampleModel> JsonNetParseArray(String jsonSource)
        {
            var result = new List<SampleModel>();

            var jArray = JArray.Parse(jsonSource);
            foreach (var jToken in jArray)
            {
                var jObject = (JObject) jToken;

                var id = jObject.GetValue("_id");
                var index = jObject.GetValue("index");
                var guid = jObject.GetValue("guid");
                var balance = jObject.GetValue("balance");
                var picture = jObject.GetValue("picture");
                var age = jObject.GetValue("age");
                var name = jObject.GetValue("name");
                var gender = jObject.GetValue("gender");
                var company = jObject.GetValue("company");
                var email = jObject.GetValue("email");
                var phone = jObject.GetValue("phone");
                var address = jObject.GetValue("address");
                var about = jObject.GetValue("about");
                var latitude = jObject.GetValue("latitude");
                var longitude = jObject.GetValue("longitude");
                var greeting = jObject.GetValue("greeting");
                var isActive = jObject.GetValue("isActive");
                var tags = (JArray) jObject.GetValue("tags");

                result.Add(new SampleModel((String) id,
                                           (UInt16) index,
                                           (String) guid,
                                           (Double) balance,
                                           (String) picture,
                                           (UInt16) age,
                                           (String) name,
                                           (Gender) Enum.Parse(typeof (Gender), (String) gender, true),
                                           (String) company,
                                           (String) email,
                                           (String) phone,
                                           (String) address,
                                           (String) about,
                                           (Double) latitude,
                                           (Double) longitude,
                                           tags.Select(token => (String) token).ToList(),
                                           (String) greeting,
                                           (Boolean) isActive));
            }
            return result;
        }

        private static SampleModel JsonNetParseModel(String jsonSource)
        {
            var jObject = JObject.Parse(jsonSource);

            var id = jObject.GetValue("_id");
            var index = jObject.GetValue("index");
            var guid = jObject.GetValue("guid");
            var balance = jObject.GetValue("balance");
            var picture = jObject.GetValue("picture");
            var age = jObject.GetValue("age");
            var name = jObject.GetValue("name");
            var gender = jObject.GetValue("gender");
            var company = jObject.GetValue("company");
            var email = jObject.GetValue("email");
            var phone = jObject.GetValue("phone");
            var address = jObject.GetValue("address");
            var about = jObject.GetValue("about");
            var latitude = jObject.GetValue("latitude");
            var longitude = jObject.GetValue("longitude");
            var greeting = jObject.GetValue("greeting");
            var isActive = jObject.GetValue("isActive");
            var tags = (JArray) jObject.GetValue("tags");

            return new SampleModel((String) id,
                                   (UInt16) index,
                                   (String) guid,
                                   (Double) balance,
                                   (String) picture,
                                   (UInt16) age,
                                   (String) name,
                                   (Gender) Enum.Parse(typeof (Gender), (String) gender, true),
                                   (String) company,
                                   (String) email,
                                   (String) phone,
                                   (String) address,
                                   (String) about,
                                   (Double) latitude,
                                   (Double) longitude,
                                   tags.Select(token => (String) token).ToList(),
                                   (String) greeting,
                                   (Boolean) isActive);
        }

        private static IReadOnlyList<SampleModel> JsonNetReflectionParseArray(String jsonSource)
        {
            return JsonConvert.DeserializeObject<List<SampleModel>>(jsonSource);
        }

        private static SampleModel JsonNetReflectionParseModel(String jsonSource)
        {
            return JsonConvert.DeserializeObject<SampleModel>(jsonSource);
        }

        private static void PerformComputations()
        {
            var jsonSource = Data.GetBigJson();

            var stopwatch = Stopwatch.StartNew();

            //var models = new List<SampleModel>(10000);

            //for (var i = 0; i < 10000; ++i)
            //{
            //    models.Add(JsonNetParseModel(jsonSource));
            //}

            var models = JsonNetParseArray(jsonSource);

            GC.Collect();

            stopwatch.Stop();

            var localSettingsValues = ApplicationData.Current.LocalSettings.Values;
            localSettingsValues["milliseconds"] = stopwatch.ElapsedMilliseconds;
            localSettingsValues["count"] = models.Count;

            CoreApplication.Exit();
        }

        private static void ShowResult()
        {
            var localSettingsValues = ApplicationData.Current.LocalSettings.Values;

            Debug.WriteLine(localSettingsValues["milliseconds"]);
            Debug.WriteLine(localSettingsValues["count"]);

            Debugger.Break();
        }

        private static IReadOnlyList<SampleModel> SystemApiParseArray(String jsonSource)
        {
            var result = new List<SampleModel>();

            var jsonArray = JsonArray.Parse(jsonSource);

            foreach (var jsonValue in jsonArray)
            {
                var jsonObject = jsonValue.GetObject();
                var tags = jsonObject.GetNamedArray("tags")
                                     .Select(value => value.GetString())
                                     .ToList();

                var id = jsonObject.GetNamedString("_id");
                var index = jsonObject.GetNamedNumber("index");
                var guid = jsonObject.GetNamedString("guid");
                var balance = jsonObject.GetNamedNumber("balance");
                var picture = jsonObject.GetNamedString("picture");
                var age = jsonObject.GetNamedNumber("age");
                var name = jsonObject.GetNamedString("name");
                var gender = jsonObject.GetNamedString("gender");
                var company = jsonObject.GetNamedString("company");
                var email = jsonObject.GetNamedString("email");
                var phone = jsonObject.GetNamedString("phone");
                var address = jsonObject.GetNamedString("address");
                var about = jsonObject.GetNamedString("about");
                var latitude = jsonObject.GetNamedNumber("latitude");
                var longitude = jsonObject.GetNamedNumber("longitude");
                var greeting = jsonObject.GetNamedString("greeting");
                var isActive = jsonObject.GetNamedBoolean("isActive");

                result.Add(new SampleModel(id,
                                           (UInt16) index,
                                           guid,
                                           balance,
                                           picture,
                                           (UInt16) age,
                                           name,
                                           (Gender) Enum.Parse(typeof (Gender), gender, true),
                                           company,
                                           email,
                                           phone,
                                           address,
                                           about,
                                           latitude,
                                           longitude,
                                           tags,
                                           greeting,
                                           isActive));
            }

            return result;
        }

        private static SampleModel SystemApiParseModel(String jsonSource)
        {
            var jsonObject = JsonObject.Parse(jsonSource);
            var tags = jsonObject.GetNamedArray("tags")
                                 .Select(value => value.GetString())
                                 .ToList();

            var id = jsonObject.GetNamedString("_id");
            var index = jsonObject.GetNamedNumber("index");
            var guid = jsonObject.GetNamedString("guid");
            var balance = jsonObject.GetNamedNumber("balance");
            var picture = jsonObject.GetNamedString("picture");
            var age = jsonObject.GetNamedNumber("age");
            var name = jsonObject.GetNamedString("name");
            var gender = jsonObject.GetNamedString("gender");
            var company = jsonObject.GetNamedString("company");
            var email = jsonObject.GetNamedString("email");
            var phone = jsonObject.GetNamedString("phone");
            var address = jsonObject.GetNamedString("address");
            var about = jsonObject.GetNamedString("about");
            var latitude = jsonObject.GetNamedNumber("latitude");
            var longitude = jsonObject.GetNamedNumber("longitude");
            var greeting = jsonObject.GetNamedString("greeting");
            var isActive = jsonObject.GetNamedBoolean("isActive");

            return new SampleModel(id,
                                   (UInt16) index,
                                   guid,
                                   balance,
                                   picture,
                                   (UInt16) age,
                                   name,
                                   (Gender) Enum.Parse(typeof (Gender), gender, true),
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
}