/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_VertexLayout_HPP___
#define ___Renderer_VertexLayout_HPP___
#pragma once

#include "Attribute.hpp"

#include <vector>

namespace renderer
{
	/**
	*\brief
	*	Classe template wrappant un vk::VertexBuffer.
	*/
	class VertexLayout
	{
	protected:
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

	public:
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~VertexLayout() = default;
		/**
		*\brief
		*	Crée un attribut de sommet.
		*\param[in] location
		*	La position de l'attribut dans le shader.
		*\param[in] offset
		*	La position de l'attribut dans le tampon.
		*/
		virtual AttributeBase createAttribute( uint32_t location
			, AttributeFormat format
			, uint32_t offset ) = 0;
		/**
		*\brief
		*	Crée un attribut de sommet.
		*\param[in] location
		*	La position de l'attribut dans le shader.
		*\param[in] offset
		*	La position de l'attribut dans le tampon.
		*/
		template< typename T >
		inline AttributeBase createAttribute( uint32_t location
			, uint32_t offset )
		{
			return createAttribute( location
				, details::FormatGetter< T >::value
				, offset );
		}
	};
	/**
	*\brief
	*	Crée un layout de sommets.
	*\param[in] bindingSlot
	*	Le point d'attache du tampon associé.
	*\return
	*	Le layout créé.
	*/
	template< typename T >
	VertexLayoutPtr makeLayout( Device const & device
		, uint32_t bindingSlot )
	{
		return device.createVertexLayout( bindingSlot
			, uint32_t( sizeof( T ) ) );
	}
}

#endif
