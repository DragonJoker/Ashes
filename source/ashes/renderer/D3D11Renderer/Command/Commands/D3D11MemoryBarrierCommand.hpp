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
			, ArrayView< VkMemoryBarrier const > const & memoryBarriers
			, ArrayView< VkBufferMemoryBarrier const > const & bufferBarriers
			, ArrayView< VkImageMemoryBarrier const > const & imageBarriers );

		void apply( Context const & context )const;
		CommandPtr clone()const;

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
