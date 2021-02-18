/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#ifndef ___AshGL_CGLView_H___
#define ___AshGL_CGLView_H___

#include <OpenGL/OpenGL.h>

struct GLView
{
	void * view;
	CGLContextObj context;
};

GLView makeGLView( const void * handle );
void freeGLView( GLView view );

#endif
