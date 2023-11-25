using System;
using System.Collections.Generic;
using System.Text;

namespace USBManagement
{
    /// <summary>
    /// Cihaz ile ba�lant� kapal� iken tekrar kapat�lmak istendi�inde 
    /// f�rlat�l�r.
    /// </summary>
    public class AlReadyCloseException : ApplicationException
    {
        public AlReadyCloseException() : base() { }
        public AlReadyCloseException(string msg) : base(msg) { }
    }
}
