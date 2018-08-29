/**
*\file
*	Renderer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <Core/Renderer.hpp>

#include <atomic>
#include <mutex>
#include <thread>

namespace gl_renderer
{
	class ContextSelector
	{
	public:
		void registerContext( Context const & context )const;
		void unregisterContext( Context const & context )const;
		void enableContextForCurrentThread()const;

	private:
		Context const * getCurrentContext()const;
		void setCurrentContext( std::thread::id id )const;
		Context const * getContext( std::thread::id id )const;

	private:
		mutable std::atomic< Context const * > m_currentContext{ nullptr };
		mutable std::mutex m_contextsMtx;
		mutable std::vector< Context const * > m_contexts;
	};
}
