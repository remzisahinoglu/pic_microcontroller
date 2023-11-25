using System;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace USBManagement
{
    internal class Win32
    {
        #region Enums

        internal enum PNP_VETO_TYPE
        {
            Ok,
            TypeUnknown,
            LegacyDevice,
            PendingClose,
            WindowsApp,
            WindowsService,
            OutstandingOpen,
            Device,
            Driver,
            IllegalDeviceRequest,
            InsufficientPower,
            NonDisableable,
            LegacyDriver,
        }

        #endregion

        #region Sabitler

        //usbiodef.h
        public const string GUID_DEVINTERFACE_USB_DEVICE = "A5DCBF10-6530-11D2-901F-00C04FB951ED";
        public const string USB_DISK_GUID = "53F56307-B6BF-11D0-94F2-00A0C91EFB8B";
        //winbase.h
        public static IntPtr INVALID_HANDLE_VALUE = new IntPtr(-1);
        //setupapi.h
        public const Int32 DIGCF_PRESENT = 0x00000002;
        public const Int32 DIGCF_DEFIVEINTERFACE = 0x00000010;

        public const uint GENERIC_READ = 0x80000000;
        public const uint GENERIC_WRITE = 0x40000000;
        public const uint FILE_SHARE_READ = 0x1;
        public const uint FILE_SHARE_WRITE = 0x2;
        public const uint FILE_FLAG_OVERLAPPED = 0x40000000;
        public const short OPEN_EXISTING = 3;
        public const int WAIT_TIMEOUT = 0x102;
        public const short WAIT_OBJECT_0 = 0;

        //dbt.h
        public const int DBT_DEVICEARRIVAL = 0x8000;
        public const int DBT_DEVICEREMOVECOMPLETE = 0x8004;
        public const int DEVICE_NOTIFY_ALL_INTERFACE_CLASSES = 4;
        public const int DEVICE_NOTIFY_SERVICE_HANDLE = 1;
        public const int DEVICE_NOTIFY_WINDOW_HANDLE = 0;
        public const int WM_DEVICECHANGE = 0x219;

        public const int DBT_DEVTYP_OEM = 0;
        public const int DBT_DEVTYP_DEVNODE = 1;
        public const int DBT_DEVTYP_VOLUME = 2;
        public const int DBT_DEVTYP_PORT = 3;
        public const int DBT_DEVTYP_NET = 4;
        public const int DBT_DEVTYP_DEVICEINTERFACE = 5; 
        public const int DBT_DEVTYP_HANDLE = 6;

        public const int BROADCAST_QUERY_DENY = 0x424D5144;
        public const int DBT_DEVICEQUERYREMOVE = 0x8001;

        public const Int32 FORMAT_MESSAGE_FROM_SYSTEM = 0x1000;

        //HID
        public const short HidP_Input = 0;
        public const short HidP_Output = 1;
        public const short HidP_Feature = 2;



        #endregion

        #region API Yap�lar�

        //Ayg�t aray�z yap�s�
        [StructLayout(LayoutKind.Sequential)]
        public class SP_DEVICE_INTERFACE_DATA
        {
            public Int32 cbSize = Marshal.SizeOf(typeof(SP_DEVICE_INTERFACE_DATA));
            public Guid InterfaceClassGuid = Guid.Empty;
            public Int32 Flags = 0;
            public Int32 Reserved = 0;
        }

        //Se�ilen aray�z i�in detaylar yap�s�(yol bilgisi)
        [StructLayout(LayoutKind.Sequential, Pack = 2)]
        public class SP_DEVICE_INTERFACE_DETAIL_DATA
        {
            public Int32 cbSize = Marshal.SizeOf(typeof(SP_DEVICE_INTERFACE_DETAIL_DATA));
            public Int16 DevicePath;
        }

        //Security Attributes yap�s�
        [StructLayout(LayoutKind.Sequential)]
        public struct SECURITY_ATTRIBUTES
        {
            public Int32 iLength;
            public Int32 iSecurityDescriptor;
            public Int32 iInheritHandle;
        }

        //Cihaz'�n �r�n ve �retici ID'lerini tutan yap�
        [StructLayout(LayoutKind.Sequential)]
        public struct HIDD_ATTRIBUTES
        {
            public Int32 Size;
            public Int16 VendorID;
            public Int16 ProductID;
            public Int16 VersionNumber;
        }

        //Cihaz kapasitesini tutan yap�
        [StructLayout(LayoutKind.Sequential)]
        public struct HIDP_CAPS
        {
            public Int16 Usage;
            public Int16 UsagePage;
            public Int16 InputReportByteLength;
            public Int16 OutputReportByteLength;
            public Int16 FeatureReportByteLength;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
            public Int16[] Reserved;
            public Int16 NumberLinkCollectionNodes;
            public Int16 NumberInputButtonCaps;
            public Int16 NumberInputValueCaps;
            public Int16 NumberInputDataIndices;
            public Int16 NumberOutputButtonCaps;
            public Int16 NumberOutputValueCaps;
            public Int16 NumberOutputDataIndices;
            public Int16 NumberFeatureButtonCaps;
            public Int16 NumberFeatureValueCaps;
            public Int16 NumberFeatureDataIndices;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct HidP_Value_Caps
        {
            public short UsagePage;
            public byte ReportID;
            public int IsAlias;
            public short BitField;
            public short LinkCollection;
            public short LinkUsage;
            public short LinkUsagePage;
            public int IsRange;
            public int IsStringRange;
            public int IsDesignatorRange;
            public int IsAbsolute;
            public int HasNull;
            public byte Reserved;
            public short BitSize;
            public short ReportCount;
            public short Reserved2;
            public short Reserved3;
            public short Reserved4;
            public short Reserved5;
            public short Reserved6;
            public int LogicalMin;
            public int LogicalMax;
            public int PhysicalMin;
            public int PhysicalMax;
            public short UsageMin;
            public short UsageMax;
            public short StringMin;
            public short StringMax;
            public short DesignatorMin;
            public short DesignatorMax;
            public short DataIndexMin;
            public short DataIndexMax;
        }

        //dbt.h
        [StructLayout(LayoutKind.Sequential)]
        public class DEV_BROADCAST_DEVICEINTERFACE
        {
            public int dbcc_size;
            public int dbcc_devicetype;
            public int dbcc_reserved;
            public Guid dbcc_classguid;
            public short dbcc_name;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public class DEV_BROADCAST_DEVICEINTERFACE_1
        {
            public int dbcc_size;
            public int dbcc_devicetype;
            public int dbcc_reserved;
            [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.U1, SizeConst = 16)]
            public byte[] dbcc_classguid;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 255)]
            public char[] dbcc_name;
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

        [StructLayout(LayoutKind.Sequential)]
        public class DEV_BROADCAST_HDR
        {
            public int dbch_size;
            public int dbch_devicetype;
            public int dbch_reserved;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct SP_DEVINFO_DATA
        {
            public int cbSize;
            public System.Guid ClassGuid;
            public int DevInst;
            public int Reserved;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct OVERLAPPED
        {
            public int Internal;
            public int InternalHigh;
            public int Offset;
            public int OffsetHigh;
            public int hEvent;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct DEV_BROADCAST_VOLUME
        {
            public int dbcv_size;
            public int dbcv_devicetype;
            public int dbcv_reserved;
            public int dbcv_unitmask;
        }

        #endregion

        #region USB API

        [DllImport("setupapi.dll")]
        public static extern int CM_Request_Device_Eject(
                                 int dnDevInst,
                                 out PNP_VETO_TYPE pVetoType,
                                 StringBuilder pszVetoName,
                                 int ulNameLength,
                                 int ulFlags
                                 );

        [DllImport("setupapi.dll", EntryPoint = "CM_Request_Device_Eject")]
        public static extern int CM_Request_Device_Eject_NoUi(
                                 int dnDevInst,
                                 IntPtr pVetoType,
                                 StringBuilder pszVetoName,
                                 int ulNameLength,
                                 int ulFlags
                                 );

        [DllImport("kernel32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        public static extern bool GetDiskFreeSpace(
                                  string lpRootPathName,
                                  out uint lpSectorsPerCluster,
                                  out uint lpBytesPerSector,
                                  out uint lpNumberOfFreeClusters,
                                  out uint lpTotalNumberOfClusters
                                  );

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

        [DllImport("shell32.dll")]
        public static extern int ShellAbout(IntPtr hWnd, string szApp, string szOtherStuff, IntPtr hIcon);

        [DllImport("kernel32.dll")]
        public static extern int CancelIo(IntPtr hFile);

        //Cihaz'�n �retici string'ini okur
        [DllImport("hid.dll")]
        public static extern bool HidD_GetManufacturerString(
                                  IntPtr hDeviceObject,
                                  IntPtr Buffer,
                                  int BufferLength
                                  );

        //Cihaz'�n �r�n string'ini okur
        [DllImport("hid.dll")]
        public static extern bool HidD_GetProductString(
                                  IntPtr hDeviceObject,
                                  IntPtr Buffer,
                                  int BufferLength
                                  );

        //Usb ayg�t�n ��kart�lma yada tak�lma olaylar�n� uygulama taraf�ndan
        //yakalayabilmek i�in ayg�t'� kay�t eden api
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern IntPtr RegisterDeviceNotification(
                                    IntPtr hRecipient,
                                    IntPtr NotificationFilter,
                                    int Flags
                                    );

        //RegisterDeviceNotification ile yap�lan kayd� kald�r�r
        [DllImport("user32.dll")]
        public static extern Boolean UnregisterDeviceNotification(IntPtr Handle);

        //Windows hata mesajlar�n� g�steren API
        [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
        public static extern Int32 FormatMessage(
                                   int dwFlags,
                                   ref long lpSource,
                                   int dwMessageId,
                                   int dwLanguageZId,
                                   string lpBuffer,
                                   int nSize,
                                   int Arguments
                                   );

        //Cihazdan feature raporunu al�r
        [DllImport("hid.dll")]
        public static extern bool HidD_GetFeature(
                                  IntPtr HidDeviceObject,
                                  ref byte lpReportBuffer,
                                  int ReportBufferLength
                                  );

        //Cihazdan Input raporu al�r
        [DllImport("hid.dll")]
        public static extern bool HidD_GetInputReport(
                                  IntPtr HidDeviceObject,
                                  ref byte lpReportBuffer,
                                  int ReportBufferLength
                                  );

        //Input raporu i�in ayr�lan tampon b�y�kl���n� al�r
        [DllImport("hid.dll")]
        public static extern bool HidD_GetNumInputBuffers(IntPtr HidDeviceObject, ref int NumberBuffers);

        //Cihaz'a feature raporu g�nderir
        [DllImport("hid.dll")]
        public static extern bool HidD_SetFeature(
                                  IntPtr HidDeviceObject,
                                  ref byte lpReportBuffer,
                                  int ReportBufferLength
                                  );

        //Input rapor tampon b�y�kl���n� de�i�tirmek i�in kullan�l�r.
        [DllImport("hid.dll")]
        public static extern bool HidD_SetNumInputBuffers(IntPtr HidDeviceObject, int NumberBuffers);

        //Cihaz'a output raporu g�nderir.
        [DllImport("hid.dll")]
        public static extern bool HidD_SetOutputReport(IntPtr HidDeviceObject, ref byte lpReportBuffer, int ReportBufferLength);

        [DllImport("hid.dll")]
        public static extern int HidP_GetValueCaps(
                                 short ReportType,
                                 ref byte ValueCaps,
                                 ref short ValueCapsLength,
                                 IntPtr PreparsedData
                                 );

        //HID S�n�f� Guid numaras�n� al�r
        [DllImport("hid.dll", SetLastError = true)]
        public static extern void HidD_GetHidGuid(ref Guid HidGuid);

        //CreateFile API'si ile elde edilen cihaza ait handle'� alarak
        //�retici ve �r�n ID'lerini alan API
        //"0" d���nda bir de�er d�nerse ba�ar�l� aksi halde ba�ar�s�zd�r.
        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_GetAttributes(
                                     IntPtr HidDeviceObject,
                                     ref HIDD_ATTRIBUTES Attributes
                                     );

        //Bu API cihaz yeteneklerini i�eren Tampona bir i�aret�i al�r.
        //Bu i�aret�i bir sonraki a�ama, yani HidP_GetCaps i�in gereklidir.
        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_GetPreparsedData(
                                     IntPtr HidDeviceObject,
                                     ref IntPtr PreparsedData
                                     );

        //PreParsedData'ya ihtiya� kalmad���nda sistem kaynaklar�n�
        //serbest b�rakmak i�in kullan�l�r.
        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_FreePreparsedData(
                                     ref IntPtr PreparsedData
                                     );

        //Bir �nceki API ile ��renilen i�aret�i kullan�larak 
        [DllImport("hid.dll", SetLastError = true)]
        public static extern Int32 HidP_GetCaps(
                                   IntPtr PreparsedData,
                                   ref HIDP_CAPS Capabilities
                                   );

        //Belirtilen s�n�fa ait ayg�tlar�n aray�z setini alan fonksiyon
        //Geriye bu aray�z setine bir i�aret�i d�nd�r�r.
        [DllImport("setupapi.dll", SetLastError = true, CharSet = CharSet.Auto)]
        public static extern IntPtr SetupDiGetClassDevs(
                                    ref Guid ClassGuid,     //Ayg�t s�n�f� Guid numaras�
                                    int Enumerator,         //Opsiyonel, 0 veya null olabilir.
                                    IntPtr hwndParent,      //0
                                    int Flags               //DIGCF_PRESENT, DIGCF_DEFIVEINTERFACE de�erlerini al�r
                                    );

        //Ayg�t aray�z setinden bir aray�z yap�s� elde eden fonksiyon
        //Ba�ar�l� ise geriye 0 d���nda bir say�, ba�ar�s�z ise "0" d�nd�r�r.
        [DllImport("setupapi.dll", SetLastError = true)]
        public static extern Boolean SetupDiEnumDeviceInterfaces(
                                     IntPtr DeviceInfoSet,                            //Aray�z setine i�aret eden i�aret�i
                                     IntPtr DeviceInfoData,                           //Kullan�lm�yor
                                     ref Guid InterfaceClassGuid,                     //Ayg�t s�n�f� guid numaras�
                                     int MemberIndex,                               //Aray�z seti index'i
                                     SP_DEVICE_INTERFACE_DATA DeviceInterfaceData     //SP_DEVICE_INTERFACE_DATA yap�s�.Bilgiler buna doluyor.
                                     );

        //Belirtilen ayg�t aray�z�n'den yol bilgisini ve di�er detaylar� al�r.
        //Ba�ar�l� ise "0" d���nda bir de�er, ba�ar�s�z ise "0" d�nd�r�r.
        [DllImport("setupapi.dll", SetLastError = true, CharSet = CharSet.Auto)]
        public static extern Boolean SetupDiGetDeviceInterfaceDetail(
                                     IntPtr DeviceInfoSet,                            //Aray�z setine i�aret eden i�aret�i
                                     SP_DEVICE_INTERFACE_DATA DeviceInterfaceData,    //SP_DEVICE_INTERFACE_DATA yap�s�
                                     IntPtr DeviceInterfaceDetailData,                //SP_DEVICE_INTERFACE_DETAIL_DATA yap�s�na i�aret�i    
                                     int DeviceInterfaceDetailDataSize,               //SP_DEVICE_INTERFACE_DETAIL_DATA + device path uzunlu�u
                                     ref Int32 RequiredSize,                          //Yukar�daki tampon i�in gerekli uzunluk
                                     IntPtr DeviceInfoData                            //Kullan�lm�yor
                                     );

        //Bu fonksyion ayg�t aray�z setini sonland�r�r ve ayr�lan haf�zay� serbest b�rak�r.
        //Ba�ar�l� ise "0" d���nda bir de�er, ba�ar�s�z ise "0" d�nd�r�r.
        [DllImport("setupapi.dll", SetLastError = true)]
        public static extern Boolean SetupDiDestroyDeviceInfoList(IntPtr DeviceInfoSet);

        //Cihaz ile ili�kili bir handle de�eri elde eden fonksiyon
        [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
        public static extern IntPtr CreateFile(
                                    [MarshalAs(UnmanagedType.LPTStr)] string lpFileName,
                                    uint dwDesiredAccess,
                                    uint dwShareMode,
                                    ref SECURITY_ATTRIBUTES lpSecurityAttributes,
                                    short dwCreationDisposition,
                                    uint dwFlagsAndAttributes,
                                    IntPtr hTemplateFile
                                    );

        //CreateFile ile ekde edilen handle kullan�larak veri yazar
        [DllImport("kernel32.dll")]
        public static extern int WriteFile(
                                 IntPtr hFile,
                                 ref byte lpBuffer,
                                 int nNumberOfBytesToWrite,
                                 ref int lpNumberOfBytesWritten,
                                 int lpOverlapped
                                 );

        //CreateFile ile ekde edilen handle kullan�larak veri yazar
        [DllImport("kernel32.dll")]
        public static extern int ReadFile(
                                 IntPtr hFile,
                                 ref byte lpBuffer,
                                 int nNumberOfBytesToRead,
                                 ref int lpNumberOfBytesRead,
                                 ref OVERLAPPED lpOverlapped
                                 );

        //Handle de�eri verilen ayg�t i�in ayr�lan kaynaklar� serbest b�rak�r.
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern Boolean CloseHandle(IntPtr hFileHandle);

        //ReadFile veya WriteFile bitti�inde sinyalli duruma kurulacak
        //bir olay nesnesi olu�turur.Geriye d�nen nesne de�eri OVERLAPPED yap�s�n�n
        //hEvent elaman�na atan�r.Bu yap� ise ReadFile veya WriteFile'�n lpOverlapped
        //parametresine ge�ilir.
        [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
        public static extern int CreateEvent(
                                 ref SECURITY_ATTRIBUTES SecurityAttributes,
                                 int bManualReset,
                                 int bInitialState,
                                 string lpName
                                 );

        //CreateEvent ile olu�turulan olay nesnesi ReadFile veya WriteFile
        //apileri i�in OVERLAPPED yap�s�n�n hEvent eleman�na verildikten sonra
        //ReadFile veya WriteFile'�n bitmesini bekler.Okuma i�lemi bitti�inde CreateEvent
        //ile olu�turulan olay nesnesi sinyalli duruma set olur ve fonksiyon sona erer.
        //dwMiliseconds ile s�re belirtilirse bu s�re kadar beklenir.
        [DllImport("kernel32.dll")]
        public static extern int WaitForSingleObject(int hHandle, int dwMiliseconds);

        //Buffer'da bekleyen input rapor'unu temizler.
        [DllImport("hid.dll")]
        public static extern bool HidD_FlushQueue(IntPtr hDeviceObject);

        #endregion

        #region Static Methodlar

        static long lRefZero = 0;
        static int Result;
        static int ResultCode;
        static string ResultString = string.Empty;

        public static string ResultDisplayOfApiCall(string FunctionName)
        {
            try
            {
                ResultString = new string(Convert.ToChar(0), 129);
                ResultCode = Marshal.GetLastWin32Error();

                Result = FormatMessage(
                         FORMAT_MESSAGE_FROM_SYSTEM,
                         ref lRefZero,
                         ResultCode,
                         0,
                         ResultString,
                         128,
                         0
                );

                if (Result > 2) ResultString = ResultString.Remove(Result - 2, 2);
            }
            catch (Exception err)
            {
                return err.Message;
            }

            return FunctionName + " : " + ResultString.ToString();
        }

        #endregion
    }
}
