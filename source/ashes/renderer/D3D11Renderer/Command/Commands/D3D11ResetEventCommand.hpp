/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	/**
	*\brief
	*	Commande de d�marrage d'une requ�te.
	*/
	class ResetEventCommand
		: public CommandBase
	{
	public:
		ResetEventCommand( VkDevice device
			, ashes::Event const & event
			, VkPipelineStageFlags stageFlags );
		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		ashes::Event const & m_event;
	};
}
