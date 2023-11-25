using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace UsbToolManager
{
    internal class Win32
    {
        #region Const

        public const int DBT_DEVTYP_VOLUME = 2;
        public const int DBT_DEVTYP_HANDLE = 6;
        public const int BROADCAST_QUERY_DENY = 0x424D5144;
        public const int DBT_DEVICEQUERYREMOVE = 0x8001;
        public const int WM_DEVICECHANGE = 0x219;
        public const int DBT_DEVICEARRIVAL = 0x8000;
        public const int DBT_DEVICEREMOVECOMPLETE = 0x8004;

        #endregion

        #region Struct

        [StructLayout(LayoutKind.Sequential)]
        public struct DEV_BROADCAST_VOLUME
        {
            public int dbcv_size;
            public int dbcv_devicetype;
            public int dbcv_reserved;
            public int dbcv_unitmask;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct DEV_BROADCAST_HANDLE
        {
            public int dbch_size;
            public int dbch_devicetype;
            public int dbch_reserved;
            public IntPtr dbch_handle;
            public IntPtr dbch_hdevnotify;
            public Guid dbch_eventguid;
            public long dbch_nameoffset;
            public byte dbch_data;
            public byte dbch_data1;
        }

        #endregion

        #region API

        /// <summary>
        /// 
        /// </summary>
        /// <param name="section"></param>
        /// <param name="key"></param>
        /// <param name="def"></param>
        /// <param name="retVal"></param>
        /// <param name="size"></param>
        /// <param name="filePath"></param>
        /// <returns></returns>
        [DllImport("kernel32.dll")]
        public static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="hInst"></param>
        /// <param name="lpszExeFileName"></param>
        /// <param name="nIconIndex"></param>
        /// <returns></returns>
        [DllImport("shell32.dll")]
        public static extern IntPtr ExtractIcon(IntPtr hInst, string lpszExeFileName, int nIconIndex);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="lpRootPathName"></param>
        /// <param name="lpSectorsPerCluster"></param>
        /// <param name="lpBytesPerSector"></param>
        /// <param name="lpNumberOfFreeClusters"></param>
        /// <param name="lpTotalNumberOfClusters"></param>
        /// <returns></returns>
        [DllImport("kernel32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        public static extern bool GetDiskFreeSpace(
                                  string lpRootPathName,
                                  out uint lpSectorsPerCluster,
                                  out uint lpBytesPerSector,
                                  out uint lpNumberOfFreeClusters,
                                  out uint lpTotalNumberOfClusters
                                  );

        /// <summary>
        /// 
        /// </summary>
        /// <param name="RootPathName"></param>
        /// <param name="VolumeNameBuffer"></param>
        /// <param name="VolumeNameSize"></param>
        /// <param name="VolumeSerialNumber"></param>
        /// <param name="MaximumComponentLength"></param>
        /// <param name="FileSystemFlags"></param>
        /// <param name="FileSystemNameBuffer"></param>
        /// <param name="nFileSystemNameSize"></param>
        /// <returns></returns>
        [DllImport("Kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public extern static bool GetVolumeInformation(
                                  string RootPathName,
                                  StringBuilder VolumeNameBuffer,
                                  int VolumeNameSize,
                                  out uint VolumeSerialNumber,
                                  out uint MaximumComponentLength,
                                  out uint FileSystemFlags,
                                  StringBuilder FileSystemNameBuffer,
                                  int nFileSystemNameSize
                                  );

        //Usb aygýtýn çýkartýlma yada takýlma olaylarýný uygulama tarafýndan
        //yakalayabilmek için aygýt'ý kayýt eden api
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern IntPtr RegisterDeviceNotification(
                                    IntPtr hRecipient,
                                    IntPtr NotificationFilter,
                                    int Flags
                                    );

        //RegisterDeviceNotification ile yapýlan kaydý kaldýrýr
        [DllImport("user32.dll")]
        public static extern Boolean UnregisterDeviceNotification(IntPtr Handle);

        #endregion
    }
}
