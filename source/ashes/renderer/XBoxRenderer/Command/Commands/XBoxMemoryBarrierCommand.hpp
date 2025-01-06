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
			, ArrayView< VkBufferMemoryBarrier const > const & bufferBarriers );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		struct BufferLock
		{
			VkDeviceSize offset;
			VkDeviceSize size;
			VkBuffer buffer;
		};
		using BufferLockArray = std::vector< BufferLock >;
		BufferLockArray m_uploadBuffers;
		BufferLockArray m_downloadBuffers;
	};
}
