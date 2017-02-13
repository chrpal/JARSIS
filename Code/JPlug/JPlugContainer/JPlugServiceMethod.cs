using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace JPlugContainer
{

    //This is needed to build up the GATT server on Bluetooth side
    [AttributeUsage(AttributeTargets.Method)]
    public class JPlugServiceMethod : Attribute
    {
        public String[] ServiceNames { get; set; }
        public String MethodName { get; set; }
        public byte VisibilityPrivilegeLevel { get; set; }
        public byte ReadPrivilegeLevel { get; set; }
        public byte WritePrivilegeLevel { get; set; }
    }
}
