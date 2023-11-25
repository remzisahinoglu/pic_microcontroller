using System;
using System.IO;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;
using System.Diagnostics;
using Microsoft.Win32;
using System.Drawing;
using System.Threading;
using System.Media;

namespace UsbToolManager
{
    public partial class frmMain : Form
    {
        #region Variable

        bool mHide;
        Volume diskInfo;
        UsbDiskDevice UsbDisk;

        #endregion

        #region Constructor

        public frmMain()
        {
            InitializeComponent();
            Startup();
        }

        #endregion

        #region Message Loop

        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);
            UsbDisk.ProcessDeviceMessage(m);
        }

        #endregion

        #region Private Methods

        /// <summary>
        /// 
        /// </summary>
        private void Startup()
        {
            try
            {
                //UsbDiskDevice'ý hazýrla
                UsbDisk = new UsbDiskDevice();

                UsbDisk.DiskDeviceArrived += new DiskDeviceArrivedEventHandler(OnDeviceArrived);
                UsbDisk.DiskDeviceRemoved += new DiskDeviceRemovedEventHandler(OnDeviceRemoved);

                //Sytem tray'de balon mesaj göster
                SystemTray.Icon = System.Drawing.Icon.FromHandle(Properties.Resources.USB_Orange.GetHicon());
                SystemTray.BalloonTipTitle = "USBAutoRun Manager";
                SystemTray.BalloonTipText = "USBAutoRun aktif durumda...";
                SystemTray.BalloonTipIcon = ToolTipIcon.Info;
                SystemTray.ShowBalloonTip(3000);

                //Ayarlarý oku ve gerekli enable-disable ayarlarýný yap
                tmiAutoStart.Checked = Properties.Settings.Default.AutoStartEnable;
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// </summary>
        /// <param name="sender">Mesajý gönderen</param>
        /// <param name="e">Disk bilgilerini tutan olay nesnesi</param>
        private void OnDeviceArrived(object sender, DiskDeviceArrivedEventArgs e)
        {
            SystemTray.Icon = System.Drawing.Icon.FromHandle(Properties.Resources.bell_add.GetHicon());
            PlayNotifySound();

            CheckFormHideProperty();
            //DiskDeviceArrivedEventArgs tarafýndan verilen disk bilgilerini al
            diskInfo = e.DiskInfo;

            //Daha önceden kayýtlý bir disk varsa iþlemleri bu disk'e göre yap
            if (Properties.Settings.Default.OnlyRegisteredUsbEnable)
            {
                if (Properties.Settings.Default.UsbDiskSerialNumber.CompareTo(diskInfo.SerialNumber) == 0)
                {
                    CheckInfFile();
                    tmiRemoveDevice.Enabled = true;
                }
                else
                {
                    return;
                }
            }
            else
            {
                CheckInfFile();
                tmiSaveDevice.Enabled = true;
            }
            AddProgramToMenu();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OnDeviceRemoved(object sender, DiskDeviceRemovedEventArgs e)
        {
            if (tmiTools.DropDownItems.Count > 0) tmiTools.DropDownItems.Clear();

            tmiSaveDevice.Enabled = false;
            tmiRemoveDevice.Enabled = false;
            diskInfo.Drive = string.Empty;

            PlayNotifySound();
            SystemTray.Icon = System.Drawing.Icon.FromHandle(Properties.Resources.bell_delete.GetHicon());
            Thread.Sleep(2000);
            SystemTray.Icon = System.Drawing.Icon.FromHandle(Properties.Resources.USB_Orange.GetHicon());
        }

        /// <summary>
        /// 
        /// </summary>
        private void CheckInfFile()
        {
            try
            {
                string inf = diskInfo.Drive + "Autorun.inf";
                if (File.Exists(inf))
                {
                    //Eðer inf dosyasý var ise open section'ý oku ve belirtilen exe'yi
                    //çalýþtýr.
                    StringBuilder sb = new StringBuilder(255);
                    Win32.GetPrivateProfileString("autorun", "open", "", sb, 255, inf);
                    string proc = diskInfo.Drive + sb.ToString();
                    if(File.Exists(proc)) Process.Start(proc);
                }
                else
                {
                    string tool = diskInfo.Drive + Path.DirectorySeparatorChar + "USBTools.exe";
                    if (File.Exists(tool)) Process.Start(tool);
                }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "RUN ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private void CheckFormHideProperty()
        {
            //Form baþlangýçta görünür durumdadýr.Fakat þeffaflýk ayarý 0
            //olduðundan görünmez.Ýlk aygýt algýlandýktan sonra form gerçekten
            //gizlenir.
            try
            {
                if (!mHide)
                {
                    Hide();
                    mHide = true;
                    Opacity = 100;
                }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="enable"></param>
        private void EnableOnlyRegisteredDevice(bool enable)
        {
            try
            {
                Properties.Settings.Default.OnlyRegisteredUsbEnable = enable;
                Properties.Settings.Default.UsbDiskSerialNumber = enable ? diskInfo.SerialNumber : "00000";
                Properties.Settings.Default.Save();
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private void AddProgramToMenu()
        {
            int mMemberIndex = 0;
            string[] exe = Directory.GetFiles(diskInfo.Drive, "*.exe");

            foreach (string prog in exe)
            {
                IntPtr hIcon = Win32.ExtractIcon(Handle, exe[mMemberIndex], 0);
                if (hIcon != IntPtr.Zero)
                {
                    Image exeIcon = Bitmap.FromHicon(hIcon);
                    ToolStripItem tsi = tmiTools.DropDownItems.Add(Path.GetFileNameWithoutExtension(exe[mMemberIndex]), exeIcon);
                    tsi.Tag = exe[mMemberIndex];
                }
                else
                {
                    ToolStripItem tsi = tmiTools.DropDownItems.Add(Path.GetFileNameWithoutExtension(exe[mMemberIndex]));
                    tsi.Tag = exe[mMemberIndex];
                }
                mMemberIndex++;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private void PlayNotifySound()
        {
            UnmanagedMemoryStream ums = Properties.Resources.Notify;
            SoundPlayer player = new SoundPlayer(ums);

            player.Load();
            player.Play();

            ums.Close();
        }

        #endregion

        #region Form Component Codes

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void tmiExit_Click(object sender, EventArgs e)
        {
            Close();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void tmiAbout_Click(object sender, EventArgs e)
        {
            About frmAbout = new About();
            frmAbout.TopMost = true;
            frmAbout.Show();
        }

        private void tmiSaveDevice_Click(object sender, EventArgs e)
        {
            EnableOnlyRegisteredDevice(true);

            tmiSaveDevice.Enabled = false;
            tmiRemoveDevice.Enabled = true;
        }

        private void tmiRemoveDevice_Click(object sender, EventArgs e)
        {
            EnableOnlyRegisteredDevice(false);

            tmiSaveDevice.Enabled = true;
            tmiRemoveDevice.Enabled = false;
        }

        private void tmiAutoStart_Click(object sender, EventArgs e)
        {
            try
            {
                tmiAutoStart.Checked = !tmiAutoStart.Checked;
                Properties.Settings.Default.AutoStartEnable = tmiAutoStart.Checked;
                Properties.Settings.Default.Save();

                RegistryKey Run = Registry.CurrentUser.
                                           OpenSubKey("Software").
                                           OpenSubKey("Microsoft").
                                           OpenSubKey("Windows").
                                           OpenSubKey("CurrentVersion").
                                           OpenSubKey("Run", true);

                if (tmiAutoStart.Checked)
                    Run.SetValue("UsbToolsManager", "\"" + Application.ExecutablePath + "\"");
                else
                    Run.SetValue("UsbToolsManager", "");

                Run.Close();
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void tmiCreateAutorun_Click(object sender, EventArgs e)
        {
            string drive = diskInfo == null ? string.Empty : diskInfo.Drive;

            AutoRunCreater autocrt = new AutoRunCreater(drive);
            autocrt.TopMost = true; autocrt.Show();
        }

        private void tmiTools_DropDownItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            string exe = (string)e.ClickedItem.Tag;
            Process.Start(exe);
        }

        #endregion
    }
}