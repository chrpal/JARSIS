using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Web.Script.Serialization;

namespace JPlug
{
    public class DataPipe
    {
        public static void SendData(String data)
        {
            Console.Write(data);
        }

        public static String ReceiveData()
        {
            return Console.ReadLine();
        }

        public static void UpdateMsgProcModuleMessages(String data)
        {
            JavaScriptSerializer js = new JavaScriptSerializer();
            object obj = js.Deserialize(data, typeof(JPlugContainer.JPlugMessage));
            MsgProcModule.GetInstance().AddRxMsg(obj);
        }
    }
}
