using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LMDBServiceNet
{
    class Program
    {
        static void Main(string[] args)
        {
            // Listen on port 51111, serving files in d:\webroot:
            string ip = HelperServer.GetIP();
            string url = String.Format("http://{0}:7001/", ip);
            var server = new WebServer(url); // "http://172.26.240.1:7001/"); // localhost:7001/");
            try
            {
                server.Start();
                Console.WriteLine("Server running... press Enter to stop");
                Console.ReadLine();
            }
            finally { server.Stop(); }
        }
    }
}
