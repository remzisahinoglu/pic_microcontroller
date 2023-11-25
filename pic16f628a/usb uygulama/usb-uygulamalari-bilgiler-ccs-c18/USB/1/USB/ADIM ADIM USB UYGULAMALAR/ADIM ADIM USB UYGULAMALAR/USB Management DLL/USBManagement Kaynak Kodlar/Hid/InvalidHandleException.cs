using System;
using System.Collections.Generic;
using System.Text;

namespace USBManagement
{
    /// <summary>
    /// Geçersiz okuma handle'ý tespit edildiðinde fýrlatýlýr.
    /// </summary>
    public class InvalidHandleException : ApplicationException
    {
        public InvalidHandleException() : base() { }
        public InvalidHandleException(string msg) : base(msg) { }
    }
}
