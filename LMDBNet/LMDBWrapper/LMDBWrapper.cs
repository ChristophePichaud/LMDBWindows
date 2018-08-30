using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LMDBNet
{
    public class LMDBWrapper
    {
        private LMDBEnvironment _env;
        private string _dbName;

        public LMDBWrapper()
        {
        }

        ~LMDBWrapper()
        {
            _env.Dispose();
        }

        public void Init(string dbName)
        {
            _dbName = dbName;

            string ctemp = "c:\\temp";
            string path = String.Format("{0}\\{1}", ctemp, dbName);

            Directory.CreateDirectory(path);

            _env = new LMDBEnvironment(path);
            _env.MaxDatabases = 2;
            _env.MapSize = 10485760 * 100;
            _env.Open();
        }

        public void SetData(string key, string value)
        {
            LMDBTransaction tx = _env.BeginTransaction();
            LMDBDatabase db = tx.OpenDatabase(_dbName, new DatabaseConfiguration { Flags = DatabaseOpenFlags.Create });
            tx.Put(db, key, value);
            tx.Commit();
            db.Dispose();
        }

        public void GetData(string key, out string value)
        {
            LMDBTransaction tx = _env.BeginTransaction();
            LMDBDatabase db = tx.OpenDatabase(_dbName, new DatabaseConfiguration { Flags = DatabaseOpenFlags.Create });
            value = tx.Get(db, key);
            tx.Commit();
            db.Dispose();
        }
    }
}
