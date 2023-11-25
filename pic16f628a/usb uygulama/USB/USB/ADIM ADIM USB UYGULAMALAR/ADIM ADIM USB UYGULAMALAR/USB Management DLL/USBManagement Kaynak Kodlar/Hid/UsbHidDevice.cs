#region Namespace Declaration

using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Threading;
using System.ComponentModel;
using System.Drawing.Design;
using System.Diagnostics;

#endregion

namespace USBManagement
{
    #region Enums

    /// <summary>
    /// Veri tranferi iþlemlerinin kontrol yada
    /// kesme transfer ile yapýlmasýný belirten numaralandýrýcý
    /// </summary>
    public enum TransactionType 
    { 
        Control, 
        Interrupt 
    }

    /// <summary>
    /// Verilen bilgilere göre bulunan cihazýn tipini
    /// gösteren numaralandýrýcý
    /// </summary>
    public enum DeviceType 
    { 
        Unknown, 
        Keyboard, 
        Mouse 
    }

    /// <summary>
    /// Baðlantý durumundaki deðiþiklikleri gösteren
    /// numaralandýrýcý
    /// </summary>
    public enum DeviceState 
    { 
        None, 
        Opened, 
        Closed, 
        Waiting, 
        Ready 
    }

    /// <summary>
    /// 
    /// </summary>
    public enum InterfaceType
    {
        Hid,
        Other
    }

    #endregion

    #region Event Delegate

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    public delegate void HidDeviceArrivedEventHandler(object sender, EventArgs e);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    public delegate void HidDeviceRemovedEventHandler(object sender, EventArgs e);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    public delegate void DeviceStateChangeEventHandler(object sender, DeviceStateChangeEventArgs e);

    #endregion

    public sealed class UsbHidDevice : Component
    {
        #region Constructor and Deconstructor

        /// <summary>
        /// Default Constructor
        /// </summary>
        public UsbHidDevice()
        {
        }

        /// <summary>
        /// Aþýrý yüklenmiþ kurucu method.Nesne oluþturulurken
        /// bu kurucu method ile oluþturulursa USB cihaz ile baðlantý açýlýr.
        /// </summary>
        /// <param name="VendorId">Üretici kimliði</param>
        /// <param name="ProductId">Ürün kimliði</param>
        public UsbHidDevice(short VendorId, short ProductId)
        {
            OpenPipe(VendorId, ProductId);
        }

        //Yok edici method
        ~UsbHidDevice()
        {
            this.Dispose(false);
        }

        #endregion
         
        #region Dispose Component

        /// <summary>
        /// 
        /// </summary>
        /// <param name="disposing"></param>
        protected override void Dispose(bool disposing)
        {
            if (!isDisposed)
            {
                if (disposing)
                {
                    //Unmanaged Resource
                    if (IsOpen) ClosePipe();
                    isDisposed = true;

                    //Managed Resource
                }
                else
                {
                    if (IsOpen) ClosePipe();
                    isDisposed = true;
                }
            }

            base.Dispose(disposing);
        }

        #endregion

        #region Events

        /// <summary>
        /// 
        /// </summary>
        public event HidDeviceRemovedEventHandler DeviceRemoved;
        
        /// <summary>
        /// 
        /// </summary>
        public event HidDeviceArrivedEventHandler DeviceArrived;
        
        /// <summary>
        /// 
        /// </summary>
        public event DeviceStateChangeEventHandler DeviceStateChanged;

        #endregion

        #region Sabitler

        /// <summary>
        /// 
        /// </summary>
        const int MOUSE = 0x102;

        /// <summary>
        /// 
        /// </summary>
        const byte REPORT_ID = 0;

        /// <summary>
        /// 
        /// </summary>
        const int KEYBOARD = 0x106;

        /// <summary>
        /// 
        /// </summary>
        const int WM_DEVICECHANGE = 0x219;

        #endregion

        #region Global Variable

