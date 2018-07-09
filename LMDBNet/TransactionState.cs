using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LMDBNet
{
    /// <summary>
    /// Transaction state.
    /// </summary>
    public enum TransactionState
    {
        /// <summary>
        /// Transaction is currently active.
        /// </summary>
        Active,

        /// <summary>
        /// Transaction is currently reseted.
        /// </summary>
        Reseted,

        /// <summary>
        /// Transaction is aborted.
        /// </summary>
        Aborted,

        /// <summary>
        /// Transaction is commited.
        /// </summary>
        Commited
    }
}
