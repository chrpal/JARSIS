using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using RobotDrivers;

namespace KR30_16_GC_Driver_Test
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        KRC2_GC_M driver = null;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            driver = new KRC2_GC_M();
            driver.InitializeTCP("127.0.0.1", 6008);
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            if (driver != null)
            {
                driver.Terminate();
                driver = null;
            }
        }
    }
}
