/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/VkLibPrerequisites.hpp>

namespace renderer
{
	/**
	*\brief
	*	Encapsulation d'un VkBufferMemoryBarrier.
	*/
	class BufferMemoryBarrier
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] resources
		*	Les ressources de rendu.
		*/
		BufferMemoryBarrier();
		/**
		*\return
		*	Le barrière mémoire vulkan.
		*/
		VkBufferMemoryBarrier const & getBarrier()const
		{
			return m_barrier;
		}

	private:
		VkBufferMemoryBarrier m_barrier;
	};
}
