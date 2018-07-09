using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LMDBWcf_Client
{
    class Program
    {
        static void Main(string[] args)
        {
            LMDBServiceReference.LMDBServiceClient client = new LMDBServiceReference.LMDBServiceClient();
            string str = client.GetData("DB", "Path", null);
            Console.WriteLine(str);

            AzureLMDBServiceReference.LMDBServiceClient client2 = new AzureLMDBServiceReference.LMDBServiceClient();
            str = client2.GetData("DB", "Path", null);
            Console.WriteLine(str);
        }
    }
}
