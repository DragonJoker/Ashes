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
	*	Commande de d�marrage d'une requ�te.
	*/
	class BeginQueryCommand
		: public CommandBase
	{
	public:
		BeginQueryCommand( VkDevice device
			, VkQueryPool pool
			, uint32_t query
			, VkQueryControlFlags flags );
		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		ID3D11Query * m_query;
	};
}
