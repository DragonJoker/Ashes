/*
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

#include <cassert>

namespace ashes::gl
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
			, PreExecuteActions & preExecuteActions
			, uint32_t firstViewport
			, ArrayView< VkViewport const > const & viewports
			, bool force );
		void apply( CmdList & list
			, PreExecuteActions & preExecuteActions
			, uint32_t firstScissor
			, ArrayView< VkRect2D const > const & scissors
			, bool force );
		void applySRGBStatus( CmdList & list
			, bool enable );
		void applyPackAlign( CmdList & list
			, int32_t align );
		void applyUnpackAlign( CmdList & list
			, int32_t align );
		void applyDisableBlend( CmdList & list );
		void applyDepthBias( CmdList & list
			, float constantFactor
			, float clamp
			, float slopeFactor );
		void applyStencilCompareMask( CmdList & list
			, uint32_t compareMask
			, VkStencilFaceFlags faceFlags );
		void applyStencilWriteMask( CmdList & list
			, uint32_t compareMask
			, VkStencilFaceFlags faceFlags );
		void applyStencilReference( CmdList & list
			, uint32_t reference
			, VkStencilFaceFlags faceFlags );
		void setCurrentFramebuffer( VkFramebuffer value );

		inline bool isPrimitiveRestartEnabled()const
		{
			assert( m_save != nullptr );
			return m_save->iaState.primitiveRestartEnable != 0;
		}

		inline VkExtent2D const & getRenderArea()const
		{
			return m_renderArea;
		}

		inline void setRenderArea( VkExtent2D const & value )
		{
			m_renderArea = value;
		}

		inline ArrayView< VkRect2D const > getCurrentScissors()const
		{
			return makeArrayView( m_scissors.data()
				, m_scissors.data() + m_scissors.size() );
		}

		inline void setCurrentScissors( ArrayView< VkRect2D const > const & value )
		{
			m_scissors.clear();
			m_scissors.insert( m_scissors.end()
				, value.begin()
				, value.end() );
		}

		inline ArrayView< VkViewport const > getCurrentViewports()const
		{
			return makeArrayView( m_viewports.data()
				, m_viewports.data() + m_viewports.size() );
		}

		inline void setCurrentViewports( ArrayView< VkViewport const > const & value )
		{
			m_viewports.clear();
			m_viewports.insert( m_viewports.end()
				, value.begin()
				, value.end() );
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
			return m_currentFbo != nullptr;
		}

		inline VkFramebuffer getCurrentFramebuffer()const
		{
			assert( hasCurrentFramebuffer() );
			return m_currentFbo;
		}

		inline bool isRtot()const
		{
			return m_isRtot;
		}

	private:
		void doApply( CmdList & list
			, VkPipelineInputAssemblyStateCreateInfo const & state
			, bool force );
		void doApply( CmdList & list
			, VkPipelineColorBlendStateCreateInfo const & newState
			, bool force );
		void doApply( CmdList & list
			, VkPipelineRasterizationStateCreateInfo const & newState
			, VkPipelineDynamicStateCreateInfo newDyState
			, bool force );
		void doApply( CmdList & list
			, VkPipelineMultisampleStateCreateInfo const & newState
			, bool force );
		void doApply( CmdList & list
			, VkPipelineDepthStencilStateCreateInfo const & newState
			, bool force );
		void doApply( CmdList & list
			, VkPipelineTessellationStateCreateInfo const & newState
			, bool force );
		void doApply( CmdList & list
			, PreExecuteActions & preExecuteActions
			, VkPipelineViewportStateCreateInfo const & newState
			, bool force );
		bool doCheckSave( ContextState * state = nullptr );

	private:
		std::unique_ptr< ContextState > m_ownInitial;
		ContextState * m_save{ nullptr };
		VkExtent2D m_renderArea{ ~( 0u ), ~( 0u ) };
		VkScissorArray m_scissors;
		VkViewportArray m_viewports;
		GLuint m_currentProgram{ 0u };
		VkFramebuffer m_currentFbo{};
		bool m_tessellation{ false };
		bool m_viewportArrays{ false };
		bool m_isRtot{ false };
	};
}
