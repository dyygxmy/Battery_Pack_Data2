using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using BIW.Data2.Data;
using System.Threading;
using System.Net;
using TimLogFile;
namespace BIW.Data2.Client
{
  
    public class StationDriver
    {
        private UdpClient uClient;
        public StationData cData;
        private bool isExit = false;
        private const int REALTIMEDATALENGTH=19;
        /// <summary>
        /// 已经接受到的数据长度
        /// </summary>
        private int receivedLength;
        /// <summary>
        /// 记录上次错误
        /// </summary>
        private string lastErr = string.Empty;
        public StationDriver(StationData data)
        {
            cData = data;
            uClient = new UdpClient(data.IPAddress, 9000);
            uClient.Client.ReceiveTimeout = 8000;
            ThreadPool.QueueUserWorkItem(new WaitCallback(Communicate));
        }
        /// <summary>
        /// 停止线程
        /// </summary>
        public void Stop()
        {
            isExit = true;
        }
        private void Communicate(object state)
        {
            while (isExit == false)
            {
                GetRealTimeData();
                switch (cData.Command)
                {
                    case CommandEnum.PowedOff:
                        PoweredOff();
                        break;
                    case CommandEnum.ReStart:
                        ReStart();
                        break;
                    case CommandEnum.UpgradeOver:
                        UpgradeOver();
                        break;
                    case CommandEnum.RecoveryOver:
                        RecoveryOver();
                        break;
                }
                Thread.Sleep(200);
            }
        }

