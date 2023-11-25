using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using System.Windows.Forms;
using System.Globalization;
using USBManagement;

namespace USBApplication
{
    public partial class frmMain : Form
    {
        short vid, pid;
        UsbHidDevice USBDevice;

        public frmMain()
        {
            InitializeComponent();

            USBDevice = new UsbHidDevice();
            vid = short.Parse(txtVendorId.Text, NumberStyles.HexNumber);
            pid = short.Parse(txtProductId.Text, NumberStyles.HexNumber);

            cbRegister.Enabled = false;
            StatusBar.Panels[0].Text = "JaBBaSoft USB Application - Hazýr";
        }

        private void btnCheckUsb_Click(object sender, EventArgs e)
        {
            try
            {
                USBDevice.DeviceStateChanged += new DeviceStateChangeEventHandler(OnChangeState);
                if (USBDevice.OpenPipe(vid, pid))
                {
                    StatusBar.Panels[0].Text = "Cihaz ile baðlantý kuruldu";
                    DeviceType devType = USBDevice.UsbDeviceType;
                    StatusBar.Panels[2].Text = "Device Type : " + devType.ToString("G");

                    ListViewItem kayit = USBList.Items.Add(USBDevice.DeviceProduct);
                    kayit.SubItems.Add(txtVendorId.Text);
                    kayit.SubItems.Add(txtProductId.Text);
                    kayit.SubItems.Add(USBDevice.DeviceManufacturer);
                    btnCheckUsb.Enabled = false;
                    cbRegister.Enabled = true;

                    USBDevice.DeviceArrived += new HidDeviceArrivedEventHandler(OnDeviceAttached);
                    USBDevice.DeviceRemoved += new HidDeviceRemovedEventHandler(OnDeviceDetached);
                }
                else
                    StatusBar.Panels[0].Text = "Cihaz ile baðlantý kurulamadý";
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message,"Find USB Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void frmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            try
            {
                if (USBDevice.IsOpen)
                    if (USBDevice.ClosePipe())
                        StatusBar.Panels[0].Text = "Akýþ borusu kapatýldý";
                    else
                        StatusBar.Panels[0].Text = "Baðlantý kapatýlamýyor..";

                if (USBDevice.IsRegistered) USBDevice.UnRegisterDevice();
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnSend_Click(object sender, EventArgs e)
        {
            try
            {
                if (USBDevice.IsOpen)
                {
                    byte[] OutputReport = Encoding.ASCII.GetBytes(txtSendData.Text);
                    if (rbControlTrf.Checked)
                    {
                        if (USBDevice.WritePipe(OutputReport, TransactionType.Control))
                            StatusBar.Panels[0].Text = "Veriler gönderilirdi..";
                        else
                            StatusBar.Panels[0].Text = "Veriler Gönderilemedi";
                    }
                    else if (rbInterruptTrf.Checked)
                    {
                        if (!v98GoldExceptionMsg())
                        {
                            if (USBDevice.WritePipe(OutputReport, TransactionType.Interrupt))
                                StatusBar.Panels[0].Text = "Veriler gönderilirdi..";
                            else
                                StatusBar.Panels[0].Text = "Veriler Gönderilemedi";
                        }
                    }
                }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "Send Data Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnRead_Click(object sender, EventArgs e)
        {
            try
            {
                if (USBDevice.IsOpen)
                {
                    byte[] InputReport = new byte[USBDevice.InputBufferSize];
                    if (rbControlTrf.Checked)
                    {
                        if (USBDevice.ReadPipe(ref InputReport, InputReport.Length, TransactionType.Control))
                        {
                            StatusBar.Panels[0].Text = "Veriler Okundu..";
                            for (int i = 0; i < InputReport.Length; i++) lbxRecData.Items.Add(InputReport[i].ToString("x"));
                        }
                        else
                            StatusBar.Panels[0].Text = "Veriler Okunamýyor";
                    }
                    else if (rbInterruptTrf.Checked)
                    {
                        if (v98GoldExceptionMsg()) return;
                        if (USBDevice.ReadPipe(ref InputReport, InputReport.Length, TransactionType.Interrupt))
                        {
                            StatusBar.Panels[0].Text = "Veriler Okundu..";
                            for (int i = 0; i < InputReport.Length; i++) lbxRecData.Items.Add(InputReport[i].ToString("x"));
                        }
                        else
                            StatusBar.Panels[0].Text = "Veriler Okunamýyor";
                    }
                }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "Read Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            lbxRecData.Items.Clear();
            txtSendData.Text = string.Empty;
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            try
            {
                if (USBDevice.IsOpen)
                {
                    if (USBDevice.ClosePipe())
                        StatusBar.Panels[0].Text = "Akýþ borusu kapatýldý.";
                    else
                        StatusBar.Panels[0].Text = "Baðlantý kapatýlamýyor";
                }

                USBList.Items.Clear();
                USBDevice.UnRegisterDevice();
                btnCheckUsb.Enabled = true;
                cbRegister.Checked = false;
                cbRegister.Enabled = false;
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "Close Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void cbRegister_Click(object sender, EventArgs e)
        {
            bool Success = false;

            try
            {
                if (cbRegister.Checked)
                {
                    Success = USBDevice.RegisterDevice(this.Handle);
                    if (Success)
                    {
                        cbRegister.Text = "Cihaz sisteme kaydedildi..";
                        btnCheckUsb.Enabled = false;
                        cbRegister.Text = "Bu cihazý kaldýr";
                    }
                }
                else
                {
                    Success = USBDevice.UnRegisterDevice();
                    if (Success)
                    {
                        StatusBar.Panels[0].Text = "Cihaz sistemden kaldýrýldý..";
                        if (USBDevice.IsOpen) btnCheckUsb.Enabled = false; else btnCheckUsb.Enabled = true;
                        cbRegister.Text = "Bu cihazý kaydet";
                    }
                    else
                    {
                        StatusBar.Panels[0].Text = "Cihaz sistemden kaldýrýlamadý..";
                    }
                }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "Register Device Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void tsHakkinda_Click(object sender, EventArgs e)
        {
            AboutBox hakkinda = new AboutBox();
            hakkinda.TopMost = true;
            hakkinda.Show();
        }

        private void OnChangeState(object sender, DeviceStateChangeEventArgs e)
        {
            if (e.CurrentState == DeviceState.Opened) StatusBar.Panels[1].Text = "Device Current State : Opened";
            else if (e.CurrentState == DeviceState.Closed) StatusBar.Panels[1].Text = "Device Current State : Closed";
            else if (e.CurrentState == DeviceState.Waiting) this.Text = "Cihaz Bekleniyor...";
            else if (e.CurrentState == DeviceState.Ready) this.Text = "Cihaz Hazýr...";
        }

        private bool v98GoldExceptionMsg()
        {
            if (USBDevice.IsWindows98Gold)
            {
                MessageBox.Show("Windows 98 iþletim sisteminde Kesme OUT transferleri desteklenmez.Kontrol transferi kullanýn.",
                "UYARI", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return true;
            }
            else
                return false;
        }

        private void OnDeviceAttached(object sender, EventArgs arg)
        {
            StatusBar.Panels[0].Text = "Attached Device";
            if (!USBDevice.IsOpen) USBDevice.OpenPipe(vid, pid);
        }

        private void OnDeviceDetached(object sender, EventArgs arg)
        {
            StatusBar.Panels[0].Text = "Detached Device";
            if (USBDevice.IsOpen) USBDevice.ClosePipe();
        }

        protected override void WndProc(ref Message m)
        {
            USBDevice.ProcessDeviceMessage(m);
            base.WndProc(ref m);
        }
    }
}