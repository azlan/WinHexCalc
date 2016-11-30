#include <windows.h>
#include <commctrl.h >
#include "hexcalc.h"
#include "helper.h"
#include "test.h"

#if _DEBUG
#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:WinMainCRTStartup ")
#endif

#define IDC_MAIN_INPUT	101
#define IDC_MAIN_OUTPUT	102
#define IDC_ALWAYSTOP_CHECKBOX 103
#pragma comment( lib, "comctl32.lib") 
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

const char g_szClassName[] = "WinHexCalc";

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hOutput;
	static HWND hInput;
	static HWND hAlwaysTop;
	static HBRUSH hBrush;

	char buffer[256];
	char outputString[256];
	char binaryOutput[256];
	char binaryOutput2[256];
	Evaluate eval;

	switch (msg)
	{
		case WM_CREATE:
		{
			HFONT hfDefault;

			hOutput = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
				0, 0, 0, 0, hwnd, (HMENU)IDC_MAIN_OUTPUT, GetModuleHandle(NULL), NULL);
			if (hOutput == NULL)
				MessageBox(hwnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);

			hInput = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
				WS_CHILD | WS_VISIBLE,
				0, 0, 0, 0, hwnd, (HMENU)IDC_MAIN_INPUT, GetModuleHandle(NULL), NULL);
			if (hOutput == NULL)
				MessageBox(hwnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);

			hAlwaysTop = CreateWindowEx(0, "BUTTON", "Always on Top",
				WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
				0, 0, 0, 0,
				hwnd, (HMENU)IDC_ALWAYSTOP_CHECKBOX, GetModuleHandle(NULL), NULL);
			if (hAlwaysTop == NULL)
				MessageBox(hwnd, "Could not create check box.", "Error", MB_OK | MB_ICONERROR);

			hfDefault = CreateFont(-17, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Courier New");
			//hfDefault=CreateFont(-17,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE,"Consolas");

			hBrush = CreateSolidBrush(RGB(255, 255, 255));

			//hfDefault = GetStockObject(DEFAULT_GUI_FONT);
			SendMessage(hInput, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
			SendMessage(hOutput, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
			
			// set default focus on text input control
			SetFocus(hInput);
			
			break;
		}
		case WM_SIZE:
		{
			// x, y = 640, 480

			RECT rcClient;
			GetClientRect(hwnd, &rcClient);
			SetWindowPos(hOutput, NULL, 10, 50, rcClient.right - 20, rcClient.bottom - 100, SWP_NOZORDER);
			SetWindowPos(hInput, NULL, 10, 10, rcClient.right - 20, 30, SWP_NOZORDER);
			SetWindowPos(hAlwaysTop, NULL, rcClient.right - 140, rcClient.bottom - 40, 160, 30, SWP_NOZORDER);
			break;
		}
		case WM_COMMAND:
		{
			if (lParam == (LPARAM)hInput &&  HIWORD(wParam) == EN_CHANGE)
			{
				GetWindowText(hInput, buffer, sizeof(buffer));
				int64_t result = eval.Eval(buffer);
				printToBinary((result >> 32), binaryOutput);
				printToBinary((result & 0xffffffff), binaryOutput2);
				int64_t strBuffer[2] = {};
				*(strBuffer) = result;
				sprintf(outputString,
					"\r\n Signed  : %I64d"
					"\r\n"
					"\r\n Unsigned: %I64u"
					"\r\n"
					"\r\n Hex     : 0x%016llX"
					"\r\n"
					"\r\n Binary  : %s"
					"\r\n           %s"
					"\r\n"
					"\r\n String  : %s", result, result, result, binaryOutput, binaryOutput2, (char *)strBuffer);
				SetWindowText(hOutput, outputString);

			}
			else if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == IDC_ALWAYSTOP_CHECKBOX)
			{

				RECT rect;
				GetWindowRect(hwnd, &rect);

				LRESULT chk = SendMessage((HWND)lParam, BM_GETCHECK, 0, 0);

				SetWindowPos(hwnd,
					chk == BST_CHECKED ? HWND_TOPMOST : HWND_NOTOPMOST,
					rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
					SWP_SHOWWINDOW);
			}
			break;
		}
		case WM_CTLCOLORSTATIC:
		{
			HDC hdc = (HDC)wParam;
			HWND hWnd = (HWND)lParam;
			if (hWnd == hAlwaysTop)
			{
				SetBkMode(hdc, TRANSPARENT);
				SetTextColor(hdc, RGB(0, 0, 0));
				return (LRESULT)hBrush;
			}
			break;
		}
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{

	// run unit test
#ifdef _DEBUG
	testHexCalc();
#endif

	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;


	//Step 1: Registering the Window Class
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	InitCommonControls();
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Step 2: Creating the Window
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"WinHexCalc v0.2 - https://www.github.com/azlan/WinHexCalc",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 630, 400,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// Step 3: The Message Loop
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
