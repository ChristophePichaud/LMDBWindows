using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LMDBWcf_Client
{
    public class Logger
    {
        public static void LogInfo(string message)
        {
            DateTime dt = DateTime.Now;
            string str = String.Format("INFO - {0} - {1}", dt.ToLongTimeString(), message);
            Console.WriteLine(str);
        }

        public static void LogError(string message, Exception ex)
        {
            DateTime dt = DateTime.Now;
            string str = String.Format("ERROR - {0} - {1}\n{2}", dt.ToLongTimeString(), message, ex.Message);
            Console.WriteLine(str);
        }
    }
}
