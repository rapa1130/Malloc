//
//! \file   main.cpp 
//! \brief  애플리케이션 메인 / 윈도 프레임워크 소스
//!			Yena S/W Renderer v2
//!
//! \author	김기홍 Kihong Kim / onlysonim@gmail.com 
//! \date   2003.11.04. Updated. DX9.x 
//! \date   2010.12.01. Updated. DX11, Jun.2010
//! \date   2016.12.27. Updated. DX11/12, Window SDK 8.1 / Window 10 SDK 10.0.18362
//! \date   2018.12.30. Updated. DX12.x, Windows 10 SDK 10.0.18362
//! \date   2020.08.22. Updated. DX12.x, Windows 10 SDK 10.0.19041 
//! \date   2024.12.10. Updated. DX12.x, Windows 10 SDK 10.0.22621 (VS22)
//

#include "windows.h"
#include "tchar.h"
#include "Render.h"

//////////////////////////////////////////////////////////////////////////////
//
// 전역 데이터들.
//

HWND g_hWnd = NULL; // 윈도우의 핸들. 창을 구분하는 ID같은 값
//핸들 : 윈도우 운영체제가 파일, 메모리 블록, 버튼, 창 같은 리소스 식별
//제어하기 위해 부여하는 고유한 정수값 (식별자)

/* 
HDC : 그리기 대상인 디바이스 컨텍스트(DC)의 핸들
HFONT : 폰트 객체 핸들
HANDLE : 파일 프로세스 등 일반적인 커널 객체 핸들
*/

BOOL g_bLoop = TRUE;
// 메인 루프를 게속 돌릴지 말지 정하는 플래그 (FALSE시 종료)

HINSTANCE g_hInstance = NULL;
// 현재 실행중인 프로그램 인스턴스 핸들.
// WinAPI 윈도우 클래스 등록 등에 사용

TCHAR* g_ClassName = _T("Yena");
// 윈도우 클래스 이름 :g_ClassName _T("Yena")는 유니코드/멀티바이트
// 설정에 맞춰 문자열 타입을 맞춰주는 매크로이다

extern TCHAR* g_WindowName;
// 현재 cpp파일에서 직접 만든 변수가 아니고 다른 cpp 파일 어딘가에
// 이미 정의된 전역 변수를 여기서 사용하겠다는 선언 
// 만약 extern이 있다면 똑같은 TCHAR* g_WindowName을 선언하면 안된다!
// 지역변수로 extern불가, 다른cpp에서 똑같은 이름 시 링커 에러

// bool 안쓰고 BOOL인 이유 -> 예전 관례 (사용자 정의 타입은 대문자)
// modern C++ -> 그냥 bool씁니다

//윈도 설정 관련 함수들 (그저 선언)
BOOL InitWindow(int width, int height);
// 윈도우 생성/초기화 함수 
// 내부에서 보통 윈도우 클래스 등록, CreateWindow호출, ShowWindow호출
void ResizeWindow(HWND hwnd, UINT NewWidth, UINT NewHeight);
// 창 핸들 , 가로,세로 크기 -> 윈도우의 창 크기를 변경하는 함수
BOOL MessagePump();
// Windows 메시지 처리 함수
/* 
키보드 입력, 마우스 이동, 창 닫기, 창 이동 < 전부 메시지로 처리
게임 루프에서 사용되며 while문안에서 계속해서 돈다 
이게 있어야 응답이 되고 클릭이되고 종료가되고 창이 움직인다
*/
void MoveToCenter(HWND hwnd, int width, int height);
// 창을 화면 중앙으로 이동시키는 함수 (핸들,좌우상하 크기)
LRESULT CALLBACK MsgProc(HWND, UINT, WPARAM, LPARAM);
/*
MsgProc : Windows가 자동으로 호출하는 함수
Windows : 이벤트 기반 (키 누름, 마우스 클릭, 창 닫기 등)
이런 이벤트 발생시 OS : MsgProc을 호출
HWND-> 어떤창에서 메시지가 발생했는가 ?
UINT -> 무슨 메시지인지 
WPARAM / LPARAM -> 추가 데이터(키보드 입력이면 wParam == VK_ESCAPE)같은 정보 들어온다.


CALLBACK -> win api 호출 규약 매크로
LRESULT 메시지 처리 결과 반환 타입 return 0; 이나 
return DefWindowProc(...) 같은 호출


*/

