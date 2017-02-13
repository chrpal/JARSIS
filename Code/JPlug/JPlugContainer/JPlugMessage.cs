using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Web.Script.Serialization;

namespace JPlugContainer
{
    //General definition of a JPlugMessage
    //This is needed to establish a generic communication between
    //node.js and the process on a arm based linux host system on
    //which communication takes places using stdin and stdout
    public class JPlugMessage
    {

        public enum MsgType
        {
            NONE = 0, 
            EXECUTE, //with further parameters
            EXECUTION_RESULT, //Used when a service method was executed successfully
            EXECUTION_ERROR, //used when a service method has failed execution
            SEARCH_FOR_COMPONENTS, 
            STOP_JPLUG, 
            RESTART_JPLUG, 
            GET_COMPONENT_LIST, //with filter parameters etc.
            GET_SERVICE_METHOD_LIST,
            UPLOAD_COMPONENT
        };

        public MsgType type = MsgType.NONE;
        public object MsgContent = null;

        public String ToJson()
        {
            String result = "";
            JavaScriptSerializer js = new JavaScriptSerializer();
            result = js.Serialize(this);
            return result;
        }
    }
}
