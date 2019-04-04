/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
{
	/**
	*\brief
	*	Commande de génération de mipmaps.
	*/
	class GenerateMipmapsCommand
		: public CommandBase
	{
	public:
		GenerateMipmapsCommand( VkDevice device
			, VkImage texture );
		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		GLuint m_texture;
		GLenum m_target;
	};
}
