using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LMDBNet
{
    public struct ValueStructure
    {
        public IntPtr size;

        public IntPtr data;

        public byte[] GetBytes()
        {
            var buffer = new byte[size.ToInt32()];
            Marshal.Copy(data, buffer, 0, buffer.Length);
            return buffer;
        }
    }

}
