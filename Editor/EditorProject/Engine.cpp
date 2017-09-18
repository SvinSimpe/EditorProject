#include "Engine.h"
#include "GameTimer.h"
#include "Renderer.h"
#include "Resolution.h"

#include <string>
#include <sstream>


Engine* gEngine = 0;

LRESULT CALLBACK MainWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return gEngine->WndProc( hwnd, msg, wParam, lParam );
}

LRESULT CALLBACK Engine::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch ( msg )
	{
		case WM_PAINT :
		{ 
			hdc = BeginPaint( hWnd, &ps );
			EndPaint( hWnd, &ps );
			break;
		}
		case WM_DESTROY :
		{
			PostQuitMessage( 0 );
			break;
		}

		default:
			return DefWindowProc( hWnd, msg, wParam, lParam );
	}

	return 0;

}

int Engine::Run()
{
	GameTimer gameTimer;
	gameTimer.Start();

	MSG msg = { 0 };

	while ( WM_QUIT != msg.message )
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage ( &msg );
		}
		else
		{
			gameTimer.Tick();
			// Update Engine modules
			mRenderer->Update( gameTimer.DeltaTime() );
			mRenderer->Render();

			SetWindowTitleBar( gameTimer.DeltaTime() );
		}
	}
	
	return static_cast<int>( msg.wParam );
}

Engine::Engine()
{
	mHInstance = nullptr;
	mHWnd = nullptr;
	mRenderer = nullptr;
}

bool Engine::Initialize( HINSTANCE hInstance )
{
	mHInstance = hInstance;

	// Register Window Class
	WNDCLASS wc;
	//wc.cbSize			= sizeof( WNDCLASS );
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= MainWndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= mHInstance;
	wc.hIcon			= 0;
	wc.hCursor			= LoadCursor( NULL, IDC_HAND );
	wc.hbrBackground	= (HBRUSH)( COLOR_WINDOW + 1 );
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= L"Editor";
	//wc.hIconSm			= 0;

	if( !RegisterClass( &wc ) )
		return false;


	// Adjust & Create Window
	RECT rc = { 0, 0, Resolution::WIDTH, Resolution::HEIGHT };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	if( !( mHWnd = CreateWindow( L"Editor",
								 L"Editor",
								  WS_OVERLAPPEDWINDOW,
								  CW_USEDEFAULT,
								  CW_USEDEFAULT,
								  rc.right - rc.left,
								  rc.bottom - rc.top,
								  NULL,
								  NULL,
								  mHInstance,
								  NULL ) ) )
	{	
		return false;
	}

	ShowWindow( mHWnd, SW_SHOW );
	ShowCursor( TRUE );
	
	return InitializeModules();
}

Engine::~Engine()
{
	if( mRenderer )
		delete mRenderer;
}

bool Engine::InitializeModules()
{
	mRenderer = new Renderer( mHWnd );
	if( !mRenderer )
		return false;

	return true;
}

void Engine::SetWindowTitleBar( const float deltaTime )
{
	std::wstringstream wss;
	std::wstring ws;

	wss.clear();

	wss << "Editor" << "             DeltaTime: " << deltaTime ;
	ws = wss.str();

	SetWindowTextW( mHWnd, ws.c_str() );
	ws.clear();
	wss.str(ws);
}
