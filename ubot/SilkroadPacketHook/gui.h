#pragma once
#include <d3d9.h>


namespace gui {
	 bool isVisible = true;
	 bool isSetuped = false;
	 HWND window = nullptr;


	 WNDCLASSEX windowClass = { };
	 WNDPROC originalWindowProcess = nullptr;

	 LPDIRECT3DDEVICE9 device = nullptr;
	 LPDIRECT3D9 d3d9 = nullptr;

	 bool SetupWindowClass(const char* windowClass);
	 void DestroyWindowClass();

	 bool SetupWindow(const char* name);
	 void DestroyWindow();


	 bool SetupDirectX();
	 void DestroyDirectX();




	 void Setup();

	 void SetupMenu(LPDIRECT3DDEVICE9 device);
	 void Destroy();


	 void Render();
}