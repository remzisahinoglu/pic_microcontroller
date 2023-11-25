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
        /// Kullanýlan bellek miktarýný döndürür.
        /// </summary>
        long mUsed;
        public long Used
        {
            get { return mUsed; }
            set { mUsed = value; }
        }

        /// <summary>
        /// Disk'in boþ kapasite bilgisini döndürür.
        /// </summary>
        long mFree;
        public long Free
        {
            get { return mFree; }
            set { mFree = value; }
        }

        /// <summary>
        /// Disk içerisindeki toplam dosya adedini döndürür.
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
        /// Disk'in toplam boyutunu byte olarak döndürür.
        /// </summary>
        long mTotalSize;
        public long TotalSize
        {
            get { return mTotalSize; }
            set { mTotalSize = value; }
        }

        /// <summary>
        /// Disk içerisindeki toplam dizin sayýsýný döndürür.
        /// </summary>
        public int TotalDirectory
        {
            get
            {
                return Directory.GetDirectories(mDrive).Length;
            }
        }

        /// <summary>
        /// Cihazýn GUID numarasýný döndürür.
        /// </summary>
        Guid mGuid;
        public Guid DeviceGuid
        {
            get { return mGuid; }
            set { mGuid = value; }
        }

        /// <summary>
        /// Cihazýn Path bilgisini döndürür.
        /// </summary>
        string mPath;
        public string DevicePath
        {
            get { return mPath; }
            set { mPath = value; }
        }

        /// <summary>
        /// Cihazýn sürücü ismini döndürür.
        /// </summary>
        string mDrive;
        public string Drive
        {
            get { return mDrive; }
            set { mDrive = value; }
        }

        /// <summary>
        /// Cihazýn disk ismini döndürür.
        /// </summary>
        string mVolumeName;
        public string VolumeName
        {
            get { return mVolumeName; }
            set { mVolumeName = value; }
        }

        /// <summary>
        /// Cihazýn seri numarasýný döndürür.
        /// </summary>
        string mSerialNumber;
        public string SerialNumber
        {
            get { return mSerialNumber; }
            set { mSerialNumber = value; }
        }

        /// <summary>
        /// Disk'in biçimlendirildiði dosya sistemini döndürür.
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
