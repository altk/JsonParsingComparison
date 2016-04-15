using Windows.ApplicationModel.Core;

namespace Cs
{
    public sealed class FrameworkViewSource : IFrameworkViewSource
    {
        public IFrameworkView CreateView()
        {
            return new FrameworkView();
        }
    }
}