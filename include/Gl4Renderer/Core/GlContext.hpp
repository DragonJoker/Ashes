/*
See LICENSE file in root folder
*/
#pragma once

#include "GlContextState.hpp"

#include <atomic>
#include <mutex>

namespace ashes::gl4
{
	extern char const VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME[VK_MAX_EXTENSION_NAME_SIZE];
	class ContextLock;

	class Context
	{
	public:
		class ContextImpl
		{
		protected:
			ContextImpl( VkInstance instance
				, VkSurfaceCreateInfoKHR createInfo )
				: createInfo{ std::move( createInfo ) }
				, instance{ instance }
			{
			}

		public:
			virtual void initialise( Context & parent ) = 0;
			virtual void loadSystemFunctions() = 0;
			/**
			*\brief
			*	Active le contexte.
			*/
			virtual void enable()const = 0;
			/**
			*\brief
			*	Désactive le contexte.
			*/
			virtual void disable()const = 0;
			/**
			*\brief
			*	Echange les tampons.
			*/
			virtual void swapBuffers()const = 0;

			VkSurfaceCreateInfoKHR createInfo;
			VkInstance instance;
		};

	private:
		Context( std::unique_ptr< ContextImpl > impl );

	public:
		~Context();

		void apply( ContextLock const & context
			, Device const & device
			, ContextState const & state );
		void onBaseContextCreated();

		static ContextPtr create( VkInstance instance
			, VkSurfaceCreateInfoKHR createInfo
			, Context const * mainContext );
		static ContextPtr create( VkInstance instance
			, VkSurfaceKHR surface
			, Context const * mainContext );

		inline void swapBuffers()const
		{
			m_impl->swapBuffers();
		}

		inline bool isEnabled()const
		{
			return m_enabled;
		}

		inline GLuint getCurrentFramebuffer()const
		{
			return m_fbo;
		}

		inline ContextImpl const & getImpl()const
		{
			return *m_impl;
		}

		inline VkRect2D const & getCurrentScissor()const
		{
			return m_scissor;
		}

		inline VkViewport const & getCurrentViewport()const
		{
			return m_viewport;
		}

		inline GLuint const & getCurrentProgram()const
		{
			return m_currentProgram;
		}

		inline void lock()
		{
			m_mutex.lock();
			m_enabled = true;
			m_impl->enable();
		}

		inline void unlock()
		{
			m_impl->disable();
			m_enabled = false;
			m_mutex.unlock();
		}

		inline void setCurrentFramebuffer( GLuint value )
		{
			m_fbo = value;
		}

		inline void setCurrentProgram( GLuint value )
		{
			m_currentProgram = value;
		}

		inline void setCurrentScissor( VkRect2D const & value )
		{
			m_scissor = value;
		}

		inline void setCurrentViewport( VkViewport const & value )
		{
			m_viewport = value;
		}

#define GL_LIB_BASE_FUNCTION( fun )\
		PFN_gl##fun m_gl##fun = nullptr;\
		template< typename ... Params >\
		inline auto gl##fun( Params... params )const\
		{\
			return m_gl##fun( params... );\
		}
#define GL_LIB_FUNCTION( fun )\
		PFN_gl##fun m_gl##fun = nullptr; \
		template< typename ... Params >\
		inline auto gl##fun( Params... params )const\
		{\
			return m_gl##fun( params... );\
		}
#define GL_LIB_FUNCTION_OPT( fun )\
		PFN_gl##fun m_gl##fun = nullptr; \
		template< typename ... Params >\
		inline auto gl##fun( Params... params )const\
		{\
			return m_gl##fun( params... );\
		}\
		inline bool has##fun()const\
		{\
			return bool( m_gl##fun );\
		}
#include "Gl4Renderer/Miscellaneous/OpenGLFunctionsList.inl"

		PFN_glObjectLabel glObjectLabel = nullptr;
		PFN_glObjectPtrLabel glObjectPtrLabel = nullptr;
		PFN_glDebugMessageCallback glDebugMessageCallback = nullptr;
		PFN_glDebugMessageCallbackAMD glDebugMessageCallbackAMD = nullptr;

	private:
		void loadBaseFunctions();
		void loadDebugFunctions();
		void apply( ContextLock const & context
			, VkPipelineInputAssemblyStateCreateInfo const & state );
		void apply( ContextLock const & context
			, VkPipelineColorBlendStateCreateInfo const & newState );
		void apply( ContextLock const & context
			, VkPipelineRasterizationStateCreateInfo const & newState
			, VkPipelineDynamicStateCreateInfo newDyState );
		void apply( ContextLock const & context
			, VkPipelineMultisampleStateCreateInfo const & newState );
		void apply( ContextLock const & context
			, VkPipelineDepthStencilStateCreateInfo const & newState );
		void apply( ContextLock const & context
			, VkPipelineTessellationStateCreateInfo const & newState );
		void apply( ContextLock const & context
			, VkPipelineViewportStateCreateInfo const & newState );

	private:
		std::unique_ptr< ContextImpl > m_impl;

	public:
		VkSurfaceCreateInfoKHR const & createInfo;

	protected:
		std::mutex m_mutex;
		ContextState m_state;
		GLuint m_fbo{ 0u };
		VkRect2D m_scissor{ 0, 0, 0, 0 };
		VkViewport m_viewport{ 0, 0, 0, 0 };
		GLuint m_currentProgram;
		std::atomic< bool > m_enabled{ false };
	};
}
