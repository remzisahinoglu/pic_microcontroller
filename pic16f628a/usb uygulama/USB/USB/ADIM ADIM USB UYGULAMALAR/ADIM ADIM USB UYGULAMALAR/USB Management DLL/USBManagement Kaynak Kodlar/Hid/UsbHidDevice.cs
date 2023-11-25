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
    /// Veri tranferi i�lemlerinin kontrol yada
    /// kesme transfer ile yap�lmas�n� belirten numaraland�r�c�
    /// </summary>
    public enum TransactionType 
    { 
        Control, 
        Interrupt 
    }

    /// <summary>
    /// Verilen bilgilere g�re bulunan cihaz�n tipini
    /// g�steren numaraland�r�c�
    /// </summary>
    public enum DeviceType 
    { 
        Unknown, 
        Keyboard, 
        Mouse 
    }

    /// <summary>
    /// Ba�lant� durumundaki de�i�iklikleri g�steren
    /// numaraland�r�c�
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
        /// A��r� y�klenmi� kurucu method.Nesne olu�turulurken
        /// bu kurucu method ile olu�turulursa USB cihaz ile ba�lant� a��l�r.
        /// </summary>
        /// <param name="VendorId">�retici kimli�i</param>
        /// <param name="ProductId">�r�n kimli�i</param>
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
        /// Bu �zellik HID s�n�f�'n�n GUID numaras�n� d�nd�r�r.
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
        /// Bu �zellik ba�lant� kurulan ayg�t�n fiziksel yol 
        /// bilgisini d�nd�r�r.
        /// </summary>
        [Browsable(false)]
        public string UsbDevicePath
        {
            get { return DevicePath; }
        }

        /// <summary>
        /// Bu �zellik okuma zaman a��m� s�resini ayarlar yada
        /// d�nd�r�r.
        /// </summary>
        [Browsable(true)]
        public int ReadTimeOut
        {
            get { return iReadTimeOut; }
            set { iReadTimeOut = value; }
        }

        /// <summary>
        /// Bu �zellik cihaz�n tipini d�nd�r�r.
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
        /// Bu �zellik ba�lant� kurulan cihaz'�n Output rapor
        /// boyutunu d�nd�r�r.
        /// </summary>
        [Browsable(false)]
        public int OutputBufferSize
        {
            get { return Capabilities.OutputReportByteLength; }
        }

        /// <summary>
        /// Bu �zellik ba�lant� kurulan cihaz�n Input rapor
        /// b�y�kl���n� d�nd�r�r.
        /// </summary>
        [Browsable(false)]
        public int InputBufferSize
        {
            get { return Capabilities.InputReportByteLength; }
        }

        /// <summary>
        /// Bu �zellik cihaz ile mevcut ba�lant� varsa true,
        /// aksi halde false d�nd�r�r.
        /// </summary>
        [Browsable(false)]
        public bool IsOpen
        {
            get { return Open; }
        }

        /// <summary>
        /// Bu �zellik cihaz�n sisteme kay�tl� olup olmad���n� 
        /// d�nd�r�r.
        /// </summary>
        [Browsable(false)]
        public bool IsRegistered
        {
            get { return Register; }
        }

        /// <summary>
        /// Bu �zellik i�letim sistemi 98 Gold ise true
        /// aksi halde false d�nd�r�r.
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
        /// Bu �zellik i�letim sistemi XP veya daha y�kse�i
        /// ise true aksi halde false d�nd�r�r.
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
        /// Bu �zellik cihaz�n �retici string'ini d�nd�r�r.
        /// </summary>
        [Browsable(false)]
        public string DeviceManufacturer
        {
            get { return ManufacturerString; }
        }

        /// <summary>
        /// Bu �zellik cihaz�n �r�n string'ini d�nd�r�r.
        /// </summary>
        [Browsable(false)]
        public string DeviceProduct
        {
            get { return ProductString; }
        }

        /// <summary>
        /// Bu �zellik cihaz�n okunabilir olup olmad���n� 
        /// d�nd�r�r.
        /// </summary>
        [Browsable(false)]
        public bool CanRead
        {
            get { return mCanRead; }
        }

        /// <summary>
        /// Bu �zellik cihaz�n yaz�labilir olup olmad���n�
        /// d�nd�r�r.
        /// </summary>
        [Browsable(false)]
        public bool CanWrite
        {
            get { return mCanWrite; }
        }

        #endregion

        #region USBManagement Kod

        /// <summary>
        /// Bu method bile�en i�in Hakk�nda penceresini g�sterir
        /// </summary>
        /// <param name="ApplicationHandle">Ana uygulama form handle'�</param>
        public void About(IntPtr ApplicationHandle)
        {
            Win32.ShellAbout(
                  ApplicationHandle,
                  "USB HID Y�netim K�t�phanesi",
                  "DigitalCastle-CoderTeam : Jabba",
                  IntPtr.Zero
                  );
        }

        /// <summary>
        /// USB cihaz ile haberle�mek i�in ba�lant� kurar.
        /// </summary>
        /// <param name="VendorId">�retici ID</param>
        /// <param name="ProductId">�r�n ID</param>
        /// <returns>Ba�lant� kurulursa true, aksi halde false</returns>
        /// <exception cref="AlReadyDeviceException">Cihaz ile ba�lant� mevcut iken
        /// tekrar ba�lant� kurulmaya �al���ld���nda f�rlat�l�r.</exception>
        public bool OpenPipe(short VendorId, short ProductId)
        {
            if (IsOpen)
                throw new AlReadyDeviceException("Cihaz ile ba�lant� zaten mevcut.Tekrar ba�lant� a��lamaz.");

            int iResult = 0;
            devGuid = new Guid();
            int iMemberIndex = 0;
            string[] AllDevice = new string[128];
            IntPtr PreparsedDataPointer = new IntPtr();
            bool bResult = false, bDeviceDetected = false;
            Win32.SECURITY_ATTRIBUTES Security = new Win32.SECURITY_ATTRIBUTES();

            //GUID numaras�n� al
            Win32.HidD_GetHidGuid(ref devGuid);

            //Bu GUID numaras�na uyan t�m cihazlar�n aray�z setini al
            bResult = USBService.FindUsbDevice(devGuid, ref AllDevice);
            //Hi� ayg�t bulunamad�ysa geri d�n
            if (!bResult) return false;

            Security.iSecurityDescriptor = 0;
            Security.iInheritHandle = Convert.ToInt32(true);
            Security.iLength = Marshal.SizeOf(Security);

            //Bir d�ng� i�inde aray�z setindeki elemanlar� incele
            do
            {   //Index'i verilen path bilgisi ile bir dosya a�
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
                    //Cihaz'a bir dosya a��ld�ysa bu cihaz�n �zelliklerini ��renmek i�in Get_Attributes
                    //methodunu �a��r
                    HidDeviceAttributes.Size = Marshal.SizeOf(HidDeviceAttributes);
                    bResult = Win32.HidD_GetAttributes(hDeviceHandle, ref HidDeviceAttributes);

                    if (bResult)
                    {
                        //OpenPipe ile verilen vendorid ve productid'si okunan cihaz'�nki ile ayn� ise
                        //ayg�t bulunmu�tur.
                        if ((HidDeviceAttributes.VendorID == VendorId) && (HidDeviceAttributes.ProductID == ProductId))
                        {
                            DevicePath = AllDevice[iMemberIndex];
                            bDeviceDetected = true;
                        }//Bulunmad�ysa cihaz'la ili�kili dosyay� kapat
                        else
                        {
                            bDeviceDetected = false;
                            Win32.CloseHandle(hDeviceHandle);
                        }
                    }
                    else
                    {   //Ayg�t �zellikleri okunamam��t�r.
                        bResult = Win32.CloseHandle(hDeviceHandle);
                    }
                }
                iMemberIndex++;

            } while ((!bDeviceDetected) && AllDevice[iMemberIndex] != null);

            //E�er cihaz bulunduysa yazma,okuma handle'lar�n� olu�tur ve
            //�retici bilgilerini al
            if (bDeviceDetected)
            {
                //Cihaz'�n yeteneklerini i�eren tampona i�aret�i al
                bResult = Win32.HidD_GetPreparsedData(hDeviceHandle, ref PreparsedDataPointer);
                //Cihaz kapasite bilgilerini al
                iResult = Win32.HidP_GetCaps(PreparsedDataPointer, ref Capabilities);
                //Tampon i�aret�isini serbest b�rak
                bResult = Win32.HidD_FreePreparsedData(ref PreparsedDataPointer);

                //Cihazdan okuma yapmak i�in bir handle al
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
                //Cihaz'a yazma yapmak i�in bir handle al
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

                    //Ba�lant� a��k
                    Open = true;

                    //Ba�lant� a��ld�ktan sonra olay tetikle
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
        /// Bu method OpenPipe'�n a��r� y�klenmi� versiyonudur ve bu methoda
        /// ek olarak ba�lant� a��ld�ktan sonra verilen s�re sonunda ba�lant�y�
        /// otomatik olarak kapat�r.
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
        /// Ba�lant� kurulan cihazdan Input Rapor'u okur.
        /// </summary>
        /// <param name="buffer">Verilerin okunaca�� tampon</param>
        /// <param name="Length">Okunacak veri adedi</param>
        /// <param name="transaction">��lem tipi.Kesme yada Kontrol transfer</param>
        /// <exception cref="DeviceNotConnectException"></exception>
        /// <exception cref="InvalidReadHandleException"></exception>
        /// <returns>��lem ba�ar�l� ise true, aksi halde false</returns>
        public bool ReadPipe(ref byte[] buffer, int Length, TransactionType transaction)
        {
            bool Success = false;
            UsbHidManagement.InputReport HidInputReport = new UsbHidManagement.InputReport();

            if (!IsOpen)
                throw new DeviceNotConnectException("Cihaz ile ba�lant� yok");

            if ((hReadHandle == Win32.INVALID_HANDLE_VALUE) || hReadHandle == IntPtr.Zero)
                throw new InvalidHandleException("Ge�ersiz okuma handle'�.Cihazdan okuma yap�lamaz.");

            //E�er verilen tampon de�eri InputReportByteLength'den k���k veya b�y�kse
            //tampon bu de�ere e�itleniyor.
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
        /// Ba�lant� kurulan cihazdan Input Rapor'u okur, autoClose true ise
        /// ba�lant�y� kapat�r.
        /// </summary>
        /// <param name="buffer">Verilerin okunaca�� tampon</param>
        /// <param name="Length">Okunacak veri adedi</param>
        /// <param name="transaction">��lem tipi.Kesme yada Kontrol transfer</param>
        /// <param name="autoClose">E�er true ise ba�lant� kapat�l�r</param>
        /// <returns>��lem ba�ar�l� ise true, aksi halde false</returns>
        public bool ReadPipe(ref byte[] buffer, int Length, TransactionType transaction, bool autoClose)
        {
            bool Success;

            Success = ReadPipe(ref buffer, Length, transaction);
            if (Success)
                if (autoClose) ClosePipe();

            return Success;
        }

        /// <summary>
        /// Ba�lant� kurulan cihazdan input raporu okur.Okuma i�lemi tamamland���nda
        /// are setlenir ve autoClose true ise ba�lant� kapat�l�r.
        /// </summary>
        /// <param name="buffer">Verilerin okunaca�� tampon</param>
        /// <param name="Length">Okunacak veri adedi</param>
        /// <param name="transaction">��lem tipi.Kesme yada Kontrol transfer</param>
        /// <param name="are">Okuma i�lemi bitince setlenecek olay nesnesi</param>
        /// <param name="autoClose">E�er true ise ba�lant� kapat�l�r</param>
        /// <returns>��lem ba�ar�l� ise true, aksi halde false</returns>
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
        /// Ba�lant� kurulan cihaz'a Output Report g�nderir.
        /// </summary>
        /// <param name="buffer">Yaz�lacak verilerin bulundu�u tampon</param>
        /// <param name="Length">Yaz�lacak veri adedi</param>
        /// <param name="transaction">��lem tipi.Kesme yada Kontrol transfer</param>
        /// <returns>��lem ba�ar�l� ise true, aksi halde false</returns>
        public bool WritePipe(byte[] buffer, TransactionType transaction)
        {
            bool Success = false;
            UsbHidManagement.OutputReport HidOutputReport = new UsbHidManagement.OutputReport();

            if (!IsOpen)
                throw new DeviceNotConnectException("Cihaz ile ba�lant� yok");

            if ((hWriteHandle == Win32.INVALID_HANDLE_VALUE) || hWriteHandle == IntPtr.Zero)
                throw new InvalidHandleException("Ge�ersiz yazma handle'�.Cihaz'a veri yaz�lamaz.");

            if (transaction == TransactionType.Control)
                Success = HidOutputReport.HidControlWriteTransfer(hWriteHandle, ref buffer, Capabilities.OutputReportByteLength);
            else
                Success = HidOutputReport.HidInterruptWriteTransfer(hWriteHandle, ref buffer, Capabilities.OutputReportByteLength);

            return Success;
        }

        /// <summary>
        /// Ba�lant� kurulan cihaz'a Output Report g�nderir, autoClose true ise
        /// ba�lant�y� kapat�r.
        /// </summary>
        /// <param name="buffer">Yaz�lacak verilerin bulundu�u tampon</param>
        /// <param name="transaction">��lem tipi.Kesme yada Kontrol transfer</param>
        /// <param name="autoClose">E�er true ise ba�lant� kapat�l�r.</param>
        /// <returns>��lem ba�ar�l� ise true, aksi halde false</returns>
        public bool WritePipe(byte[] buffer, TransactionType transaction, bool autoClose)
        {
            bool Success;

            Success = WritePipe(buffer, transaction);
            if (Success)
                if (autoClose) ClosePipe();

            return Success;
        }

        /// <summary>
        /// Ba�lant� kurulan cihaz'a Output Report g�nderir, okuma i�lemi tamamland���nda
        /// are setlenir ve autoClose true ise ba�lant� kapat�l�r.
        /// </summary>
        /// <param name="buffer">Yaz�lacak verilerin bulundu�u tampon</param>
        /// <param name="transaction">��lem tipi.Kesme yada Kontrol transfer</param>
        /// <param name="are">Yazma i�lemi bitince setlenecek olay nesnesi</param>
        /// <param name="autoClose">E�er true ise ba�lant� kapat�l�r.</param>
        /// <returns>��lem ba�ar�l� ise true, aksi halde false</returns>
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
        /// Cihaz ile ba�lant�y� kapat�r.
        /// </summary>
        /// <returns>��lem ba�ar�l� ise true, aksi halde false d�nd�r�r.</returns>
        public bool ClosePipe()
        {
            bool Success;
            byte bCls = 0;

            if (!IsOpen)
                throw new AlReadyCloseException("Cihaz ile ba�lant� zaten yok.");

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
        /// Cihaz'�n s�k�l�p tak�ld���n� takip etmek i�in sisteme
        /// kaydeder.
        /// </summary>
        /// <param name="FormHandle">Mesajlar�n iletilece�i Windows Form'u handle'�</param>
        /// <returns>��lem ba�ar�l� ise true, aksi halde false d�nd�r�r</returns>
        public bool RegisterDevice(IntPtr FormHandle)
        {
            bool Success = false;

            if (hDeviceNotification != IntPtr.Zero)
                throw new AlReadyRegisteredException("Bu cihaz zaten sisteme kay�tl�.");
            if (!IsOpen)
                throw new DeviceNotConnectException("Cihaz ile ba�lant� yok");

            Success = USBService.RegisterUsbDeviceNotifications(FormHandle, devGuid, ref hDeviceNotification);
            if (Success) Register = true;

            return Success;
        }

        /// <summary>
        /// RegisterDevice ile kaydedilen cihaz'� sistemden kald�r�r.
        /// </summary>
        /// <returns>��lem ba�ar�l� ise true, aksi halde false d�nd�r�r.</returns>
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
        /// <returns>��lem ba�ar�l� ise true aksi halde false</returns>
        public bool FlushBuffer()
        {
            bool Success;

            if (!IsOpen)
                throw new DeviceNotConnectException("Cihaz ile ba�lant� yok");

            if ((hDeviceHandle == IntPtr.Zero) || (hDeviceHandle == Win32.INVALID_HANDLE_VALUE))
                throw new InvalidHandleException("Ge�ersiz cihaz handle de�eri");

            Success = Win32.HidD_FlushQueue(hDeviceHandle);
            return Success;
        }

        /// <summary>
        /// Bu method sisteme kay�tl� Usb ayg�t�n porttan ��kart�lmas�
        /// ve tak�lmas� s�ras�ndan uygulama mesaj kuyru�una b�rak�lan mesajlar�
        /// filtreler.Bu mesajlardan WM_DEVICECHANGE mesaj� al�nd���nda olay�n meydana geldi�i
        /// cihaz�n kay�tl� cihaz olup olmad���na bak�l�r, e�er �yleyse ilgili olaylar
        /// tetiklenir.
        /// Bu method ana uygulaman�n pencere fonksiyonu i�inden �a�r�lmal�d�r.
        /// </summary>
        /// <param name="m">Sistem mesaj�</param>
        public void ProcessDeviceMessage(Message m)
        {
            if (!IsRegistered) return;
            if (m.Msg == WM_DEVICECHANGE) CheckDevice(m);
        }

        /// <summary>
        /// Bu method yeni bir thread i�inde olu�turulur ve verilen
        /// TimeOut s�resi dolana kadar kendini bloke eder.S�re bitimi 
        /// Usb ile ba�lant� kapat�l�r.
        /// </summary>
        /// <param name="TimeOut"></param>
        private void TimeOutClosePipe(object TimeOut)
        {
            Thread.Sleep((int)TimeOut);
            ClosePipe();
        }

        /// <summary>
        /// Sisteme tak�lan yada s�k�len cihaz�n tespitini yapar.
        /// </summary>
        /// <param name="msg">WndProc'a ge�irilen Message tipi yap�</param>
        /// <param name="devStat">Tak�l� yada s�k�l� durumunu g�steren numaraland�r�c�</param>
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
