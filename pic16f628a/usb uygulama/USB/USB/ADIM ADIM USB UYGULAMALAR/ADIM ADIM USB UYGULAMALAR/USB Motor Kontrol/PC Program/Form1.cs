using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using System.Globalization;
using System.Windows.Forms;
using USBManagement;
using System.Threading;
using System.Diagnostics;

namespace USBMotorKontrol
{
    public partial class frmMain : Form
    {
        #region Sabitler

        //PIC'e gönderilerek motor'un çalýþmasý için TMR2'yi
        //aktif-pasif yapacak komutlar
        const byte OPEN_MOTOR = 25;
        const byte CLOSE_MOTOR = 26;

        #endregion

        #region Global Deðiþkenler

        UsbHidDevice USBService;

        byte[] Pwm = new byte[2];

        short VendorID = short.Parse("1986", NumberStyles.HexNumber);
        short ProductID = short.Parse("3543", NumberStyles.HexNumber);

        #endregion

        #region Kurucu Method

        public frmMain()
        {
            InitializeComponent();

            USBService = new UsbHidDevice();

            USBService.DeviceStateChanged += new DeviceStateChangeEventHandler(OnChangeDeviceState);

            btnFast.Enabled = false;
            btnSlow.Enabled = false;
            cbRegister.Enabled = false;
        }

        #endregion

        #region Motor Kontrol Kodlarý

