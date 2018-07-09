using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using LMDBNet;

namespace LMDBNet_ClientFiles
{
    class Program
    {
        static void Main(string[] args)
        {
            Logger.LogInfo("Main...");
            string path = String.Empty;
            if (args.Length == 1)
            {
                path = args[0];
            }

            string str = String.Format("Path={0}", path);
            Logger.LogInfo(str);


            bool bFile = false;
            if( File.Exists(path))
            {
                bFile = true;
            }

            string key = path;
            string value = String.Empty;
            byte[] buffer = File.ReadAllBytes(path);

            int count = 0;
            LMDBEnvironment _env;
            string dir = "c:\\temp\\cache_net10B";
            _env = new LMDBEnvironment(dir);
            //This is here to assert that previous issues with the way manager
            //classes (since removed) worked don't happen anymore.
            _env.MaxDatabases = 2;
            _env.MapSize = 10485760 * 100;
            _env.Open();

            DateTime dtStart = DateTime.Now;
            var tx = _env.BeginTransaction();
            var db = tx.OpenDatabase("DB", new DatabaseConfiguration { Flags = DatabaseOpenFlags.Create });
            var enc = System.Text.Encoding.UTF8;
            tx.Put(db, enc.GetBytes(key), buffer);
            tx.Commit();
            db.Dispose();

            DateTime dtStop = DateTime.Now;
            TimeSpan ts = dtStop - dtStart;
            str = String.Format("Time elapsed for set:{0} ms", ts.TotalMilliseconds);
            Logger.LogInfo(str);

            _env.Dispose();
        }
    }
}
