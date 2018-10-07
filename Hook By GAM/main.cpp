//Hook By GAM & Stickey

#include <windows.h>
#include <d3d9.h>
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "main.h"
#include <intrin.h>
#pragma intrinsic(_ReturnAddress)

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define sleep Sleep
#define SAMP_CHAT_INFO_OFFSET 0x21A0E4

bool Chams = false;
bool ChamsMenu = false;
bool WireFrame = false;
bool SkinWall = false;

//DIP
bool ShaderTrick;
int ChamsVisible = 1;
int ChamsInvisible = 0;
IDirect3DPixelShader9	*Chams_Green;
IDirect3DPixelShader9	*Chams_Blue;
IDirect3DPixelShader9	*Chams_Red;

bool g_bwasInitialized = false;
bool menuOpen = false;
bool wndproc = false;
bool* p_open = NULL;
static int item = 0;
int close;
int hwndd;
int startmenu;

DWORD key;
HMODULE samp = GetModuleHandleA("samp.dll");
DWORD procID;
DWORD g_dwSAMP_Addr = NULL;
DWORD *g_Chat = NULL;
HANDLE handle;;
HWND hWnd;

WNDPROC oriWndProc = NULL;
extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void everything()
{
	GetWindowThreadProcessId(hWnd, &procID);
	handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
}

void toggleSAMPCursor(int iToggle)
{
	void        *obj = *(void **)((DWORD)samp + 0x21A10C);
	((void(__thiscall *) (void *, int, bool)) ((DWORD)samp + 0x9BD30))(obj, iToggle ? 3 : 0, !iToggle);
	if (!iToggle)
		((void(__thiscall *) (void *)) ((DWORD)samp + 0x9BC10))(obj);
}

void toggleChat(int toggle)
{
	int togchattrue = 0xC3;
	int togchatfalse = 2347862870;
	everything();
	if (toggle == 1)
	{
		WriteProcessMemory(handle, (LPVOID)((DWORD)samp + 0x64230), &togchattrue, sizeof(togchattrue), 0);
	}
	else
	{
		WriteProcessMemory(handle, (LPVOID)((DWORD)samp + 0x64230), &togchatfalse, sizeof(togchatfalse), 0);
	}
}

void Shutdown()
{
	menuOpen = false;
	toggleSAMPCursor(0);
	toggleChat(0);
	close = 1;
}

SAMPFramework *pSAMP;
CD3DHook *pD3DHook;

