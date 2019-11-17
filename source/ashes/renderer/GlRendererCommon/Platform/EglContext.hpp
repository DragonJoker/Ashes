/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#ifndef _WIN32

#include <EGL/egl.h>
#include <memory>

namespace ashes::gl
{
	class EglContext
	{
	public:
		EglContext( EGLNativeDisplayType display
			, EGLNativeWindowType window
			, int reqMajor
			, int reqMinor
			, EGLContext shared );
		~EglContext();

		EGLBoolean enable()const;
		EGLBoolean disable()const;
		EGLBoolean swap()const;

		inline EGLContext getContext()const
		{
			return m_context;
		}

	private:
		void doCleanup();

	private:
		EGLDisplay m_display{ nullptr };
		EGLContext m_context{ nullptr };
		EGLSurface m_surface{ nullptr };
	};

	using EglContextPtr = std::unique_ptr< EglContext >;
}

#endif
