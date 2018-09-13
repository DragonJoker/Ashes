/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11RendererPrerequisites.hpp"

#include <Command/CommandPool.hpp>

namespace d3d11_renderer
{
	/**
	*\brief
	*	Encapsulation d'un TestCommandPool.
	*\~english
	*\brief
	*	TestCommandPool wrapper.
	*/
	class CommandPool
		: public ashes::CommandPool
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le device parent.
		*\param[in] queueFamilyIndex
		*	L'index de la famille à laquelle appartient le pool.
		*\param[in] flags
		*	Combinaison binaire de ashes::CommandPoolCreateFlag.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical connection to the GPU.
		*\param[in] queueFamilyIndex
		*	The pool's owner family index.
		*\param[in] flags
		*	ashes::CommandPoolCreateFlag bitwise OR combination.
		*/
		CommandPool( Device const & device
			, uint32_t queueFamilyIndex
			, ashes::CommandPoolCreateFlags flags = 0 );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~CommandPool();
		/**
		*\brief
		*	Crée un tampon de commandes.
		*\param[in] primary
		*	Dit si le tampon est un tampon de commandes primaire (\p true) ou secondaire (\p false).
		*\return
		*	Le tampon de commandes créé.
		*\~english
		*\brief
		*	Creates a command buffer.
		*\param[in] primary
		*	Tells if the command buffer is primary (\p true), or not (\p false).
		*\return
		*	The created command buffer.
		*/
		ashes::CommandBufferPtr createCommandBuffer( bool primary )const override;

	private:
		Device const & m_device;
	};
}