        /// <summary>
        /// 
        /// </summary>
        bool mCanRead;

        /// <summary>
        /// 
        /// </summary>
        bool mCanWrite;

        /// <summary>
        /// 
        /// </summary>
        bool Open = false;

        /// <summary>
        /// 
        /// </summary>
        bool Register = false;

        /// <summary>
        /// 
        /// </summary>
        bool isDisposed = false;

        /// <summary>
        /// 
        /// </summary>
        int iReadTimeOut = 3000;

        /// <summary>
        /// 
        /// </summary>
        Guid devGuid;

        /// <summary>
        /// 
        /// </summary>
        IntPtr hReadHandle;

        /// <summary>
        /// 
        /// </summary>
        IntPtr hWriteHandle;

        /// <summary>
        /// 
        /// </summary>
        IntPtr hDeviceHandle;

        /// <summary>
        /// 
        /// </summary>
        IntPtr hDeviceNotification = IntPtr.Zero;

        /// <summary>
        /// 
        /// </summary>
        Win32.HIDP_CAPS Capabilities;

        /// <summary>
        /// 
        /// </summary>
        Win32.HIDD_ATTRIBUTES HidDeviceAttributes;

        /// <summary>
        /// 
        /// </summary>
        UsbDeviceManagement USBService = new UsbDeviceManagement();
        
        /// <summary>
        /// 
        /// </summary>
        string DevicePath;

        /// <summary>
        /// 
        /// </summary>
        string ProductString;

        /// <summary>
        /// 
        /// </summary>
        string ManufacturerString;

        /// <summary>
        /// 
        /// </summary>
        Thread PipeBehaviorThread;

        #endregion

        #region Properties

        // P R O P E R T I E S 

        /// <summary>
        /// Bu özellik HID sýnýfý'nýn GUID numarasýný döndürür.
        /// </summary>
        [Browsable(true)]
        public Guid HidGuid
        {
            get
            {
                Guid g = new Guid();
                Win32.HidD_GetHidGuid(ref g);
                return g;
            }
        }

        /// <summary>
        /// Bu özellik baðlantý kurulan aygýtýn fiziksel yol 
        /// bilgisini döndürür.
        /// </summary>
        [Browsable(false)]
        public string UsbDevicePath
        {
            get { return DevicePath; }
        }

        /// <summary>
        /// Bu özellik okuma zaman aþýmý süresini ayarlar yada
        /// döndürür.
        /// </summary>
        [Browsable(true)]
        public int ReadTimeOut
        {
            get { return iReadTimeOut; }
            set { iReadTimeOut = value; }
        }

        /// <summary>
        /// Bu özellik cihazýn tipini döndürür.
        /// </summary>
        [Browsable(false)]
        public DeviceType UsbDeviceType
        {
            get
            {
                int iDevUsage = Capabilities.UsagePage * 256 + Capabilities.Usage;
                if (iDevUsage == MOUSE) return DeviceType.Mouse;
                if (iDevUsage == KEYBOARD) return DeviceType.Keyboard;
                else
                    return DeviceType.Unknown;
            }
        }

        /// <summary>
        /// Bu özellik baðlantý kurulan cihaz'ýn Output rapor
        /// boyutunu döndürür.
        /// </summary>
        [Browsable(false)]
        public int OutputBufferSize
        {
            get { return Capabilities.OutputReportByteLength; }
        }

        /// <summary>
        /// Bu özellik baðlantý kurulan cihazýn Input rapor
        /// büyüklüðünü döndürür.
        /// </summary>
        [Browsable(false)]
        public int InputBufferSize
        {
            get { return Capabilities.InputReportByteLength; }
        }

        /// <summary>
        /// Bu özellik cihaz ile mevcut baðlantý varsa true,
        /// aksi halde false döndürür.
        /// </summary>
        [Browsable(false)]
        public bool IsOpen
        {
            get { return Open; }
        }

