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
	*	Classe de base d'une commande.
	*/
	class BindGeometryBuffersCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*/
		BindGeometryBuffersCommand( VkDevice device
			, GeometryBuffers const & vao );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		GeometryBuffers const & m_vao;
	};
}
