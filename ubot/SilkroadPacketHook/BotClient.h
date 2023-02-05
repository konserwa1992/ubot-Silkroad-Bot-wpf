#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>


bool SetupSocket();

struct RecivedPacket
{
    int size;
    char* rawData;
};


struct Packet
{
    short DataSize;
    BYTE* OpCode;
    BYTE* Packet;
};


RecivedPacket recive();
int Send();