        /// <summary>
        /// Bu özellik cihazýn sisteme kayýtlý olup olmadýðýný 
        /// döndürür.
        /// </summary>
        [Browsable(false)]
        public bool IsRegistered
        {
            get { return Register; }
        }

        /// <summary>
        /// Bu özellik iþletim sistemi 98 Gold ise true
        /// aksi halde false döndürür.
        /// </summary>
        [Browsable(false)]
        public bool IsWindows98Gold
        {
            get
            {
                Version v98Gold = new Version(4, 10, 2183);
                OperatingSystem OS = Environment.OSVersion;

                if (OS.Version >= v98Gold)
                    return false;
                else
                    return true;
            }
        }

        /// <summary>
        /// Bu özellik iþletim sistemi XP veya daha yükseði
        /// ise true aksi halde false döndürür.
        /// </summary>
        [Browsable(false)]
        public bool IsWindowsXpOrLater
        {
            get
            {
                Version vXP = new Version(5, 1);
                OperatingSystem OS = Environment.OSVersion;

                if (OS.Version >= vXP)
                    return true;
                else
                    return false;
            }
        }

        /// <summary>
        /// Bu özellik cihazýn üretici string'ini döndürür.
        /// </summary>
        [Browsable(false)]
        public string DeviceManufacturer
        {
            get { return ManufacturerString; }
        }

        /// <summary>
        /// Bu özellik cihazýn ürün string'ini döndürür.
        /// </summary>
        [Browsable(false)]
        public string DeviceProduct
        {
            get { return ProductString; }
        }

        /// <summary>
        /// Bu özellik cihazýn okunabilir olup olmadýðýný 
        /// döndürür.
        /// </summary>
        [Browsable(false)]
        public bool CanRead
        {
            get { return mCanRead; }
        }

        /// <summary>
        /// Bu özellik cihazýn yazýlabilir olup olmadýðýný
        /// döndürür.
        /// </summary>
        [Browsable(false)]
        public bool CanWrite
        {
            get { return mCanWrite; }
        }

        #endregion

        #region USBManagement Kod

        /// <summary>
        /// Bu method bileþen için Hakkýnda penceresini gösterir
        /// </summary>
        /// <param name="ApplicationHandle">Ana uygulama form handle'ý</param>
        public void About(IntPtr ApplicationHandle)
        {
            Win32.ShellAbout(
                  ApplicationHandle,
                  "USB HID Yönetim Kütüphanesi",
                  "DigitalCastle-CoderTeam : Jabba",
                  IntPtr.Zero
                  );
        }

