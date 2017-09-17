
#include <crtdbg.h>
#include <ctime>
#include "Engine.h"


int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	#if defined(DEBUG) || defined(_DEBUG)

		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
		//_CrtSetBreakAlloc(13515); // Break at specific memory allocation point

	#endif

	// Seed RNG
	srand( static_cast<unsigned int>( time( NULL ) ) );


	Engine* gameEngine = new Engine();

	if( gameEngine->Initialize( hInstance ) )
		return 0;
	else
		return gameEngine->Run();
}