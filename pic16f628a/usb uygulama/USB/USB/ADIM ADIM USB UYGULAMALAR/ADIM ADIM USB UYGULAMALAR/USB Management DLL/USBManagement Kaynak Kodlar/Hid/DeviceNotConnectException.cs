using System;
using System.Collections.Generic;
using System.Text;

namespace USBManagement
{
    /// <summary>
    /// Cihaz ile ba�lant� mevcut de�ilken okuma,yazma veya kay�t
    /// i�lemi yap�lmaya �al���ld���nda f�rlat�l�r.
    /// </summary>
    public class DeviceNotConnectException : ApplicationException
    {
        public DeviceNotConnectException() : base() { }
        public DeviceNotConnectException(string msg) : base(msg) { }
    }
}