        /// <summary>
        /// USB cihaz ile haberleþmek için baðlantý kurar.
        /// </summary>
        /// <param name="VendorId">Üretici ID</param>
        /// <param name="ProductId">Ürün ID</param>
        /// <returns>Baðlantý kurulursa true, aksi halde false</returns>
        /// <exception cref="AlReadyDeviceException">Cihaz ile baðlantý mevcut iken
        /// tekrar baðlantý kurulmaya çalýþýldýðýnda fýrlatýlýr.</exception>
        public bool OpenPipe(short VendorId, short ProductId)
        {
            if (IsOpen)
                throw new AlReadyDeviceException("Cihaz ile baðlantý zaten mevcut.Tekrar baðlantý açýlamaz.");

            int iResult = 0;
            devGuid = new Guid();
            int iMemberIndex = 0;
            string[] AllDevice = new string[128];
            IntPtr PreparsedDataPointer = new IntPtr();
            bool bResult = false, bDeviceDetected = false;
            Win32.SECURITY_ATTRIBUTES Security = new Win32.SECURITY_ATTRIBUTES();

            //GUID numarasýný al
            Win32.HidD_GetHidGuid(ref devGuid);

            //Bu GUID numarasýna uyan tüm cihazlarýn arayüz setini al
            bResult = USBService.FindUsbDevice(devGuid, ref AllDevice);
            //Hiç aygýt bulunamadýysa geri dön
            if (!bResult) return false;

            Security.iSecurityDescriptor = 0;
            Security.iInheritHandle = Convert.ToInt32(true);
            Security.iLength = Marshal.SizeOf(Security);

            //Bir döngü içinde arayüz setindeki elemanlarý incele
            do
            {   //Index'i verilen path bilgisi ile bir dosya aç
                hDeviceHandle = Win32.CreateFile(
                                      AllDevice[iMemberIndex],
                                      0,
                                      Win32.FILE_SHARE_READ |
                                      Win32.FILE_SHARE_WRITE,
                                      ref Security,
                                      Win32.OPEN_EXISTING,
                                      0,
                                      IntPtr.Zero
                                      );

                if (hDeviceHandle != Win32.INVALID_HANDLE_VALUE)
                {
                    //Cihaz'a bir dosya açýldýysa bu cihazýn özelliklerini öðrenmek için Get_Attributes
                    //methodunu çaðýr
                    HidDeviceAttributes.Size = Marshal.SizeOf(HidDeviceAttributes);
                    bResult = Win32.HidD_GetAttributes(hDeviceHandle, ref HidDeviceAttributes);

                    if (bResult)
                    {
                        //OpenPipe ile verilen vendorid ve productid'si okunan cihaz'ýnki ile ayný ise
                        //aygýt bulunmuþtur.
                        if ((HidDeviceAttributes.VendorID == VendorId) && (HidDeviceAttributes.ProductID == ProductId))
                        {
                            DevicePath = AllDevice[iMemberIndex];
                            bDeviceDetected = true;
                        }//Bulunmadýysa cihaz'la iliþkili dosyayý kapat
                        else
                        {
                            bDeviceDetected = false;
                            Win32.CloseHandle(hDeviceHandle);
                        }
                    }
                    else
                    {   //Aygýt özellikleri okunamamýþtýr.
                        bResult = Win32.CloseHandle(hDeviceHandle);
                    }
                }
                iMemberIndex++;

            } while ((!bDeviceDetected) && AllDevice[iMemberIndex] != null);

            //Eðer cihaz bulunduysa yazma,okuma handle'larýný oluþtur ve
            //üretici bilgilerini al
            if (bDeviceDetected)
            {
                //Cihaz'ýn yeteneklerini içeren tampona iþaretçi al
                bResult = Win32.HidD_GetPreparsedData(hDeviceHandle, ref PreparsedDataPointer);
                //Cihaz kapasite bilgilerini al
                iResult = Win32.HidP_GetCaps(PreparsedDataPointer, ref Capabilities);
                //Tampon iþaretçisini serbest býrak
                bResult = Win32.HidD_FreePreparsedData(ref PreparsedDataPointer);

                //Cihazdan okuma yapmak için bir handle al
                hReadHandle = Win32.CreateFile(
                                               DevicePath,
                                               Win32.GENERIC_READ,
                                               Win32.FILE_SHARE_READ | Win32.FILE_SHARE_WRITE,
                                               ref Security,
                                               Win32.OPEN_EXISTING,
                                               Win32.FILE_FLAG_OVERLAPPED,
                                               IntPtr.Zero
                                               );

                if (hReadHandle != Win32.INVALID_HANDLE_VALUE) mCanRead = true;
                //Cihaz'a yazma yapmak için bir handle al
                hWriteHandle = Win32.CreateFile(
                                                DevicePath,
                                                Win32.GENERIC_WRITE,
                                                Win32.FILE_SHARE_READ | Win32.FILE_SHARE_WRITE,
                                                ref Security,
                                                Win32.OPEN_EXISTING,
                                                0,
                                                IntPtr.Zero
                                                );

                if (hWriteHandle != Win32.INVALID_HANDLE_VALUE) mCanWrite = true;
                if (mCanRead & mCanWrite)
                {
                    //Manufacturer String'i al
                    IntPtr mfcBufferPointer = Marshal.AllocHGlobal(128);
                    bResult = Win32.HidD_GetManufacturerString(hDeviceHandle, mfcBufferPointer, 128);
                    ManufacturerString = Marshal.PtrToStringAuto(mfcBufferPointer);
                    Marshal.FreeHGlobal(mfcBufferPointer);

                    //Product String'i al
                    IntPtr prdBufferPointer = Marshal.AllocHGlobal(128);
                    bResult = Win32.HidD_GetProductString(hDeviceHandle, prdBufferPointer, 128);
                    ProductString = Marshal.PtrToStringAuto(prdBufferPointer);
                    Marshal.FreeHGlobal(prdBufferPointer);

                    //Baðlantý açýk
                    Open = true;

                    //Baðlantý açýldýktan sonra olay tetikle
                    if (DeviceStateChanged != null)
                    {
                        DeviceStateChangeEventArgs devEvnt = new DeviceStateChangeEventArgs();

                        devEvnt.CurrentState = DeviceState.Opened;
                        DeviceStateChanged(this, devEvnt);
                    }
                }
            }

            return bDeviceDetected;
        }

