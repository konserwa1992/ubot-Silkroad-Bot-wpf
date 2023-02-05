using BotEngine;
using BotEngine.Packets.Actions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace uBot
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
       public static ServerSocket server;
        public MainWindow()
        {
            InitializeComponent();
            server = new ServerSocket();
            server.SetupServer();
        }

        private void Button1_Click(object sender, RoutedEventArgs e)
        {
            server.Send(new Chat(Textbox1.Text));
        }
    }
}
