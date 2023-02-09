using BotEngine.Packets;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Threading;
using static System.Net.Mime.MediaTypeNames;

namespace BotEngine
{
    public  static class PacketRecorder
    {
        private static Dispatcher _dispatcher = Dispatcher.CurrentDispatcher;
        public static ObservableCollection<Packet> PacketList { get;private set; } = new ObservableCollection<Packet>();

        public static void Add(byte[] packet)
        {
            _dispatcher.Invoke(() =>
            {
                PacketList.Add(new Packet(packet));
            });
        }

        public static ObservableCollection<byte[]> GetAllRecordedPackets()
        {
            ObservableCollection<byte[]> outList = new ObservableCollection<byte[]>();
            foreach(Packet packet in PacketList)
            {
                outList.Add(packet.RawPacket);
            }

            return outList;
        }
        public static ObservableCollection<byte[]>  Filtr(string Opcode) 
        {
            ObservableCollection<byte[]> outList;

            byte[] opCodeByte = Enumerable.Range(0, Opcode.Length)
                     .Where(x => x % 2 == 0)
                     .Select(x => Convert.ToByte(Opcode.Substring(x, 2), 16))
                     .Reverse().ToArray();

           // if (Opcode=="")
         //   {
                outList = GetAllRecordedPackets();
           // }else
          //  {
                //outList = GetAllRecordedPackets().Where(x => (byte)x.GetValue(2) == opCodeByte[1] && (byte)x.GetValue(3) == opCodeByte[2]).;
          //  }

            return outList;
        }
        public static void Remove(byte[] packet) { }



    }
}
