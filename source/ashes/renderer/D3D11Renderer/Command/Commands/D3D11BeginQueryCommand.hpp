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
	class BeginQueryCommand
		: public CommandBase
	{
	public:
		BeginQueryCommand( VkDevice device
			, ashes::QueryPool const & pool
			, uint32_t query
			, ashes::QueryControlFlags flags );
		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		ID3D11Query * m_query;
	};
}
