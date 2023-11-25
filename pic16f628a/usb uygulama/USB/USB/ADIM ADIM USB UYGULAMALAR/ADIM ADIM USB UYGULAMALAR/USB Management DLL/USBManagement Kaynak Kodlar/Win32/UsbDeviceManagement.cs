using System;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace USBManagement
{
    #region Device Management Base Codes

    internal class UsbDeviceManagement
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="HidGuid"></param>
        /// <param name="DevicePathName"></param>
        /// <returns></returns>
        internal bool FindUsbDevice(Guid HidGuid, ref string[] DevicePathName)
        {
            IntPtr hDeviceSet = IntPtr.Zero;
            int iSize = 0;
            int iDeviceCount = 0;
            string sDevicePath = string.Empty;
            bool Success = false;
            bool bDeviceFound = false;

            //Verilen GUID numarasýna uyan tüm cihazlar ile ilgili bir
            //aygýt arayüz seti al
            hDeviceSet = Win32.SetupDiGetClassDevs(ref HidGuid,
                                          0,
                                          IntPtr.Zero,
                                          Win32.DIGCF_PRESENT |
                                          Win32.DIGCF_DEFIVEINTERFACE
                                          );

            if (hDeviceSet == Win32.INVALID_HANDLE_VALUE) return false;

            Win32.SP_DEVICE_INTERFACE_DATA devInterfaceData = new Win32.SP_DEVICE_INTERFACE_DATA();
            Win32.SP_DEVICE_INTERFACE_DETAIL_DATA devInterfaceDetailData = new Win32.SP_DEVICE_INTERFACE_DETAIL_DATA();

            //Bir döngü içinde aygýt arayüz setinden bir eleman al ve incele
            while (Win32.SetupDiEnumDeviceInterfaces(
                                    hDeviceSet,
                                    IntPtr.Zero,
                                    ref HidGuid,
                                    iDeviceCount,
                                    devInterfaceData
                                    ))
            {

                //Adým 1
                Success = Win32.SetupDiGetDeviceInterfaceDetail(
                                           hDeviceSet,
                                           devInterfaceData,
                                           IntPtr.Zero,
                                           0,
                                           ref iSize,
                                           IntPtr.Zero
                                           );

                IntPtr DetailDataBuffer = Marshal.AllocHGlobal(iSize);
                Marshal.StructureToPtr(devInterfaceDetailData, DetailDataBuffer, false);

                //Adým 2
                Success = Win32.SetupDiGetDeviceInterfaceDetail(
                                           hDeviceSet,
                                           devInterfaceData,
                                           DetailDataBuffer,
                                           iSize,
                                           ref iSize,
                                           IntPtr.Zero
                                           );

                if (Success)
                {
                    IntPtr devPath = (IntPtr)((int)DetailDataBuffer + Marshal.SizeOf(typeof(int)));
                    sDevicePath = Marshal.PtrToStringAuto(devPath);
                    DevicePathName[iDeviceCount] = sDevicePath;
                    Marshal.FreeHGlobal(DetailDataBuffer);

                    bDeviceFound = true;
                }
                iDeviceCount++;
            }

            Win32.SetupDiDestroyDeviceInfoList(hDeviceSet);

            return bDeviceFound;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="msg"></param>
        /// <param name="myDevicePathName"></param>
        /// <returns></returns>
        internal bool CheckUsbDeviceName(Message msg, string myDevicePathName)
        {
            Win32.DEV_BROADCAST_DEVICEINTERFACE_1 DevBroadCastInterface = new Win32.DEV_BROADCAST_DEVICEINTERFACE_1();
            Win32.DEV_BROADCAST_HDR DevBroadCastHeader = new Win32.DEV_BROADCAST_HDR();

            Marshal.PtrToStructure(msg.LParam, DevBroadCastHeader);
            if (DevBroadCastHeader.dbch_devicetype == Win32.DBT_DEVTYP_DEVICEINTERFACE)
            {
                int iSize = Convert.ToInt32((DevBroadCastHeader.dbch_size - 26) / 2);
                DevBroadCastInterface.dbcc_name = new char[iSize + 1];
                Marshal.PtrToStructure(msg.LParam, DevBroadCastInterface);

                string deviceName = new string(DevBroadCastInterface.dbcc_name, 0, iSize);

                int index = deviceName.IndexOf("\0");
                string name = deviceName.Substring(0, index);

                if (string.Compare(name, myDevicePathName, StringComparison.OrdinalIgnoreCase) == 0) return true; else return false;
            }
            return false;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="FormHandle"></param>
        /// <param name="hidGuid"></param>
        /// <param name="hDeviceNotification"></param>
        /// <returns></returns>
        internal bool RegisterUsbDeviceNotifications(IntPtr FormHandle, Guid hidGuid, ref IntPtr hDeviceNotification)
        {
            int iSize = 0;
            IntPtr dbiBuffer;
            Win32.DEV_BROADCAST_DEVICEINTERFACE DevBroadCastInterface = new Win32.DEV_BROADCAST_DEVICEINTERFACE();

            iSize = Marshal.SizeOf(DevBroadCastInterface);

            DevBroadCastInterface.dbcc_size = iSize;
            DevBroadCastInterface.dbcc_devicetype = Win32.DBT_DEVTYP_DEVICEINTERFACE;
            DevBroadCastInterface.dbcc_reserved = 0;
            DevBroadCastInterface.dbcc_classguid = hidGuid;

            dbiBuffer = Marshal.AllocHGlobal(iSize);
            Marshal.StructureToPtr(DevBroadCastInterface, dbiBuffer, true);

            hDeviceNotification = Win32.RegisterDeviceNotification(
                                                   FormHandle,
                                                   dbiBuffer,
                                                   Win32.DEVICE_NOTIFY_WINDOW_HANDLE
                                                   );

            Marshal.PtrToStructure(dbiBuffer, DevBroadCastInterface);
            Marshal.FreeHGlobal(dbiBuffer);

            return (hDeviceNotification.ToInt32() == IntPtr.Zero.ToInt32()) ? false : true;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="hDeviceNotification"></param>
        /// <returns></returns>
        internal bool UnRegisterUsbDeviceNotifications(IntPtr hDeviceNotification)
        {
            bool Success = false;

            Success = Win32.UnregisterDeviceNotification(hDeviceNotification);
            return Success;
        }
    }

    #endregion
}
