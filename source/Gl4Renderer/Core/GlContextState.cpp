#include "Gl4Renderer/Core/GlContextState.hpp"

namespace ashes::gl4
{
	ContextState::ContextState( ContextState const & rhs )
		: cbStateAttachments{ rhs.cbStateAttachments }
		, cbState{ rhs.cbState }
		, dsState{ rhs.dsState }
		, sampleMask{ rhs.sampleMask }
		, msState{ rhs.msState }
		, tsState{ rhs.tsState }
		, iaState{ rhs.iaState }
		, viewports{ rhs.viewports }
		, scissors{ rhs.scissors }
		, vpState{ rhs.vpState }
		, rsState{ rhs.rsState }
		, dynamicStates{ rhs.dynamicStates }
		, dyState{ rhs.dyState }
	{
		doInit();
	}

	ContextState::ContextState( ContextState && rhs )
		: cbStateAttachments{ std::move( rhs.cbStateAttachments ) }
		, cbState{ std::move( rhs.cbState ) }
		, dsState{ std::move( rhs.dsState ) }
		, sampleMask{ std::move( rhs.sampleMask ) }
		, msState{ std::move( rhs.msState ) }
		, tsState{ std::move( rhs.tsState ) }
		, iaState{ std::move( rhs.iaState ) }
		, viewports{ std::move( rhs.viewports ) }
		, scissors{ std::move( rhs.scissors ) }
		, vpState{ std::move( rhs.vpState ) }
		, rsState{ std::move( rhs.rsState ) }
		, dynamicStates{ std::move( rhs.dynamicStates ) }
		, dyState{ std::move( rhs.dyState ) }
	{
		doInit();
	}

	ContextState::ContextState( VkPipelineColorBlendStateCreateInfo cbState
		, VkPipelineDepthStencilStateCreateInfo const * dsState
		, VkPipelineMultisampleStateCreateInfo const * msState
		, VkPipelineTessellationStateCreateInfo const * tsState
		, VkPipelineInputAssemblyStateCreateInfo const * iaState
		, VkPipelineViewportStateCreateInfo const * vpState
		, VkPipelineRasterizationStateCreateInfo const * rsState
		, VkPipelineDynamicStateCreateInfo const * dyState )
		: cbStateAttachments{ makeVector( cbState.pAttachments
			, cbState.attachmentCount ) }
		, cbState{ cbState }
		, dsState{ ( dsState
			? *dsState
			: getDeactivatedDepthStencilState() ) }
		, msState{ ( msState
			? *msState
			: getDeactivatedMultisampleState() ) }
		, tsState{ ( tsState
			? *tsState
			: getDeactivatedTessellationState() ) }
		, iaState{ ( iaState
			? *iaState
			: getDeactivatedInputAssemblyState() ) }
		, viewports{ ( vpState && vpState->pViewports
			? makeVector( vpState->pViewports, vpState->viewportCount )
			: VkViewportArray{} ) }
		, scissors{ ( vpState && vpState->pScissors
			? makeVector( vpState->pScissors, vpState->scissorCount )
			: VkScissorArray{} ) }
		, vpState{ ( vpState
			? *vpState
			: getDeactivatedViewportState() ) }
		, rsState{ ( rsState
			? *rsState
			: getDeactivatedRasterizationState() ) }
		, dynamicStates{ ( dyState
			? makeVector( dyState->pDynamicStates, dyState->dynamicStateCount )
			: VkDynamicStateArray{} ) }
		, dyState{ ( dyState
			? *dyState
			: getDeactivatedDynamicState() ) }
	{
		doInit();
	}

	ContextState::ContextState()
		: cbStateAttachments{ 1u, getColourBlendStateAttachment() }
		, cbState{ getDefaultColorBlendState( cbStateAttachments ) }
		, dsState{ getDefaultDepthStencilState() }
		, msState{ getDefaultMultisampleState() }
		, rsState{ getDefaultRasterisationState() }
		, tsState{ getDefaultTessellationState() }
		, iaState{ getDefaultInputAssemblyState() }
		, viewports{}
		, scissors{}
		, vpState{ getDefaultViewportState() }
		, dynamicStates{}
		, dyState{ getDefaultDynamicState() }
	{
		doInit();
	}

	void ContextState::doInit()
	{
		cbState.pAttachments = cbStateAttachments.data();
		msState.pSampleMask = bool( sampleMask )
			? &sampleMask.value()
			: nullptr;
		vpState.pScissors = scissors.data();
		vpState.pViewports = viewports.data();
		dyState.pDynamicStates = dynamicStates.data();
	}
}