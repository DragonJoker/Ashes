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
	/**
	*\brief
	*	Construit un layout, ayant pour stride la taille du type template donné.
	*\param[in] bindingSlot
	*	Le point d'attache du tampon associé.
	*\return
	*	Le layout.
	*/
	template< typename T >
	inline VertexLayoutPtr makeLayout( uint32_t bindingSlot )
	{
		return std::make_unique< VertexLayout >( bindingSlot
			, uint32_t( sizeof( T ) ) );
	}
}

#include "VertexLayout.inl"

#endif
