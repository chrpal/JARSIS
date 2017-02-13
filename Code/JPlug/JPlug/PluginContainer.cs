using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Text;
using JPlugContainer;
using System.ComponentModel.Composition.Hosting;

namespace JPlug
{
    public class PluginContainer
    {
        [ImportMany(typeof(JPlugContainer.IJPlugComponent))]
        private JPlugContainer.IJPlugComponent[] components = null;

        protected static PluginContainer instance = null;

        protected PluginContainer()
        {

        }

        public static PluginContainer GetInstance()
        {
            if (PluginContainer.instance == null)
            {
                PluginContainer.instance = new PluginContainer();
            }
            return PluginContainer.instance;
        }

        public void SearchForPlugins(String path = ".")
        {
            DirectoryCatalog catalog = new DirectoryCatalog(path);
            CompositionContainer cc = new CompositionContainer(catalog);
            cc.ComposeParts(this);
        }
    }
}
