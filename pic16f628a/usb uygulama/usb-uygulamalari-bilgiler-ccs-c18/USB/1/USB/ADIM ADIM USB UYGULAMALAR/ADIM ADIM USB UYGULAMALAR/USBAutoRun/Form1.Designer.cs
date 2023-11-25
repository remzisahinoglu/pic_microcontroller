namespace UsbToolManager
{
    partial class frmMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.SystemTray = new System.Windows.Forms.NotifyIcon(this.components);
            this.cms = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.tmiTools = new System.Windows.Forms.ToolStripMenuItem();
            this.tmiRemoveDevice = new System.Windows.Forms.ToolStripMenuItem();
            this.tmiSaveDevice = new System.Windows.Forms.ToolStripMenuItem();
            this.tmiAutoStart = new System.Windows.Forms.ToolStripMenuItem();
            this.tmiCreateAutorun = new System.Windows.Forms.ToolStripMenuItem();
            this.sep = new System.Windows.Forms.ToolStripSeparator();
            this.tmiAbout = new System.Windows.Forms.ToolStripMenuItem();
            this.tmiExit = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.cms.SuspendLayout();
            this.SuspendLayout();
            // 
            // SystemTray
            // 
            this.SystemTray.ContextMenuStrip = this.cms;
            this.SystemTray.Text = "USB Tools Manager";
            this.SystemTray.Visible = true;
            // 
            // cms
            // 
            this.cms.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tmiTools,
            this.tmiRemoveDevice,
            this.tmiSaveDevice,
            this.tmiAutoStart,
            this.tmiCreateAutorun,
            this.sep,
            this.tmiAbout,
            this.tmiExit});
            this.cms.Name = "cms";
            this.cms.Size = new System.Drawing.Size(200, 164);
            // 
            // tmiTools
            // 
            this.tmiTools.Image = global::UsbToolManager.Properties.Resources.wrench_orange;
            this.tmiTools.Name = "tmiTools";
            this.tmiTools.Size = new System.Drawing.Size(199, 22);
            this.tmiTools.Text = "Araç Kutusu";
            this.tmiTools.DropDownItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.tmiTools_DropDownItemClicked);
            // 
            // tmiRemoveDevice
            // 
            this.tmiRemoveDevice.Enabled = false;
            this.tmiRemoveDevice.Image = global::UsbToolManager.Properties.Resources.drive_delete;
            this.tmiRemoveDevice.Name = "tmiRemoveDevice";
            this.tmiRemoveDevice.Size = new System.Drawing.Size(199, 22);
            this.tmiRemoveDevice.Text = "Bu cihazý kaldýr";
            this.tmiRemoveDevice.Click += new System.EventHandler(this.tmiRemoveDevice_Click);
            // 
            // tmiSaveDevice
            // 
            this.tmiSaveDevice.Enabled = false;
            this.tmiSaveDevice.Image = global::UsbToolManager.Properties.Resources.drive_add;
            this.tmiSaveDevice.Name = "tmiSaveDevice";
            this.tmiSaveDevice.Size = new System.Drawing.Size(199, 22);
            this.tmiSaveDevice.Text = "Bu cihazý kaydet";
            this.tmiSaveDevice.Click += new System.EventHandler(this.tmiSaveDevice_Click);
            // 
            // tmiAutoStart
            // 
            this.tmiAutoStart.Image = global::UsbToolManager.Properties.Resources.cog_add;
            this.tmiAutoStart.Name = "tmiAutoStart";
            this.tmiAutoStart.Size = new System.Drawing.Size(199, 22);
            this.tmiAutoStart.Text = "Otomatik baþlangýç";
            this.tmiAutoStart.Click += new System.EventHandler(this.tmiAutoStart_Click);
            // 
            // tmiCreateAutorun
            // 
            this.tmiCreateAutorun.Image = global::UsbToolManager.Properties.Resources.page_gear;
            this.tmiCreateAutorun.Name = "tmiCreateAutorun";
            this.tmiCreateAutorun.Size = new System.Drawing.Size(199, 22);
            this.tmiCreateAutorun.Text = "Autorun dosyasý oluþtur";
            this.tmiCreateAutorun.Click += new System.EventHandler(this.tmiCreateAutorun_Click);
            // 
            // sep
            // 
            this.sep.BackColor = System.Drawing.SystemColors.Control;
            this.sep.ForeColor = System.Drawing.SystemColors.ActiveBorder;
            this.sep.Name = "sep";
            this.sep.Size = new System.Drawing.Size(196, 6);
            // 
            // tmiAbout
            // 
            this.tmiAbout.Image = global::UsbToolManager.Properties.Resources.information;
            this.tmiAbout.Name = "tmiAbout";
            this.tmiAbout.Size = new System.Drawing.Size(199, 22);
            this.tmiAbout.Text = "Hakkýnda";
            this.tmiAbout.Click += new System.EventHandler(this.tmiAbout_Click);
            // 
            // tmiExit
            // 
            this.tmiExit.Image = global::UsbToolManager.Properties.Resources.door_in;
            this.tmiExit.Name = "tmiExit";
            this.tmiExit.Size = new System.Drawing.Size(199, 22);
            this.tmiExit.Text = "Çýkýþ";
            this.tmiExit.Click += new System.EventHandler(this.tmiExit_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(152, 22);
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(115, 26);
            this.ControlBox = false;
            this.Font = new System.Drawing.Font("TR Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "frmMain";
            this.Opacity = 0;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "USBAutoRun";
            this.cms.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.NotifyIcon SystemTray;
        private System.Windows.Forms.ContextMenuStrip cms;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem tmiAbout;
        private System.Windows.Forms.ToolStripMenuItem tmiExit;
        private System.Windows.Forms.ToolStripMenuItem tmiRemoveDevice;
        private System.Windows.Forms.ToolStripMenuItem tmiSaveDevice;
        private System.Windows.Forms.ToolStripSeparator sep;
        private System.Windows.Forms.ToolStripMenuItem tmiAutoStart;
        private System.Windows.Forms.ToolStripMenuItem tmiCreateAutorun;
        private System.Windows.Forms.ToolStripMenuItem tmiTools;
    }
}

