#pragma once
#include <MTL.h>
#include <Cpp_h.h>

namespace Cpp
{
    class FrameworkViewSource final : public MTL::RuntimeClass<ABI::Windows::ApplicationModel::Core::IFrameworkViewSource>
    {
    public:
        STDMETHODIMP GetRuntimeClassName(HSTRING* className) override;
        
        STDMETHODIMP CreateView(ABI::Windows::ApplicationModel::Core::IFrameworkView** viewProvider) override;
    };
}
