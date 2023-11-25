using System;
using System.Runtime.InteropServices;

namespace USBManagement
{
    internal class UsbHidManagement
    {
        #region InputReport

        public class InputReport
        {
            bool isReadyEventObject = false;

            /// <summary>
            /// 
            /// </summary>
            /// <param name="hReadHandle"></param>
            /// <param name="buffer"></param>
            /// <param name="Length"></param>
            /// <returns></returns>
            public bool HidControlReadTransfer(IntPtr hReadHandle, ref byte[] buffer, int Length)
            {
                bool Success = false;

                Success = Win32.HidD_GetInputReport(hReadHandle, ref buffer[0], buffer.Length);
                return Success;
            }

            /// <summary>
            /// 
            /// </summary>
            /// <param name="hReadHandle"></param>
            /// <param name="buffer"></param>
            /// <param name="Length"></param>
            /// <param name="timeOut"></param>
            /// <returns></returns>
            public bool HidInterruptReadTransfer(IntPtr hReadHandle, ref byte[] buffer, int Length, int timeOut)
            {
                long lResult = 0;
                int iEventObject = 0;
                int iNumberOfBytesRead = 0;
                Win32.OVERLAPPED FileOverlapped = new Win32.OVERLAPPED();

                if (!isReadyEventObject) InitFileOverlapped(ref FileOverlapped, ref iEventObject);
                lResult = Win32.ReadFile(
                                           hReadHandle,
                                           ref buffer[0],
                                           buffer.Length,
                                           ref iNumberOfBytesRead,
                                           ref FileOverlapped
                                           );
                //WaitForSingleObject okuma iþlemi tamamlandýðýnda yada zaman aþýmý
                //gerçekleþtiðinde sonlanýr.
                lResult = Win32.WaitForSingleObject(iEventObject, timeOut);
                switch (lResult)
                {
                    case Win32.WAIT_OBJECT_0:   //Okuma baþarýlý
                        return true;
                    case Win32.WAIT_TIMEOUT:    //Zaman aþýmý
                        throw new ReadTimeOutException("Okuma iþlemi zaman aþýmýna uðradý.");
                }
                return false;
            }

            /// <summary>
            /// 
            /// </summary>
            /// <param name="FileOverlapped"></param>
            /// <param name="eventObject"></param>
            private void InitFileOverlapped(ref Win32.OVERLAPPED FileOverlapped, ref int eventObject)
            {
                Win32.SECURITY_ATTRIBUTES Security = new Win32.SECURITY_ATTRIBUTES();

                Security.iSecurityDescriptor = 0;
                Security.iInheritHandle = System.Convert.ToInt32(true);
                Security.iLength = Marshal.SizeOf(Security);

                eventObject = Win32.CreateEvent(ref Security, Convert.ToInt32(false), Convert.ToInt32(true), "");

                FileOverlapped.Offset = 0;
                FileOverlapped.OffsetHigh = 0;
                FileOverlapped.hEvent = eventObject;
            }
        }
        #endregion

        #region OutputReport

        public class OutputReport
        {
            /// <summary>
            /// 
            /// </summary>
            /// <param name="hWriteHandle"></param>
            /// <param name="buffer"></param>
            /// <param name="Length"></param>
            /// <returns></returns>
            public bool HidControlWriteTransfer(IntPtr hWriteHandle, ref byte[] buffer, int Length)
            {
                bool Success = false;

                if (buffer.Length == 0) return false;
                byte[] CBuff = CreateOutputReportBuffer(buffer, Length);

                Success = Win32.HidD_SetOutputReport(hWriteHandle, ref CBuff[0], Length);
                return Success;
            }

            /// <summary>
            /// 
            /// </summary>
            /// <param name="hWriteHandle"></param>
            /// <param name="buffer"></param>
            /// <param name="Length"></param>
            /// <returns></returns>
            public bool HidInterruptWriteTransfer(IntPtr hWriteHandle, ref byte[] buffer, int Length)
            {
                int iResult = 0;
                int NumberOfBytesWritten = 0;

                if (buffer.Length == 0) return false;
                byte[] CBuff = CreateOutputReportBuffer(buffer, Length);

                iResult = Win32.WriteFile(hWriteHandle, ref CBuff[0], Length, ref NumberOfBytesWritten, 0);
                return (iResult != 0) ? true : false;
            }

            /// <summary>
            /// 
            /// </summary>
            /// <param name="buffer"></param>
            /// <param name="Length"></param>
            /// <returns></returns>
            private byte[] CreateOutputReportBuffer(byte[] buffer, int Length)
            {
                int iCount = 0;
                byte[] CBuff = new byte[Length];

                //Ýlk byte rapor ID olarak ayarlandý
                CBuff[0] = 0;
                for (int i = 1; i < Length; i++)
                {
                    if ((buffer.Length < Length) && (buffer.Length == i)) return CBuff;
                    CBuff[i] = buffer[iCount];
                    iCount++;
                }

                return CBuff;
            }
        }

        #endregion
    }
}
