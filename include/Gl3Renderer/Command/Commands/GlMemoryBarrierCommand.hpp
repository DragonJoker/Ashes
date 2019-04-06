/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl3Renderer/Command/Commands/GlCommandBase.hpp"

namespace gl_renderer
{
	/**
	*\brief
	*	Classe de base d'une commande.
	*/
	class MemoryBarrierCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*/
		MemoryBarrierCommand( Device const & device
			, ashes::PipelineStageFlags after
			, ashes::PipelineStageFlags before
			, ashes::DependencyFlags dependencyFlags
			, ashes::MemoryBarrierArray const & memoryBarriers
			, ashes::BufferMemoryBarrierArray const & bufferMemoryBarriers
			, ashes::VkImageMemoryBarrierArray const & imageMemoryBarriers );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		GlMemoryBarrierFlags m_flags;
	};
}
