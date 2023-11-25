using System;
using System.Collections.Generic;
using System.Text;

namespace USBManagement
{
    /// <summary>
    /// Cihaz ile baðlantý mevcut deðilken okuma,yazma veya kayýt
    /// iþlemi yapýlmaya çalýþýldýðýnda fýrlatýlýr.
    /// </summary>
    public class DeviceNotConnectException : ApplicationException
    {
        public DeviceNotConnectException() : base() { }
        public DeviceNotConnectException(string msg) : base(msg) { }
    }
}
