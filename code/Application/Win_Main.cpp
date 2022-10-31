// SIliconKadai1.cpp : アプリケーションのエントリ ポイントを定義します。

#include "Win_Framework.h"
#include "Win_Main.h"

#include <stdio.h>
#include <Windows.h>
#include <timeapi.h>
#include <windowsx.h>

#include "../Game/Game_Main.h"

#pragma comment (lib,"winmm.lib")

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE   gInstance;                      // 現在のインターフェイス
WCHAR       gTitle[MAX_LOADSTRING];         // タイトル バーのテキスト
WCHAR       gWindowClass[MAX_LOADSTRING];   // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE , int);
LRESULT CALLBACK    WndProc(HWND , UINT , WPARAM , LPARAM);
INT_PTR CALLBACK    About(HWND , UINT , WPARAM , LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance ,
    _In_opt_ HINSTANCE hPrevInstance ,
    _In_ LPWSTR    lpCmdLine ,
    _In_ int       nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // グローバル文字列を初期化する
    LoadStringW(hInstance , IDS_APP_TITLE , gTitle , MAX_LOADSTRING);
    LoadStringW(hInstance , IDC_SILICONKADAI1 , gWindowClass , MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance(hInstance , nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance , MAKEINTRESOURCE(IDC_SILICONKADAI1));

    MSG msg;

    uint64_t currentTime = 0;
    uint64_t lastTime = 0;

    // ゲームループ
    while (1) {
        if (PeekMessage(&msg , nullptr , 0 , 0 , PM_REMOVE)) { // if
            // 終了メッセージ処理
            if (msg.message == WM_QUIT) { // if
                break;
            }
            else { // else
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        unsigned long start = timeGetTime();

        GameMain::GetInstance().Update();
        GameMain::GetInstance().Draw();

        unsigned long last = timeGetTime();
        unsigned long sleeptime = 16 - (int)( last - start );

        // フレームレート固定 
        Sleep(( 1000 / FLAME_LATE ) - (int)( last - start ));

    }

    GameMain::GetInstance().Release();

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance , MAKEINTRESOURCE(IDI_SILICONKADAI1));
    wcex.hCursor = LoadCursor(nullptr , IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SILICONKADAI1);
    wcex.lpszClassName = gWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance , MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance , int nCmdShow) {
    gInstance = hInstance; // グローバル変数にインスタンス ハンドルを格納する

    HWND hWnd = CreateWindowW(gWindowClass , gTitle , WS_OVERLAPPEDWINDOW ,
        CW_USEDEFAULT , 0 , CW_USEDEFAULT , 0 , nullptr , nullptr , hInstance , nullptr);

    if (!hWnd) {
        return FALSE;
    }


    ShowWindow(hWnd , nCmdShow);
    UpdateWindow(hWnd);

    GameMain::GetInstance().Initialize(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd , UINT message , WPARAM wParam , LPARAM lParam) {
    switch (message) {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 選択されたメニューの解析:
        switch (wmId) {
        case IDM_ABOUT:
            DialogBox(gInstance , MAKEINTRESOURCE(IDD_ABOUTBOX) , hWnd , About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd , message , wParam , lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd , &ps);
        // TODO: HDC を使用する描画コードをここに追加してください...
        EndPaint(hWnd , &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd , message , wParam , lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg , UINT message , WPARAM wParam , LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg , LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
