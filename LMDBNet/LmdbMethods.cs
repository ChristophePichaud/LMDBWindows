using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace LMDBNet
{
    internal static class LmdbMethods
    {
        public const string dll = "LMDBWindowsDllD64";

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mdb_env_create(out IntPtr env);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mdb_env_close(IntPtr env);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int mdb_env_open(IntPtr env, string path, EnvironmentOpenFlags flags, UnixAccessMode mode);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mdb_env_set_mapsize(IntPtr env, IntPtr size);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mdb_env_get_maxreaders(IntPtr env, out uint readers);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mdb_env_set_maxreaders(IntPtr env, uint readers);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mdb_env_set_maxdbs(IntPtr env, uint dbs);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mdb_dbi_open(IntPtr txn, string name, DatabaseOpenFlags flags, out uint db);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mdb_dbi_close(IntPtr env, uint dbi);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mdb_drop(IntPtr txn, uint dbi, bool del);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mdb_txn_begin(IntPtr env, IntPtr parent, TransactionBeginFlags flags, out IntPtr txn);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mdb_txn_commit(IntPtr txn);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mdb_txn_abort(IntPtr txn);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mdb_txn_reset(IntPtr txn);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mdb_txn_renew(IntPtr txn);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mdb_version(out int major, out int minor, out int patch);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mdb_strerror(int err);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mdb_stat(IntPtr txn, uint dbi, out MDBStat stat);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mdb_env_copy(IntPtr env, string path);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mdb_env_copy2(IntPtr env, string path, EnvironmentCopyFlags copyFlags);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mdb_env_info(IntPtr env, out MDBEnvInfo stat);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mdb_env_stat(IntPtr env, out MDBStat stat);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mdb_env_sync(IntPtr env, bool force);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mdb_get(IntPtr txn, uint dbi, ref ValueStructure key, out ValueStructure data);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mdb_put(IntPtr txn, uint dbi, ref ValueStructure key, ref ValueStructure data, PutOptions flags);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mdb_del(IntPtr txn, uint dbi, ref ValueStructure key, ref ValueStructure data);

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mdb_del(IntPtr txn, uint dbi, ref ValueStructure key, IntPtr data);
    }
}
