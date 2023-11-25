namespace UsbToolManager
{
    partial class AutoRunCreater
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
            this.lbIco = new System.Windows.Forms.Label();
            this.lbExe = new System.Windows.Forms.Label();
            this.txtIco = new System.Windows.Forms.TextBox();
            this.txtExe = new System.Windows.Forms.TextBox();
            this.txtDir = new System.Windows.Forms.TextBox();
            this.lbHedef = new System.Windows.Forms.Label();
            this.btnFindIco = new System.Windows.Forms.Button();
            this.btnFindExe = new System.Windows.Forms.Button();
            this.btnFindDir = new System.Windows.Forms.Button();
            this.btnKaydet = new System.Windows.Forms.Button();
            this.cbFullPck = new System.Windows.Forms.CheckBox();
            this.toolTip = new System.Windows.Forms.ToolTip(this.components);
            this.fbd = new System.Windows.Forms.FolderBrowserDialog();
            this.Ofd = new System.Windows.Forms.OpenFileDialog();
            this.SuspendLayout();
            // 
            // lbIco
            // 
            this.lbIco.AutoSize = true;
            this.lbIco.Location = new System.Drawing.Point(50, 22);
            this.lbIco.Name = "lbIco";
            this.lbIco.Size = new System.Drawing.Size(74, 13);
            this.lbIco.TabIndex = 0;
            this.lbIco.Text = "Icon dosyasý :";
            // 
            // lbExe
            // 
            this.lbExe.AutoSize = true;
            this.lbExe.Location = new System.Drawing.Point(12, 47);
            this.lbExe.Name = "lbExe";
            this.lbExe.Size = new System.Drawing.Size(112, 13);
            this.lbExe.TabIndex = 1;
            this.lbExe.Text = "Çalýþtýrýlabilir Dosya :";
            // 
            // txtIco
            // 
            this.txtIco.Location = new System.Drawing.Point(130, 14);
            this.txtIco.Name = "txtIco";
            this.txtIco.Size = new System.Drawing.Size(228, 21);
            this.txtIco.TabIndex = 2;
            // 
            // txtExe
            // 
            this.txtExe.Location = new System.Drawing.Point(130, 39);
            this.txtExe.Name = "txtExe";
            this.txtExe.Size = new System.Drawing.Size(228, 21);
            this.txtExe.TabIndex = 3;
            // 
            // txtDir
            // 
            this.txtDir.Location = new System.Drawing.Point(130, 80);
            this.txtDir.Name = "txtDir";
            this.txtDir.Size = new System.Drawing.Size(228, 21);
            this.txtDir.TabIndex = 4;
            // 
            // lbHedef
            // 
            this.lbHedef.AutoSize = true;
            this.lbHedef.Location = new System.Drawing.Point(81, 88);
            this.lbHedef.Name = "lbHedef";
            this.lbHedef.Size = new System.Drawing.Size(43, 13);
            this.lbHedef.TabIndex = 5;
            this.lbHedef.Text = "Hedef :";
            // 
            // btnFindIco
            // 
            this.btnFindIco.Location = new System.Drawing.Point(360, 14);
            this.btnFindIco.Name = "btnFindIco";
            this.btnFindIco.Size = new System.Drawing.Size(35, 21);
            this.btnFindIco.TabIndex = 6;
            this.btnFindIco.Text = "...";
            this.btnFindIco.UseVisualStyleBackColor = true;
            this.btnFindIco.Click += new System.EventHandler(this.btnFindIco_Click);
            // 
            // btnFindExe
            // 
            this.btnFindExe.Location = new System.Drawing.Point(360, 39);
            this.btnFindExe.Name = "btnFindExe";
            this.btnFindExe.Size = new System.Drawing.Size(35, 21);
            this.btnFindExe.TabIndex = 7;
            this.btnFindExe.Text = "...";
            this.btnFindExe.UseVisualStyleBackColor = true;
            this.btnFindExe.Click += new System.EventHandler(this.btnFindExe_Click);
            // 
            // btnFindDir
            // 
            this.btnFindDir.Location = new System.Drawing.Point(360, 80);
            this.btnFindDir.Name = "btnFindDir";
            this.btnFindDir.Size = new System.Drawing.Size(35, 21);
            this.btnFindDir.TabIndex = 8;
            this.btnFindDir.Text = "...";
            this.btnFindDir.UseVisualStyleBackColor = true;
            this.btnFindDir.Click += new System.EventHandler(this.btnFindDir_Click);
            // 
            // btnKaydet
            // 
            this.btnKaydet.Location = new System.Drawing.Point(320, 128);
            this.btnKaydet.Name = "btnKaydet";
            this.btnKaydet.Size = new System.Drawing.Size(75, 23);
            this.btnKaydet.TabIndex = 9;
            this.btnKaydet.Text = "&Kaydet";
            this.btnKaydet.UseVisualStyleBackColor = true;
            this.btnKaydet.Click += new System.EventHandler(this.btnKaydet_Click);
            // 
            // cbFullPck
            // 
            this.cbFullPck.AutoSize = true;
            this.cbFullPck.Location = new System.Drawing.Point(130, 107);
            this.cbFullPck.Name = "cbFullPck";
            this.cbFullPck.Size = new System.Drawing.Size(173, 17);
            this.cbFullPck.TabIndex = 10;
            this.cbFullPck.Text = "Hedef dizine full paket oluþtur";
            this.toolTip.SetToolTip(this.cbFullPck, "Bu seçenek iþaretlendiði takdirde, çalýþtýrýlabilir dosya,\r\nicon dosyasý ve Autor" +
                    "un dosyasý hedef dizine taþýnýr.");
            this.cbFullPck.UseVisualStyleBackColor = true;
            // 
            // toolTip
            // 
            this.toolTip.AutoPopDelay = 5000;
            this.toolTip.InitialDelay = 500;
            this.toolTip.IsBalloon = true;
            this.toolTip.ReshowDelay = 100;
            this.toolTip.ShowAlways = true;
            this.toolTip.ToolTipIcon = System.Windows.Forms.ToolTipIcon.Info;
            this.toolTip.ToolTipTitle = "Yardým";
            // 
            // AutoRunCreater
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(406, 163);
            this.Controls.Add(this.cbFullPck);
            this.Controls.Add(this.btnKaydet);
            this.Controls.Add(this.btnFindDir);
            this.Controls.Add(this.btnFindExe);
            this.Controls.Add(this.btnFindIco);
            this.Controls.Add(this.lbHedef);
            this.Controls.Add(this.txtDir);
            this.Controls.Add(this.txtExe);
            this.Controls.Add(this.txtIco);
            this.Controls.Add(this.lbExe);
            this.Controls.Add(this.lbIco);
            this.Font = new System.Drawing.Font("TR Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "AutoRunCreater";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "AutoRunCreater";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lbIco;
        private System.Windows.Forms.Label lbExe;
        private System.Windows.Forms.TextBox txtIco;
        private System.Windows.Forms.TextBox txtExe;
        private System.Windows.Forms.TextBox txtDir;
        private System.Windows.Forms.Label lbHedef;
        private System.Windows.Forms.Button btnFindIco;
        private System.Windows.Forms.Button btnFindExe;
        private System.Windows.Forms.Button btnFindDir;
        private System.Windows.Forms.Button btnKaydet;
        private System.Windows.Forms.CheckBox cbFullPck;
        private System.Windows.Forms.ToolTip toolTip;
        private System.Windows.Forms.FolderBrowserDialog fbd;
        private System.Windows.Forms.OpenFileDialog Ofd;
    }
}