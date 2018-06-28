using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace LMDBNet
{
    public class DatabaseConfiguration
    {
        private IComparer<byte[]> _comparer;
        private IComparer<byte[]> _duplicatesComparer;

        public DatabaseConfiguration()
        {
            Flags = DatabaseOpenFlags.None;
        }

        public DatabaseOpenFlags Flags { get; set; }


        internal IDisposable ConfigureDatabase(LightningTransaction tx, LightningDatabase db)
        {
            var pinnedComparer = new ComparerKeepAlive();
            if (_comparer != null)
            {
                CompareFunction compare = Compare;
                pinnedComparer.AddComparer(compare);
            }
            if (_duplicatesComparer != null)
            {
                CompareFunction dupCompare = IsDuplicate;
                pinnedComparer.AddComparer(dupCompare);
            }
            return pinnedComparer;
        }

        private int Compare(ref ValueStructure left, ref ValueStructure right)
        {
            return _comparer.Compare(left.GetBytes(), right.GetBytes());
        }

        private int IsDuplicate(ref ValueStructure left, ref ValueStructure right)
        {
            return _duplicatesComparer.Compare(left.GetBytes(), right.GetBytes());
        }

        public void CompareWith(IComparer<byte[]> comparer)
        {
            _comparer = comparer;
        }

        public void FindDuplicatesWith(IComparer<byte[]> comparer)
        {
            _duplicatesComparer = comparer;
        }

        private class ComparerKeepAlive : IDisposable
        {
            private readonly List<GCHandle> _comparisons = new List<GCHandle>();

            public void AddComparer(CompareFunction compare)
            {
                var handle = GCHandle.Alloc(compare);
                _comparisons.Add(handle);
            }

            public void Dispose()
            {
                for (int i = 0; i < _comparisons.Count; ++i)
                {
                    _comparisons[i].Free();
                }
            }
        }
    }
}
