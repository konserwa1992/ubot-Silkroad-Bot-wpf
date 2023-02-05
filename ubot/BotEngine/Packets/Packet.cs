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

        byte[] IPacket.PreperePacket(IAction action)
        {
            return action.RawPacket;
        }
    }
}
