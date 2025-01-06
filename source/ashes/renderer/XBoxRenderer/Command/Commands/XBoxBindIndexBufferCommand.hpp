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
	*	Classe de base d'une commande.
	*/
	class BindIndexBufferCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*/
		BindIndexBufferCommand( VkDevice device
			, VkBuffer ibo
			, uint64_t offset
			, VkIndexType indexType );

		void apply( Context const & context )const override;
		void remove( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		VkBuffer m_ibo;
		UINT m_offset;
		DXGI_FORMAT m_indexType;
	};
}
