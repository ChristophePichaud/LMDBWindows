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
        static void Main(string[] args)
        {
            Logger.LogInfo("Main...");

            int count = 100000;
            if (args.Length == 1)
            {
                count = Convert.ToInt32(args[0]);
            }

            string str = String.Format("count={0}", count);
            Logger.LogInfo(str);

            using (LMDBEnvironment env = CreateEnv())
            {
                CreateAnEntry(env);
                CreateSimplesKeyValue(env, count);
                GetSimplesKeyValue(env, count);
                EnumData(env);
                Logger.LogInfo("auto env.Dispose...");
                //env.Dispose();
            }

            Logger.LogInfo("exit...");
        }

        private static LMDBEnvironment CreateEnv()
        {
            Logger.LogInfo("CreateEnv");

            DateTime dt = DateTime.Now;

            LMDBEnvironment env;
            string dir = "c:\\temp\\mycache_" 
                + dt.Hour.ToString() + dt.Minute.ToString() + dt.Second.ToString(); // cache_net10A";
            env = new LMDBEnvironment(dir);
            //This is here to assert that previous issues with the way manager
            //classes (since removed) worked don't happen anymore.
            env.MaxDatabases = 2;
            env.MapSize = 10485760 * 10;
            env.Open();
            return env;
        }

        private static void CreateAnEntry(LMDBEnvironment env)
        {
            Logger.LogInfo("CreateAnEntry");

            var tx = env.BeginTransaction();
            using (var db = tx.OpenDatabase(null, 
                    new DatabaseConfiguration { Flags = DatabaseOpenFlags.Create }))
            {
                tx.Put(db, "hello", "world");

                var result = tx.Get(db, "hello");
                Logger.LogInfo("Value for key hello =>" + result);
                tx.Commit();
            }
        }

        private static void CreateSimplesKeyValue(LMDBEnvironment env, int count)
        {
            Logger.LogInfo("CreateSimplesKeyValue");

            DateTime dtStart = DateTime.Now;
            LMDBTransaction tx = env.BeginTransaction(TransactionBeginFlags.NoSync);
            using (var db = tx.OpenDatabase(null, 
                    new DatabaseConfiguration { Flags = DatabaseOpenFlags.Create }))
            {
                Logger.LogInfo("count:" + count);
                for (int i = 0; i < count; i++)
                {
                    string key = String.Format("key_{0}", i);
                    string value = String.Format("value_{0}", i);

                    tx.Put(db, key, value);
                }
                tx.Commit();
            }

            DateTime dtStop = DateTime.Now;
            TimeSpan ts = dtStop - dtStart;
            string str = String.Format("Time elapsed for set:{0} ms", ts.TotalMilliseconds);
            Logger.LogInfo(str);
        }

        private static void GetSimplesKeyValue(LMDBEnvironment env, int count)
        {
            Logger.LogInfo("GetSimplesKeyValue");

            DateTime dtStart = DateTime.Now;
            LMDBTransaction tx = env.BeginTransaction(TransactionBeginFlags.ReadOnly);
            using (LMDBDatabase db = tx.OpenDatabase(null))
            {
                Logger.LogInfo("count:" + count);
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
            }

            DateTime dtStop = DateTime.Now;
            TimeSpan ts = dtStop - dtStart;
            string str = String.Format("Time elapsed for get:{0} ms", ts.TotalMilliseconds);
            Logger.LogInfo(str);
        }

    
        private static void EnumData(LMDBEnvironment env)
        {
            Logger.LogInfo("EnumData");

            DateTime dtStart = DateTime.Now;
            LMDBTransaction tx = env.BeginTransaction(TransactionBeginFlags.NoSync);
            using (LMDBDatabase db = tx.OpenDatabase(null, 
                    new DatabaseConfiguration { Flags = DatabaseOpenFlags.Create }))
            {
                var cur = tx.CreateCursor(db);
                //cur.MoveToFirst();
                int count1 = 0;
                while (cur.MoveNext())
                {
                    var fKey = Encoding.UTF8.GetString(cur.Current.Key);
                    var fValue = Encoding.UTF8.GetString(cur.Current.Value);
                    if (count1 % 10000 == 0)
                    {
                        string str3 = String.Format("count:{2} - key:{0} => value:{1}", fKey, fValue, count1);
                        Logger.LogInfo(str3);
                    }

                    ++count1;
                }
                tx.Commit();
            }

            DateTime dtStop = DateTime.Now;
            TimeSpan ts = dtStop - dtStart;
            string str = String.Format("Time elapsed for enum:{0} ms", ts.TotalMilliseconds);
            Logger.LogInfo(str);
        }

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
    }
}