/////////////////////////////////////////////////////////////////////////////
//
//! 애플리케이션 메인 함수.

int APIENTRY WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR IpCmdLine,
	int nCmdShow)
	// 윈도우가 프로그램 시작할 때, 자동으로 호출하는 함수
	//WinMain -> main() 같은 역할입니다
	// 현재 실행 중인 프로그램의 인스턴스 핸들 HINSTANCE hInstance
{	// 인스턴스 : 현재 실행중인 프로그램 자체를 가르키는 ID
	// 실행파일, 리소스, 아이콘, 윈도우 클래스 관리 -> hInstance
	
	// HINSTANCE hPrevInstance -> 16비트 windows 호환용 흔족
	// 지금은 NULL로 들어온다

	// LPSTR IpCmdLine -> 명령줄 문자열
	// IpCmndLine = 'stage1 debug' 들어온다 
	// LPSTR 은 char*과 비슷한 의미이다
	// modern 느낌으로 char* IpCmdLine정도로 생각해라
	// 런처나 툴 만들때 -editor, -debug, -map test01같은 실행옵션 전달

	// int nCmdShow -> 창을 어떻게 보여줄지에 대해서
	// 최대화,최소화, 기본크기, 숨김등 같은 상태 정보

	// APIENTRY -> 호출 규약 매크로 __stdcall과 비슷한것
	// 호출 규약 ? 매개변수 어떻게 전달할지 스택 정리 누가할지 정하는 규칙


	//기본 윈도우 생성
	if (!InitWindow(960, 600))return 0;
	// 960,600 크기의 창을 생성해라
	// 만약(InitWindow == false)(윈도우창없으면) 리턴
	// 있다면 넘어가라 -> 창 생성 실패했는가 확인
	// CreateWindow , ShowWindow같은 Api 코드 있다

	// Back-Buffer : Render Target 생성!!!
	RenderTargetCreate(g_hWnd);
	// 렌더링용 백버퍼(RenderTarget) 생성
	// 메모리 버퍼에 먼저 그린다!


	//데이터 로딩.
	if (!DataLoading()) {
		g_bLoop = FALSE;
	} // 게임에 필요한 리소스 로딩 
	// 이미지,텍스처,맵,사운드,렌더버퍼등
	// 실패할 경우g_bLoop = FALSE < 메인 루프 종료 상태로 만들어라
	// return 안하는 이유? -> 정리 코드 실행 구조

	// g_bLoop 가 True인경우에는 계속해서 반복해서 실행해라
	while (g_bLoop) {
		if (!MessagePump()) // Windows 메시지 처리
			break; //키보드,마우스,창 닫기등
		SceneRender(); // 실제 화면 렌더링해라(백버퍼 클리어,픽셀
		// 라인그리기, 비트맵 출력 등)
	} // FALSE반환 시 -> 창 종료, WM_QUIT등 발생하면 FALSE-> break해라

	DataRelease(); // 정리해라 (메모리 해제, 비트맵 제거, 버퍼해제 등)
	// cpp는 직접 메모리 관리하니까 -> 안하면 메모리 누수
	RenderTargetRelease();
	// 백버퍼 관련 GDI 자원 해제

	return 0;

}



