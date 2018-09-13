/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11CommandBase.hpp"

namespace d3d11_renderer
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
		BindIndexBufferCommand( Device const & device
			, Buffer const & ibo
			, uint64_t offset
			, ashes::IndexType indexType );

		void apply( Context const & context )const override;
		void remove( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		Buffer const & m_ibo;
		UINT m_offset;
		DXGI_FORMAT m_indexType;
	};
}
