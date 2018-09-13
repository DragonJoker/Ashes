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
	class BufferMemoryBarrierCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*/
		BufferMemoryBarrierCommand( Device const & device
			, renderer::PipelineStageFlags after
			, renderer::PipelineStageFlags before
			, renderer::BufferMemoryBarrier const & transitionBarrier );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		GlMemoryBarrierFlags m_flags;
	};
}