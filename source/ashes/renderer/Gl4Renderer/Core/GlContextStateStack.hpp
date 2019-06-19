/*
See LICENSE file in root folder
*/
#pragma once

#include "GlContextState.hpp"

#include <cassert>

namespace ashes::gl4
{
	class ContextStateStack
	{
	public:
		ContextStateStack( bool tessellation );
		ContextStateStack( VkDevice device );

		void apply( ContextLock const & context
			, ContextState & state );
		void apply( CmdList & list
			, ContextState & state
			, bool force = false );

		inline bool isPrimitiveRestartEnabled()const
		{
			assert( m_save != nullptr );
			return m_save->iaState.primitiveRestartEnable;
		}

		inline VkRect2D const & getCurrentScissor()const
		{
			return m_scissor;
		}

		inline void setCurrentScissor( VkRect2D const & value )
		{
			m_scissor = value;
		}

		inline VkViewport const & getCurrentViewport()const
		{
			return m_viewport;
		}

		inline void setCurrentViewport( VkViewport const & value )
		{
			m_viewport = value;
		}

		inline GLuint getCurrentProgram()const
		{
			return m_currentProgram;
		}

		inline void setCurrentProgram( GLuint value )
		{
			m_currentProgram = value;
		}

		inline bool hasCurrentFramebuffer()const
		{
			return m_currentFbo != VK_NULL_HANDLE;
		}

		inline VkFramebuffer getCurrentFramebuffer()const
		{
			assert( hasCurrentFramebuffer() );
			return m_currentFbo;
		}

		inline void setCurrentFramebuffer( VkFramebuffer value )
		{
			m_currentFbo = value;
		}

	private:
		void apply( CmdList & list
			, VkPipelineInputAssemblyStateCreateInfo const & state
			, bool force );
		void apply( CmdList & list
			, VkPipelineColorBlendStateCreateInfo const & newState
			, bool force );
		void apply( CmdList & list
			, VkPipelineRasterizationStateCreateInfo const & newState
			, VkPipelineDynamicStateCreateInfo newDyState
			, bool force );
		void apply( CmdList & list
			, VkPipelineMultisampleStateCreateInfo const & newState
			, bool force );
		void apply( CmdList & list
			, VkPipelineDepthStencilStateCreateInfo const & newState
			, bool force );
		void apply( CmdList & list
			, VkPipelineTessellationStateCreateInfo const & newState
			, bool force );
		void apply( CmdList & list
			, VkPipelineViewportStateCreateInfo const & newState
			, bool force );

	private:
		ContextState * m_save{ nullptr };
		VkRect2D m_scissor{ 0, 0, 0, 0 };
		VkViewport m_viewport{ 0, 0, 0, 0 };
		GLuint m_currentProgram{ 0u };
		VkFramebuffer m_currentFbo{ VK_NULL_HANDLE };
		bool m_tessellation{ false };
	};
}
