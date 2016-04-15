#define DEBUG
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using Windows.ApplicationModel.Core;
using Windows.Data.Json;
using Windows.Storage;

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

        private static void PerformComputations()
        {
            var jsonSource = Data.GetJsonSource();

            var stopwatch = Stopwatch.StartNew();

            var models = SystemApi(jsonSource);

            stopwatch.Stop();

            var localSettingsValues = ApplicationData.Current.LocalSettings.Values;
            localSettingsValues["milliseconds"] = stopwatch.ElapsedMilliseconds;
            localSettingsValues["count"] = models.Count;

            CoreApplication.Exit();
        }

        private static IReadOnlyList<SampleModel> SystemApi(String jsonSource)
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

        private static void ShowResult()
        {
            var localSettingsValues = ApplicationData.Current.LocalSettings.Values;

            Debug.WriteLine(localSettingsValues["milliseconds"]);
            Debug.WriteLine(localSettingsValues["count"]);

            Debugger.Break();
        }
    }
}