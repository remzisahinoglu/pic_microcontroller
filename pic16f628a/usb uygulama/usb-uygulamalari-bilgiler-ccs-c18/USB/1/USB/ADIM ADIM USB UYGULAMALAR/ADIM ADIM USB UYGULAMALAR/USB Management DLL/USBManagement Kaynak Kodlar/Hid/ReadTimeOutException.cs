using System;
using System.Collections.Generic;
using System.Text;

namespace USBManagement
{
    /// <summary>
    /// Okuma iþlemi zaman aþýmýna uðradýðý zaman fýrlatýlýr.
    /// </summary>
    public class ReadTimeOutException : ApplicationException
    {
        public ReadTimeOutException() : base() { }
        public ReadTimeOutException(string msg) : base(msg) { }
    }
}
