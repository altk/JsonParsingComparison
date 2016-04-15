#include <pch.h>
#include <FrameworkViewSource.h>
#include <FrameworkView.h>

using namespace Cpp;
using namespace MTL;

STDMETHODIMP FrameworkViewSource::GetRuntimeClassName(HSTRING * className)
{
    *className = HString(RuntimeClass_Cpp_FrameworkViewSource).Detach();

    return S_OK;
}

STDMETHODIMP Cpp::FrameworkViewSource::CreateView(ABI::Windows::ApplicationModel::Core::IFrameworkView ** viewProvider)
{
    *viewProvider = CreateComPtr(new FrameworkView()).Detach();

    return S_OK;
}
