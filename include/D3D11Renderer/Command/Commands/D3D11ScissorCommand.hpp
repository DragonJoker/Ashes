/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

#include <Ashes/Pipeline/Scissor.hpp>

namespace d3d11_renderer
{
	/**
	*\brief
	*	Commande d'application d'un scissor.
	*/
	class ScissorCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] scissor
		*	Le scissor.
		*/
		ScissorCommand( Device const & device
			, uint32_t first
			, ashes::VkScissorArray const & scissors );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		std::vector< RECT > m_scissors;
	};
}
