using JPlugContainer;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace JPlug
{
    public class MsgProcModule
    {
        protected static MsgProcModule instance = null;

        protected List<object> MsgRxQueue = new List<object>();
        protected List<object> MsgTxQueue = new List<object>();

        protected ThreadStart msgprocmodulethreadstart = null;
        protected Thread msgprocmodulethread = null;

        protected Dictionary<JPlugMessage.MsgType, Func<object, JPlugMessage>> functionLookUpTable = new Dictionary<JPlugMessage.MsgType, Func<object, JPlugMessage>>();

        public Boolean ThreadRunning
        {
            set;
            get;
        }

        public Boolean AllTxMsgFlushed
        {
            get
            {
                return (this.MsgTxQueue.Count == 0);
            }
        }

        protected MsgProcModule()
        {
            this.Initialize();
        }

        protected JPlugMessage ProcessExecute(object content)
        {
            JPlugMessage result = new JPlugMessage();

            return result;
        }

        protected JPlugMessage ProcessSearchForComponents(object content)
        {
            JPlugMessage result = new JPlugMessage();

            return result;
        }

        protected JPlugMessage ProcessStopJPlug(object content)
        {
            JPlugMessage result = new JPlugMessage();
            result.type = JPlugMessage.MsgType.STOP_JPLUG;
            result.MsgContent = (Boolean)true;

            //Close app
            MainDataManager.MainAppRunning = false;

            return result;
        }

        protected JPlugMessage ProcessRestartJPlug(object content)
        {
            JPlugMessage result = new JPlugMessage();

            return result;
        }

        protected JPlugMessage ProcessGetComponentList(object content)
        {
            JPlugMessage result = new JPlugMessage();

            return result;
        }

        protected JPlugMessage ProcessGetServiceMethodList(object content)
        {
            JPlugMessage result = new JPlugMessage();

            return result;
        }

        protected JPlugMessage ProcessUploadComponent(object content)
        {
            JPlugMessage result = new JPlugMessage();

            return result;
        }

        protected JPlugMessage ProcessNone(object content)
        {
            JPlugMessage result = new JPlugMessage();

            return result;
        }

        protected void Initialize()
        {
            this.functionLookUpTable[JPlugMessage.MsgType.NONE] = this.ProcessNone;
            this.functionLookUpTable[JPlugMessage.MsgType.EXECUTE] = this.ProcessExecute;
            this.functionLookUpTable[JPlugMessage.MsgType.SEARCH_FOR_COMPONENTS] = this.ProcessSearchForComponents;
            this.functionLookUpTable[JPlugMessage.MsgType.STOP_JPLUG] = this.ProcessStopJPlug;
            this.functionLookUpTable[JPlugMessage.MsgType.RESTART_JPLUG] = this.ProcessRestartJPlug;
            this.functionLookUpTable[JPlugMessage.MsgType.GET_COMPONENT_LIST] = this.ProcessGetComponentList;
            this.functionLookUpTable[JPlugMessage.MsgType.GET_SERVICE_METHOD_LIST] = this.ProcessGetServiceMethodList;
            this.functionLookUpTable[JPlugMessage.MsgType.UPLOAD_COMPONENT] = this.ProcessUploadComponent;
        }


        public static MsgProcModule GetInstance()
        {
            if (MsgProcModule.instance == null)
            {
                MsgProcModule.instance = new MsgProcModule();
            }
            return MsgProcModule.instance;
        }

        protected void ProcessRx()
        {
            Func<object, JPlugMessage> funcPtr = null;
            JPlugMessage txMsg = null;
            lock (this.MsgRxQueue)
            {
                foreach (Object obj in this.MsgRxQueue)
                {
                    JPlugMessage msg = (JPlugMessage)obj;
                    funcPtr = this.functionLookUpTable[JPlugMessage.MsgType.NONE];
                    txMsg = null;

                    if (this.functionLookUpTable.ContainsKey(msg.type) == true)
                    {
                        funcPtr = this.functionLookUpTable[msg.type];
                        txMsg = funcPtr(msg.MsgContent);
                    }
                    else
                    {
                        txMsg = funcPtr(null);
                    }

                    this.MsgTxQueue.Add(txMsg);
                }
                this.MsgRxQueue.Clear();
            }
        }

        protected void ProcessTx()
        {
            foreach(Object obj in this.MsgTxQueue)
            {
                JPlugMessage msg = (JPlugMessage)obj;
                DataPipe.SendData(msg.ToJson());
            }
            this.MsgTxQueue.Clear();
        }

        protected void ThreadMain()
        {
            while (this.ThreadRunning == true)
            {
                //Process incoming messages
                this.ProcessRx();
                //Process outgoing messages
                this.ProcessTx();

                Thread.Sleep(1);
            }
        }

        public void Start()
        {
            this.msgprocmodulethreadstart = new ThreadStart(this.ThreadMain);
            this.msgprocmodulethread = new Thread(this.msgprocmodulethreadstart);
            this.ThreadRunning = true;
            this.msgprocmodulethread.Start();
        }

        public void Stop()
        {
            this.ThreadRunning = false;
        }

        public void AddRxMsg(object obj)
        {
            lock(this.MsgRxQueue)
            {
                this.MsgRxQueue.Add(obj);
            }
        }
    }
}
