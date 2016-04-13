#include <pch.h>
#include <thread>
#include <FrameworkView.h>
#include <Executor.h>

using namespace CppRapidJson;
using namespace MTL;

HRESULT FrameworkView::GetRuntimeClassName(HSTRING* className)
{
    *className = HString(RuntimeClass_CppRapidJson_FrameworkView).Detach();
    return S_OK;
}

HRESULT FrameworkView::Initialize(ABI::Windows::ApplicationModel::Core::ICoreApplicationView*)
{
    return S_OK;
}

HRESULT FrameworkView::SetWindow(ABI::Windows::UI::Core::ICoreWindow* window)
{
    using namespace ABI::Windows::UI::Core;
    
    _coreWindow = ComPtr<ICoreWindow>(window);
    return S_OK;
}

HRESULT FrameworkView::Load(HSTRING)
{
    return S_OK;
}

HRESULT FrameworkView::Run()
{
    using namespace std;
    using namespace ABI::Windows::ApplicationModel::Core;
    using namespace ABI::Windows::UI::Core;

    ComPtr<ICoreDispatcher> dispatcher;

    Check(_coreWindow->get_Dispatcher(&dispatcher));

    thread(Executor::Execute).detach();

    Check(dispatcher->ProcessEvents(CoreProcessEventsOption_ProcessUntilQuit));

    return S_OK;    
}

HRESULT FrameworkView::Uninitialize()
{
    _coreWindow.Release();
    return S_OK;
}
