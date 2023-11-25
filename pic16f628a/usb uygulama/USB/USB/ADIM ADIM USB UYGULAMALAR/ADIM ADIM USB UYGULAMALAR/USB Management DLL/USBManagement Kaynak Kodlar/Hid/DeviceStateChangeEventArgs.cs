using System;
using System.Collections.Generic;
using System.Text;

namespace USBManagement
{
    /// <summary>
    /// Ba�lant� de�i�ikliklerinde durum bilgisini veren
    /// olay arg�man�
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
