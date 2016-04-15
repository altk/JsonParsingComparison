#pragma once
#include <MTL.h>
#include <Cpp_h.h>

namespace Cpp
{
    class FrameworkView final : public MTL::RuntimeClass<ABI::Windows::ApplicationModel::Core::IFrameworkView>
    {
    public:
        STDMETHODIMP GetRuntimeClassName(HSTRING* className) override;
        STDMETHODIMP Initialize(ABI::Windows::ApplicationModel::Core::ICoreApplicationView* applicationView) override;
        STDMETHODIMP SetWindow(ABI::Windows::UI::Core::ICoreWindow* window) override;
        STDMETHODIMP Load(HSTRING entryPoint) override;
        STDMETHODIMP Run() override;
        STDMETHODIMP Uninitialize() override;
    private:
        MTL::ComPtr<ABI::Windows::UI::Core::ICoreWindow> _coreWindow;
    };
}
