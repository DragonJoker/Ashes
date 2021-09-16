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
		ScissorCommand( VkDevice device
			, uint32_t first
			, ArrayView< VkRect2D const > const & scissors );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		std::vector< RECT > m_scissors;
	};
}
