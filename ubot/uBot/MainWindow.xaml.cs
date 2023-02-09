﻿using BotEngine;
using BotEngine.Packets;
using BotEngine.Packets.Actions;
using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Reflection.Emit;
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
using uBot.ViewModels;

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
         //  DllInjectionResult res = DllInjector.GetInstance.Inject("sro_client", "SilkroadPacketHook.dll");
            DataContext = new MainWindowViewModel();

            server = new ServerSocket();
            server.SetupServer();
        }


        private void OpCodeTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            PacketFilters.Instance.FilterText = OpCodeTextBox.Text;
            List1.ItemsSource = PacketFilters.Instance.FilteredPackets;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            List1.ItemsSource = PacketFilters.Instance.FilteredPackets;
        }
    }
}
