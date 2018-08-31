using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace LMDBServiceNet
{
    partial class MyService : ServiceBase
    {
        public MyService()
        {
            InitializeComponent();
        }

        protected override void OnStart(string[] args)
        {
            // TODO: Add code here to start your service.
            Logger.LogInfo("OnStart...");
            ServiceLauncher.IsService = true;
            Thread t = new Thread(new ParameterizedThreadStart(ServiceLauncher.Run));
            t.Start(args);
        }

        protected override void OnStop()
        {
            // TODO: Add code here to perform any tear-down necessary to stop your service.
            Logger.LogInfo("OnStop...");
            try { ServiceLauncher.Stop(); }
            catch (Exception ex)
            {
                UnhandledException(ex);
            }
        }

        private void UnhandledException(Exception e)
        {
            // HACK: Do nothing but eliminates warning on empty catch...
            System.Diagnostics.Trace.WriteLine("Unhandled exception " + e.Message + "\n" + e.StackTrace);
        }
    }
}
