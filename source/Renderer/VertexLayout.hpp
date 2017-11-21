/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___Renderer_VertexLayout_HPP___
#define ___Renderer_VertexLayout_HPP___
#pragma once

#include "Attribute.hpp"

#include <VkLib/VertexLayout.hpp>

#include <vector>

namespace renderer
{
	/**
	*\brief
	*	Classe template wrappant un vk::VertexBuffer.
	*/
	class VertexLayout
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] bindingSlot
		*	Le point d'attache du tampon associé.
		*/
		VertexLayout( uint32_t bindingSlot );
		/**
		*\brief
		*	Crée un attribut de sommet.
		*\param[in] location
		*	La position de l'attribut dans le shader.
		*\param[in] offset
		*	La position de l'attribut dans le tampon.
		*/
		template< typename AttributeType >
		inline Attribute< AttributeType > createAttribute( uint32_t location
			, uint32_t offset );
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

#include "VertexLayout.inl"

#endif