////////////////////////////////////////////////////////////////////////////////
//
//! 윈도 메시지 콜백 함수. 윈도 메세지를 처리합니다.
//
// Windows에서 발생하는 이벤트들을 처리하는 함수
// 키보드, 마우스, 창 이동,닫기 등 이벤트들을 처리한다
LRESULT CALLBACK MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	// MsgProc(직접호출보단 메시지 생기면 호출해주는 함수)
	// Type -> LRESULT는 메시지를 처리한 결과값
	// 직접 처리했으면 return 0; 안 한 메시지는 DefWindowProc(...)
	// CALLBACK -> 윈도우가 호출할 함수라는 표시

	//HWND hwnd -> 어떤 창에서 메시지가 발생했는지
	//UINT msg -> 무슨 메시지야? WMKEYDOWN DESTROY MOUSEMOVE SIZE등
	// WPARAM -> 여기서는 키보디 입력일 때 사용한다
	// LPARAM -> lParam 또 다른 추가 정보 (반복횟수, 스캔 코드등)
	switch (msg) { // 들어온 메시지에 따라 다르게 처리

	case WM_CREATE: // 창을 생성할 시 호출되는 메시지
		::SetCursor(LoadCursor(g_hInstance, IDC_ARROW));
		// 마우스 커서 로드 (LoadCursor) IDC_ARROW 기본화살표 커서
		// SetCursor(현재 마우스커서를 화살표모양(IDC_ARROW)으로 설정해라)
		// 현재 스레드/윈도우 에서 사용할 마우스 커서 모양 설정!
		break;
	case WM_LBUTTONDOWN: // 마우스 왼쪽 버튼 눌림 시
		{
			POINT pt; // 좌표 저장용 구조체 생성
			pt.x = LOWORD(lParam); // lParam안에 마우스 좌표 들어있다
			// lParam = [y좌표][x 좌표](16+16비트) Loword = 하위 16비트 = x좌표
			pt.y = HIWORD(lParam);
			// 상위 16비트 꺼내기 = y좌표 저장
			LineUpdate(pt); // 이전 단계에서 만든 함수 
			// 첫 시작이면 시작점, 두번째면 끝점 저장
		}
		break;

	case WM_MOUSEMOVE :  // 마우스가 움직일 때마다 실행한다
		{
			//POINT pt;
			g_VtxTemp.x = LOWORD(lParam); // lParam마우스좌표 하위 x
			g_VtxTemp.y = HIWORD(lParam); // 상위 y
			// LineUpdate(pt);
		}
		break;
		 
		case WM_RBUTTONDOWN : // 우클릭시 실행된다 (선 초기화)
		{
			LineClear();
		}
		break;

	case WM_KEYDOWN: // 키보드키가 눌렸다는 메시지이다
		switch (wParam) { // 무슨키가 눌렸는지 확인한다(wParam)
		case VK_ESCAPE: // ESC키 = VK_ESCAPE < 이다 키가 눌렸는데 ESC키일 경우
			SendMessage(hwnd, WM_DESTROY, 0, 0);
			// 메시지를 보낸다 (무슨창에, 어떤 메시지를, 추가값1,추가값2)
			// hwnd 창에게 WM_Destroy 메시지를 추가값 없이 보내라! 라는 뜻
			break; // 처리 끝 !
		}
		return 0; // WM_KEYDOWN 메시지를 처리했다( ESC든 아니든)

	case WM_DESTROY: // 창 파괴 케이스(ESC를 눌렀을 경우) -> MsgProc 호출
		// msg -> WM_DESTROY가 된다
		PostQuitMessage(0);
		// 프로그램 종료 메시지를 메시지 큐에 넣어라(WM_QUIT 메시지를 발생시켜라)
		// 메인 루프에서 MessagePump()가 확인하다 WM_QUIT를 보면 종료하기에
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
	// 기본 윈도우 처리 방식대로 처리해줘라라는 뜻
	// 이게 없으면 창이 정상적으로 동작하지 않을 수 있다

	//자연어로 -> Windows가 메시지를 보내면 Msgproc이 받고
	// 메시지가 키 눌림이면 : 어떤 키인지 확인
	// ESC인 경우 : WM_DESTROY 메시지를 보낸다(현재창에)
	// 키눌림 메시지는 처리 완료한다.

	// 메시지가 WM_Destroy : 프로그램 종료 메시지를 보낸다
	// 처리 완료한다

	// 그 외 메시지 -> Windows 기본 처리에 맡긴다
}

