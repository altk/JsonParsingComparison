using System;
using Windows.ApplicationModel.Core;

namespace Cs
{
    public static class Application
    {
        [STAThread]
        public static void Main()
        {
            CoreApplication.Run(new FrameworkViewSource());
        }
    }
}