using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using LMDBNet;

namespace LMDBNet_Client
{
    class Program
    {
        static void Main2()
        {
            IntPtr env;
            Logger.LogInfo("mdb_env_create...");
            Lmdb.mdb_env_create(out env);
            Logger.LogInfo("mdb_env_set_maxreaders...");
            Lmdb.mdb_env_set_maxreaders(env, 1);

            long lValue = 10485760 * 100;
            Logger.LogInfo("mdb_env_set_mapsize...");
            Lmdb.mdb_env_set_mapsize(env, lValue);
            string dir = "c:\\temp2\\cache_net4";
            Directory.CreateDirectory(dir);
            Logger.LogInfo("mdb_env_open...");
            Lmdb.mdb_env_open(env, dir, EnvironmentOpenFlags.NoSync, UnixAccessMode.Default); // UnixAccessMode.Value); // UnixAccessMode.Default);

            IntPtr txn = IntPtr.Zero;
            IntPtr parent = IntPtr.Zero;
            uint dbi = 0;

            Logger.LogInfo("mdb_txn_begin...");
            Lmdb.mdb_txn_begin(env, parent, TransactionBeginFlags.NoSync, out txn);
            Logger.LogInfo("mdb_dbi_open...");
            Lmdb.mdb_dbi_open(txn, "", DatabaseOpenFlags.Create, out dbi);

            Logger.LogInfo("mdb_dbi_close...");
            Lmdb.mdb_dbi_close(env, dbi);
            Logger.LogInfo("mdb_env_close...");
            Lmdb.mdb_env_close(env);
        }

        static void Main(string[] args)
        {
            Logger.LogInfo("Main...");
            //Main2()
            int count = 0;
            if (args.Length == 1)
            {
                count = Convert.ToInt32(args[0]);
            }

            string str = String.Format("count={0}", count);
            Logger.LogInfo(str);

            LMDBEnvironment _env;
            string dir = "c:\\temp\\cache_net10A";
            _env = new LMDBEnvironment(dir);
            //This is here to assert that previous issues with the way manager
            //classes (since removed) worked don't happen anymore.
            _env.MaxDatabases = 2;
            _env.MapSize = 10485760 * 100;
            _env.Open();

            DateTime dtStart = DateTime.Now;
            var tx = _env.BeginTransaction();
            var db = tx.OpenDatabase("DB", new DatabaseConfiguration { Flags = DatabaseOpenFlags.Create });
            for (int i = 0; i < count; i++)
            {
                string key = String.Format("key_{0}", i);
                string value = String.Format("value_{0}", i);

                tx.Put(db, key, value);
            }
            tx.Put(db, "hello", "world");
            tx.Commit();
            db.Dispose();

            DateTime dtStop = DateTime.Now;
            TimeSpan ts = dtStop - dtStart;
            str = String.Format("Time elapsed for set:{0} ms", ts.TotalMilliseconds);
            Logger.LogInfo(str);

            dtStart = DateTime.Now;
            tx = _env.BeginTransaction(TransactionBeginFlags.ReadOnly);
            db = tx.OpenDatabase("DB");
            for (int i = 0; i < count; i++)
            {
                string key = String.Format("key_{0}", i);
                //string value = String.Format("value_{0}", i);

                var value = tx.Get(db, key);
                if (i % 10000 == 0)
                {
                    string strD = String.Format("key:{0} => value:{1}", key, value);
                    Logger.LogInfo(strD);
                }
            }
            var result = tx.Get(db, "hello");
            Logger.LogInfo(result);
            tx.Commit();
            db.Dispose();

            dtStop = DateTime.Now;
            ts = dtStop - dtStart;
            str = String.Format("Time elapsed for get:{0} ms", ts.TotalMilliseconds);
            Logger.LogInfo(str);

            _env.Dispose();
        }
    }
}