        /// <summary>
        /// Bu method OpenPipe'ýn aþýrý yüklenmiþ versiyonudur ve bu methoda
        /// ek olarak baðlantý açýldýktan sonra verilen süre sonunda baðlantýyý
        /// otomatik olarak kapatýr.
        /// </summary>
        /// <param name="VendorId"></param>
        /// <param name="ProductId"></param>
        /// <param name="timeOut"></param>
        /// <returns></returns>
        public bool OpenPipe(short VendorId, short ProductId, int timeOut)
        {
            bool Success;

            Success = OpenPipe(VendorId, ProductId);
            if (Success)
            {
                PipeBehaviorThread = new Thread(new ParameterizedThreadStart(TimeOutClosePipe));
                PipeBehaviorThread.Start(timeOut);
            }
            return Success;
        }

        /// <summary>
        /// Baðlantý kurulan cihazdan Input Rapor'u okur.
        /// </summary>
        /// <param name="buffer">Verilerin okunacaðý tampon</param>
        /// <param name="Length">Okunacak veri adedi</param>
        /// <param name="transaction">Ýþlem tipi.Kesme yada Kontrol transfer</param>
        /// <exception cref="DeviceNotConnectException"></exception>
        /// <exception cref="InvalidReadHandleException"></exception>
        /// <returns>Ýþlem baþarýlý ise true, aksi halde false</returns>
        public bool ReadPipe(ref byte[] buffer, int Length, TransactionType transaction)
        {
            bool Success = false;
            UsbHidManagement.InputReport HidInputReport = new UsbHidManagement.InputReport();

            if (!IsOpen)
                throw new DeviceNotConnectException("Cihaz ile baðlantý yok");

            if ((hReadHandle == Win32.INVALID_HANDLE_VALUE) || hReadHandle == IntPtr.Zero)
                throw new InvalidHandleException("Geçersiz okuma handle'ý.Cihazdan okuma yapýlamaz.");

            //Eðer verilen tampon deðeri InputReportByteLength'den küçük veya büyükse
            //tampon bu deðere eþitleniyor.
            if ((Length > Capabilities.InputReportByteLength) || (Length < Capabilities.InputReportByteLength))
                buffer = new byte[Capabilities.InputReportByteLength];

            if (transaction == TransactionType.Control)
                //Cihazdan Get_Report ile okuma yap
                Success = HidInputReport.HidControlReadTransfer(hReadHandle, ref buffer, buffer.Length);
            else
                //Cihaz'dan kesme transfer kullanarak okuma yap
                Success = HidInputReport.HidInterruptReadTransfer(hReadHandle, ref buffer, buffer.Length, iReadTimeOut);

            return Success;
        }

