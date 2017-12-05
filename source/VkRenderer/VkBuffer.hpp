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

#include <VkLib/Buffer.hpp>
#include <Renderer/Buffer.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Classe regroupant les ressources de rendu nécessaires au dessin d'une image.
	*/
	class BufferBase
		: public renderer::BufferBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] count
		*	Le nombre d'éléments du tampon.
		*\param[in] target
		*	Les indicateurs d'utilisation du tampon.
		*\param[in] flags
		*	Les indicateurs de mémoire du tampon.
		*/
		BufferBase( renderer::Device const & device
			, uint32_t size
			, renderer::BufferTargets target
			, renderer::MemoryPropertyFlags flags );
		/**
		*\return
		*	Le tampon.
		*/
		inline vk::Buffer const & getBuffer()const
		{
			return m_buffer;
		}

	private:
		vk::Buffer m_buffer;
	};
}

#endif
