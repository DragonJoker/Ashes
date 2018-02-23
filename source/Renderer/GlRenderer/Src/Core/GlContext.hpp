/*
See LICENSE file in root folder
*/
#ifndef ___GL_CONTEXT_H___
#define ___GL_CONTEXT_H___

#include "GlRendererPrerequisites.hpp"

#include "Core/GlConnection.hpp"

#include <Miscellaneous/PhysicalDeviceInfo.hpp>

namespace gl_renderer
{
	class Context
	{
	protected:
		Context( renderer::ConnectionPtr && connection );

	public:
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

		inline renderer::PhysicalDeviceInfo const & getInfo()const
		{
			return m_info;
		}

		inline uint32_t getGlslVersion()const
		{
			return m_glslVersion;
		}

	protected:
		void doInitialiseBaseInfo();
		void doInitialiseInfo();

	private:
		bool doFindExtension( std::string const & name );

	protected:
		renderer::ConnectionPtr m_connection;
		renderer::PhysicalDeviceInfo m_info;
		renderer::StringArray m_extensions;
		int m_major{ 0 };
		int m_minor{ 0 };
		uint32_t m_glslVersion{ 0u };
	};
}

#endif
