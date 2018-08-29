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
            var server = new WebServer("http://localhost:51111/");
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
