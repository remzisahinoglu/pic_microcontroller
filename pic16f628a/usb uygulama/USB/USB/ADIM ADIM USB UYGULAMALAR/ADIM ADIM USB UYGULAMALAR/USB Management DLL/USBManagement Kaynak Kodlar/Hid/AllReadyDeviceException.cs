using System;
using System.Collections.Generic;
using System.Text;

namespace USBManagement
{
    /// <summary>
    /// Cihaz ile ba�lant� kuruluyken tekrar ba�lant� a��lmak istendi�inde
    /// f�rlat�l�r.
    /// </summary>
    public class AlReadyDeviceException : ApplicationException
    {
        public AlReadyDeviceException() : base() { }
        public AlReadyDeviceException(string msg) : base(msg) { }
    }
}
