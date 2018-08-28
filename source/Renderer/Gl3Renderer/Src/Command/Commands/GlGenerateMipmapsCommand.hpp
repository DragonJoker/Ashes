/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "GlCommandBase.hpp"

namespace gl_renderer
{
	/**
	*\brief
	*	Classe de base d'une commande.
	*/
	class GenerateMipmapsCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*/
		GenerateMipmapsCommand( Device const & device
			, Texture const & texture );

		void apply()const override;
		CommandPtr clone()const override;

	private:
		Texture const & m_texture;
	};
}
