using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using JPlugContainer;

namespace Agilus
{
    [JPlugComponent(ComponentType="KUKA Agilus", ControlHostAddress="https://46.3242.243.234./")]
    public class Class1
    {
        [JPlugServiceMethod(MethodName = "MoveLIN", ServiceNames = new string[]{"Agilus Movecontrol", "Agilus commissioning"})]
        public void MoveLIN(float x, float y, float z)
        {

        }
    }
}
