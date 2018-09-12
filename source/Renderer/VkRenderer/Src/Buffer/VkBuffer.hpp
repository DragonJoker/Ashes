/**
*\file
*	RenderingResources.h
*\author
*	Sylvain Doremus
*/
#ifndef ___VkRenderer_Buffer_HPP___
#define ___VkRenderer_Buffer_HPP___
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <Buffer/Buffer.hpp>

namespace vk_renderer
{
	/**
	*\~french
	*\brief
	*	Classe regroupant les ressources de rendu nécessaires au dessin d'une image.
	*/
	class Buffer
		: public renderer::BufferBase
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] count
		*	Le nombre d'éléments du tampon.
		*\param[in] target
		*	Les indicateurs d'utilisation du tampon.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical connection to the GPU.
		*\param[in] size
		*	The buffer data size.
		*\param[in] target
		*	The buffer usage flags.
		*/
		Buffer( Device const & device
			, uint32_t size
			, renderer::BufferTargets target );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~Buffer();
		/**
		*\copydoc	renderer::BufferBase::getMemoryRequirements
		*/
		renderer::MemoryRequirements getMemoryRequirements()const override;
		/**
		*\~french
		*\brief
		*	Opérateur de conversion implicite vers VkBuffer.
		*\~english
		*\brief
		*	VkBuffer implicit cast operator.
		*/
		inline operator VkBuffer const &( )const
		{
			return m_buffer;
		}

	private:
		void doBindMemory()override;

	private:
		Device const & m_device;
		VkBuffer m_buffer{ VK_NULL_HANDLE };
	};
}

#endif
