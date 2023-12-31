using System;
using System.Collections.Generic;
using System.Text;

namespace USBManagement
{
    /// <summary>
    /// Cihaz sisteme kayıtlı iken tekrar kaydedilmek istendiğinde
    /// fırlatılır.
    /// </summary>
    public class AlReadyRegisteredException : ApplicationException
    {
        public AlReadyRegisteredException() : base() { }
        public AlReadyRegisteredException(string msg) : base(msg) { }
    }
}