/////////////////////////////////////////////////////////////////////////////
//
//! 윈도 생성 함수.
//! 입력받은 크기에 맞추어 렌더링 영역을 생성하기 위해서 
//! 클라이언트 영역을 포함 윈도 전체 크기를 재조정합니다.
//! 
//! \param width	생성할 윈도의 가로 크기.
//! \param height	생성할 윈도의 세로 크기.
//! \return			성공시 OK, 실패시 FAIL.
//
BOOL InitWindow(int width, int height) {
	// InitWindow함수 -> int weight, height (만들고 싶은 창 크기)
	WNDCLASSEX wc = { // 창 설계도를 만드는 부분
		// WNDCLASSEX 는 윈도우에게 -> 이런 특징을 가진 창을 만들거야라는
		// 구조체 !
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0,0,
		// 구조체 크기 WinAPI가 이 구조체 버전/크기를 알기 위해 필요(WNDCLASSEX)
		// CS_CLASSDC -> 윈도우 클래스 스타일(이 창 클래스는 하나의 DC를 공유)
		// DC < Device Context가 화면 출력과 관련되기에 사용된다
		// MsgProc ->이 창이 받을 메시지를 처리할 함수 (키 입력마우스창닫기등)
		// 추가 클래스 메모리 추가 윈도우 메모리 (0,0) 필요없다!

		::GetModuleHandle(NULL), NULL, NULL,
		// 현재 실행중인 프로그램의 인스턴스 핸들을 가져온다
		// hInstance와 비슷한 값 NULL이란건 -> 현재 실행 파일의 모듈 핸들 가져와
		// 아이콘 커서 (Null,null) (따로 지정하지 않았다)

		(HBRUSH)GetStockObject(DKGRAY_BRUSH), NULL,
		// 창 배경색 브러시 (DKGRAY_BRUSH -> 어두운 회색 브러시) 창배경 회색으로
		//NULL -> 메뉴를 안쓰니까 
		
		g_ClassName, NULL
		// 윈도우 클래스 이름(TCHAR* g_ClassName = _T("Yena"); 그 이름
		// 설계도의 이름입니다 (작은 아이콘 NULL -> 지금 안씀)

	};
	RegisterClassEx(&wc);
	// 방금 만든 wc설계도를 Windows에 등록합니다 (이런 종류창 만들 수 있게 등록)
	// 나중에 CreateWindow -> g_ClassName으로 창만들수 있음

	// 실제 창을 만드는 부분 -> CreateWindow WinAPI함수< 반환값 HWND<
	HWND hWnd = ::CreateWindow(g_ClassName, g_WindowName,
		// g_ClassName 설계도로 창을 만든다, 그 뒤 이름은 창 제목이다(타이틀바)
								WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		//창 스타일 |은 여러 옵션을 합치라는 뜻
		// OverLapped = 기본 겹침 창 스타일/Caption - 제목 표시줄을 가지는 창
		// SysMenu = 시스템 메뉴를 가지는 창(닫기버튼과 관련)
		// WS_MinImizeBox = 최소화 버튼을 가지는 창

		// 제목 표시줄, 닫기 버튼, 최소화 버튼, 크기조절 안함 이라는 뜻
		
								//WS_OVERLAPPEDWINDOW,
								// 위에는 더 일반적인 기본 창 스타일
								//(제목줄, 시스템 메뉴, 최소최대화, 크기조절등)
								0, 0,
								// 창의 처음 위치
								//(ResizeWindow,MoveToCenter같은 함수로 다시바뀔 수 있음)

								width, height,
								// 처음 창 크기(클라이언트 영역X)
								// 전체 윈도우 크기로 들어간다(제목 표시줄,테두리, 클라이언트 영역등)
								GetDesktopWindow(), NULL,
								//부모 윈도우 데스트톱 윈도우를 부모처럼 넣는다
								// 일반 최상위 창은 이 자리에 NULL을 넣기도한다
								// 이창이 데스크톱 위에 만들어진다(NULL은 메뉴핸들)
								wc.hInstance, NULL
								// 이 창을 만든 프로그램 인스턴스
								// WNDCLASSEX에 넣었던 현재 프로그램 핸들
								// NULL은 추가 생성 데이터 (필요없어서 NULL)
								);
	
	if (hWnd == NULL) return FALSE;
	//CreateWindow가 실패하면 NULL 반환하면 FALSE반환 ->
	// if(!InitWindow(960,600)) return 0;실행
	// 프로그램 종료!!
	 
	
	// 윈도우 크기 조정(필수)
	ResizeWindow(hWnd, width, height);
	// CreateWindow의 width,height는 전체 창 크기 기준이라
	// 렌더링에서 원하는건 실제로 그림을 그릴 수 있는 영역
	// 근데 제목줄, 테두리 가 있으면 실제 내부 영역 작아지므로
	// ResizeWindow에서 전체 창 크기를 다시 계산한다

	::ShowWindow(hWnd, SW_SHOWDEFAULT);
	// 창을 화면에 표시합니다, 창을 만드는 CreateWindow
	// ShowWindow는 창을 보이게 만드는 것(DEFAULT는 기본 표시 방식으로 보여줘라)
	::UpdateWindow(hWnd);
	//창을 즉시 갱신하라고 요청한다
	//창을 보인 직후 한 번 호출해서, 필요한 그리기 메시지가 바로 처리되도록한다
	// 창 지금 한번 갱신 해라 ! 

	::SetCursor(LoadCursor(NULL, IDC_ARROW));
	// 마우스 커서를 기본 화살표로 설정한다
	// LoadCursor(는 기본 화살표 커서를 가져오는 코드)
	// SetCursor(는 현재 커서를 그 모양으로 바꾼다는 코드)

	g_hWnd = hWnd;
	// 전역 윈도우 핸들 저장
	// 방금 만든 창 핸들을 전역 변수에 저장한다
	// 다른 함수에서도 창을 사용할 수 있다

	// ex) SceneRender, ResizeWindow, MessagePump등
	return TRUE;
	// 여기까지 온 경우에는 창을 생성 성공했으므로 True를 반환한다
} // InitWindow - WndClassEx 구조체로 창 설계도 작성
//RegisterClassEx로 설계도 등록 / CreateWindows로 실제 창 생성
//실패시 - FALSE (종료)
// ReSizeWindow(성공시)로 클라이언트 영역 크기 맞추기
// ShowWindow로 창 표시/ UpdateWindow로 창 갱신
// SetCursor로 기본 커서 설정 / g_hWnd에 창 핸들 저장 -> True 반환
// 핵심 목적 : 렌더링할 클라이언트 영역 width * height가 되도록 WinApi창을 만들고 준비



