using BotEngine.Packets;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace BotEngine
{
    public class PacketFilters
    {
        public static PacketFilters Instance { get; private set; } = new PacketFilters();

        private PacketFilters() { }

        private string FilterRegx { get; set; }


        public string FilterText
        {
            get { return FilterRegx; }
            set
            {
                FilterRegx = value;
                FilterPackets();
            }
        }


        private ObservableCollection<Packet> _filteredPackets;
        public ObservableCollection<Packet> FilteredPackets
        {
           
            get {
               
                if (_filteredPackets == null) _filteredPackets = PacketRecorder.PacketList;
                return _filteredPackets; 
            }
            set
            {
                _filteredPackets = value;
            }
        }

        private void FilterPackets()
        {
            if (FilterText != "")
            {
                Regex filterRegex = new Regex(FilterText, RegexOptions.IgnoreCase);

                FilteredPackets = new ObservableCollection<Packet>(
                            PacketRecorder.PacketList.Where(p => filterRegex.IsMatch(p.RawPacketInText))
                );

            }
            else
                FilteredPackets = PacketRecorder.PacketList;
        }
    }
}
