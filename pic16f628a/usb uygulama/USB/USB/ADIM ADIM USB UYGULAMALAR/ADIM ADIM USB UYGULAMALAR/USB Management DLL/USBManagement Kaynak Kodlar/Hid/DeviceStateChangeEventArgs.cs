using System;
using System.Collections.Generic;
using System.Text;

namespace USBManagement
{
    /// <summary>
    /// Bağlantı değişikliklerinde durum bilgisini veren
    /// olay argümanı
    /// </summary>
    public class DeviceStateChangeEventArgs : EventArgs
    {
        DeviceState state = DeviceState.None;

        public DeviceStateChangeEventArgs() : base() { }
        public DeviceState CurrentState
        {
            get { return state; }
            set { state = value; }
        }
    }
}