/////////////////////////////////////////////////////////////////////////////
// 
//! 메세지 펌핑 함수.  \n
//! 메인 루프 매 프레임마다 호출 되어야 하며 종료 이벤트(ESC 키 눌림) 발생시 
//! 메인 루프를 빠져나와 전체 프로세스를 종료시킨다.
//! 
//! \return	애플리케이션의 현재 동작상태
// 

// ::는 전역 영역의 함수/변수를 사용하겠다 라는 뜻이다
// 여기서는 ::PeekMessage() < 전역 namespace에 있는 WinAPI함수를 써라라는 의미
int MessagePump() { // Windows 메시지를 처리하는 함수 
	// TRUE : 계속 실행, FALSE : 종료

	MSG msg; ::ZeroMemory(&msg, sizeof(msg));
	// Windows 메시지 구조체에 (키입력,마우스,창이동,종료메시지등) 정보 들어감
	// ZeroMemory (메모리를 0으로 전부 초기화한다)
	// msg 구조체 전체를 0으로 채워라 (쓰레기값 제거, 이전 메모리 값 제거)
	// modern C++에서는 ? MSG msg = {};로 같이 초기화 하기도 한다
	while (1) { // 무한 루프
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// PeekMessage = Windows 메시지 큐에서 메시지를 꺼내온다
			// 메시지큐란 ? 키 입력, 마우스 ,창이동등을 큐에 저장해서 하나씩 가져옴
			// &msg = 꺼낸 메시지 저장할 곳, NULL(모든 창 메시지 검사),
			// 0,0 < 메시지 범위 (0~0이면 전체 메시지)
			// PM_REMOVE : (중요) 메시지를 큐에서 꺼내고 제거하라
			// 즉 -> 메시지 확인 + 삭제 / 메시지 있으면TRUE 없으면 FALSE 

			// 프로그램 종료 메시지 검사
			if (msg.message == WM_QUIT) 
				// PostQuitMessage(0);가 생성 -> 닫았을 경우
				// WM_QUIT 생성 MessagePump 감지 -> 루프 종료
				return FALSE;
				// 프로그램 종료해라


			::TranslateMessage(&msg);
			// 키보드 메시지를 문자메시지로 변환(키보드 입력 해석 보조)
			::DispatchMessage(&msg);
			// (중요) -> MsgProc을 호출합니다
			// PeekMessage - 메시지 가져옴- DispatchMessage- MsgProc호출
		}
		else { // 중요 (처리할 메시지가 없다)
			return TRUE;
			// 지금은 OS 이벤트 없음, 렌더링 계속해라
			// 바로 return하는 이유 -> 게임 루프와 같이 계속 돌리기 때문
			// 이 구조 덕에 OS 메시지 처리 + 게임 렌더링 동시에 가능하다
		}

	}
	return FALSE; // 사실상 도달하지 않지만 안전 코드 느낌이다
} // MessagePump시작 - PeekMessage - 메시지 있는가 ? - if(NO) - return True
// or Yes -> WM_QUIT - Yes (Return False) or No -> TranslateMessage
// - DispatchMessageMsgProc호출 - 다시 While