        /// <summary>
        /// Baðlantý kurulan cihazdan Input Rapor'u okur, autoClose true ise
        /// baðlantýyý kapatýr.
        /// </summary>
        /// <param name="buffer">Verilerin okunacaðý tampon</param>
        /// <param name="Length">Okunacak veri adedi</param>
        /// <param name="transaction">Ýþlem tipi.Kesme yada Kontrol transfer</param>
        /// <param name="autoClose">Eðer true ise baðlantý kapatýlýr</param>
        /// <returns>Ýþlem baþarýlý ise true, aksi halde false</returns>
        public bool ReadPipe(ref byte[] buffer, int Length, TransactionType transaction, bool autoClose)
        {
            bool Success;

            Success = ReadPipe(ref buffer, Length, transaction);
            if (Success)
                if (autoClose) ClosePipe();

            return Success;
        }

        /// <summary>
        /// Baðlantý kurulan cihazdan input raporu okur.Okuma iþlemi tamamlandýðýnda
        /// are setlenir ve autoClose true ise baðlantý kapatýlýr.
        /// </summary>
        /// <param name="buffer">Verilerin okunacaðý tampon</param>
        /// <param name="Length">Okunacak veri adedi</param>
        /// <param name="transaction">Ýþlem tipi.Kesme yada Kontrol transfer</param>
        /// <param name="are">Okuma iþlemi bitince setlenecek olay nesnesi</param>
        /// <param name="autoClose">Eðer true ise baðlantý kapatýlýr</param>
        /// <returns>Ýþlem baþarýlý ise true, aksi halde false</returns>
        public bool ReadPipe(ref byte[] buffer, int Length, TransactionType transaction, AutoResetEvent are, bool autoClose)
        {
            bool Success;

            Success = ReadPipe(ref buffer, Length, transaction);
            if (Success)
            {
                if (autoClose) ClosePipe();
                are.Set();
            }

            return Success;
        }

        /// <summary>
        /// Baðlantý kurulan cihaz'a Output Report gönderir.
        /// </summary>
        /// <param name="buffer">Yazýlacak verilerin bulunduðu tampon</param>
        /// <param name="Length">Yazýlacak veri adedi</param>
        /// <param name="transaction">Ýþlem tipi.Kesme yada Kontrol transfer</param>
        /// <returns>Ýþlem baþarýlý ise true, aksi halde false</returns>
        public bool WritePipe(byte[] buffer, TransactionType transaction)
        {
            bool Success = false;
            UsbHidManagement.OutputReport HidOutputReport = new UsbHidManagement.OutputReport();

            if (!IsOpen)
                throw new DeviceNotConnectException("Cihaz ile baðlantý yok");

            if ((hWriteHandle == Win32.INVALID_HANDLE_VALUE) || hWriteHandle == IntPtr.Zero)
                throw new InvalidHandleException("Geçersiz yazma handle'ý.Cihaz'a veri yazýlamaz.");

            if (transaction == TransactionType.Control)
                Success = HidOutputReport.HidControlWriteTransfer(hWriteHandle, ref buffer, Capabilities.OutputReportByteLength);
            else
                Success = HidOutputReport.HidInterruptWriteTransfer(hWriteHandle, ref buffer, Capabilities.OutputReportByteLength);

            return Success;
        }

        /// <summary>
        /// Baðlantý kurulan cihaz'a Output Report gönderir, autoClose true ise
        /// baðlantýyý kapatýr.
        /// </summary>
        /// <param name="buffer">Yazýlacak verilerin bulunduðu tampon</param>
        /// <param name="transaction">Ýþlem tipi.Kesme yada Kontrol transfer</param>
        /// <param name="autoClose">Eðer true ise baðlantý kapatýlýr.</param>
        /// <returns>Ýþlem baþarýlý ise true, aksi halde false</returns>
        public bool WritePipe(byte[] buffer, TransactionType transaction, bool autoClose)
        {
            bool Success;

            Success = WritePipe(buffer, transaction);
            if (Success)
                if (autoClose) ClosePipe();

            return Success;
        }

