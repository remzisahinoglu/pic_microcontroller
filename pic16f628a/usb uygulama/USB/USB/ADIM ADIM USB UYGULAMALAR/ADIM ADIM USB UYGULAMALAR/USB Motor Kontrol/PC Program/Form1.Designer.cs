namespace USBMotorKontrol
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
            this.btnStart = new System.Windows.Forms.Button();
            this.btnStop = new System.Windows.Forms.Button();
            this.btnFast = new System.Windows.Forms.Button();
            this.btnSlow = new System.Windows.Forms.Button();
            this.lbStatus = new System.Windows.Forms.Label();
            this.lbDeviceStatus = new System.Windows.Forms.Label();
            this.lblCCP = new System.Windows.Forms.Label();
            this.cbRegister = new System.Windows.Forms.CheckBox();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.tslStatusText = new System.Windows.Forms.ToolStripStatusLabel();
            this.statusStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnStart
            // 
            this.btnStart.Image = ((System.Drawing.Image)(resources.GetObject("btnStart.Image")));
            this.btnStart.Location = new System.Drawing.Point(12, 12);
            this.btnStart.Name = "btnStart";
            this.btnStart.Size = new System.Drawing.Size(89, 33);
            this.btnStart.TabIndex = 0;
            this.btnStart.Text = "Baþlat";
            this.btnStart.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.btnStart.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnStart.UseVisualStyleBackColor = true;
            this.btnStart.Click += new System.EventHandler(this.btnStart_Click);
            // 
            // btnStop
            // 
            this.btnStop.Image = ((System.Drawing.Image)(resources.GetObject("btnStop.Image")));
            this.btnStop.Location = new System.Drawing.Point(12, 48);
            this.btnStop.Name = "btnStop";
            this.btnStop.Size = new System.Drawing.Size(89, 33);
            this.btnStop.TabIndex = 1;
            this.btnStop.Text = "Durdur";
            this.btnStop.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.btnStop.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnStop.UseVisualStyleBackColor = true;
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
            // 
            // btnFast
            // 
            this.btnFast.Image = ((System.Drawing.Image)(resources.GetObject("btnFast.Image")));
            this.btnFast.Location = new System.Drawing.Point(107, 48);
            this.btnFast.Name = "btnFast";
            this.btnFast.Size = new System.Drawing.Size(89, 33);
            this.btnFast.TabIndex = 2;
            this.btnFast.Text = "Hýzlandýr";
            this.btnFast.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.btnFast.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnFast.UseVisualStyleBackColor = true;
            this.btnFast.Click += new System.EventHandler(this.btnFast_Click);
            // 
            // btnSlow
            // 
            this.btnSlow.Image = ((System.Drawing.Image)(resources.GetObject("btnSlow.Image")));
            this.btnSlow.Location = new System.Drawing.Point(107, 12);
            this.btnSlow.Name = "btnSlow";
            this.btnSlow.Size = new System.Drawing.Size(89, 33);
            this.btnSlow.TabIndex = 3;
            this.btnSlow.Text = "Yavaþlat";
            this.btnSlow.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.btnSlow.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnSlow.UseVisualStyleBackColor = true;
            this.btnSlow.Click += new System.EventHandler(this.btnSlow_Click);
            // 
            // lbStatus
            // 
            this.lbStatus.AutoSize = true;
            this.lbStatus.Font = new System.Drawing.Font("TR Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.lbStatus.Location = new System.Drawing.Point(261, 12);
            this.lbStatus.Name = "lbStatus";
            this.lbStatus.Size = new System.Drawing.Size(30, 13);
            this.lbStatus.TabIndex = 4;
            this.lbStatus.Text = "0x00";
            // 
            // lbDeviceStatus
            // 
            this.lbDeviceStatus.AutoSize = true;
            this.lbDeviceStatus.Location = new System.Drawing.Point(203, 32);
            this.lbDeviceStatus.Name = "lbDeviceStatus";
            this.lbDeviceStatus.Size = new System.Drawing.Size(111, 13);
            this.lbDeviceStatus.TabIndex = 5;
            this.lbDeviceStatus.Text = "Cihaz ile baðlantý yok";
            // 
            // lblCCP
            // 
            this.lblCCP.AutoSize = true;
            this.lblCCP.Location = new System.Drawing.Point(202, 12);
            this.lblCCP.Name = "lblCCP";
            this.lblCCP.Size = new System.Drawing.Size(62, 13);
            this.lblCCP.TabIndex = 6;
            this.lblCCP.Text = "Pwm Duty :";
            // 
            // cbRegister
            // 
            this.cbRegister.AutoSize = true;
            this.cbRegister.Location = new System.Drawing.Point(205, 62);
            this.cbRegister.Name = "cbRegister";
            this.cbRegister.Size = new System.Drawing.Size(104, 17);
            this.cbRegister.TabIndex = 7;
            this.cbRegister.Text = "Bu cihazý kaydet";
            this.cbRegister.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.cbRegister.UseVisualStyleBackColor = true;
            this.cbRegister.Click += new System.EventHandler(this.cbRegister_Click);
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tslStatusText});
            this.statusStrip.Location = new System.Drawing.Point(0, 90);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(347, 22);
            this.statusStrip.TabIndex = 8;
            this.statusStrip.Text = "statusStrip1";
            // 
            // tslStatusText
            // 
            this.tslStatusText.Name = "tslStatusText";
            this.tslStatusText.Size = new System.Drawing.Size(0, 17);
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(347, 112);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.cbRegister);
            this.Controls.Add(this.lblCCP);
            this.Controls.Add(this.lbDeviceStatus);
            this.Controls.Add(this.lbStatus);
            this.Controls.Add(this.btnSlow);
            this.Controls.Add(this.btnFast);
            this.Controls.Add(this.btnStop);
            this.Controls.Add(this.btnStart);
            this.Font = new System.Drawing.Font("TR Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "frmMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Adým Adým USB - Motor Kontrol";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmMain_FormClosing);
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnStart;
        private System.Windows.Forms.Button btnStop;
        private System.Windows.Forms.Button btnFast;
        private System.Windows.Forms.Button btnSlow;
        private System.Windows.Forms.Label lbStatus;
        private System.Windows.Forms.Label lbDeviceStatus;
        private System.Windows.Forms.Label lblCCP;
        private System.Windows.Forms.CheckBox cbRegister;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.ToolStripStatusLabel tslStatusText;
    }
}

