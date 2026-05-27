// Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Client.h"

// 1. CEngine.h와 같은 파일들을 수정 시 마다 복사해줘야 한다.
// 이를 자동화하기 위한 방법으로 복사하는 매크로를 bat 파일로 작성
// 윈도우 커맨드 명령어가 담긴 bat 파일을 빌드때마다 실행시켜줄 수 있도록
// Engine 프로젝트의 속성 - 빌드 이벤트의 명령줄에 해당 파일을 실행 작성
// 2. Client는 Engine 라이브러리에 종속된다.
// Engine 라이브러리를 Client에서 참조하기 때문이다.
// 따라서 프로젝트 빌드 종속성을 설정해줘야 빌드할 때 제대로 동작한다. (여기서는 Client -> Engine 종속)
// Engine 라이브러리 빌드 -> 라이브러리 링크를 위한 빌드 후 이벤트(헤더 복사 등) -> Client에서 라이브러리 호출
#include <Engine/global.h>
#include <Engine/CEngine.h>
#include <Engine/CDevice.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CPathMgr.h>

#include "TestLevel.h"
#include "CImGuiMgr.h"
#include "CEditorMgr.h"
#include "imgui/imgui.h"
#include "CLevelSaveLoad.h"


// comptr 구현
template<typename T>
class MyPtr
{
private:
    T* m_Ptr;
    
public:
    T* Get() { return m_Ptr; }
    T** GetAdressOf() { return &m_Ptr; }

public:
    void operator = (T* _Int)
    {
        m_Ptr = _Int;
        m_Ptr->AddRef();
    }

public:
    MyPtr()
        : m_Ptr(nullptr)
    {

    }
    ~MyPtr()
    {
        if (nullptr != m_Ptr)
            m_Ptr->Release();
    }
};



// 전역 변수:
HINSTANCE g_hInst = nullptr;    // 현재 인스턴스입니다.
HWND g_hWnd = nullptr;          

long long Pow(long long _ll, long long _Pow);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// 템플릿
// 1. 클래스 템플릿
// 2. 함수 템플릿
// 3. 변수 템플릿 new!
template <typename T>
int g_Int = 0;

// 템플릿 특수화
// 특정 자료형에만 다르게 동작할 수 있는 템플릿
template <typename T>
float g_Float = 0.f;
template <>
float g_Float<int> = 3.14f;

// 변수 템플릿 + 부분 특수화 + constexpr
// constexpr는 컴파일 시점에 값 확정됨
template <typename T1, typename T2>
constexpr bool IsSame = false;
template <typename T>
constexpr bool IsSame<T, T> = true;



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtMemDumpAllObjectsSince(0);

    // 메모리 누수 체크 중단점 설정
    //_CrtSetBreakAlloc(6761); // 실제 문제 발생 번호로 수정 필요
#endif


    {
        // const
        // 변수를 상수 취급한다.
        // a 의 값의 변경을 문법으로 막아줄 뿐, 실제 a의 초기값은 runtime 중에 결정됨
        // 진짜 상수는 아님, 변수값으로 초기화 받을 수 있기 때문
        int b = 100;
        const int a = 0;

        // const expression
        // 컴파일 시간 안에 값이 확정되는 상수
        // 변수값으로 초기화하면 컴파일 시간 안에 값을 알 수 없기 때문에 오류
        // 진짜 상수는 컴파일에서 값이 결정됨
        int c = 100;
        constexpr int d = 100/*c 에러*/;


        // 변수 템플릿 사용
        // 둘다 다른 별 개의 int 변수
        g_Int<int> = 100;
        g_Int<float> = 20;


        // 템플릿 특수화 사용
        float f = 0.f;
        f = g_Float<int>;
        f = g_Float<float>;

        // 템플릿 특수화와 constexpr 사용
        // 같은 표현, 컴파일 시점에 아래와 같이 변경됨
        IsSame<int, float>;
        false;
    }




    g_hInst = hInstance;

    // 1. 윈도우 클래스 등록
    WNDCLASSEXW wcex;
    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.lpszClassName  = L"Key";
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassExW(&wcex);

    // 2. 윈도우 생성
    g_hWnd = CreateWindowW(L"Key", L"DirectX", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!g_hWnd)
        return FALSE;

    // 3. 윈도우 출력
    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

   /* FILE* pFile;
    freopen_s(&pFile, "CONOUT$", "w", stdout);
    freopen_s(&pFile, "CONIN$", "r", stdin);*/

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));
    MSG msg = {};

    // Engine 초기화
    if (FAILED(CEngine::GetInst()->Init(g_hWnd, 1280, 768
        , &CLevelSaveLoad::SaveGameObject, &CLevelSaveLoad::LoadGameObject)))
    {
        return 0;
    }


    


    // 테스트용 레벨 생성
    //CImGuiMgr::GetInst()->LoadAssets();

    
    CImGuiMgr::GetInst()->Init();

    TestLevel::CreateTestLevel();

    CEditorMgr::GetInst()->Init();


    // 기본 메시지 루프입니다:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            // Engine 초기화
            CEngine::GetInst()->Progress();

            CEditorMgr::GetInst()->Progress();

            CImGuiMgr::GetInst()->Progress();

            CDevice::GetInst()->Present();
        }
    }

    SetCurrentDirectory(CPathMgr::GetInst()->GetBinPath().c_str());



    //HMODULE dxgidebugdll = GetModuleHandleW(L"dxgidebug.dll");
    //decltype(&DXGIGetDebugInterface) GetDebugInterface = reinterpret_cast<decltype(&DXGIGetDebugInterface)>(GetProcAddress(dxgidebugdll, "DXGIGetDebugInterface"));

    //IDXGIDebug* debug;

    //GetDebugInterface(IID_PPV_ARGS(&debug));

    //OutputDebugStringW(L"================================================MEMORY LEAK====================================================\r\n");
    //debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_DETAIL);
    //OutputDebugStringW(L"================================================MEMORY LEAK====================================================\r\n");

    //debug->Release();


    return (int)msg.wParam;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Alt 단독 메뉴 모드(SC_KEYMENU) 선 차단
    if (message == WM_SYSCOMMAND && ((wParam & 0xFFF0) == SC_KEYMENU))
        return 0;

    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_KEYDOWN:
    {
        CKeyMgr::GetInst()->UpdateKeyState(KEY::ANY);
    }
        break;
    case WM_MOUSEWHEEL:
    {
        // wheel delta 값 얻기 (120 또는 -120)
        int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        if (zDelta > 0)
        {
            CKeyMgr::GetInst()->UpdateKeyState(KEY::WHEEL_UP);
        }
        else if (zDelta < 0)
        {
            CKeyMgr::GetInst()->UpdateKeyState(KEY::WHEEL_DOWN);
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
