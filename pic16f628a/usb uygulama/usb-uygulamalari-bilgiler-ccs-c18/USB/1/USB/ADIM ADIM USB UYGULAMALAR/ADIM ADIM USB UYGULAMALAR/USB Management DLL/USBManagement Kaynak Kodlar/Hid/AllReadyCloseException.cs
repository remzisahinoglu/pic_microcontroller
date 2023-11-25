using System;
using System.Collections.Generic;
using System.Text;

namespace USBManagement
{
    /// <summary>
    /// Cihaz ile baðlantý kapalý iken tekrar kapatýlmak istendiðinde 
    /// fýrlatýlýr.
    /// </summary>
    public class AlReadyCloseException : ApplicationException
    {
        public AlReadyCloseException() : base() { }
        public AlReadyCloseException(string msg) : base(msg) { }
    }
}
