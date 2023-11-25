using System;
using System.Collections.Generic;
using System.Text;

namespace USBManagement
{
    /// <summary>
    /// Cihaz ile baðlantý kuruluyken tekrar baðlantý açýlmak istendiðinde
    /// fýrlatýlýr.
    /// </summary>
    public class AlReadyDeviceException : ApplicationException
    {
        public AlReadyDeviceException() : base() { }
        public AlReadyDeviceException(string msg) : base(msg) { }
    }
}
