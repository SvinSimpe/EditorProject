#pragma once

#include <Windows.h>

class Engine
{
	public:
		LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		
		int Run();		
		bool Initialize( HINSTANCE hInstance );

		Engine();
		~Engine();


	private:
		bool InitializeModules();
	private:
		HINSTANCE		mHInstance;
		HWND			mHWnd;
		class Renderer* mRenderer;


};