// 윈도우 이벤트를 처리하면서 메시지가 없을 경우에는
// 게임 렌더링이 계속 돌게 만드는 함수이다 (WIN API 기반 게임 루프 핵심)



//////////////////////////////////////////////////////////////////////////////
//
//! 윈도 크기를 조정합니다. \n
//! 입력된 크기는 클라이언트 영역의 새 크기로 사용되며 윈도 크기도 함께 조정됩니다.
//! 
//! \param hWnd		조정할 윈도 핸들
//! \param width	변경할 새 가로 크기
//! \param height	변경할 새 세로 크기
//! \return			없음
//
//! \date 2004.05.28. 버전업.
//

void ResizeWindow(HWND hWnd, UINT width, UINT height) {
	// 렌더링할 실제 내부영역(Client Area)를 원하는 크기로 맞추기
	// 크기를 바꿀 창(핸들), UINT(Unsigned Int) 좌우상하 크기
	// -> 실제로 그림 그릴 내부 영역 크기
	// 제목 표시줄같은 부분은 제외하고 진짜 그림만을 그려주는게 클라이언트 영역
	//CreateWindow(...,960,600,...) - 은 실제 내부 960,600 아닐 수 잇음
	// 제목줄,테두리등이 포함 -> 내부 렌더링만 960,600하고 싶어 할때 이 함수

	//현재 윈도 정보 획득.
	RECT oldrc; ::GetWindowRect(hWnd, &oldrc);
	// 현재 윈도우 정보 얻기(현재 창 위치와 크기를 가져온다
	// Rect = left,top,right,bottom (Long type)
	// GetWindowRect -> 현재 창의 전체 영역 정보를 가져와라 
	// oldrc -> 현재 창 위치 저장 ( 나중에 크기 바꿀 때 기존 위치 유지)

	DWORD style = (DWORD) ::GetWindowLong(hWnd, GWL_STYLE);
	// 현재 스타일 가져오기 (style : caption, sysmenu등 창 옵션들)
	// DWORD -> Win32 API(부호없는 32비트 정수) Double Word
	// 초기 16비트 컴퓨팅 시절 Word (16비트) 두번이라 DWord(32비트)
	// 아무튼 그래서 제목줄있냐, 최소화버튼있냐 크기조절되냐등의 정보

	// GetWindowLong - 윈도우의 설정값 가져오기
	// GWL_STYLE : 일반 창 스타일 갖고온나 <가 된다

	DWORD exstyle = (DWORD) ::GetWindowLong(hWnd, GWL_EXSTYLE);
	// 추가 스타일 (항상 위 표시, 투명, 툴윈도우 같은 옵션)

	//새로 생성될 윈도우의 '클라이언트' 영역.
	RECT newrc = { 0,0, (LONG)width, (LONG)height };
	// (중요) 원하는 내부 렌더링 영역을 정한다
	// {0,0,960,600} => 왼쪽 위 (0,0) 부터 오른쪽 아래 (960,600)
	// RECT 구조체 -> 좌표 데이터의 묶음 왼쪽위, 왼쪽아래, 오른쪽위, 오른쪽아래
	// -> 실제창 더 커져야함 (클라이언트가 960,600이니까)
	// 아직 문제 있음 ㅇㅇ
	
	//newrc 만큼의 클라이언트 영역을 포함하는 윈도 '전체' 크기를 구합니다.
	//현재 메뉴는 없다는 가정하에 처리되고 있습니다.
	//계산된 결과는 newrc 에 다시 들어옵니다. (스크린 좌표)
	//exstyle &= ~WS_EX_TOPMOST;
	::AdjustWindowRectEx(&newrc, style, NULL, exstyle);
	// Client Area가 width x height가 되도록 전체 윈도우 크기를 계산해준다
	// client area 960 600 => 제목줄 테두리 크기 자동계산 => 전체 창 크기 보정


	//보정된 윈도우의 너비와 폭을 구합니다. 
	int newWidth = (newrc.right - newrc.left);
	int newHeight = (newrc.bottom - newrc.top);
	// right - left => 끝점 - 시작점 (폭 구하기) (976(960+테두리등) - 0(시작점)
	// height도 동일하게 bottom - top 


	// 새로운 크기를 윈도우에 설정합니다
	::SetWindowPos(hWnd, HWND_NOTOPMOST, oldrc.left, oldrc.top,
		newWidth, newHeight, SWP_SHOWWINDOW);
	// 계산된 크기들을 넣어 실제 창 크기를 변경합니다
	// hWnd (어떤 창 바꿀지), NOTOPMOST (항상 위(topmost) 창 아니다
	// oldrc.left,oldrc.top (기존 위치를 유치한다) 
	// ->현재 위치 그대로 크기만 변경할거다

	// newWidth , newHeight -> 보정된 전체 창 크기
	// SWP_SHOWWINDOW - 창 표시 옵션( 실제 내부링 영역이 정확히 widthxheight이
	// 되도록 창 전체 크기 조절을 한다)


	//화면 중앙으로 이동
	MoveToCenter(hWnd, newWidth, newHeight);
	// 계산된 이후 창을 화면 중앙으로 이동시킨다
	// 내부 : 모니터 크기 얻고, 중앙 좌표 계산 MoveWindow 같은거 한다
} // 원하는 클라 영역 입력 / 창 스타일 확인 / 제목,테두리 크기 계산
// 전체 윈도우 크기 보정 - SetWindowPos로 적용 - 화면 중앙 이동

