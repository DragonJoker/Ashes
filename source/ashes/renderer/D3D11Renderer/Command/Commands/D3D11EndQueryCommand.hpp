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
	*	Commande de fin d'une requ�te.
	*/
	class EndQueryCommand
		: public CommandBase
	{
	public:
		EndQueryCommand( VkDevice device
			, VkQueryPool pool
			, uint32_t query );
		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		ID3D11Query * m_query;
	};
}
