/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#if defined( __linux__ )

#include <EGL/egl.h>
#include <memory>

typedef uint32_t xcb_window_t;
struct wl_display;
struct wl_egl_window;

namespace ashes::gl
{
	class ContextEgl
	{
	public:
		ContextEgl( Display * display
			, uint64_t window
			, int reqMajor
			, int reqMinor
			, EGLContext shared );
		ContextEgl( wl_display * display
			, wl_egl_window * window
			, int reqMajor
			, int reqMinor
			, EGLContext shared );
		~ContextEgl();

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

	using ContextEglPtr = std::unique_ptr< ContextEgl >;
}

#endif
