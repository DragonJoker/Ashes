/*
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl3Renderer/GlRendererPrerequisites.hpp"

#include <renderer/GlRendererCommon/GlContextState.hpp>

#include <cassert>

namespace ashes::gl3
{
	class ContextStateStack
	{
	public:
		ContextStateStack( bool tessellation
			, bool viewportArrays );
		ContextStateStack( VkDevice device );

		void apply( ContextLock const & context
			, ContextState & state );
		void apply( CmdList & list
			, ContextState & state
			, bool force = false );
		void apply( CmdList & list
			, uint32_t firstViewport
			, VkViewportArray const & viewports
			, bool force );
		void apply( CmdList & list
			, uint32_t firstScissor
			, VkScissorArray const & scissors
			, bool force );
		void applyStencilCompareMask( CmdList & list
			, uint32_t compareMask
			, VkStencilFaceFlags faceFlags );
		void applyStencilWriteMask( CmdList & list
			, uint32_t compareMask
			, VkStencilFaceFlags faceFlags );
		void applyStencilReference( CmdList & list
			, uint32_t reference
			, VkStencilFaceFlags faceFlags );

		inline void setRenderArea( VkExtent2D const & value )
		{
			m_renderArea = value;
		}

		inline bool isPrimitiveRestartEnabled()const
		{
			assert( m_save != nullptr );
			return m_save->iaState.primitiveRestartEnable;
		}

		inline VkExtent2D const & getRenderArea()const
		{
			return m_renderArea;
		}

		inline VkScissorArray const & getCurrentScissors()const
		{
			return m_scissors;
		}

		inline void setCurrentScissors( VkScissorArray const & value )
		{
			m_scissors = value;
		}

		inline VkViewportArray const & getCurrentViewports()const
		{
			return m_viewports;
		}

		inline void setCurrentViewports( VkViewportArray const & value )
		{
			m_viewports = value;
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
		gl::ContextState * m_save{ nullptr };
		VkScissorArray m_scissors;
		VkViewportArray m_viewports;
		GLuint m_currentProgram{ 0u };
		VkFramebuffer m_currentFbo{ VK_NULL_HANDLE };
		bool m_tessellation{ false };
		bool m_viewportArrays{ false };
		VkExtent2D m_renderArea;
	};
}
