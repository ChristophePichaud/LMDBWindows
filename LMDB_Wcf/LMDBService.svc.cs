using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;

using LMDBNet;

namespace LMDB_Wcf
{
    public class LMDBService : ILMDBService
    {
        public string GetData(string dbName, string fileName, byte[] buffer)
        {
            string temp = Directory.GetCurrentDirectory();
            return temp;
        }
    }
}
