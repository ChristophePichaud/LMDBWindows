    using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LMDBNet
{
    /// <summary>
    /// An exception caused by lmdb operations.
    /// </summary>
    public class LMDBException : Exception
    {
        internal LMDBException(string message, int statusCode) : base(message)
        {
            StatusCode = statusCode;
        }

        internal LMDBException(string message, Exception innerException) : base(message, innerException)
        {
        }

        /// <summary>
        /// The status code LMDB returned from an operation.
        /// </summary>
        public int StatusCode { get; }

        public override string ToString()
        {
            return $"LMDB Exception {StatusCode}: {Message}";
        }
    }
}
