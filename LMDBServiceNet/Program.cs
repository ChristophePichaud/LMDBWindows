using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading.Tasks;

namespace LMDBServiceNet
{
    class Program
    {
        static void Main(string[] args)
        {
            Logger.LogInfo("Main...");
            ServiceBase[] ServicesToRun;
            AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(WorkerThreadHandler);
            if (System.AppDomain.CurrentDomain.BaseDirectory != Directory.GetCurrentDirectory() + @"\")
            {
                ServicesToRun = new ServiceBase[] { new MyService() };

                Directory.SetCurrentDirectory(System.AppDomain.CurrentDomain.BaseDirectory);
                Process.GetCurrentProcess().StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
                ServiceBase.Run(ServicesToRun);
            }
            else
            {
                ServiceLauncher.Run(args);
            }
        }

        static void WorkerThreadHandler(object sender, UnhandledExceptionEventArgs args)
        {
            Trace.WriteLine("Unhandledexception occured");
            if (args != null && args.ExceptionObject != null)
            {
                if (args.ExceptionObject.GetType().IsSubclassOf(typeof(Exception)))
                {
                    Exception ex = args.ExceptionObject as Exception;
                    Trace.WriteLine("Message " + ex.Message);
                    Trace.WriteLine("StackTrace " + ex.StackTrace);
                }
                else
                {
                    Trace.WriteLine(args.ExceptionObject.ToString());
                }
            }
        }
    }
}
