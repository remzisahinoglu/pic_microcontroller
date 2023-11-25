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

        //PIC'e g�nderilerek motor'un �al��mas� i�in TMR2'yi
        //aktif-pasif yapacak komutlar
        const byte OPEN_MOTOR = 25;
        const byte CLOSE_MOTOR = 26;

        #endregion

        #region Global De�i�kenler

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

        #region Motor Kontrol Kodlar�

        /*
         * Kullan�c� ba�lat butonuna bast���nda ilk olarak cihaz ile ba�lant�
         * kurulmaya �al���l�r.Ba�lant� kurulursa ak�� borusu a��lm�� demektir.Bu 
         * i�lem i�in Device s�n�f�n�n OpenPipe() methodu kullan�l�r.Bu method VendorID
         * ve ProductID'si verilen USB cihaz'� bulur ve ba�lant�y� a�ar.��lem ba�ar�l� ise
         * true d�nd�r�r.Ba�lant� kurulduysa Pwm buffer'�n�n ilk byte'�na OPEN_MOTOR
         * komutu y�klenir ve WritePipe ile PIC'e yollan�r.Bu durumda motor �al��maya ba�lar.
         *
         */
        private void btnStart_Click(object sender, EventArgs e)
        {
            bool Success = false;

            try
            {
                if (!USBService.IsOpen)
                {
                    //VendorId ve ProductId'si verilen cihaz� bul ve ak��
                    //borusunu a�
                    if (USBService.OpenPipe(VendorID, ProductID))
                    {
                        lbDeviceStatus.Text = "Ba�lant� kuruldu";
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
                        lbDeviceStatus.Text = "Ba�lant� kurulamad�";
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
                    //CCPR2L ve CCP2CON<5:4> yazma�lar�n� temizle
                    //Bu sayede Pwm i�areti low konumunda tutulur
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
                        lbDeviceStatus.Text = "Ba�lant� kapat�ld�";
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
                        lbDeviceStatus.Text = "Ba�lant� kapat�lamad�";
                }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "Stop Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnSlow_Click(object sender, EventArgs e)
        {
            if(Pwm[1] != 0)           //�st byte 0'a e�it olmad��� s�rece azalt
            {
                Pwm[1]--;
                LoadUSBDeviceToPwmSpeed(Pwm);
            }
            else if (Pwm[0] == 255)   //Alt byte 255'e e�i ise 5 ��kar.��nk� 50 birimlik ad�mlarla azalt�lacak(en son s�f�r kalmas� i�in)
            {
                Pwm[0] -= 5;
                LoadUSBDeviceToPwmSpeed(Pwm);
            }
            else if (Pwm[0] != 0)     //Alt byte'� s�f�r olmad��� s�rece 50 birimlik ad�mlarla azalt
            {
                Pwm[0] -= 50;
                LoadUSBDeviceToPwmSpeed(Pwm);
            }
        }

        private void btnFast_Click(object sender, EventArgs e)
        {
            if (Pwm[0] == 250)        //50 birimlik artt�rma sonucu alt byte 250 olduysa 255'e tamamla
            {
                Pwm[0] += 5;
                LoadUSBDeviceToPwmSpeed(Pwm);
            }
            else if (Pwm[0] == 255)   //Alt byte 255 olduysa �st byte'� 3'e kadar artt�r
            {
                if (Pwm[1] != 3) Pwm[1]++;
                LoadUSBDeviceToPwmSpeed(Pwm);
            }
            else
            {
                Pwm[0] += 50;        //50 birimlik ad�mlarla artt�r
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
                        lbDeviceStatus.Text = "Bilgiler y�klendi";
                    else
                        lbDeviceStatus.Text = "Bilgiler y�klenemedi";
                else
                    if (USBService.WritePipe(buffer, TransactionType.Control))
                        lbDeviceStatus.Text = "Bilgiler y�klendi";
                    else
                        lbDeviceStatus.Text = "Bilgiler y�klenemedi";

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
                        cbRegister.Text = "Bu cihaz� kald�r";
                        btnStart.Enabled = false;
                    }
                }
                else
                {
                    Success = USBService.UnRegisterDevice();
                    if (Success)
                    {
                        lbDeviceStatus.Text = "Cihaz kald�r�ld�";
                        cbRegister.Text = "Bu cihaz� kaydet";
                        if (USBService.IsOpen) btnStart.Enabled = false; else btnStart.Enabled = true;
                        btnStart.Enabled = false;
                    }
                    else
                    {
                        lbDeviceStatus.Text = "Cihaz kald�r�lamad�";
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
                lbDeviceStatus.Text = "Ba�lant� kuruldu";

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
            lbDeviceStatus.Text = "Cihaz ile ba�lant� kesildi";
            this.Text = "Cihaz bekleniyor..";

            Pwm[0] = 0;
            Pwm[1] = 0;
            lbStatus.Text = "0x00";
        }

        #endregion
    }
}