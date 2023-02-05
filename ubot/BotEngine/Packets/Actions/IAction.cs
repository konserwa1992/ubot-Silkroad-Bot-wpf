using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BotEngine.Packets.Actions
{
    public interface IAction
    {
        bool toServer { get; set; }
        byte[] OpCode { get; set; }
        byte[] RawPacket { get; set; }
        byte[] Process();
    }
}
