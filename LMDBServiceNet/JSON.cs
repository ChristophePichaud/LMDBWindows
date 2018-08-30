using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LMDBServiceNet
{
    public class Data
    {
        public string Key;
        public string Value;
    }

    public class DataUsage
    {
        public string company;
        public string developer;
        public string version;
    }

    public class DataPing
    {
        public string server;
        public string port;
        public string ip;
    }
}
