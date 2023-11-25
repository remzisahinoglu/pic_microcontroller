using System;
using System.Collections.Generic;
using System.Text;

namespace USBManagement
{
    /// <summary>
    /// Okuma i�lemi zaman a��m�na u�rad��� zaman f�rlat�l�r.
    /// </summary>
    public class ReadTimeOutException : ApplicationException
    {
        public ReadTimeOutException() : base() { }
        public ReadTimeOutException(string msg) : base(msg) { }
    }
}
