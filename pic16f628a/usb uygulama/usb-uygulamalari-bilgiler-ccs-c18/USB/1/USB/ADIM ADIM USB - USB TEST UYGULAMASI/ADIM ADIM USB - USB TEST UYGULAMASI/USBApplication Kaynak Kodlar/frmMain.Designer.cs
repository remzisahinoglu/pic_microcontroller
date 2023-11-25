namespace USBApplication
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmMain));
            this.mnuMain = new System.Windows.Forms.MenuStrip();
            this.mnuAraclar = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuDebug = new System.Windows.Forms.ToolStripMenuItem();
            this.tsHakkinda = new System.Windows.Forms.ToolStripMenuItem();
            this.lblVendorId = new System.Windows.Forms.Label();
            this.lblProductId = new System.Windows.Forms.Label();
            this.txtVendorId = new System.Windows.Forms.TextBox();
            this.txtProductId = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.lbxRecData = new System.Windows.Forms.ListBox();
            this.gbTrfType = new System.Windows.Forms.GroupBox();
            this.rbInterruptTrf = new System.Windows.Forms.RadioButton();
            this.rbControlTrf = new System.Windows.Forms.RadioButton();
            this.btnRead = new System.Windows.Forms.Button();
            this.txtSendData = new System.Windows.Forms.TextBox();
            this.btnSend = new System.Windows.Forms.Button();
            this.USBList = new System.Windows.Forms.ListView();
            this.clmUsbName = new System.Windows.Forms.ColumnHeader();
            this.clmVendorId = new System.Windows.Forms.ColumnHeader();
            this.clmProductID = new System.Windows.Forms.ColumnHeader();
            this.clmVendorString = new System.Windows.Forms.ColumnHeader();
            this.cbRegister = new System.Windows.Forms.CheckBox();
            this.btnClose = new System.Windows.Forms.Button();
            this.btnClear = new System.Windows.Forms.Button();
            this.btnCheckUsb = new System.Windows.Forms.Button();
            this.StatusBar = new System.Windows.Forms.StatusBar();
            this.StatusText = new System.Windows.Forms.StatusBarPanel();
            this.pnDeviceState = new System.Windows.Forms.StatusBarPanel();
            this.pnDeviceType = new System.Windows.Forms.StatusBarPanel();
            this.mnuMain.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.gbTrfType.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.StatusText)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pnDeviceState)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pnDeviceType)).BeginInit();
            this.SuspendLayout();
            // 
            // mnuMain
            // 
            this.mnuMain.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.mnuMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuAraclar,
            this.tsHakkinda});
            this.mnuMain.Location = new System.Drawing.Point(0, 0);
            this.mnuMain.Name = "mnuMain";
            this.mnuMain.Size = new System.Drawing.Size(554, 24);
            this.mnuMain.TabIndex = 2;
            this.mnuMain.Text = "menuStrip1";
            // 
            // mnuAraclar
            // 
            this.mnuAraclar.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuDebug});
            this.mnuAraclar.Font = new System.Drawing.Font("TR Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.mnuAraclar.Image = ((System.Drawing.Image)(resources.GetObject("mnuAraclar.Image")));
            this.mnuAraclar.Name = "mnuAraclar";
            this.mnuAraclar.Size = new System.Drawing.Size(70, 20);
            this.mnuAraclar.Text = "&Araçlar";
            // 
            // mnuDebug
            // 
            this.mnuDebug.Font = new System.Drawing.Font("TR Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.mnuDebug.Image = ((System.Drawing.Image)(resources.GetObject("mnuDebug.Image")));
            this.mnuDebug.Name = "mnuDebug";
            this.mnuDebug.Size = new System.Drawing.Size(188, 22);
            this.mnuDebug.Text = "Device Debug Window";
            // 
            // tsHakkinda
            // 
            this.tsHakkinda.Font = new System.Drawing.Font("TR Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.tsHakkinda.Image = ((System.Drawing.Image)(resources.GetObject("tsHakkinda.Image")));
            this.tsHakkinda.Name = "tsHakkinda";
            this.tsHakkinda.Size = new System.Drawing.Size(79, 20);
            this.tsHakkinda.Text = "Hakkýnda";
            this.tsHakkinda.Click += new System.EventHandler(this.tsHakkinda_Click);
            // 
            // lblVendorId
            // 
            this.lblVendorId.AutoSize = true;
            this.lblVendorId.Location = new System.Drawing.Point(6, 27);
            this.lblVendorId.Name = "lblVendorId";
            this.lblVendorId.Size = new System.Drawing.Size(63, 13);
            this.lblVendorId.TabIndex = 5;
            this.lblVendorId.Text = "Vendor ID :";
            // 
            // lblProductId
            // 
            this.lblProductId.AutoSize = true;
            this.lblProductId.Location = new System.Drawing.Point(6, 51);
            this.lblProductId.Name = "lblProductId";
            this.lblProductId.Size = new System.Drawing.Size(65, 13);
            this.lblProductId.TabIndex = 6;
            this.lblProductId.Text = "Product ID :";
            // 
            // txtVendorId
            // 
            this.txtVendorId.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.txtVendorId.Location = new System.Drawing.Point(74, 19);
            this.txtVendorId.Name = "txtVendorId";
            this.txtVendorId.Size = new System.Drawing.Size(52, 21);
            this.txtVendorId.TabIndex = 7;
            this.txtVendorId.Text = "1986";
            // 
            // txtProductId
            // 
            this.txtProductId.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.txtProductId.Location = new System.Drawing.Point(74, 42);
            this.txtProductId.Name = "txtProductId";
            this.txtProductId.Size = new System.Drawing.Size(52, 21);
            this.txtProductId.TabIndex = 8;
            this.txtProductId.Text = "3543";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.lbxRecData);
            this.groupBox1.Controls.Add(this.gbTrfType);
            this.groupBox1.Controls.Add(this.btnRead);
            this.groupBox1.Controls.Add(this.txtSendData);
            this.groupBox1.Controls.Add(this.btnSend);
            this.groupBox1.Controls.Add(this.lblVendorId);
            this.groupBox1.Controls.Add(this.txtVendorId);
            this.groupBox1.Controls.Add(this.lblProductId);
            this.groupBox1.Controls.Add(this.txtProductId);
            this.groupBox1.Location = new System.Drawing.Point(9, 200);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(409, 145);
            this.groupBox1.TabIndex = 14;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Veri Alým-Gönderim";
            // 
            // lbxRecData
            // 
            this.lbxRecData.FormattingEnabled = true;
            this.lbxRecData.Location = new System.Drawing.Point(132, 19);
            this.lbxRecData.Name = "lbxRecData";
            this.lbxRecData.Size = new System.Drawing.Size(130, 69);
            this.lbxRecData.TabIndex = 20;
            // 
            // gbTrfType
            // 
            this.gbTrfType.Controls.Add(this.rbInterruptTrf);
            this.gbTrfType.Controls.Add(this.rbControlTrf);
            this.gbTrfType.Location = new System.Drawing.Point(9, 70);
            this.gbTrfType.Name = "gbTrfType";
            this.gbTrfType.Size = new System.Drawing.Size(117, 64);
            this.gbTrfType.TabIndex = 19;
            this.gbTrfType.TabStop = false;
            this.gbTrfType.Text = "Transfer Tipi";
            // 
            // rbInterruptTrf
            // 
            this.rbInterruptTrf.AutoSize = true;
            this.rbInterruptTrf.Checked = true;
            this.rbInterruptTrf.Location = new System.Drawing.Point(7, 39);
            this.rbInterruptTrf.Name = "rbInterruptTrf";
            this.rbInterruptTrf.Size = new System.Drawing.Size(101, 17);
            this.rbInterruptTrf.TabIndex = 1;
            this.rbInterruptTrf.TabStop = true;
            this.rbInterruptTrf.Text = "Kesme Transfer";
            this.rbInterruptTrf.UseVisualStyleBackColor = true;
            // 
            // rbControlTrf
            // 
            this.rbControlTrf.AutoSize = true;
            this.rbControlTrf.Location = new System.Drawing.Point(7, 20);
            this.rbControlTrf.Name = "rbControlTrf";
            this.rbControlTrf.Size = new System.Drawing.Size(104, 17);
            this.rbControlTrf.TabIndex = 0;
            this.rbControlTrf.Text = "Kontrol Transfer";
            this.rbControlTrf.UseVisualStyleBackColor = true;
            // 
            // btnRead
            // 
            this.btnRead.Image = ((System.Drawing.Image)(resources.GetObject("btnRead.Image")));
            this.btnRead.Location = new System.Drawing.Point(132, 100);
            this.btnRead.Name = "btnRead";
            this.btnRead.Size = new System.Drawing.Size(129, 34);
            this.btnRead.TabIndex = 18;
            this.btnRead.Text = "Oku";
            this.btnRead.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.btnRead.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnRead.UseVisualStyleBackColor = true;
            this.btnRead.Click += new System.EventHandler(this.btnRead_Click);
            // 
            // txtSendData
            // 
            this.txtSendData.AcceptsReturn = true;
            this.txtSendData.AcceptsTab = true;
            this.txtSendData.Location = new System.Drawing.Point(267, 19);
            this.txtSendData.Multiline = true;
            this.txtSendData.Name = "txtSendData";
            this.txtSendData.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtSendData.Size = new System.Drawing.Size(130, 69);
            this.txtSendData.TabIndex = 17;
            this.txtSendData.WordWrap = false;
            // 
            // btnSend
            // 
            this.btnSend.Image = ((System.Drawing.Image)(resources.GetObject("btnSend.Image")));
            this.btnSend.Location = new System.Drawing.Point(267, 100);
            this.btnSend.Name = "btnSend";
            this.btnSend.Size = new System.Drawing.Size(130, 34);
            this.btnSend.TabIndex = 16;
            this.btnSend.Text = "Gönder";
            this.btnSend.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.btnSend.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnSend.UseVisualStyleBackColor = true;
            this.btnSend.Click += new System.EventHandler(this.btnSend_Click);
            // 
            // USBList
            // 
            this.USBList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.clmUsbName,
            this.clmVendorId,
            this.clmProductID,
            this.clmVendorString});
            this.USBList.Dock = System.Windows.Forms.DockStyle.Top;
            this.USBList.Location = new System.Drawing.Point(0, 24);
            this.USBList.Name = "USBList";
            this.USBList.Size = new System.Drawing.Size(554, 170);
            this.USBList.TabIndex = 17;
            this.USBList.UseCompatibleStateImageBehavior = false;
            this.USBList.View = System.Windows.Forms.View.Details;
            // 
            // clmUsbName
            // 
            this.clmUsbName.Text = "Device Name";
            this.clmUsbName.Width = 200;
            // 
            // clmVendorId
            // 
            this.clmVendorId.Text = "Vendor ID";
            this.clmVendorId.Width = 100;
            // 
            // clmProductID
            // 
            this.clmProductID.Text = "Product ID";
            this.clmProductID.Width = 100;
            // 
            // clmVendorString
            // 
            this.clmVendorString.Text = "Üretici";
            this.clmVendorString.Width = 200;
            // 
            // cbRegister
            // 
            this.cbRegister.AutoSize = true;
            this.cbRegister.Location = new System.Drawing.Point(424, 328);
            this.cbRegister.Name = "cbRegister";
            this.cbRegister.Size = new System.Drawing.Size(104, 17);
            this.cbRegister.TabIndex = 19;
            this.cbRegister.Text = "Bu cihazý kaydet";
            this.cbRegister.UseVisualStyleBackColor = true;
            this.cbRegister.Click += new System.EventHandler(this.cbRegister_Click);
            // 
            // btnClose
            // 
            this.btnClose.Image = global::USBApplication.Properties.Resources.disconnect;
            this.btnClose.Location = new System.Drawing.Point(424, 246);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(122, 34);
            this.btnClose.TabIndex = 18;
            this.btnClose.Text = "Baðlantý Kapat";
            this.btnClose.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.btnClose.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // btnClear
            // 
            this.btnClear.Image = global::USBApplication.Properties.Resources.bin;
            this.btnClear.Location = new System.Drawing.Point(424, 208);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(122, 32);
            this.btnClear.TabIndex = 3;
            this.btnClear.Text = "Temizle";
            this.btnClear.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.btnClear.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnClear.UseVisualStyleBackColor = true;
            this.btnClear.Click += new System.EventHandler(this.btnClear_Click);
            // 
            // btnCheckUsb
            // 
            this.btnCheckUsb.Image = global::USBApplication.Properties.Resources.zoom;
            this.btnCheckUsb.Location = new System.Drawing.Point(424, 286);
            this.btnCheckUsb.Name = "btnCheckUsb";
            this.btnCheckUsb.Size = new System.Drawing.Size(122, 34);
            this.btnCheckUsb.TabIndex = 0;
            this.btnCheckUsb.Text = "USB Bul";
            this.btnCheckUsb.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.btnCheckUsb.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnCheckUsb.UseVisualStyleBackColor = true;
            this.btnCheckUsb.Click += new System.EventHandler(this.btnCheckUsb_Click);
            // 
            // StatusBar
            // 
            this.StatusBar.Location = new System.Drawing.Point(0, 355);
            this.StatusBar.Name = "StatusBar";
            this.StatusBar.Panels.AddRange(new System.Windows.Forms.StatusBarPanel[] {
            this.StatusText,
            this.pnDeviceState,
            this.pnDeviceType});
            this.StatusBar.ShowPanels = true;
            this.StatusBar.Size = new System.Drawing.Size(554, 22);
            this.StatusBar.TabIndex = 20;
            // 
            // StatusText
            // 
            this.StatusText.Icon = ((System.Drawing.Icon)(resources.GetObject("StatusText.Icon")));
            this.StatusText.Name = "StatusText";
            this.StatusText.Width = 200;
            // 
            // pnDeviceState
            // 
            this.pnDeviceState.Icon = ((System.Drawing.Icon)(resources.GetObject("pnDeviceState.Icon")));
            this.pnDeviceState.Name = "pnDeviceState";
            this.pnDeviceState.Width = 190;
            // 
            // pnDeviceType
            // 
            this.pnDeviceType.Icon = ((System.Drawing.Icon)(resources.GetObject("pnDeviceType.Icon")));
            this.pnDeviceType.Name = "pnDeviceType";
            this.pnDeviceType.Width = 160;
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(554, 377);
            this.Controls.Add(this.StatusBar);
            this.Controls.Add(this.cbRegister);
            this.Controls.Add(this.btnClose);
            this.Controls.Add(this.USBList);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.btnClear);
            this.Controls.Add(this.btnCheckUsb);
            this.Controls.Add(this.mnuMain);
            this.Font = new System.Drawing.Font("TR Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.mnuMain;
            this.MaximizeBox = false;
            this.Name = "frmMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "USB Yöneticisi";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmMain_FormClosing);
            this.mnuMain.ResumeLayout(false);
            this.mnuMain.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.gbTrfType.ResumeLayout(false);
            this.gbTrfType.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.StatusText)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pnDeviceState)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pnDeviceType)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnCheckUsb;
        private System.Windows.Forms.MenuStrip mnuMain;
        private System.Windows.Forms.Button btnClear;
        private System.Windows.Forms.ToolStripMenuItem mnuAraclar;
        private System.Windows.Forms.Label lblVendorId;
        private System.Windows.Forms.Label lblProductId;
        private System.Windows.Forms.TextBox txtVendorId;
        private System.Windows.Forms.TextBox txtProductId;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ToolStripMenuItem mnuDebug;
        private System.Windows.Forms.Button btnSend;
        private System.Windows.Forms.ListView USBList;
        private System.Windows.Forms.ColumnHeader clmUsbName;
        private System.Windows.Forms.ColumnHeader clmVendorId;
        private System.Windows.Forms.ColumnHeader clmProductID;
        private System.Windows.Forms.ColumnHeader clmVendorString;
        private System.Windows.Forms.TextBox txtSendData;
        private System.Windows.Forms.Button btnRead;
        private System.Windows.Forms.GroupBox gbTrfType;
        private System.Windows.Forms.RadioButton rbInterruptTrf;
        private System.Windows.Forms.RadioButton rbControlTrf;
        private System.Windows.Forms.ListBox lbxRecData;
        private System.Windows.Forms.Button btnClose;
        private System.Windows.Forms.CheckBox cbRegister;
        private System.Windows.Forms.ToolStripMenuItem tsHakkinda;
        private System.Windows.Forms.StatusBar StatusBar;
        private System.Windows.Forms.StatusBarPanel StatusText;
        private System.Windows.Forms.StatusBarPanel pnDeviceState;
        private System.Windows.Forms.StatusBarPanel pnDeviceType;
    }
}

