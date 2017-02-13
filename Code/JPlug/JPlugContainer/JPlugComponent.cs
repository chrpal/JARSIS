using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace JPlugContainer
{
    //Besserer Name
    //This is needed for the Physical Web
    [AttributeUsage(AttributeTargets.Class)]
    public class JPlugComponent : Attribute
    {
        public String ComponentType{get; set;}
        public String ControlHostAddress { get; set; }
        public String ControlWebContentFileName { get; set; }
        public String RemoteDataHostAddress { get; set; }
        public String RemoteDataContentDatabase { get; set; }
        public byte VisibilityPrivilegeLevel { get; set; }
        public byte ReadPrivilegeLevel { get; set; }
        public byte WritePrivilegeLevel { get; set; }

        //<= these are the information needed for
        // providing useful component access and usage
        // Maybe needs a special privilege (SuperUser, Admin, user etc.)
    }
}
