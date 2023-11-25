using System;
using System.Collections.Generic;
using System.Text;

namespace USBManagement
{
    /// <summary>
    /// Ge�ersiz okuma handle'� tespit edildi�inde f�rlat�l�r.
    /// </summary>
    public class InvalidHandleException : ApplicationException
    {
        public InvalidHandleException() : base() { }
        public InvalidHandleException(string msg) : base(msg) { }
    }
}
