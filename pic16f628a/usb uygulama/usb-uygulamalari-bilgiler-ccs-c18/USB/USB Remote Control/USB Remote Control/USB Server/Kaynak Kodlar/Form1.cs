#region Namespace Decleration

using System;
using System.Globalization;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Windows.Forms;
using USBManagement;

#endregion

namespace USBServer
{
    public partial class frmMain : Form
    {
        #region Sabitler

        /// <summary>
        /// 
        /// </summary>
        const int ON_OFF_INDEX = 1;
        const int ROLE_NO_INDEX = 0;
        const int PORT_NUMBER = 8090;
        const int WM_DEVICECHANGE = 0x219;
        const int DBT_DEVICEARRIVAL = 0x8000;
        const int DBT_DEVTYP_DEVICEINTERFACE = 5;

        #endregion

        #region Kurucu Method

        public frmMain()
        {
            InitializeComponent();
            StartupServer();
        }

        #endregion

        #region Global Nesneler ve De�i�enler

        bool bControl;
        short vid, pid;
        byte[] bSocketBuffer;
        Thread thListener;
        Socket sckMainSocket;
        UsbHidDevice hidDevice;
        delegate void MarshalToForm();

        #endregion

        #region Server Startup Kodlar�

        /// <summary>
        /// Bu method ilk olarak USB ayg�t ile haberle�me ve kontrol i�lemleri
        /// i�in UsbHidDevice nesnesini yap�land�r�r ard�ndan USB cihaz ile ba�lant�
        /// kurmaya �al���r.E�er ba�lant� kurulursa 8090 n�lu portu dinlemeye alacak thread'i
        /// yap�land�ran ActivateServerSocket() methodunu �a��r�r.USB cihaz ile ba�lant� kurulamazsa
        /// port dinleme i�lemi yap�lmaz, pencere fonksiyonunda cihaz de�i�imleri takip edilerek
        /// USB cihaz�n sisteme ba�lanmas� beklenir.
        /// </summary>
        private void StartupServer()
        {
            //USB Initialize
            vid = short.Parse("1986", NumberStyles.HexNumber);
            pid = short.Parse("3543", NumberStyles.HexNumber);

            try
            {
                hidDevice = new UsbHidDevice();
                hidDevice.DeviceArrived += new HidDeviceArrivedEventHandler(OnArrivedUsbDevice);
                hidDevice.DeviceRemoved += new HidDeviceRemovedEventHandler(OnRemovedUsbDevice);
                hidDevice.DeviceStateChanged += new DeviceStateChangeEventHandler(OnStateChangeUsbDevice);

                if (hidDevice.OpenPipe(vid, pid))
                {
                    bSocketBuffer = new byte[hidDevice.OutputBufferSize];

                    ActivateServerSocket();
                    hidDevice.RegisterDevice(Handle);
                    UsbStatus.Icon = Properties.Resources.hard_drive;
                    UsbStatus.Text = "USB ile ba�lant� kuruldu";
                }
                else
                {
                    UsbStatus.Icon = Properties.Resources.help;
                    UsbStatus.Text = "USB ile ba�lant� kurulamad�";
                    SckStatus.Text = "Server ba�lat�lamad�...";
                }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "SetupServer Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// Bu method 8090 nolu portu dinleyecek thread'i yap�land�r�r ve 
        /// ba�lat�r.Bu method �a�r�m�ndan sonra Server aktif olur ve gelen ba�lant�
        /// isteklerini dinlemeye ba�lar.
        /// </summary>
        private void ActivateServerSocket()
        {
            thListener = new Thread(new ThreadStart(ListenPort));
            thListener.IsBackground = true;
            thListener.Priority = ThreadPriority.Highest;
            thListener.Start();

            SckStatus.Text = "Server ba�lant� bekliyor...";
        }

        #endregion

        #region USB Kodlar�

        /// <summary>
        /// Bu olay UsbHidDevice nesnesi taraf�ndan, sisteme �nceden kaydedilmi�
        /// fakat sonradan USB portundan ��kar�lan bir cihaz�n tekrar USB portuna tak�lmas�ndan
        /// sonra �a�r�l�r.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OnArrivedUsbDevice(object sender, EventArgs e)
        {
            try
            {
                if (!hidDevice.IsOpen)
                {
                    if (hidDevice.OpenPipe(vid, pid))
                    {
                        UsbStatus.Icon = Properties.Resources.hard_drive;
                        UsbStatus.Text = "USB ile ba�lant� kuruldu";
                        WriteMessageToDisplay("USB ile tekrar ba�lant� kuruldu");
                    }
                }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "Device Arrived Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// Bu olay UsbHidDevice nesnesi taraf�ndan, sisteme �nceden kaydedilmi� bir
        /// cihaz USB portundan herhangi bir zamanda ��kart�ld���nda �a�r�l�r.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OnRemovedUsbDevice(object sender, EventArgs e)
        {
            try
            {
                if (hidDevice.IsOpen)
                {
                    if (hidDevice.ClosePipe())
                    {
                        UsbStatus.Icon = Properties.Resources.help;
                        UsbStatus.Text = "USB ile ba�lant� kesildi";
                        WriteMessageToDisplay(DateTime.Now.ToLongTimeString() + " USB cihaz ile ba�lant� kesildi.Bekliyor...");
                    }
                }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "Device Removed Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// Bu olay UsbHidDevice nesnesi taraf�ndan, ba�lant�da bir de�i�iklik oldu�unda
        /// �a�r�l�r.Ba�lant�da ne gibi bir de�i�iklik oldu�u DeviceStateChangeEventArgs
        /// tipindeki e adl� olay arg�man�n�n CurrentState �zelli�i ile ��renilir.Bu �zellik
        /// DeviceState tipinde bir numaraland�r�c� de�eri d�nd�r�r.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OnStateChangeUsbDevice(object sender, DeviceStateChangeEventArgs e)
        {
            WriteMessageToDisplay("Usb Current State : " + e.CurrentState.ToString());
        }


        /// <summary>
        /// Bu method buffer b�lgesindeki bilgileri USB cihaza g�nderir.
        /// </summary>
        /// <param name="buffer">USB cihaza g�nderilecek bilgiler</param>
        private void SendCommandToUsbDevice(byte[] buffer)
        {
            try
            {
                if (hidDevice.IsOpen)
                {
                    if (hidDevice.WritePipe(bSocketBuffer, TransactionType.Interrupt))
                        WriteMessageToDisplay("Komut USB ayg�t'a g�nderildi");
                    else
                        WriteMessageToDisplay("Komut USB ayg�t taraf�ndan okunamad�");
                }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "SendCommand Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        #endregion

        #region Command Parse

        /// <summary>
        /// 
        /// </summary>
        /// <param name="dataCount"></param>
        private void ParseCommand(int dataCount)
        {
            try
            {
                MarshalToForm mtof = delegate
                {
                    if (dataCount > 0)
                    {
                        if (bSocketBuffer[ON_OFF_INDEX] == 1)
                            WriteMessageToDisplay(bSocketBuffer[ROLE_NO_INDEX].ToString() + " numaral� r�le aktif edildi");
                        else
                            WriteMessageToDisplay(bSocketBuffer[ROLE_NO_INDEX].ToString() + " numaral� r�le kapat�ld�");

                        SendCommandToUsbDevice(bSocketBuffer);
                    }
                };
                Invoke(mtof);
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "ParseCommand Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        #endregion

        #region Socket Kodlar�

        /// <summary>
        /// 
        /// </summary>
        private void ListenPort()
        {
            try
            {
                IPEndPoint IPEndP = new IPEndPoint(IPAddress.Any, PORT_NUMBER);
                sckMainSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                sckMainSocket.Bind(IPEndP);
                sckMainSocket.Listen(10);

                while (true)
                {
                    Socket sckForeignSocket = sckMainSocket.Accept();
                    if (sckForeignSocket.Connected)
                    {
                        MarshalToForm mtof = delegate {
                            WriteMessageToDisplay(sckForeignSocket.RemoteEndPoint.ToString() + " ile ba�lant� kuruldu");
                        };
                        Invoke(mtof);
                        ProcessSocket(sckForeignSocket);
                    }
                    Thread.Sleep(50);
                }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "ListenPort Error ", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private void ProcessSocket(Socket sckClient)
        {
            try
            {
                while (sckClient.Connected)
                {
                    int totalRec = sckClient.Receive(bSocketBuffer);
                    ParseCommand(totalRec);

                    Thread.Sleep(50);
                }
            }
            catch
            {
            }
        }

        #endregion

        #region Pencere Fonksiyonu

        /// <summary>
        /// 
        /// </summary>
        /// <param name="m"></param>
        protected override void WndProc(ref Message m)
        {
            if (hidDevice != null && hidDevice.IsRegistered)
            {
                bControl = true;
                hidDevice.ProcessDeviceMessage(m);
            }
            else
            {
                bControl = false;
            }

            if (!bControl)
            {
                if (m.Msg == WM_DEVICECHANGE)
                {
                    if (hidDevice != null && !hidDevice.IsOpen)
                    {
                        if (hidDevice.OpenPipe(vid, pid))
                        {
                            bSocketBuffer = new byte[hidDevice.OutputBufferSize];
                            hidDevice.RegisterDevice(Handle);
                            ActivateServerSocket();

                            UsbStatus.Icon = Properties.Resources.hard_drive;
                            UsbStatus.Text = "USB ile ba�lant� kuruldu";
                        }
                    }
                }
            }
            base.WndProc(ref m);
        }

        #endregion

        #region Display Mesaj Fonksiyonu

        /// <summary>
        /// 
        /// </summary>
        /// <param name="txt"></param>
        private void WriteMessageToDisplay(string txt)
        {
            txtLog.Text = txtLog.Text + DateTime.Now.ToLongTimeString() + " " + txt + "\r\n";
            txtLog.SelectionStart = 0;
        }

        #endregion

        #region Standart Form Kodlar�

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void frmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            try
            {
                if (hidDevice.IsRegistered) hidDevice.UnRegisterDevice();
                if (hidDevice.IsOpen) hidDevice.ClosePipe();
            }
            catch
            {
            }
        }

        #endregion
    }
}