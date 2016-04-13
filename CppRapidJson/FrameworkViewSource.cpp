#include <pch.h>
#include <FrameworkViewSource.h>
#include <FrameworkView.h>

using namespace CppRapidJson;
using namespace MTL;

STDMETHODIMP FrameworkViewSource::GetRuntimeClassName(HSTRING * className)
{
    *className = HString(RuntimeClass_CppRapidJson_FrameworkViewSource).Detach();

    return S_OK;
}

STDMETHODIMP CppRapidJson::FrameworkViewSource::CreateView(ABI::Windows::ApplicationModel::Core::IFrameworkView ** viewProvider)
{
    *viewProvider = CreateComPtr(new FrameworkView()).Detach();

    return S_OK;
}
