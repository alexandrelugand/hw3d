#pragma once

//Error exception helper macro
#define CHWND_EXCEPT( hr ) Windowing::Window::HrException( __LINE__,__FILE__,(hr) )
#define CHWND_LAST_EXCEPT() Windowing::Window::HrException( __LINE__,__FILE__,GetLastError() )
#define CHWND_NOGFX_EXCEPT() Windowing::Window::NoGfxException( __LINE__,__FILE__ )
