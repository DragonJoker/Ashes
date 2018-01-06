/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___VkRenderer_VertexLayout_HPP___
#define ___VkRenderer_VertexLayout_HPP___
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <VkLib/VertexLayout.hpp>

#include <Renderer/VertexLayout.hpp>

#include <vector>

namespace vk_renderer
{
	/**
	*\brief
	*	Classe template wrappant un vk::VertexBuffer.
	*/
	class VertexLayout
		: public renderer::VertexLayout
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] bindingSlot
		*	Le point d'attache du tampon associé.
		*\param[in] stride
		*	La taille en octets séparant un élément du suivant, dans le tampon.
		*/
		VertexLayout( uint32_t bindingSlot
			, uint32_t stride );
		/**
		*\brief
		*	Crée un attribut de sommet.
		*\param[in] location
		*	La position de l'attribut dans le shader.
		*\param[in] offset
		*	La position de l'attribut dans le tampon.
		*/
		renderer::AttributeBase createAttribute( uint32_t location
			, renderer::AttributeFormat format
			, uint32_t offset )override;
		/**
		*\return
		*	Le layout vulkan.
		*/
		inline vk::VertexLayoutPtr getLayout()const
		{
			return m_layout;
		}

	private:
		vk::VertexLayoutPtr m_layout;
	};
}

#endif
