/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace gl_renderer
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
		BindGeometryBuffersCommand( Device const & device
			, GeometryBuffers const & vao );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		GeometryBuffers const & m_vao;
	};
}
