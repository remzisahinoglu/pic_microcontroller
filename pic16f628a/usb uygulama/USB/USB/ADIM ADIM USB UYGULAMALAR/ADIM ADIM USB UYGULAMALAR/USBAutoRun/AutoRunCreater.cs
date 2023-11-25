using System;
using System.IO;
using System.ComponentModel;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace UsbToolManager
{
    public partial class AutoRunCreater : Form
    {
        public AutoRunCreater(string drive)
        {
            InitializeComponent();
            txtDir.Text = drive;
        }

        private void btnKaydet_Click(object sender, EventArgs e)
        {
            if (txtIco.Text == string.Empty)
            {
                MessageBox.Show("Icon dosyas�n� belirtmelisiniz!!", "Uyar�", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }
            else if (txtExe.Text == string.Empty)
            {
                MessageBox.Show("�al��t�r�labilir dosyay� belirtmelisiniz!!", "Uyar�", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }
            else if (txtDir.Text == string.Empty)
            {
                MessageBox.Show("INF dosyas�n�n olu�turulaca�� hedefi belirtmelisiniz!!", "Uyar�", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }
            CreateAutoRunFile(txtIco.Text, txtExe.Text, txtDir.Text, cbFullPck.Checked);
        }

        private void CreateAutoRunFile(string icofile, string executablePath, string direction, bool fullPath)
        {
            try
            {
                string dir;
                StringBuilder sbInf = new StringBuilder();

                sbInf.AppendLine("[autorun]");
                sbInf.AppendLine("icon=" + Path.GetFileName(icofile));
                sbInf.AppendLine("open=" + Path.GetFileName(executablePath));

                dir = direction.EndsWith("\\") ? direction : direction + "\\";
                StreamWriter strWr = new StreamWriter(dir + "Autorun.inf");

                strWr.Write(sbInf.ToString());
                strWr.Close();

                if (fullPath)
                {
                    File.Copy(icofile, dir + Path.GetFileName(icofile));
                    File.Copy(executablePath, dir + Path.GetFileName(executablePath));
                }

                MessageBox.Show("Autorun dosyas� " + dir + " dizininde olu�turuldu..", "AutoRun Create", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "HATA!!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnFindIco_Click(object sender, EventArgs e)
        {
            if (Ofd.ShowDialog() == DialogResult.OK) txtIco.Text = Ofd.FileName;
        }

        private void btnFindExe_Click(object sender, EventArgs e)
        {
            if (Ofd.ShowDialog() == DialogResult.OK) txtExe.Text = Ofd.FileName;
        }

        private void btnFindDir_Click(object sender, EventArgs e)
        {
            if (fbd.ShowDialog() == DialogResult.OK) txtDir.Text = fbd.SelectedPath;
        }
    }
}