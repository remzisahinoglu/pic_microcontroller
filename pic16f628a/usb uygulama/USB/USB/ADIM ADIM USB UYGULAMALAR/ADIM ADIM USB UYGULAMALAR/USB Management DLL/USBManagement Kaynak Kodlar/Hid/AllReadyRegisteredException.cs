using System;
using System.Collections.Generic;
using System.Text;

namespace USBManagement
{
    /// <summary>
    /// Cihaz sisteme kayýtlý iken tekrar kaydedilmek istendiðinde
    /// fýrlatýlýr.
    /// </summary>
    public class AlReadyRegisteredException : ApplicationException
    {
        public AlReadyRegisteredException() : base() { }
        public AlReadyRegisteredException(string msg) : base(msg) { }
    }
}
