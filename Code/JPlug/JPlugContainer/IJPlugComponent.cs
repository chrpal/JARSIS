using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Text;

namespace JPlugContainer
{
    //This is needed for JPlug to find all plugins in the folder
    public interface IJPlugComponent
    {
        JPlugComponent GetComponentInformation();
        JPlugServiceMethod[] GetServiceMethods();
    }

    [Export(typeof(IJPlugComponent))]
    public class JPlugBaseComponent : IJPlugComponent
    {
        public JPlugBaseComponent()
        {

        }

        public JPlugComponent GetComponentInformation()
        {
            JPlugComponent result = null;

            System.Reflection.MemberInfo info = typeof(JPlugComponent);
            object[] attributes = info.GetCustomAttributes(true);

            foreach(object obj in attributes)
            {
                if (obj is JPlugComponent)
                {
                    result = (JPlugComponent)obj;
                    break;
                }
            }

            return result;
        }

        public JPlugServiceMethod[] GetServiceMethods()
        {
            List<JPlugServiceMethod> result = new List<JPlugServiceMethod>();

            System.Reflection.MemberInfo info = typeof(JPlugServiceMethod);
            object[] attributes = info.GetCustomAttributes(true);

            foreach (object obj in attributes)
            {
                if (obj is JPlugServiceMethod)
                {
                    result.Add((JPlugServiceMethod)obj);
                }
            }

            return result.ToArray();
        }
    }
}
