using System;
using System.Threading.Tasks;
using Windows.ApplicationModel.Core;
using Windows.UI.Core;

namespace Cs
{
    public sealed class FrameworkView : IFrameworkView
    {
        private CoreWindow _window;

        public void Initialize(CoreApplicationView applicationView) { }

        public void Load(String entryPoint) { }

        public void Run()
        {
            _window.Activate();

            Task.Run(() => Executor.Execute());

            _window.Dispatcher.ProcessEvents(CoreProcessEventsOption.ProcessUntilQuit);
        }

        public void SetWindow(CoreWindow window)
        {
            _window = window;
        }

        public void Uninitialize() { }
    }
}