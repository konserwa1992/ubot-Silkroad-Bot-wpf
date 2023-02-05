using BotEngine.Packets;
using BotEngine.Packets.Actions;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices.ComTypes;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace BotEngine
{
    public class ServerSocket
    {
        static int port = 8080;
        static IPAddress localAddress = IPAddress.Parse("127.0.0.1");
        TcpClient client;
        TcpListener server;
        Thread reciveThread;
        NetworkStream stream;
        public void SetupServer()
        {
            server = new TcpListener(localAddress, port);
            server.Start();

            Console.WriteLine("Server started on " + localAddress + ":" + port);

            client = server.AcceptTcpClient();
            Console.WriteLine("Client connected.");

            reciveThread = new Thread(new ThreadStart(Recive));
            reciveThread.Start();


            stream = client.GetStream();
        }


        void Destroy()
        {
            stream.Close();
            client.Close();
            server.Stop();
        }

       public void Send(IAction action)
        {
            action.Process();
            stream.Write(action.RawPacket, 0, action.RawPacket.Length);
        }

        public void Recive()
        {
            while (true)
            {
                byte[] data = new byte[1024];
                int bytesReceived = stream.Read(data, 0, data.Length);

                if (bytesReceived != 0)
                {
                    string msg = ASCIIEncoding.ASCII.GetString(data, 0, bytesReceived);
                    if(msg.Contains("bro"))
                    {
                        continue;
                    }
                    break;
                }

                string message = Encoding.ASCII.GetString(data, 0, bytesReceived);
                Console.WriteLine("Received: " + message);
            }
        }
    }
}
