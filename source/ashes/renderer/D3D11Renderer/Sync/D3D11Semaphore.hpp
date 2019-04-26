/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	/**
	*\~french
	*\brief
	*	Classe encapsulant un TestSemaphore.
	*\remarks
	*	Un sémaphore est un élément de synchronisation servant pour les files.
	*\~english
	*\brief
	*	TestSemaphore wrapper.
	*\remarks
	*	Semaphores are used to synchronise queues.
	*/
	class Semaphore
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le LogicalDevice parent.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical connection to the GPU.
		*/
		explicit Semaphore( VkDevice device );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~Semaphore();
	};
}
