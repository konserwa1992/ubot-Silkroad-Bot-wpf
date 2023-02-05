using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BotEngine.Packets.Actions
{
    public class Chat : IAction
    {
        public bool toServer { get; set; } = true;
        public byte[] OpCode { get; set; } = new byte[] { 0x70 ,0x25 };
        public byte[] RawPacket { get; set; }


        private string Text;

        public Chat(string text) {
            this.Text = text;
        }

        public byte[] Process()
        {
            List<byte> data = new List<byte>();
            data.Add((byte)(toServer==true ? 0x01:0x00));
            data.AddRange(OpCode);
            data.AddRange(new byte[] { 0x01, 0x06, (byte)Text.Length, 0x00 });
            data.AddRange(ASCIIEncoding.ASCII.GetBytes(Text));
            RawPacket = data.ToArray();
            return RawPacket;
        }
    }
}