        /// <summary>
        /// Baðlantý kurulan cihaz'a Output Report gönderir, okuma iþlemi tamamlandýðýnda
        /// are setlenir ve autoClose true ise baðlantý kapatýlýr.
        /// </summary>
        /// <param name="buffer">Yazýlacak verilerin bulunduðu tampon</param>
        /// <param name="transaction">Ýþlem tipi.Kesme yada Kontrol transfer</param>
        /// <param name="are">Yazma iþlemi bitince setlenecek olay nesnesi</param>
        /// <param name="autoClose">Eðer true ise baðlantý kapatýlýr.</param>
        /// <returns>Ýþlem baþarýlý ise true, aksi halde false</returns>
        public bool WritePipe(byte[] buffer, TransactionType transaction, AutoResetEvent are, bool autoClose)
        {
            bool Success;

            Success = WritePipe(buffer, transaction);
            if (Success)
            {
                if (autoClose) ClosePipe();
                are.Set();
            }
            return Success;
        }

        /// <summary>
        /// Cihaz ile baðlantýyý kapatýr.
        /// </summary>
        /// <returns>Ýþlem baþarýlý ise true, aksi halde false döndürür.</returns>
        public bool ClosePipe()
        {
            bool Success;
            byte bCls = 0;

            if (!IsOpen)
                throw new AlReadyCloseException("Cihaz ile baðlantý zaten yok.");

            if (hReadHandle != Win32.INVALID_HANDLE_VALUE)
            {
                Success = Win32.CloseHandle(hReadHandle);
                if (Success) bCls++;
            }
            if (hWriteHandle != Win32.INVALID_HANDLE_VALUE)
            {
                Success = Win32.CloseHandle(hWriteHandle);
                if (Success) bCls++;
            }
            if (hDeviceHandle != Win32.INVALID_HANDLE_VALUE)
            {
                Success = Win32.CloseHandle(hDeviceHandle);
                if (Success) bCls++;
            }

            hReadHandle = IntPtr.Zero;
            hWriteHandle = IntPtr.Zero;
            hDeviceHandle = IntPtr.Zero;

            Capabilities = default(Win32.HIDP_CAPS);

            ProductString = string.Empty;
            ManufacturerString = string.Empty;

            Open = false;
            if (bCls == 3)
            {
                if (DeviceStateChanged != null)
                {
                    DeviceStateChangeEventArgs devEvnt = new DeviceStateChangeEventArgs();

                    devEvnt.CurrentState = DeviceState.Closed;
                    DeviceStateChanged(this, devEvnt);
                }
                return true;
            }
            return false;
        }

        /// <summary>
        /// Cihaz'ýn sökülüp takýldýðýný takip etmek için sisteme
        /// kaydeder.
        /// </summary>
        /// <param name="FormHandle">Mesajlarýn iletileceði Windows Form'u handle'ý</param>
        /// <returns>Ýþlem baþarýlý ise true, aksi halde false döndürür</returns>
        public bool RegisterDevice(IntPtr FormHandle)
        {
            bool Success = false;

            if (hDeviceNotification != IntPtr.Zero)
                throw new AlReadyRegisteredException("Bu cihaz zaten sisteme kayýtlý.");
            if (!IsOpen)
                throw new DeviceNotConnectException("Cihaz ile baðlantý yok");

            Success = USBService.RegisterUsbDeviceNotifications(FormHandle, devGuid, ref hDeviceNotification);
            if (Success) Register = true;

            return Success;
        }

        /// <summary>
        /// RegisterDevice ile kaydedilen cihaz'ý sistemden kaldýrýr.
        /// </summary>
        /// <returns>Ýþlem baþarýlý ise true, aksi halde false döndürür.</returns>
        public bool UnRegisterDevice()
        {
            bool Success = false;

            if (hDeviceNotification == IntPtr.Zero) return false;
            Success = USBService.UnRegisterUsbDeviceNotifications(hDeviceNotification);
            if (Success)
            {
                Register = false;
                hDeviceNotification = IntPtr.Zero;
            }

            return Success;
        }

