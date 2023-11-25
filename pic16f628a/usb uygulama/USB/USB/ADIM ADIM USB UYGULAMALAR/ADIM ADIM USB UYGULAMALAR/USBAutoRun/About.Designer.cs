namespace UsbToolManager
{
    partial class About
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
            this.label1 = new System.Windows.Forms.Label();
            this.pbUsb = new System.Windows.Forms.PictureBox();
            this.lbComment = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pbUsb)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("TR Tahoma", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.label1.Location = new System.Drawing.Point(78, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(119, 16);
            this.label1.TabIndex = 0;
            this.label1.Text = "USB AutoRun v1.00";
            // 
            // pbUsb
            // 
            this.pbUsb.Image = global::UsbToolManager.Properties.Resources.USB_Orange;
            this.pbUsb.Location = new System.Drawing.Point(34, 21);
            this.pbUsb.Name = "pbUsb";
            this.pbUsb.Size = new System.Drawing.Size(38, 34);
            this.pbUsb.TabIndex = 1;
            this.pbUsb.TabStop = false;
            // 
            // lbComment
            // 
            this.lbComment.AutoSize = true;
            this.lbComment.ForeColor = System.Drawing.SystemColors.AppWorkspace;
            this.lbComment.Location = new System.Drawing.Point(78, 42);
            this.lbComment.Name = "lbComment";
            this.lbComment.Size = new System.Drawing.Size(221, 13);
            this.lbComment.TabIndex = 2;
            this.lbComment.Text = "USB Otomatik Program Çalýþtýrma Yöneticisi";
            // 
            // About
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Info;
            this.ClientSize = new System.Drawing.Size(321, 82);
            this.Controls.Add(this.lbComment);
            this.Controls.Add(this.pbUsb);
            this.Controls.Add(this.label1);
            this.Font = new System.Drawing.Font("TR Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "About";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "About";
            ((System.ComponentModel.ISupportInitialize)(this.pbUsb)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.PictureBox pbUsb;
        private System.Windows.Forms.Label lbComment;
    }
}