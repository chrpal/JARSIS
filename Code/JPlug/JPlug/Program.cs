using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace JPlug
{
    class Program
    {
        static void Initialize()
        {
            MainDataManager.MainAppRunning = true;
            PluginContainer.GetInstance().SearchForPlugins();
            MsgProcModule.GetInstance().Start();
        }

        static void Terminate()
        {
            MsgProcModule.GetInstance().ThreadRunning = false;
        }

        static void Main(string[] args)
        {
            //Initialize application
            Initialize();
            String rxData = "";
            while (MainDataManager.MainAppRunning == true)
            {
                //Get data from pipe
                rxData = DataPipe.ReceiveData();
                //Add data to Rx msg queue
                if (rxData != "")
                {
                    DataPipe.UpdateMsgProcModuleMessages(rxData);
                }
                Thread.Sleep(1000);
            }

            while (MsgProcModule.GetInstance().AllTxMsgFlushed == false)
            {
                //Wait until all messages where send
                Thread.Sleep(1);
            }

            //Perform actions for termination
            Terminate();
        }
    }
}
