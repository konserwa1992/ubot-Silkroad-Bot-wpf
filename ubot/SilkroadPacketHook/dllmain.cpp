// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <vector>
#include <Windows.h>
#include <stdio.h>
#include <algorithm>
#include <fcntl.h>
#include <Psapi.h>
#include <io.h>
#include <iostream>
#include <sstream> 
#include <fstream>
#include <string>

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "BotClient.h"
#include <thread>

extern WSADATA _socketInfoData;

VOID WriteBytesASM(DWORD destAddress, LPVOID patch, DWORD numBytes);
VOID Codecave(DWORD destAddress, VOID(*func)(VOID), BYTE nopCount)
{
	// Calculate the code cave for chat interception
	DWORD offset = (PtrToUlong(func) - destAddress) - 5;

	// Buffer of NOPs, static since we limit to 'UCHAR_MAX' NOPs
	BYTE nopPatch[0xFF] = { 0 };

	// Construct the patch to the function call
	BYTE patch[5] = { 0xE8, 0x00, 0x00, 0x00, 0x00 };
	memcpy(patch + 1, &offset, sizeof(DWORD));
	WriteBytesASM(destAddress, patch, 5);

	// We are done if we do not have NOPs
	if (nopCount == 0)
		return;

	// Fill it with nops
	memset(nopPatch, 0x90, nopCount);

	// Make the patch now
	WriteBytesASM(destAddress + 5, nopPatch, nopCount);
}
VOID WriteBytesASM(DWORD destAddress, LPVOID patch, DWORD numBytes)
{
	// Store old protection of the memory page
	DWORD oldProtect = 0;

	// Store the source address
	DWORD srcAddress = PtrToUlong(patch);

	// Make sure page is writeable
	VirtualProtect((void*)(destAddress), numBytes, PAGE_EXECUTE_READWRITE, &oldProtect);

	// Do the patch (oldschool style to avoid memcpy)
	__asm
	{
		nop						// Filler
		nop						// Filler
		nop						// Filler

		mov esi, srcAddress		// Save the address
		mov edi, destAddress	// Save the destination address
		mov ecx, numBytes		// Save the size of the patch
		Start :
		cmp ecx, 0				// Are we done yet?
			jz Exit					// If so, go to end of function

			mov al, [esi]			// Move the byte at the patch into AL
			mov[edi], al			// Move AL into the destination byte
			dec ecx					// 1 less byte to patch
			inc esi					// Next source byte
			inc edi					// Next destination byte
			jmp Start				// Repeat the process
			Exit :
		nop						// Filler
			nop						// Filler
			nop						// Filler
	}

	// Restore old page protection
	VirtualProtect((void*)(destAddress), numBytes, oldProtect, &oldProtect);
}


DWORD ReturnAddr;
DWORD ReciveReturnAddr;
DWORD* outPacketAddr;
DWORD* inPacketAddr;
char* packet;
int psize;
int insize;
char exitData[255];


void Setup(const HMODULE instance) {

}





std::vector <Packet> PacketToSend;
void PacketHook();

void printPacket()
{
	printf("-----------------------------------------------------------------\n");
	printf("Original packet :\n");
	for (int i = 0; i < insize; i++)
	{
		printf("%hhx ", packet[i]);
	}
	printf("\n\n");
}
template <typename T>
void putValue2memory(void* dest, T* value)
{
	memcpy(dest, value, sizeof(T));
}
void PacketHook()
{
	if (PacketToSend.size() > 0 && packet[3] == 0x70 && packet[2] == 0x4f)
	{
		printf("################################################################\n");
		printf("Change packet to:\n");
		Packet pakiet = PacketToSend.back();

		putValue2memory<short>(packet, &pakiet.DataSize);  //Put Size of PacketData
		packet[2] = pakiet.OpCode[1];
		packet[3] = pakiet.OpCode[0];

		memcpy(&packet[6], pakiet.Packet, pakiet.DataSize);

		// 6 bo size(short) + opCode(short) + randomData(short)
		// psize to ca³y rozmiar pakietu a nie poszczególnych jego elementów
		psize = 6 + pakiet.DataSize;
		PacketToSend.pop_back();

		for (int i = 0; i < psize; i++)
		{
			printf("%hhx ", packet[i]);
		}
	}

}
void CallPacketFunctionToSwap()
{
	DWORD sitFunctionCall = 0x0081E750;
	__asm
	{
		PUSH 4;

		CALL sitFunctionCall
	}
}
void SendPacket(Packet packet)
{
	PacketToSend.push_back(packet);
	CallPacketFunctionToSwap();
}



__declspec(naked) void RecivePacketHook(void)
{
	__asm
	{
		pop ReciveReturnAddr

		MOV inPacketAddr, ESI
		mov insize, EBX
		PUSHAD
		PUSHFD
	}

	if (insize == 0)
	{
		packet = (char*)inPacketAddr;
		sscanf(&packet[0], "%c", &insize);
		printf("IN Packet OpCode 0x%hhx%hhx Size:%hhx %d\n", packet[3], packet[2], packet[0], insize);
		insize += 2+2+2; //we add 2 because there is 2 byte values with are 0x00 0x00 i dont know why + 1 short of size + 1 short of OPcode
		printPacket();
		
	}

	__asm
	{
		POPFD
		POPAD

		mov eax, [esi + ecx * 0x4 - 0x04]
		mov[edi + ecx * 0x4 - 0x04], eax

		push ReciveReturnAddr
		ret
	}
}

__declspec(naked) void SendPacketHook(void)
{
	__asm
	{
		pop ReturnAddr

		MOV outPacketAddr, EAX
		MOV psize, EDX
		PUSHAD
		PUSHFD
	}

	packet = (char*)outPacketAddr;
	//printf("OUT Packet %x\n", *packet);

	//printPacket();
	PacketHook();
	__asm
	{
		POPFD
		POPAD

		mov edx, psize
		mov ebp, psize
		MOV EDI, DWORD PTR DS : [ECX]
		SHL EDI, 8

		push ReturnAddr
		ret
	}
}





BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{


	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		AllocConsole();

		printf("START \n");
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);

		Codecave(0x004BF9FF, SendPacketHook, 0);
		Codecave(0x00B43D68, RecivePacketHook, 3);

		SetupSocket();

		DisableThreadLibraryCalls(hModule);
		const auto thread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Setup), hModule, 0, nullptr);

		if (thread)
		{
			CloseHandle(thread);
		}




		while (true)
		{
			RecivedPacket packet = recive();
		}

		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

