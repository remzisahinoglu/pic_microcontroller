using System;
using System.IO;
using System.Collections.Generic;
using System.Text;

namespace UsbToolManager
{
    public sealed class Volume
    {
        #region Properties

        /// <summary>
        /// 
        /// </summary>
        uint iNumberOfFreeCluster;
        public uint NumberOfFreeCluster
        {
            get { return iNumberOfFreeCluster; }
            set { iNumberOfFreeCluster = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        uint mNumberOfSectorCluster;
        public uint NumberOfSectorCluster
        {
            get { return mNumberOfSectorCluster; }
            set { mNumberOfSectorCluster = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        uint mNumberOfSector;
        public uint NumberOfSectorByte
        {
            get { return mNumberOfSector; }
            set { mNumberOfSector = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        long mFreeSector;
        public long FreeSector
        {
            get { return mFreeSector; }
            set { mFreeSector = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        long mTotalSector;
        public long TotalSector
        {
            get { return mTotalSector; }
            set { mTotalSector = value; }
        }

        /// <summary>
        /// Kullan�lan bellek miktar�n� d�nd�r�r.
        /// </summary>
        long mUsed;
        public long Used
        {
            get { return mUsed; }
            set { mUsed = value; }
        }

        /// <summary>
        /// Disk'in bo� kapasite bilgisini d�nd�r�r.
        /// </summary>
        long mFree;
        public long Free
        {
            get { return mFree; }
            set { mFree = value; }
        }

        /// <summary>
        /// Disk i�erisindeki toplam dosya adedini d�nd�r�r.
        /// </summary>
        public int TotalFile
        {
            get
            {
                return Directory.GetFiles(
                                 mDrive,
                                 "*.*",
                                 SearchOption.AllDirectories
                                 ).Length;
            }
        }

        /// <summary>
        /// Disk'in toplam boyutunu byte olarak d�nd�r�r.
        /// </summary>
        long mTotalSize;
        public long TotalSize
        {
            get { return mTotalSize; }
            set { mTotalSize = value; }
        }

        /// <summary>
        /// Disk i�erisindeki toplam dizin say�s�n� d�nd�r�r.
        /// </summary>
        public int TotalDirectory
        {
            get
            {
                return Directory.GetDirectories(mDrive).Length;
            }
        }

        /// <summary>
        /// Cihaz�n GUID numaras�n� d�nd�r�r.
        /// </summary>
        Guid mGuid;
        public Guid DeviceGuid
        {
            get { return mGuid; }
            set { mGuid = value; }
        }

        /// <summary>
        /// Cihaz�n Path bilgisini d�nd�r�r.
        /// </summary>
        string mPath;
        public string DevicePath
        {
            get { return mPath; }
            set { mPath = value; }
        }

        /// <summary>
        /// Cihaz�n s�r�c� ismini d�nd�r�r.
        /// </summary>
        string mDrive;
        public string Drive
        {
            get { return mDrive; }
            set { mDrive = value; }
        }

        /// <summary>
        /// Cihaz�n disk ismini d�nd�r�r.
        /// </summary>
        string mVolumeName;
        public string VolumeName
        {
            get { return mVolumeName; }
            set { mVolumeName = value; }
        }

        /// <summary>
        /// Cihaz�n seri numaras�n� d�nd�r�r.
        /// </summary>
        string mSerialNumber;
        public string SerialNumber
        {
            get { return mSerialNumber; }
            set { mSerialNumber = value; }
        }

        /// <summary>
        /// Disk'in bi�imlendirildi�i dosya sistemini d�nd�r�r.
        /// </summary>
        string mFileSystem;
        public string FileSystem
        {
            get { return mFileSystem; }
            set { mFileSystem = value; }
        }

        #endregion
    }
}
