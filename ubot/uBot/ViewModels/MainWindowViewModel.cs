using BotEngine;
using BotEngine.Packets;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace uBot.ViewModels
{
    public class MainWindowViewModel
    {
        public ObservableCollection<byte[]> RecordedPackets => PacketRecorder.GetAllRecordedPackets();
        public ObservableCollection<Packet> LPacketRecorder => PacketRecorder.PacketList;

        public ObservableCollection<Packet> DataLists
        {
            get { return PacketRecorder.PacketList; }
        }


        public ObservableCollection<Packet> DataLists2
        {
            get { return PacketFilters.Instance.FilteredPackets; }
        }
    }
}
