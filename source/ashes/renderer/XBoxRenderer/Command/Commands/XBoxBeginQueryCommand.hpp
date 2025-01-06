/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/XBoxRenderer/Command/Commands/XBoxCommandBase.hpp"

namespace ashes::xbox
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
			, uint32_t query );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		ID3D11Query * m_query;
	};
}
