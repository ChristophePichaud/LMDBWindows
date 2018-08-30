using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LMDBServiceNet
{
    public class Constants
    {
        public static string MasterNodePort	    = "7001";
        public static string Request			= "?request=";
        public static string VerbPing			= "ping";
        public static string VerbUsage          = "usage";
        public static string VerbGetData		= "get-data";
        public static string VerbSetData		= "set-data";
        public static string VerbGetDataB64	    = "get-data-b64";
        public static string VerbSetDataB64	    = "set-data-b64";
        public static string VerbRegisterNode	= "register-node";
        public static string VerbShowNodes		= "show-nodes";
        public static string VerbGetNode		= "get-node";
        public static string VerbSetNode		= "set-node";
        public static string VerbReleaseNode	= "release-node";
        public static string VerbReleaseDB		= "release-db";
        public static string LMDBRootPath       = "c:\\temp";
        public static string StatusOK			= "OK";
        public static string ModeMasterNode	    = "master";
        public static string ModeWorkerNode	    = "node";
    }
}
