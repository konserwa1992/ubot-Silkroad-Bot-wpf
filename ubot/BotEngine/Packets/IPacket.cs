using BotEngine.Packets.Actions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Threading.Tasks;

namespace BotEngine.Packets
{
    internal interface IPacket
    {
        byte[] RawPacket { get; set; }
        byte[] PreperePacket(IAction action);
    }
}
