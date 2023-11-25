using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace UsbToolManager
{
    #region Event Args

    /// <summary>
    /// 
    /// </summary>
    public class DiskDeviceArrivedEventArgs : EventArgs
    {

        /// <summary>
        /// 
        /// </summary>
        Volume mVol;

        /// <summary>
        /// 
        /// </summary>
        public Volume DiskInfo
        {
            get { return mVol; }
            set { mVol = value; }
        }

        public DiskDeviceArrivedEventArgs() : base() { }
    }

    /// <summary>
    /// 
    /// </summary>
    public class DiskDeviceQueryRemoveEventArgs : EventArgs
    {
        /// <summary>
        /// 
        /// </summary>
        bool mCancel;

        /// <summary>
        /// 
        /// </summary>
        string mDrive;

        /// <summary>
        /// 
        /// </summary>
        public bool Cancel
        {
            get { return mCancel; }
            set { mCancel = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string QueryRemoveDisk
        {
            get { return mDrive; }
            set { mDrive = value; }
        }

        public DiskDeviceQueryRemoveEventArgs() : base() { }
    }

    /// <summary>
    /// 
    /// </summary>
    public class DiskDeviceRemovedEventArgs : EventArgs
    {
        /// <summary>
        /// 
        /// </summary>
        string mDrive;

        /// <summary>
        /// 
        /// </summary>
        public string RemovedDrive
        {
            get { return mDrive; }
            set { mDrive = value; }
        }

        public DiskDeviceRemovedEventArgs() : base() { }
    }

    #endregion

    #region Event Delegate

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    public delegate void DiskDeviceArrivedEventHandler(object sender, DiskDeviceArrivedEventArgs e);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    public delegate void DiskDeviceRemovedEventHandler(object sender, DiskDeviceRemovedEventArgs e);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    public delegate void DiskQueryRemoveEventHandler(object sender, DiskDeviceQueryRemoveEventArgs e);

    #endregion

    public class UsbDiskDevice
    {
        #region Events

        /// <summary>
        /// 
        /// </summary>
        public event DiskQueryRemoveEventHandler DiskQueryRemove;

        /// <summary>
        /// 
        /// </summary>
        public event DiskDeviceArrivedEventHandler DiskDeviceArrived;

        /// <summary>
        /// 
        /// </summary>
        public event DiskDeviceRemovedEventHandler DiskDeviceRemoved;

        #endregion

        #region Variable

        /// <summary>
        /// 
        /// </summary>
        IntPtr hNotify;

        /// <summary>
        /// 
        /// </summary>
        bool mLock;

        /// <summary>
        /// 
        /// </summary>
        Volume mVol;

        /// <summary>
        /// 
        /// </summary>
        int mDevType;

        /// <summary>
        /// 
        /// </summary>
        FileStream mKeyfile;

        #endregion

        #region Propertys

        /// <summary>
        /// 
        /// </summary>
        public Volume DiskInfo
        {
            get { return mVol; }
            set { mVol = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public bool IsLock
        {
            get { return mLock; }
            set { mLock = value; }
        }

        #endregion

        #region Public Methods

        /// <summary>
        /// Dosya içinde bir kilit dosyasý oluþturur.Bu dosyayý
        /// kullanýma açtýktan sonra Handle'ýný sisteme kaydeder.
        /// Disk kaldýrýlmak istendiðinde Handle'de iþletim sistemince
        /// kaldýrýlmak istendiðinden ilgili mesaj ProcessDeviceMessage tarafýndan 
        /// yakalanýr ve disk'in çýkartýlmasýna izin verilmez.
        /// </summary>
        /// <param name="disk">Kitlenecek disk bilgileri</param>
        /// <returns>Ýþlem baþarýlý ise true aksi halde false</returns>
        public bool LockDisk(Volume disk, IntPtr Handle)
        {
            mKeyfile = new FileStream(disk.Drive + "keyfile.uky", FileMode.OpenOrCreate);

            Win32.DEV_BROADCAST_HANDLE hData = new Win32.DEV_BROADCAST_HANDLE();
            hData.dbch_devicetype = Win32.DBT_DEVTYP_HANDLE;
            hData.dbch_reserved = 0;
            hData.dbch_nameoffset = 0;
            hData.dbch_hdevnotify = IntPtr.Zero;
            hData.dbch_size = Marshal.SizeOf(hData);
            hData.dbch_handle = mKeyfile.SafeFileHandle.DangerousGetHandle();

            IntPtr buffer = Marshal.AllocHGlobal(hData.dbch_size);
            Marshal.StructureToPtr(hData, buffer, true);

            hNotify = Win32.RegisterDeviceNotification(Handle, buffer, 0);
            if (hNotify.ToInt32() != IntPtr.Zero.ToInt32())
            {
                mLock = true;
                return mLock;
            }
            return false;
        }

        /// <summary>
        /// LockDisk ile kilitlenmiþ USB disk'i serbest býrakýr.
        /// </summary>
        /// <param name="disk">Serbest býrakýlacak disk bilgileri</param>
        /// <returns>Ýþlem baþarýlý ise true aksi halde false</returns>
        public bool ReleaseDisk(Volume disk)
        {
            bool Success = false;

            if (mLock)
            {
                Success = Win32.UnregisterDeviceNotification(hNotify);
                if (Success)
                {
                    mLock = false;
                    return Success;
                }
            }
            return Success;
        }

        /// <summary>
        /// Belirtilen disk'i sistemden kaldýrýr.Bu iþlem LockDisk'i
        /// geçersiz kýlar.
        /// </summary>
        /// <param name="disk">Sistemden çýkartýlacak disk bilgileri</param>
        /// <returns>Ýþlem baþarýlý ise true aksi halde false</returns>
        public bool EjectDisk(Volume disk)
        {
            return false;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="m"></param>
        public void ProcessDeviceMessage(Message m)
        {
            if (m.Msg == Win32.WM_DEVICECHANGE)
            {
                switch (m.WParam.ToInt32())
                {
                    case Win32.DBT_DEVICEARRIVAL:
                        mDevType = Marshal.ReadInt32(m.LParam, 4);
                        if (mDevType == Win32.DBT_DEVTYP_VOLUME)
                        {
                            Win32.DEV_BROADCAST_VOLUME volume = (Win32.DEV_BROADCAST_VOLUME)
                            Marshal.PtrToStructure(m.LParam, typeof(Win32.DEV_BROADCAST_VOLUME));

                            mVol = new Volume();
                            GetUsbDiskDeviceInfo(mVol, volume);
                            if (DiskDeviceArrived != null)
                            {
                                DiskDeviceArrivedEventArgs e = new DiskDeviceArrivedEventArgs();
                                e.DiskInfo = mVol;
                                DiskDeviceArrived(this, e);
                            }
                        }
                        break;

                    case Win32.DBT_DEVICEQUERYREMOVE:
                        if (!IsLock) return;
                        if (DiskQueryRemove != null)
                        {
                            DiskDeviceQueryRemoveEventArgs e = new DiskDeviceQueryRemoveEventArgs();
                            e.QueryRemoveDisk = mVol.Drive;
                            DiskQueryRemove(this, e);

                            if (!e.Cancel)
                                m.Result = (IntPtr)Win32.BROADCAST_QUERY_DENY;
                            else
                                mKeyfile.Close();
                        }
                        else
                        {
                            m.Result = (IntPtr)Win32.BROADCAST_QUERY_DENY;
                        }
                        break;

                    case Win32.DBT_DEVICEREMOVECOMPLETE:
                        if (DiskDeviceRemoved != null)
                        {
                            DiskDeviceRemovedEventArgs e = new DiskDeviceRemovedEventArgs();
                            e.RemovedDrive = mVol.Drive;
                            DiskDeviceRemoved(this, e);
                        }

                        if (mKeyfile != null) mKeyfile.Close();
                        ReleaseDisk(mVol);

                        mVol = null;
                        mLock = false;
                        break;
                }
            }
        }

        #endregion

        #region Private Methods

        private void GetUsbDiskDeviceInfo(Volume mngVol, Win32.DEV_BROADCAST_VOLUME umngVol)
        {
            char drive = GetDriveMaskToLetter(umngVol.dbcv_unitmask);
            mngVol.Drive = drive + ":\\";

            uint serialNumber, maxCompLength, fileSystemFlag;
            StringBuilder sbFileSystem = new StringBuilder(10);
            StringBuilder sbVolumeName = new StringBuilder(255);

            //Disk seri numarasý, volume adý ve dosya sistemi bilgileri
            Win32.GetVolumeInformation(
                             mngVol.Drive,
                             sbVolumeName,
                             sbVolumeName.Capacity,
                             out serialNumber,
                             out maxCompLength,
                             out fileSystemFlag,
                             sbFileSystem,
                             sbFileSystem.Capacity
                             );

            mngVol.FileSystem = sbFileSystem.ToString();
            mngVol.VolumeName = sbVolumeName.ToString();
            mngVol.SerialNumber = serialNumber.ToString("x");
            
            /*
            //Bellek kapasitesini öðren
            uint iSectorPerCluster, iBytesPerSector, iNumberOfFreeCluster, iNumberOfTotalNumber;

            UsbManagementApi.GetDiskFreeSpace(
                             mngVol.Drive,
                             out iSectorPerCluster,
                             out iBytesPerSector,
                             out iNumberOfFreeCluster,
                             out iNumberOfTotalNumber
                             );
             */
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="Mask"></param>
        /// <returns></returns>
        private char GetDriveMaskToLetter(int Mask)
        {
            string allDrives = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

            int Cnt = 0;
            int drvMask = Mask / 2;

            while (drvMask != 0)
            {
                drvMask /= 2;
                Cnt++;
            }
            if (Cnt < allDrives.Length)
                return allDrives[Cnt];
            else
                return '?';
        }

        #endregion
    }
}
