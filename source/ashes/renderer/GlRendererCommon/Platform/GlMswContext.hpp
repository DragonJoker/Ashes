/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#if _WIN32
#	include "renderer/GlRendererCommon/GlContext.hpp"

#	ifndef NOMINMAX
#		define NOMINMAX
#	endif
#	include <Windows.h>
#	include <gl/GL.h>

namespace ashes::gl
{
	class MswContext
		: public ContextImpl
	{
	public:
		MswContext( VkInstance instance
			, VkWin32SurfaceCreateInfoKHR createInfo
			, ContextImpl const * mainContext );
		~MswContext();

		void preInitialise( int major, int minor )override;
		void postInitialise()override;
		void enable()const override;
		void disable()const override;
		void swapBuffers()const override;

	private:
		bool doSelectFormat();
		bool doCreateModernContext();

	private:
		VkWin32SurfaceCreateInfoKHR createInfo;
		int m_minor{ 0 };
		int m_major{ 0 };
		HDC m_hDC{ nullptr };
		HGLRC m_hContext{ nullptr };
		MswContext const * m_mainContext;
	};
}

#endif
