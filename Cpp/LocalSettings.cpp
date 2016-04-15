#include <pch.h>
#include <LocalSettings.h>

using namespace Cpp;
using namespace MTL;
using namespace std;

LocalSettings::LocalSettings()
{
    using namespace ABI::Windows::Storage;
    using namespace ABI::Windows::Foundation::Collections;

    ComPtr<IApplicationDataStatics> applicationDataStatics;
    ComPtr<IApplicationData> currentApplicaitonData;
    ComPtr<IApplicationDataContainer> applicationDataContainer;
    ComPtr<IPropertySet> propertySet;

    Check(GetActivationFactory(HStringReference(RuntimeClass_Windows_Storage_ApplicationData).Get(),
                               &applicationDataStatics));

    Check(applicationDataStatics->get_Current(&currentApplicaitonData));

    Check(currentApplicaitonData->get_LocalSettings(&applicationDataContainer));

    Check(applicationDataContainer->get_Values(&propertySet));

    Check(propertySet.As(&_localSettings));
}