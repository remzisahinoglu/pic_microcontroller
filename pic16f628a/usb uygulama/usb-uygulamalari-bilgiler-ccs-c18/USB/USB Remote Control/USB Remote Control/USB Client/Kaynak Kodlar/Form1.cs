using System;
using System.Net;
using System.Net.Sockets;
using System.Drawing;
using System.Windows.Forms;

namespace USBClient
{
    public partial class frmMain : Form
    {
        #region Sabitler

        const int ON_OFF_INDEX = 1;
        const int ROLE_NO_INDEX = 0;
        const int PORT_NUMBER = 8090;

        #endregion

        #region Global Nesneler ve Deðiþkenler

        bool toggle;
        IPEndPoint IPEndP;
        Socket sckClientSocket;
        byte[] buffer = new byte[5];
        bool[] bOnOff = new bool[5];
        delegate void MarshalToForm();

        #endregion

        #region Kurucu Method

        public frmMain()
        {
            InitializeComponent();
            StartupClient();
        }

        #endregion

        #region Client Kodlarý

        /// <summary>
        /// 
        /// </summary>
        private void StartupClient()
        {
            if (string.IsNullOrEmpty(Properties.Settings.Default.IPServer))
                txtIpAddress.Text = GetMyIpAddress().ToString();
            else
                txtIpAddress.Text = Properties.Settings.Default.IPServer;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private IPAddress GetMyIpAddress()
        {
            string hostName = Dns.GetHostName();
            IPHostEntry ipEntry = Dns.GetHostEntry(hostName);

            return ipEntry.AddressList[0];
        }

        /// <summary>
        /// 
        /// </summary>
        private void ConnectToServer()
        {
            try
            {
                IPEndP = new IPEndPoint(IPAddress.Parse(txtIpAddress.Text), PORT_NUMBER);
                sckClientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

                IAsyncResult asyncRes = sckClientSocket.BeginConnect(
                                                        IPEndP,
                                                        new AsyncCallback(ConnectToServerCallBack),
                                                        sckClientSocket
                                                        );
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "Connect Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="asyncRes"></param>
        private void ConnectToServerCallBack(IAsyncResult asyncRes)
        {
            try
            {
                Socket sckClient = (Socket)asyncRes.AsyncState;
                sckClient.EndConnect(asyncRes);

                MarshalToForm mtof = delegate {
                    StatusText.Text = "Server ile baðlantý kuruldu";
                    btnOnOff.Enabled = true;
                    btnConnect.Enabled = false;
                };
                Invoke(mtof);
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "Connect CallBack Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private void SendDataToServer()
        {
            try
            {
                IAsyncResult asyncRes = sckClientSocket.BeginSend(
                                        buffer,
                                        0,
                                        buffer.Length,
                                        SocketFlags.None,
                                        new AsyncCallback(SendDataToServerCallBack),
                                        sckClientSocket
                                        );
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "SendData Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="asyncRes"></param>
        private void SendDataToServerCallBack(IAsyncResult asyncRes)
        {
            try
            {
                Socket sckClient = (Socket)asyncRes.AsyncState;
                int totalWrittenByte = sckClient.EndSend(asyncRes);

                MarshalToForm mtof = delegate {
                    StatusText.Text = lvRoleList.SelectedItems[0].Index.ToString() + " numaralý röle ayarlandý";
                };
                Invoke(mtof);
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "SendData CallBack Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnConnect_Click(object sender, EventArgs e)
        {
            ConnectToServer();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="deviceIndex"></param>
        private void OnOffDevice(int deviceIndex)
        {
            bOnOff[deviceIndex] = !bOnOff[deviceIndex];
            buffer[ROLE_NO_INDEX] = (byte)deviceIndex;
            buffer[ON_OFF_INDEX] = bOnOff[deviceIndex] ? (byte)1 : (byte)0;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnOnOff_Click(object sender, EventArgs e)
        {
            if (lvRoleList.SelectedItems.Count == 0) return;
            btnOnOff.Text = toggle ? "Aç" : "Kapat"; 
            OnOffDevice(lvRoleList.SelectedItems[0].Index);

            toggle = !toggle;
            SendDataToServer();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void lvRoleList_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (lvRoleList.SelectedItems.Count == 0) return;
            btnOnOff.Text = !bOnOff[lvRoleList.SelectedItems[0].Index] ? "Aç" : "Kapat";
        }

        private void tsbSave_Click(object sender, EventArgs e)
        {
            if (txtIpAddress.Text == string.Empty)
            {
                MessageBox.Show("Kaydetmek için bir IP adresi belirlemelisiniz!!", "IP Save Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }

            Properties.Settings.Default.IPServer = txtIpAddress.Text;
            Properties.Settings.Default.Save();
        }

        private void btnExit_Click(object sender, EventArgs e)
        {
            Close();
        }

        #endregion
    }
}