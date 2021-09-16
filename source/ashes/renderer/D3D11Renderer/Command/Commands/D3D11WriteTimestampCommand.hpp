/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	/**
	*\brief
	*	Commande d'�criture de timestamp.
	*/
	class WriteTimestampCommand
		: public CommandBase
	{
	public:
		WriteTimestampCommand( VkDevice device
			, VkPipelineStageFlagBits pipelineStage
			, VkQueryPool pool
			, uint32_t query );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		ID3D11Query * m_query;
	};
}