// 픽셀 버퍼 = 960x600인데 클라이언트 영역이 정확히 960x600이 아니면
// 출력 비율 , 픽셀 위치, 렌더링 (전부 어긋날 수 있다)

// 위에 함수 목적은 -> '실제 렌더링 영역을 원하는 해상도로 정확히 맞추는 함수'

////////////////////////////////////////////////////////////////////////////////
//
//! MoveToCenter 윈도우를 화면 중앙으로 이동시킵니다.
//! 
//! \param	hwnd	이동할 윈도우 핸들
//! \param	width	윈도우 창 가로 크기
//! \param	height	윈도우 창 세로 크기
//! \return			없음
//

//창을 화면 중앙으로 이동시키는 함수!
void MoveToCenter(HWND hwnd, int width, int height) {
	// 매개변수 hwnd - 이동할 창의 핸들(어떤 창 움직일건지)
	// width,height 창 가로 크기 세로 크기!
	RECT rc = {}; // 구조체 생성 + 0으로 초기화
	::GetClientRect(GetDesktopWindow(), &rc);
	// GetDesktopWindow : 바탕화면 크기 가져오기 (중요)
	// 바탕화면(Desktop) 윈도우 핸들을 가져온다 (전체 모니터)

	//GetClientRect : 클라이언트 영역 크기 가져오기
	// 즉 -> 바탕화면 크기를 rc에 저장해라
	// 1920x1080 -> 이면 rc.left 0 top 0 / rc.right bottom 1920 1080이 된다

	rc.left = rc.right / 2 - width / 2;
	rc.top = rc.bottom / 2 - height / 2;
	// 화면 높이 = 1080 중앙 = 540
	// 창 높이 600 절반 300
	// 540 - 300 => 창 시작 240 
	// 
	
	// why ? -> 창을 왼쪽 위 좌표 기준이면 창이 왼쪽위가 중앙으로 간다
	// 그러면 창은 오른쪽 아래로 밀린다(그러므로 /2를해준다)


	::MoveWindow(hwnd, rc.left, rc.top, width, height, TRUE);
	// 실제로 창 이동 + 크기 변경
	// hwnd무슨창 rc새 x,y위치 ,창너비높이, 창 다시그리기 여부(TRUE)
	//::SetWindowPos(hwnd, HWND_NOTOPMOST, rc.left, rc.top, width, height, SWP_SHOWWINDOW);
	// 간단한 이동 크기 변경용  = MoveWindow
	// SetWindowPos = 더세밀하게 옵션 가능(맨 위 창 여부, z순서, 표시 옵션등)
} // 바탕화면 크기 얻기 - 화면 중앙 계산 - 창 크기 절반빼기 - 창 시작 위치 계산
// -> MoveWindow로 이동한다





/*************** end of "main.cpp" *****************************************/