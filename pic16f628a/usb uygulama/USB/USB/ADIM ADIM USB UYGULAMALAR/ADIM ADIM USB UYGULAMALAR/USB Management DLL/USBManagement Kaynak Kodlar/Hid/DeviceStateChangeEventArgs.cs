using System;
using System.Collections.Generic;
using System.Text;

namespace USBManagement
{
    /// <summary>
    /// Baðlantý deðiþikliklerinde durum bilgisini veren
    /// olay argümaný
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
