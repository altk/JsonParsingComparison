#pragma once
#include <MTL.h>
#include <windows.storage.h>

namespace CppRapidJson
{
    class LocalSettings final
    {
    public:
        static IInspectable* GetValue(HSTRING key) noexcept
        {
            IInspectable *result = nullptr;
            if (SUCCEEDED(getInstance()._localSettings->Lookup(key, &result)))
            {
                result->AddRef();
            }
            return result;
        }

        static bool SetValue(HSTRING key, IInspectable *value) noexcept
        {
            boolean isReplaced;
            return SUCCEEDED(getInstance()._localSettings->Insert(key, value, &isReplaced));
        }

    private:
        static LocalSettings& getInstance()
        {
            static LocalSettings result;
            return result;
        }

        LocalSettings();

        MTL::ComPtr<ABI::Windows::Foundation::Collections::IMap<HSTRING, IInspectable*>> _localSettings;
    };
}
