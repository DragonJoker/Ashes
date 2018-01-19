/*
See LICENSE file in root folder
*/
#ifndef ___GL_CONTEXT_H___
#define ___GL_CONTEXT_H___

#include "GlRendererPrerequisites.hpp"

#include "Core/GlConnection.hpp"

namespace gl_renderer
{
	class Context
	{
	public:
		Context( renderer::ConnectionPtr && connection );
		virtual ~Context() = default;
		/**
		*\brief
		*	Active le contexte.
		*/
		virtual void setCurrent()const = 0;
		/**
		*\brief
		*	Désactive le contexte.
		*/
		virtual void endCurrent()const = 0;
		/**
		*\brief
		*	Echange les tampons.
		*/
		virtual void swapBuffers()const = 0;
		/**
		*\brief
		*	Crée un contexte.
		*/
		static ContextPtr create( renderer::ConnectionPtr && connection );

	protected:
		renderer::ConnectionPtr m_connection;
		std::string m_vendor;
		std::string m_renderer;
		std::string m_version;
		int m_major;
		int m_minor;
	};
}

#endif
