//Hook By GAM & Stickey

#pragma once

typedef HRESULT(__stdcall *Prototype_Present)(IDirect3DDevice9 *, CONST RECT *, CONST RECT *, HWND, CONST RGNDATA *);
typedef HRESULT(__stdcall *Prototype_Reset)(IDirect3DDevice9 *, D3DPRESENT_PARAMETERS *);
typedef HRESULT(__stdcall *Prototype_DrawIndexedPrimitive)(IDirect3DDevice9 *, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);

HRESULT __stdcall Hooked_Present(IDirect3DDevice9 *pDevice, CONST RECT *pSrcRect, CONST RECT *pDestRect, HWND hDestWindow, CONST RGNDATA *pDirtyRegion);
HRESULT __stdcall Hooked_Reset(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentParams);
HRESULT __stdcall Hooked_DrawIndexedPrimitive(IDirect3DDevice9 *pDevice, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);

class CD3DHook
{
public:
	CD3DHook()
	{
		Orginal_Present = (Prototype_Present) GetDeviceAddress(17);
		Orginal_Reset = (Prototype_Reset) GetDeviceAddress(16);
		Orginal_DrawIndexedPrimitive = (Prototype_DrawIndexedPrimitive)GetDeviceAddress(82);
		DetourRestoreAfterWith();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&) Orginal_Present, Hooked_Present);
		DetourAttach(&(PVOID&) Orginal_Reset, Hooked_Reset);
		DetourAttach(&(PVOID&) Orginal_DrawIndexedPrimitive, Hooked_DrawIndexedPrimitive);
		DetourTransactionCommit();
	};

	D3DPRESENT_PARAMETERS pPresentParam;

	CD3DRender *pRender = new CD3DRender(128);
	CD3DFont *pD3DFont = new CD3DFont("Tahoma", 10, FCR_BORDER);
	bool bD3DRenderInit = false;

	Prototype_Present Orginal_Present;
	Prototype_Reset Orginal_Reset;
	Prototype_DrawIndexedPrimitive Orginal_DrawIndexedPrimitive;

private:
	DWORD FindDevice(DWORD Len)
	{
		DWORD dwObjBase = 0;
		char infoBuf[MAX_PATH];
		GetSystemDirectory(infoBuf, MAX_PATH);
		strcat_s(infoBuf, MAX_PATH, "\\d3d9.dll");
		dwObjBase = (DWORD) LoadLibrary(infoBuf);
		while (dwObjBase++ < dwObjBase + Len)
		{
			if ((*(WORD*) (dwObjBase + 0x00)) == 0x06C7 &&
				(*(WORD*) (dwObjBase + 0x06)) == 0x8689 &&
				(*(WORD*) (dwObjBase + 0x0C)) == 0x8689)
			{
				dwObjBase += 2;
				break;
			}
		}
		return(dwObjBase);
	};

	DWORD GetDeviceAddress(int VTableIndex)
	{
		PDWORD VTable;
		*(DWORD*) &VTable = *(DWORD*) FindDevice(0x128000);
		return VTable[VTableIndex];
	};
};