        /*
         * Kullanýcý baþlat butonuna bastýðýnda ilk olarak cihaz ile baðlantý
         * kurulmaya çalýþýlýr.Baðlantý kurulursa akýþ borusu açýlmýþ demektir.Bu 
         * iþlem için Device sýnýfýnýn OpenPipe() methodu kullanýlýr.Bu method VendorID
         * ve ProductID'si verilen USB cihaz'ý bulur ve baðlantýyý açar.Ýþlem baþarýlý ise
         * true döndürür.Baðlantý kurulduysa Pwm buffer'ýnýn ilk byte'ýna OPEN_MOTOR
         * komutu yüklenir ve WritePipe ile PIC'e yollanýr.Bu durumda motor çalýþmaya baþlar.
         *
         */
        private void btnStart_Click(object sender, EventArgs e)
        {
            bool Success = false;

            try
            {
                if (!USBService.IsOpen)
                {
                    //VendorId ve ProductId'si verilen cihazý bul ve akýþ
                    //borusunu aç
                    if (USBService.OpenPipe(VendorID, ProductID))
                    {
                        lbDeviceStatus.Text = "Baðlantý kuruldu";
                        //T2CONbits.TMR2ON = 1;
                        Pwm[0] = OPEN_MOTOR;

                        if(USBService.IsWindowsXpOrLater)
                            Success = USBService.WritePipe(Pwm, TransactionType.Interrupt);
                        else
                            Success = USBService.WritePipe(Pwm, TransactionType.Control);

                        if (Success) this.Text = "Runing Motor";
                        btnFast.Enabled = true;
                        btnSlow.Enabled = true;
                        btnStart.Enabled = false;
                        cbRegister.Enabled = true;

                        Pwm[0] = 0;
                        Pwm[1] = 0;
                        lbStatus.Text = "0x00";

                        USBService.DeviceArrived += new HidDeviceArrivedEventHandler(OnDeviceAttached);
                        USBService.DeviceRemoved += new HidDeviceRemovedEventHandler(OnDeviceDetached);
                    }
                    else
                        lbDeviceStatus.Text = "Baðlantý kurulamadý";
                }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message,"Start Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            try
            {
                if (USBService.IsOpen)
                {
                    //CCPR2L ve CCP2CON<5:4> yazmaçlarýný temizle
                    //Bu sayede Pwm iþareti low konumunda tutulur
                    Pwm[0] = 0;
                    Pwm[1] = 0;
                    if(USBService.IsWindowsXpOrLater)
                        USBService.WritePipe(Pwm, TransactionType.Interrupt);
                    else
                        USBService.WritePipe(Pwm, TransactionType.Control);

                    //T2CONbits.TMR2ON = 0;
                    Pwm[0] = CLOSE_MOTOR;
                    if (USBService.IsWindowsXpOrLater)
                        USBService.WritePipe(Pwm, TransactionType.Interrupt);
                    else
                        USBService.WritePipe(Pwm, TransactionType.Control);

                    if (USBService.ClosePipe())
                    {
                        lbDeviceStatus.Text = "Baðlantý kapatýldý";
                        this.Text = "Stopped Motor";
                        btnFast.Enabled = false;
                        btnSlow.Enabled = false;
                        btnStart.Enabled = true;
                        cbRegister.Enabled = false;
                        cbRegister.Checked = false;

                        Pwm[0] = 0;
                        lbStatus.Text = "0x00";
                    }
                    else
                        lbDeviceStatus.Text = "Baðlantý kapatýlamadý";
                }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "Stop Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnSlow_Click(object sender, EventArgs e)
        {
            if(Pwm[1] != 0)           //Üst byte 0'a eþit olmadýðý sürece azalt
            {
                Pwm[1]--;
                LoadUSBDeviceToPwmSpeed(Pwm);
            }
            else if (Pwm[0] == 255)   //Alt byte 255'e eþi ise 5 çýkar.Çünkü 50 birimlik adýmlarla azaltýlacak(en son sýfýr kalmasý için)
            {
                Pwm[0] -= 5;
                LoadUSBDeviceToPwmSpeed(Pwm);
            }
            else if (Pwm[0] != 0)     //Alt byte'ý sýfýr olmadýðý sürece 50 birimlik adýmlarla azalt
            {
                Pwm[0] -= 50;
                LoadUSBDeviceToPwmSpeed(Pwm);
            }
        }

        private void btnFast_Click(object sender, EventArgs e)
        {
            if (Pwm[0] == 250)        //50 birimlik arttýrma sonucu alt byte 250 olduysa 255'e tamamla
            {
                Pwm[0] += 5;
                LoadUSBDeviceToPwmSpeed(Pwm);
            }
            else if (Pwm[0] == 255)   //Alt byte 255 olduysa üst byte'ý 3'e kadar arttýr
            {
                if (Pwm[1] != 3) Pwm[1]++;
                LoadUSBDeviceToPwmSpeed(Pwm);
            }
            else
            {
                Pwm[0] += 50;        //50 birimlik adýmlarla arttýr
                LoadUSBDeviceToPwmSpeed(Pwm);
            }
        }

        private void LoadUSBDeviceToPwmSpeed(byte[] buffer)
        {
            try
            {
                lbStatus.Text = "0x" + buffer[1].ToString("x") + buffer[0].ToString("x");

                if (USBService.IsWindowsXpOrLater)
                    if (USBService.WritePipe(buffer, TransactionType.Interrupt))
                        lbDeviceStatus.Text = "Bilgiler yüklendi";
                    else
                        lbDeviceStatus.Text = "Bilgiler yüklenemedi";
                else
                    if (USBService.WritePipe(buffer, TransactionType.Control))
                        lbDeviceStatus.Text = "Bilgiler yüklendi";
                    else
                        lbDeviceStatus.Text = "Bilgiler yüklenemedi";

                Thread.Sleep(10);
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "Load Pwm Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void frmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            try
            {
                if (USBService.IsOpen) USBService.ClosePipe();
            }
            catch
            {
            }
        }

        protected override void WndProc(ref Message m)
        {
            USBService.ProcessDeviceMessage(m);
            base.WndProc(ref m);
        }

        private void cbRegister_Click(object sender, EventArgs e)
        {
            bool Success = false;

            try
            {
                if (cbRegister.Checked)
                {
                    Success = USBService.RegisterDevice(this.Handle);
                    if (Success)
                    {
                        lbDeviceStatus.Text = "Cihaz kaydedildi";
                        cbRegister.Text = "Bu cihazý kaldýr";
                        btnStart.Enabled = false;
                    }
                }
                else
                {
                    Success = USBService.UnRegisterDevice();
                    if (Success)
                    {
                        lbDeviceStatus.Text = "Cihaz kaldýrýldý";
                        cbRegister.Text = "Bu cihazý kaydet";
                        if (USBService.IsOpen) btnStart.Enabled = false; else btnStart.Enabled = true;
                        btnStart.Enabled = false;
                    }
                    else
                    {
                        lbDeviceStatus.Text = "Cihaz kaldýrýlamadý";
                    }
                }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "Register Device Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void OnChangeDeviceState(object sender, DeviceStateChangeEventArgs e)
        {
            if (e.CurrentState == DeviceState.Opened)
            {
                tslStatusText.Text = "Device State Changed!! Curren State : Opened";
                Debug.WriteLine("Device State Changed!! Curren State : Opened");
            }
            else if (e.CurrentState == DeviceState.Closed)
            {
                tslStatusText.Text = "Device State Changed!! Curren State : Closed";
                Debug.WriteLine("Device State Changed!! Curren State : Closed");
            }
            else if (e.CurrentState == DeviceState.Waiting)
            {
                tslStatusText.Text = "Device State Changed!! Curren State : Waiting";
                Debug.WriteLine("Device State Changed!! Curren State : Waiting");
            }
            else if (e.CurrentState == DeviceState.Ready)
            {
                tslStatusText.Text = "Device State Changed!! Curren State : Ready";
                Debug.WriteLine("Device State Changed!! Curren State : Ready");
            }
            else
            {
                tslStatusText.Text = "Device State Changed!! Curren State : None";
                Debug.WriteLine("Device State Changed!! Curren State : None");
            }
        }

        private void OnDeviceAttached(object sender, EventArgs e)
        {
            bool Success;

            tslStatusText.Text = "Atached Device";
            Success = USBService.OpenPipe(VendorID, ProductID);
            if (Success)
            {
                btnStart.Enabled = false;
                cbRegister.Enabled = true;
                cbRegister.Checked = true;
                lbDeviceStatus.Text = "Baðlantý kuruldu";

                Pwm[0] = OPEN_MOTOR;
                if (USBService.IsWindowsXpOrLater)
                    Success = USBService.WritePipe(Pwm, TransactionType.Interrupt);
                else
                    Success = USBService.WritePipe(Pwm, TransactionType.Control);
                if (Success) this.Text = "Runing Motor";

                Pwm[0] = 0;
                Pwm[1] = 0;
                lbStatus.Text = "0x00";
            }
        }

        private void OnDeviceDetached(object sender, EventArgs e)
        {
            tslStatusText.Text = "Atached Device";
            if (USBService.IsOpen) USBService.ClosePipe();
            btnStart.Enabled = false;
            cbRegister.Checked = false;
            cbRegister.Enabled = false;
            lbDeviceStatus.Text = "Cihaz ile baðlantý kesildi";
            this.Text = "Cihaz bekleniyor..";

            Pwm[0] = 0;
            Pwm[1] = 0;
            lbStatus.Text = "0x00";
        }

        #endregion
    }
}