        private void GetRealTimeData()
        {
            //double seconds = 0;
            DateTime dtFrom = DateTime.Now;
            byte[] sendBytes = new byte[7];
            byte[] receivedBytes = new byte[19];
            byte[] recData;
            sendBytes[0] = 0x02;
            sendBytes[1] = 0x01;
            sendBytes[3] = 0x01;
            sendBytes[sendBytes.Length-2]=SumChk(sendBytes);
            sendBytes[6] = 0x03;
            try
            {
                uClient.Send(sendBytes, sendBytes.Length);

                IPEndPoint remoteHost = new IPEndPoint(IPAddress.Parse(cData.IPAddress), 9000);//new IPEndPoint(IPAddress.Any, 0);
                recData = uClient.Receive(ref remoteHost);
                receivedLength = recData.Length;
                Array.Copy(recData, 0, receivedBytes, 0, recData.Length);
                while (receivedLength < REALTIMEDATALENGTH)
                {
                    recData = uClient.Receive(ref remoteHost);
                    Array.Copy(recData, 0, receivedBytes, receivedLength, recData.Length);
                    receivedLength += recData.Length;
                }
                int sumchk = receivedBytes[receivedBytes.Length - 2];
                if (receivedLength == REALTIMEDATALENGTH && receivedBytes[0] == 0x02 && receivedBytes[receivedBytes.Length - 1] == 0x03
                    && SumChk(receivedBytes) == receivedBytes[receivedBytes.Length - 2])
                {
                    //Console.WriteLine(cData.IPAddress.ToString());
                    //seconds = (DateTime.Now - dtFrom).TotalSeconds;
                    //Console.WriteLine(seconds.ToString("0.000"));
                    int dataStart = 4;
                    cData.Version = string.Format("V{0}", Math.Round(BitConverter.ToInt16(receivedBytes, 4)/1000f,3));
                    dataStart += 2;
                    cData.RunMode = ((RunModeEnum)receivedBytes[dataStart++]).ToString();//==1?"工作模式":"维护模式";
                    cData.BarCodeMode = ((BarCodeModeEnum)(receivedBytes[dataStart++])).ToString();

                    cData.DeviceType = ((DeviceTypeEnum)(receivedBytes[dataStart++])).ToString();
                    
                    cData.Ready = receivedBytes[dataStart++] == 1;
                    cData.RFIDConnect = receivedBytes[dataStart++] == 1;
                    cData.DataServerConnect = receivedBytes[dataStart++] == 1;
                    cData.State = ((StateEnum)(receivedBytes[dataStart++])).ToString();
                    cData.ProgramNumber = receivedBytes[dataStart++];
                    cData.TotalScrew = receivedBytes[dataStart++];
                    cData.FinishedScrew = receivedBytes[dataStart++];
                    cData.OperatingTimeLeft = receivedBytes[dataStart++];
                    cData.IsOnline = true;
                    if (!string.IsNullOrEmpty(lastErr))
                    {
                        Logger.WriteInfo(cData.IPAddress + " connected!");
                        lastErr = string.Empty;
                    }
                }
                else
                {
                    cData.IsOnline = false;
                }
            }
            catch(Exception e)
            {
                //seconds = (DateTime.Now - dtFrom).TotalSeconds;
                //Console.WriteLine(seconds.ToString("0.000"));
                if (lastErr != e.ToString())
                {
                    lastErr = e.ToString();
                    Logger.WriteError(cData.IPAddress + ":" + e.ToString());
                }
                cData.IsOnline = false;
                Thread.Sleep(3000);
            }
        }
        private void PoweredOff()
        {
            byte[] sendBytes = new byte[7];
            byte[] receivedBytes = new byte[7];
            sendBytes[0] = 0x02;
            sendBytes[1] = 0x02;
            sendBytes[3] = 0x01;
            sendBytes[sendBytes.Length - 2] = SumChk(sendBytes);
            sendBytes[6] = 0x03;
            try
            {
                uClient.Send(sendBytes, sendBytes.Length);

                IPEndPoint remoteHost = new IPEndPoint(IPAddress.Parse(cData.IPAddress), 9000);//new IPEndPoint(IPAddress.Any, 0);
                byte[] recData = uClient.Receive(ref remoteHost);
                receivedLength = recData.Length;
                Array.Copy(recData, 0, receivedBytes, 0, recData.Length);
                while (receivedLength < 7)
                {
                    recData = uClient.Receive(ref remoteHost);
                    Array.Copy(recData, 0, receivedBytes, receivedLength, recData.Length);
                    receivedLength += recData.Length;
                }
                int sumchk = receivedBytes[receivedBytes.Length - 2];
                if (receivedLength == 5 && receivedBytes[0] == 0x02 && receivedBytes[1] == 0x02 && receivedBytes[receivedBytes.Length - 1] == 0x03
                    && SumChk(receivedBytes) == receivedBytes[receivedBytes.Length - 2])
                {
                    if (receivedBytes[4] == 0)
                    {
                        cData.CommunicateResult = CommunicateResultEnum.Success;
                        MainData.PoweroffSuccessStations.Add(cData.IPAddress);
                    }
                    else if (receivedBytes[4] == 1)
                    {
                        cData.CommunicateResult = CommunicateResultEnum.ING;
                        MainData.PoweroffFailStations.Add(cData.IPAddress);
                    }
                    cData.IsOnline = true;
                }
                else
                {
                    cData.IsOnline = false;
                    Logger.WriteError(string.Format("{0} power off failed for communicating data check!", cData.IPAddress));
                    MainData.PoweroffFailStations.Add(cData.IPAddress);
                }
            }
            catch (Exception e)
            {
                Logger.WriteError(cData.IPAddress+":"+e.Message);
                cData.IsOnline = false;
                MainData.PoweroffFailStations.Add(cData.IPAddress);
                Thread.Sleep(1000);
            }
        }
        private void ReStart()
        {
            byte[] sendBytes = new byte[7];
            byte[] receivedBytes = new byte[7];
            sendBytes[0] = 0x02;
            sendBytes[1] = 0x03;
            sendBytes[3] = 0x01;
            sendBytes[sendBytes.Length - 2] = SumChk(sendBytes);
            sendBytes[6] = 0x03;
            try
            {  
                IPEndPoint remoteHost = new IPEndPoint(IPAddress.Parse(cData.IPAddress), 9000);//new IPEndPoint(IPAddress.Any, 0);
                while(uClient.Available > 0)
                {
                    uClient.Receive(ref remoteHost);
                }
                uClient.Send(sendBytes, sendBytes.Length);

              
                byte[] recData = uClient.Receive(ref remoteHost);
                receivedLength = recData.Length;
                Array.Copy(recData, 0, receivedBytes, 0, recData.Length);
                while (receivedLength < 7)
                {
                    recData = uClient.Receive(ref remoteHost);
                    Array.Copy(recData, 0, receivedBytes, receivedLength, recData.Length);
                    receivedLength += recData.Length;
                }
                int sumchk = receivedBytes[receivedBytes.Length - 2];
                if (receivedLength == 7 && receivedBytes[0] == 0x02 && receivedBytes[1] == 0x03 && receivedBytes[receivedBytes.Length - 1] == 0x03
                    && SumChk(receivedBytes) == receivedBytes[receivedBytes.Length - 2])
                {
                    if (receivedBytes[4] == 0)
                    {
                        cData.CommunicateResult = CommunicateResultEnum.Success;
                        MainData.RestartSuccessStations.Add(cData.IPAddress);
                    }
                    else if (receivedBytes[4] == 1)
                    {
                        cData.CommunicateResult = CommunicateResultEnum.ING;
                        Logger.WriteError(string.Format("{0} restart failed for being ING state!", cData.IPAddress));
                        MainData.RestartFailStations.Add(cData.IPAddress);
                    }
                    cData.IsOnline = true;
                }
                else
                {
                    cData.CommunicateResult = CommunicateResultEnum.Error;
                    Logger.WriteError(string.Format("{0} restart failed for communicating data check!", cData.IPAddress));
                    MainData.RestartFailStations.Add(cData.IPAddress);
                }
            }
            catch (Exception e)
            {
                Logger.WriteError(cData.IPAddress + ":" + e);
                cData.IsOnline = false;
                MainData.RestartFailStations.Add(cData.IPAddress);
                Thread.Sleep(1000);
            }
        }
        private void UpgradeOver()
        {
            byte[] sendBytes = new byte[6 + cData.UpgradeFileName.Length];
            byte[] receivedBytes = new byte[7];
            sendBytes[0] = 0x02;
            sendBytes[1] = 0x05;
            sendBytes[3] = (byte)cData.UpgradeFileName.Length;
            char[] fileName = cData.UpgradeFileName.ToCharArray();
            for (int i = 0; i < fileName.Length; i++)
            {
                sendBytes[4 + i] = (byte)fileName[i];
            }
            sendBytes[sendBytes.Length - 2] = SumChk(sendBytes);
            sendBytes[sendBytes.Length - 1] = 0x03;
            try
            {
                uClient.Send(sendBytes, sendBytes.Length);

                IPEndPoint remoteHost = new IPEndPoint(IPAddress.Parse(cData.IPAddress), 9000);//new IPEndPoint(IPAddress.Any, 0);
                byte[] recData = uClient.Receive(ref remoteHost);
                receivedLength = recData.Length;
                Array.Copy(recData, 0, receivedBytes, 0, recData.Length);
                while (receivedLength < 7)
                {
                    recData = uClient.Receive(ref remoteHost);
                    Array.Copy(recData, 0, receivedBytes, receivedLength, recData.Length);
                    receivedLength += recData.Length;
                }
                int sumchk = receivedBytes[receivedBytes.Length - 2];
                if (receivedLength == 7 && receivedBytes[0] == 0x02 && receivedBytes[1] == 0x05 && receivedBytes[receivedBytes.Length - 1] == 0x03
                    && SumChk(receivedBytes) == receivedBytes[receivedBytes.Length - 2])
                {
                    if (receivedBytes[4] == 0)
                    {
                        //ReStart();
                        //if (cData.CommunicateResult != CommunicateResultEnum.Success)
                        //{
                        //    MainData.UploadFailStations.Add(cData.IPAddress);
                        //}
                        //cData.CommunicateResult = CommunicateResultEnum.Success;
                    }
                    else if (receivedBytes[4] == 1)
                    {
                        //cData.CommunicateResult = CommunicateResultEnum.ING;
                    }
                    cData.IsOnline = true;
                }
                else
                {
                    cData.IsOnline = false;
                }
            }
            catch (Exception e)
            {
                Logger.WriteException(e);
                cData.IsOnline = false;
                Thread.Sleep(1000);
            }
        }
        private void RecoveryOver()
        {
            byte[] sendBytes = new byte[7];
            byte[] receivedBytes = new byte[7];
            sendBytes[0] = 0x02;
            sendBytes[1] = 0x04;
            sendBytes[3] = 0x01;
            sendBytes[sendBytes.Length - 2] = SumChk(sendBytes);
            sendBytes[6] = 0x03;
            try
            {
                uClient.Send(sendBytes, sendBytes.Length);

                IPEndPoint remoteHost = new IPEndPoint(IPAddress.Parse(cData.IPAddress), 9000);//new IPEndPoint(IPAddress.Any, 0);
                byte[] recData = uClient.Receive(ref remoteHost);
                receivedLength = recData.Length;
                Array.Copy(recData, 0, receivedBytes, 0, recData.Length);
                while (receivedLength < 7)
                {
                    recData = uClient.Receive(ref remoteHost);
                    Array.Copy(recData, 0, receivedBytes, receivedLength, recData.Length);
                    receivedLength += recData.Length;
                }
                int sumchk = receivedBytes[receivedBytes.Length - 2];
                if (receivedLength == 7 && receivedBytes[0] == 0x02 && receivedBytes[1] == 0x04 && receivedBytes[receivedBytes.Length - 1] == 0x03
                    && SumChk(receivedBytes) == receivedBytes[receivedBytes.Length - 2])
                {
                    if (receivedBytes[4] == 0)
                    {
                        cData.CommunicateResult = CommunicateResultEnum.Success;
                    }
                    else if (receivedBytes[4] == 1)
                    {
                        cData.CommunicateResult = CommunicateResultEnum.ING;
                    }
                    cData.IsOnline = true;
                }
                else
                {
                    cData.CommunicateResult = CommunicateResultEnum.Error;
                }
            }
            catch (Exception e)
            {
                Logger.WriteException(e);
                cData.IsOnline = false;
                Thread.Sleep(1000);
            }
        }
        /// <summary>
        /// 求和校验
        /// </summary>
        /// <param name="data">要校验的数组</param>
        /// <returns>校验结果数组</returns>
        private byte SumChk(byte[] data)
        {
            int checkSum = 0;
            byte[] result = new byte[2];
            for (int i = 0; i < data.Length - 2; i++)
            {
                checkSum += data[i];
            }
            return (byte)(checkSum & 0x00ff);//data[data.Length - 2] = 
        }
    }
}
