using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace LMDBServiceNet
{
    public static class ServiceLauncher
    {
        public static bool IsService;
        public static bool DoFinish = false;
        public static DateTime Last = DateTime.Now;

        public static void Run(object args)
        {
            try
            {
                Logger.LogInfo("Run...");
                string ip = HelperServer.GetIP();
                string url = String.Format("http://{0}:7001/", ip);
                Logger.LogInfo(url);
                WebServer server = new WebServer(url);
                server.Start();
                //
                while (!DoFinish)
                {
                    if (((TimeSpan)DateTime.Now.Subtract(Last)).Seconds > 5)
                    {
                        Logger.LogInfo("Waiting...");
                        Last = DateTime.Now;
                    }

                    //if (!IsService)
                    //{
                    //}

                    Thread.Sleep(1000);
                }
                server.Stop();
                Logger.LogInfo("Exit...");
            }
            catch (Exception ex)
            {
                Logger.LogInfo("Exception...");
                Logger.LogInfo(ex.ToString());
                Logger.LogInfo(ex.Message);
                Logger.LogInfo(ex.Source);
                Logger.LogInfo(ex.StackTrace);
            }

        }

        public static void Stop()
        {
            DoFinish = true;
            Logger.LogInfo("Stop...");
        }
    }
}
