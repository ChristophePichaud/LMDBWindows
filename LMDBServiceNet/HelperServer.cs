using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.NetworkInformation;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace LMDBServiceNet
{
    public class HelperServer
    {
        public static void EnumNICInterfaces()
        {
            foreach (NetworkInterface nic in NetworkInterface.GetAllNetworkInterfaces())
            {
                if ((nic.NetworkInterfaceType == NetworkInterfaceType.Ethernet) || (nic.NetworkInterfaceType == NetworkInterfaceType.Wireless80211)) //&& (nic.OperationalStatus == OperationalStatus.Up))
                {
                    Logger.LogInfo(nic.Name + ", " + nic.Description);

                    UnicastIPAddressInformationCollection cols = nic.GetIPProperties().UnicastAddresses;
                    foreach (UnicastIPAddressInformation ip in cols)
                    {
                        if (ip.Address.AddressFamily == System.Net.Sockets.AddressFamily.InterNetwork)
                        {
                            Logger.LogInfo(ip.Address.ToString());
                        }
                    }
                }
            }
        }

        public static string GetIP()
        {
            string ip = String.Empty;
            string strHostName = Dns.GetHostName();
            IPHostEntry iphostentry = Dns.GetHostEntry(strHostName);
            foreach (IPAddress ipaddress in iphostentry.AddressList)
            {
                string str = String.Format("{0} {1}", ipaddress.AddressFamily, ipaddress.ToString());
                ip = ipaddress.ToString();
                Logger.LogInfo(str);
            }
            return ip;
        }

        public static string GetLocalIPAddress()
        {
            var host = Dns.GetHostEntry(Dns.GetHostName());
            foreach (var ip in host.AddressList)
            {
                if (ip.AddressFamily == AddressFamily.InterNetwork)
                {
                    string ipret = ip.ToString();
                    return ipret;
                }
            }
            throw new Exception("No network adapters with an IPv4 address in the system!");
        }
    }
}
