using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LMDBNet
{
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate int CompareFunction(ref ValueStructure left, ref ValueStructure right);
}
