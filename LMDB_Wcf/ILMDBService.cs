using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;

namespace LMDB_Wcf
{
    [ServiceContract]
    public interface ILMDBService
    {

        [OperationContract]
        string GetData(string dbName, string fileName, byte [] buffer);
    }
}