        /// <summary>
        /// Buffer'da bekleyen input raporunu temizler
        /// </summary>
        /// <returns>Ýþlem baþarýlý ise true aksi halde false</returns>
        public bool FlushBuffer()
        {
            bool Success;

            if (!IsOpen)
                throw new DeviceNotConnectException("Cihaz ile baðlantý yok");

            if ((hDeviceHandle == IntPtr.Zero) || (hDeviceHandle == Win32.INVALID_HANDLE_VALUE))
                throw new InvalidHandleException("Geçersiz cihaz handle deðeri");

            Success = Win32.HidD_FlushQueue(hDeviceHandle);
            return Success;
        }

        /// <summary>
        /// Bu method sisteme kayýtlý Usb aygýtýn porttan çýkartýlmasý
        /// ve takýlmasý sýrasýndan uygulama mesaj kuyruðuna býrakýlan mesajlarý
        /// filtreler.Bu mesajlardan WM_DEVICECHANGE mesajý alýndýðýnda olayýn meydana geldiði
        /// cihazýn kayýtlý cihaz olup olmadýðýna bakýlýr, eðer öyleyse ilgili olaylar
        /// tetiklenir.
        /// Bu method ana uygulamanýn pencere fonksiyonu içinden çaðrýlmalýdýr.
        /// </summary>
        /// <param name="m">Sistem mesajý</param>
        public void ProcessDeviceMessage(Message m)
        {
            if (!IsRegistered) return;
            if (m.Msg == WM_DEVICECHANGE) CheckDevice(m);
        }

        /// <summary>
        /// Bu method yeni bir thread içinde oluþturulur ve verilen
        /// TimeOut süresi dolana kadar kendini bloke eder.Süre bitimi 
        /// Usb ile baðlantý kapatýlýr.
        /// </summary>
        /// <param name="TimeOut"></param>
        private void TimeOutClosePipe(object TimeOut)
        {
            Thread.Sleep((int)TimeOut);
            ClosePipe();
        }

        /// <summary>
        /// Sisteme takýlan yada sökülen cihazýn tespitini yapar.
        /// </summary>
        /// <param name="msg">WndProc'a geçirilen Message tipi yapý</param>
        /// <param name="devStat">Takýlý yada sökülü durumunu gösteren numaralandýrýcý</param>
        /// <returns>Cihaz uygulamaya ait ise true, aksi halde false</returns>
        private void CheckDevice(Message msg)
        {
            int devType = 0;
            bool Owner = false;

            if (msg.WParam.ToInt32() == Win32.DBT_DEVICEARRIVAL)
            {
                devType = Marshal.ReadInt32(msg.LParam, 4);
                if (devType == Win32.DBT_DEVTYP_DEVICEINTERFACE)
                {
                    Owner = USBService.CheckUsbDeviceName(msg, DevicePath);
                    if (Owner)
                    {
                        if (DeviceArrived != null)
                        {
                            EventArgs attachEvnt = new EventArgs();
                            DeviceArrived(this, attachEvnt);
                        }
                    }
                }
            }
            else if (msg.WParam.ToInt32() == Win32.DBT_DEVICEREMOVECOMPLETE)
            {
                devType = Marshal.ReadInt32(msg.LParam, 4);
                if (devType == Win32.DBT_DEVTYP_DEVICEINTERFACE)
                {
                    Owner = USBService.CheckUsbDeviceName(msg, DevicePath);
                    if (Owner)
                    {
                        if (DeviceRemoved != null)
                        {
                            EventArgs detachEvnt = new EventArgs();
                            DeviceRemoved(this, detachEvnt);
                        }
                    }
                }
            }
        }

        #endregion
    }
}
