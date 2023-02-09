#include "BotClient.h"

WSADATA _socketInfoData;
sockaddr_in clientService;
SOCKET ConnectSocket;

void SendPacket(Packet packet);


bool SetupSocket()
{
 
    int iResult = WSAStartup(MAKEWORD(2, 2), &_socketInfoData);
    if (iResult != NO_ERROR)
    {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientService.sin_port = htons(8080);

    if (connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
    {
        printf("Failed to connect.\n");
        WSACleanup();
        return 1;
    }


	return false;
}


int SendToBotClient(char* packet, unsigned short length)
{
    printf("Packet Addr: %x Lenght: %d", packet,length);
    if (length < 65535 && length>0)
    {
        int iSendResult = send(ConnectSocket, packet, length, 0);
        if (iSendResult == SOCKET_ERROR)
        {
            printf("send failed: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return 1;
        }
        printf("Bytes Sent: %ld\n", iSendResult);
    }
    return 0;
}


RecivedPacket Recive()
{
    char recvbuf[255];

    RecivedPacket inComingPacket;
    inComingPacket.size= recv(ConnectSocket, recvbuf, 255, 0);

    if (inComingPacket.size > 0)
    {
        inComingPacket.rawData = recvbuf;
        printf("Bytes received: %d\n", inComingPacket.size);
        printf("Received: %s\n", inComingPacket.rawData);


        if (inComingPacket.rawData[0] == 0x01)
        {
            struct Packet sendMe;
            sendMe.OpCode = new BYTE[2]{ (BYTE)inComingPacket.rawData[1],(BYTE)inComingPacket.rawData[2] };
            sendMe.Packet = new BYTE[inComingPacket.size - 3]();

            for (int i = 0; i < inComingPacket.size - 3; i++)
            {
                sendMe.Packet[i] = (BYTE)inComingPacket.rawData[i + 3];
            }

            sendMe.DataSize = inComingPacket.size - 3;

            for (int i = 0; i < inComingPacket.size; i++)
            {
                printf("%hhx ", inComingPacket.rawData[i]);
            }

            SendPacket(sendMe);
        }
    }

    return inComingPacket;
}
