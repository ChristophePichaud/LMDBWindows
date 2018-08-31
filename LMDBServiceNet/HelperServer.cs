using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace LMDBServiceNet
{
    public class HelperServer
    {
        public static string GetIP()
        {
            string ip = String.Empty;
            string strHostName = Dns.GetHostName();
            IPHostEntry iphostentry = Dns.GetHostEntry(strHostName);
            foreach (IPAddress ipaddress in iphostentry.AddressList)
            {
                ip = ipaddress.ToString();
                Logger.LogInfo(ip);
            }
            return ip;
        }
    }
}
