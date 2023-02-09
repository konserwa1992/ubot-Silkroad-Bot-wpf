using BotEngine.Packets.Actions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BotEngine.Packets
{
    public class Packet : IPacket
    {
        public byte[] RawPacket { get; set; }

        public Packet(byte[] packet)
        {
            RawPacket = packet;
        }

        public string RawPacketInText
        {
            get
            {
                string sRaw = "";

               // sRaw = Encoding.UTF8.GetString(RawPacket);
                foreach(byte b in RawPacket)
                {
                    sRaw += b.ToString("X2") + " ";
                }
                return sRaw;
            }
        }

        byte[] IPacket.PreperePacket(IAction action)
        {
            return action.RawPacket;
        }
    }
}
