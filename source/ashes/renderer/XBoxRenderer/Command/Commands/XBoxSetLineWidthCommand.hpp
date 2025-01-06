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
	*	Commande d'application d'un scissor.
	*/
	class SetLineWidthCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] scissor
		*	Le scissor.
		*/
		SetLineWidthCommand( VkDevice device
			, float width );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		float m_width;
	};
}
