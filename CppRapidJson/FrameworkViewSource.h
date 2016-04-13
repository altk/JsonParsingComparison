#pragma once
#include <MTL.h>
#include <CppRapidJson_h.h>

namespace CppRapidJson
{
    class FrameworkViewSource final : public MTL::RuntimeClass<ABI::Windows::ApplicationModel::Core::IFrameworkViewSource>
    {
    public:
        STDMETHODIMP GetRuntimeClassName(HSTRING* className) override;
        
        STDMETHODIMP CreateView(ABI::Windows::ApplicationModel::Core::IFrameworkView** viewProvider) override;
    };
}