void Theme()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = ImGui::GetStyle().Colors;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void RenderGUI()
{
	static float f = 0.0f;
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_MenuBar;
	ImGuiWindowFlags window_flags_about = 0;
	window_flags_about |= ImGuiWindowFlags_NoResize;
	window_flags_about |= ImGuiWindowFlags_NoCollapse;
	window_flags_about |= ImGuiWindowFlags_NoMove;
	ImGui::SetNextWindowSize(ImVec2(400.f, 500.f));
	if (!ImGui::Begin("MENU", p_open = NULL, window_flags))
	{
		ImGui::End();
		return;
	}
	if (ImGui::BeginMenuBar())
	{
		ImGui::SetNextWindowSize(ImVec2(140.f, 60.f));
		if (ImGui::BeginMenu("Panic"))
		{
			if (ImGui::Button("Panic", ImVec2(125.f, 20.f)))
			{
				ImGui::OpenPopup("Panic");
			}
			if (ImGui::BeginPopupModal("Panic", p_open = NULL, window_flags_about))
			{
				ImGui::SetWindowSize(ImVec2(293.f, 80.f));
				ImGui::Text("Are you sure about this?");
				if (ImGui::Button("Yes", ImVec2(135.f, 20.f)))
				{
					Shutdown();
				}
				ImGui::SameLine();
				if (ImGui::Button("No", ImVec2(135.f, 20.f)))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings"))
		{
			const char* items[] = { "Insert", "Home", "Delete","End" };
			ImGui::PushItemWidth(125.f);
			ImGui::Combo("##2000", &startmenu, items, 4);
			if (startmenu == 0)
			{
				key = VK_INSERT;
			}
			if (startmenu == 1)
			{
				key = VK_HOME;
			}
			if (startmenu == 2)
			{
				key = VK_DELETE;
			}
			if (startmenu == 3)
			{
				key = VK_END;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Credits"))
		{
			if (ImGui::Button("Credits", ImVec2(125.f, 20.f)))
			{
				ImGui::OpenPopup("Credits");
			}
			ImGui::SetWindowSize(ImVec2(293.f, 80.f));
			if (ImGui::BeginPopupModal("Credits", p_open = NULL, window_flags_about))
			{
				ImGui::Text("DIP Hook By GAM & Stickey");
				ImGui::Separator();
				ImGui::Text("Garret");
				ImGui::Text("CentiuS");
				if (ImGui::Button("Close"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (ImGui::CollapsingHeader("Example"))
	{
		ImGui::Checkbox("Chams", &Chams);
		ImGui::Checkbox("WireFrame", &WireFrame);
		ImGui::Checkbox("Skin Color", &SkinWall);
		ImGui::BulletText("Actor Infront Wall");
		ImGui::SameLine(255.f);
		ImGui::PushItemWidth(65.0f);
		const char* chamscolor[] = { "Red", "Green", "Blue" };
		ImGui::Combo("##1000", &ChamsVisible, chamscolor, 3);
		ImGui::BulletText("Actor Behind Wall");
		ImGui::SameLine(255.f);
		ImGui::PushItemWidth(65.0f);
		const char* chamscolor2[] = { "Red", "Green", "Blue" };
		ImGui::Combo("##2000", &ChamsInvisible, chamscolor2, 3);
	}
	ImGui::End();
}


LRESULT CALLBACK hWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplDX9_WndProcHandler(hwnd, uMsg, wParam, lParam) && GetKeyState(key) == 1 && menuOpen && wndproc)
	{
		return 1l;
	}

	return CallWindowProc(oriWndProc, hwnd, uMsg, wParam, lParam);
}


HRESULT __stdcall Hooked_Reset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS *pPresentParams)
{

	if (g_bwasInitialized)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		g_bwasInitialized = false;
	}
	if (!pDevice)
		return pD3DHook->Orginal_Reset(pDevice, pPresentParams);

	if (pD3DHook->bD3DRenderInit == true)
	{
		pD3DHook->pD3DFont->Invalidate();
		pD3DHook->pRender->Invalidate();

		pD3DHook->bD3DRenderInit = false;
	}

	HRESULT ret = pD3DHook->Orginal_Reset(pDevice, pPresentParams);

	if (ret == D3D_OK)
	{
		pD3DHook->pRender->Initialize(pDevice);
		pD3DHook->pD3DFont->Initialize(pDevice);
		pD3DHook->pPresentParam = *pPresentParams;

		pD3DHook->bD3DRenderInit = true;
	}

	return ret;
}

HRESULT GenerateTexture(IDirect3DDevice9* pDevice, IDirect3DTexture9 **ppD3Dtex, DWORD colour32)
{
	if (FAILED(pDevice->CreateTexture(8, 8, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, ppD3Dtex, NULL)))
		return E_FAIL;

	WORD colour16 = ((WORD)((colour32 >> 28) & 0xF) << 12)
		| (WORD)(((colour32 >> 20) & 0xF) << 8)
		| (WORD)(((colour32 >> 12) & 0xF) << 4)
		| (WORD)(((colour32 >> 4) & 0xF) << 0);

	D3DLOCKED_RECT d3dlr;
	(*ppD3Dtex)->LockRect(0, &d3dlr, 0, 0);
	WORD *pDst16 = (WORD*)d3dlr.pBits;

	for (int xy = 0; xy < 8 * 8; xy++)
		*pDst16++ = colour16;

	(*ppD3Dtex)->UnlockRect(0);

	return S_OK;
}

HRESULT GenerateShader(IDirect3DDevice9 *pDevice, IDirect3DPixelShader9 **pShader, float alpha, float red, float green, float blue)
{
	char		szShader[256];
	ID3DXBuffer *pShaderBuffer = NULL;
	sprintf_s(szShader, sizeof(szShader), "ps.1.1\ndef c0, %f, %f, %f, %f\nmov r0,c0", red, green, blue, alpha);
	if (FAILED(D3DXAssembleShader(szShader, sizeof(szShader), NULL, NULL, 0, &pShaderBuffer, NULL)))
	{
		return E_FAIL;
	}

	if (FAILED(pDevice->CreatePixelShader((const DWORD *)pShaderBuffer->GetBufferPointer(), pShader)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT __stdcall Hooked_Present(IDirect3DDevice9 *pDevice, CONST RECT *pSrcRect, CONST RECT *pDestRect, HWND hDestWindow, CONST RGNDATA *pDirtyRegion)
{
	if (ShaderTrick == false) { //Anti Lag
		GenerateShader(pDevice, &Chams_Green, 0.8f, 0, 1.0f, 0);
		GenerateShader(pDevice, &Chams_Blue, 0.8f, 0, 0, 1.0f);
		GenerateShader(pDevice, &Chams_Red, 0.8f, 1.0f, 0, 0);
		ShaderTrick = true;
	}

	if (!g_bwasInitialized)
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		io.IniFilename = NULL;
		io.DeltaTime = 1.0f / 60.0f;
		ImFont* pFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\framd.ttf", 15);
		SetCursor(io.MouseDrawCursor ? NULL : LoadCursor(NULL, IDC_ARROW));
		D3DDEVICE_CREATION_PARAMETERS d3dcp;
		pDevice->GetCreationParameters(&d3dcp);
		hWnd = d3dcp.hFocusWindow;
		io.Fonts->AddFontDefault();
		style.AntiAliasedLines = false;
		style.AntiAliasedFill = false;
		style.WindowBorderSize = 0.0f;
		if (hwndd == 0)
		{
			oriWndProc = (WNDPROC)SetWindowLongPtr(d3dcp.hFocusWindow, GWL_WNDPROC, (LONG)(LONG_PTR)hWndProc);
			hwndd++;
		}
		ImGui_ImplDX9_Init(d3dcp.hFocusWindow, pDevice);
		g_bwasInitialized = true;
	}
	if (item == 0)
	{
		Theme();
	}
	if (startmenu == 0)
	{
		key = VK_INSERT;
	}
	if (startmenu == 1)
	{
		key = VK_HOME;
	}
	if (startmenu == 2)
	{
		key = VK_DELETE;
	}
	if (startmenu == 3)
	{
		key = VK_END;
	}
	ImGui_ImplDX9_NewFrame();
	static int once = 2;
	if (GetAsyncKeyState(key) & 1)
	{
		menuOpen = !menuOpen;
		once = 0;
	}
	if (menuOpen)
	{
		toggleSAMPCursor(1);
		RenderGUI();
	}
	else
	{
		if (once == 0)
		{
			toggleSAMPCursor(0);
			once++;
		}
	}
	ImGui::Render();

	if (!pDevice)
		return pD3DHook->Orginal_Present(pDevice, pSrcRect, pDestRect, hDestWindow, pDirtyRegion);

	if (pD3DHook->bD3DRenderInit == false)
	{
		pD3DHook->pRender->Initialize(pDevice);
		pD3DHook->pD3DFont->Initialize(pDevice);

		pD3DHook->bD3DRenderInit = true;
	}

	if (!pSAMP->isInited)
		pD3DHook->pD3DFont->Print(1, 1, D3DCOLOR_ARGB(255, rand() % 255, rand() % 255, rand() % 255), "", true);

	return pD3DHook->Orginal_Present(pDevice, pSrcRect, pDestRect, hDestWindow, pDirtyRegion);
}

HRESULT __stdcall Hooked_DrawIndexedPrimitive(IDirect3DDevice9 *pDevice, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	DWORD dwRet_addr = (DWORD)_ReturnAddress();

	//Chams furate din s0b
	if (dwRet_addr == 0x761142)
	{
		if (Chams)
		{
			pDevice->SetRenderState(D3DRS_ZENABLE, false);
			pDevice->SetRenderState(D3DRS_PATCHEDGESTYLE, D3DPATCHEDGE_CONTINUOUS);
			if (WireFrame) pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			else pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

			if (!SkinWall) {
				if (ChamsInvisible == 0) {
					pDevice->SetPixelShader(Chams_Red);
				}
				else if (ChamsInvisible == 1) {
					pDevice->SetPixelShader(Chams_Green);
				}
				else if (ChamsInvisible == 2) {
					pDevice->SetPixelShader(Chams_Blue);
				}
			}
			pDevice->DrawIndexedPrimitive(PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
			if (!SkinWall) {
				if (ChamsVisible == 0) {
					pDevice->SetPixelShader(Chams_Red);
				}
				else if (ChamsVisible == 1) {
					pDevice->SetPixelShader(Chams_Green);
				}
				else if (ChamsVisible == 2) {
					pDevice->SetPixelShader(Chams_Blue);
				}
			}
			pDevice->SetRenderState(D3DRS_ZENABLE, true);
			pDevice->DrawIndexedPrimitive(PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
			pDevice->SetPixelShader(NULL);
			return pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}

	}
	return pD3DHook->Orginal_DrawIndexedPrimitive(pDevice, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

void mainThread(void *pvParams)
{
	while (!pSAMP->tryInit())
		Sleep(100);

	while (g_dwSAMP_Addr == NULL)
	{
		g_dwSAMP_Addr = (DWORD)LoadLibraryA("samp.dll");
		Sleep(250);
	}

	while (!g_Chat)
	{
		g_Chat = *(DWORD**)(g_dwSAMP_Addr + SAMP_CHAT_INFO_OFFSET);
		sleep(25);
	}

	everything();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		pSAMP = new SAMPFramework(GetModuleHandle("samp.dll"));
		_beginthread(mainThread, NULL, NULL);
		pD3DHook = new CD3DHook();
		break;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}