namespace USBClient
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmMain));
            System.Windows.Forms.ListViewItem listViewItem5 = new System.Windows.Forms.ListViewItem("Cihaz 0", 0);
            System.Windows.Forms.ListViewItem listViewItem6 = new System.Windows.Forms.ListViewItem("Cihaz 1", 0);
            System.Windows.Forms.ListViewItem listViewItem7 = new System.Windows.Forms.ListViewItem("Cihaz 2", 0);
            System.Windows.Forms.ListViewItem listViewItem8 = new System.Windows.Forms.ListViewItem("Cihaz 3", 0);
            this.tools = new System.Windows.Forms.ToolStrip();
            this.tslIPAddress = new System.Windows.Forms.ToolStripLabel();
            this.txtIpAddress = new System.Windows.Forms.ToolStripTextBox();
            this.tsbSave = new System.Windows.Forms.ToolStripButton();
            this.Status = new System.Windows.Forms.StatusStrip();
            this.StatusText = new System.Windows.Forms.ToolStripStatusLabel();
            this.lvRoleList = new System.Windows.Forms.ListView();
            this.images = new System.Windows.Forms.ImageList(this.components);
            this.btnExit = new System.Windows.Forms.Button();
            this.btnOnOff = new System.Windows.Forms.Button();
            this.btnConnect = new System.Windows.Forms.Button();
            this.tools.SuspendLayout();
            this.Status.SuspendLayout();
            this.SuspendLayout();
            // 
            // tools
            // 
            this.tools.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tslIPAddress,
            this.txtIpAddress,
            this.tsbSave});
            this.tools.Location = new System.Drawing.Point(0, 0);
            this.tools.Name = "tools";
            this.tools.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.tools.Size = new System.Drawing.Size(495, 25);
            this.tools.TabIndex = 0;
            this.tools.Text = "toolStrip1";
            // 
            // tslIPAddress
            // 
            this.tslIPAddress.Name = "tslIPAddress";
            this.tslIPAddress.Size = new System.Drawing.Size(59, 22);
            this.tslIPAddress.Text = "IP Address";
            // 
            // txtIpAddress
            // 
            this.txtIpAddress.Name = "txtIpAddress";
            this.txtIpAddress.Size = new System.Drawing.Size(120, 25);
            // 
            // tsbSave
            // 
            this.tsbSave.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tsbSave.Image = ((System.Drawing.Image)(resources.GetObject("tsbSave.Image")));
            this.tsbSave.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsbSave.Name = "tsbSave";
            this.tsbSave.Size = new System.Drawing.Size(23, 22);
            this.tsbSave.Text = "Kaydet";
            this.tsbSave.Click += new System.EventHandler(this.tsbSave_Click);
            // 
            // Status
            // 
            this.Status.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.StatusText});
            this.Status.Location = new System.Drawing.Point(0, 162);
            this.Status.Name = "Status";
            this.Status.Size = new System.Drawing.Size(495, 22);
            this.Status.TabIndex = 2;
            // 
            // StatusText
            // 
            this.StatusText.Name = "StatusText";
            this.StatusText.Size = new System.Drawing.Size(0, 17);
            // 
            // lvRoleList
            // 
            this.lvRoleList.Dock = System.Windows.Forms.DockStyle.Left;
            this.lvRoleList.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            listViewItem5,
            listViewItem6,
            listViewItem7,
            listViewItem8});
            this.lvRoleList.LargeImageList = this.images;
            this.lvRoleList.Location = new System.Drawing.Point(0, 25);
            this.lvRoleList.Name = "lvRoleList";
            this.lvRoleList.Size = new System.Drawing.Size(385, 137);
            this.lvRoleList.TabIndex = 3;
            this.lvRoleList.UseCompatibleStateImageBehavior = false;
            this.lvRoleList.SelectedIndexChanged += new System.EventHandler(this.lvRoleList_SelectedIndexChanged);
            // 
            // images
            // 
            this.images.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("images.ImageStream")));
            this.images.TransparentColor = System.Drawing.Color.Transparent;
            this.images.Images.SetKeyName(0, "netcard_48.png");
            // 
            // btnExit
            // 
            this.btnExit.Image = ((System.Drawing.Image)(resources.GetObject("btnExit.Image")));
            this.btnExit.Location = new System.Drawing.Point(391, 118);
            this.btnExit.Name = "btnExit";
            this.btnExit.Size = new System.Drawing.Size(97, 39);
            this.btnExit.TabIndex = 4;
            this.btnExit.Text = "Çýkýþ";
            this.btnExit.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.btnExit.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnExit.UseVisualStyleBackColor = true;
            this.btnExit.Click += new System.EventHandler(this.btnExit_Click);
            // 
            // btnOnOff
            // 
            this.btnOnOff.Enabled = false;
            this.btnOnOff.Image = ((System.Drawing.Image)(resources.GetObject("btnOnOff.Image")));
            this.btnOnOff.Location = new System.Drawing.Point(391, 28);
            this.btnOnOff.Name = "btnOnOff";
            this.btnOnOff.Size = new System.Drawing.Size(97, 39);
            this.btnOnOff.TabIndex = 5;
            this.btnOnOff.Text = "Aç";
            this.btnOnOff.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.btnOnOff.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnOnOff.UseVisualStyleBackColor = true;
            this.btnOnOff.Click += new System.EventHandler(this.btnOnOff_Click);
            // 
            // btnConnect
            // 
            this.btnConnect.Image = ((System.Drawing.Image)(resources.GetObject("btnConnect.Image")));
            this.btnConnect.Location = new System.Drawing.Point(391, 73);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(97, 39);
            this.btnConnect.TabIndex = 6;
            this.btnConnect.Text = "Baðlan";
            this.btnConnect.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.btnConnect.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 14F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(495, 184);
            this.Controls.Add(this.btnConnect);
            this.Controls.Add(this.btnOnOff);
            this.Controls.Add(this.btnExit);
            this.Controls.Add(this.lvRoleList);
            this.Controls.Add(this.Status);
            this.Controls.Add(this.tools);
            this.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "frmMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "USB Remote Control - CLIENT";
            this.tools.ResumeLayout(false);
            this.tools.PerformLayout();
            this.Status.ResumeLayout(false);
            this.Status.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ToolStrip tools;
        private System.Windows.Forms.StatusStrip Status;
        private System.Windows.Forms.ToolStripLabel tslIPAddress;
        private System.Windows.Forms.ToolStripTextBox txtIpAddress;
        private System.Windows.Forms.ToolStripButton tsbSave;
        private System.Windows.Forms.ListView lvRoleList;
        private System.Windows.Forms.Button btnExit;
        private System.Windows.Forms.Button btnOnOff;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.ImageList images;
        private System.Windows.Forms.ToolStripStatusLabel StatusText;
    }
}

