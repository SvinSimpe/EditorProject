#pragma once

/*
	*Note: The d3dcompiler lib is only used by the Renderer
	so it should only be visible to it and no other object.
*/

#pragma comment( lib, "d3dcompiler.lib" ) 
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "dxgi.lib" )

#include <d3dcompiler.h>
#include <d3d11.h>