using System;
using System.Collections.Generic;
using System.Text;

namespace USBManagement
{
    /// <summary>
    /// Cihaz sisteme kay�tl� iken tekrar kaydedilmek istendi�inde
    /// f�rlat�l�r.
    /// </summary>
    public class AlReadyRegisteredException : ApplicationException
    {
        public AlReadyRegisteredException() : base() { }
        public AlReadyRegisteredException(string msg) : base(msg) { }
    }
}
