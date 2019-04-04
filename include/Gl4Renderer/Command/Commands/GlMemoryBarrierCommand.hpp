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
	class MemoryBarrierCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*/
		MemoryBarrierCommand( VkDevice device
			, VkPipelineStageFlags after
			, VkPipelineStageFlags before
			, VkDependencyFlags dependencyFlags
			, VkMemoryBarrierArray memoryBarriers
			, VkBufferMemoryBarrierArray bufferMemoryBarriers
			, VkImageMemoryBarrierArray imageMemoryBarriers );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		GlMemoryBarrierFlags m_flags;
	};
}
