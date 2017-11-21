/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/VertexLayout.hpp>

namespace renderer
{
	/**
	*\brief
	*	Classe encapsulant le concept d'attribut de sommet.
	*/
	template< typename T >
	class Attribute
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] layout
		*	Le layout de sommets associé.
		*\param[in] location
		*	La position de l'attribut dans le shader.
		*\param[in] offset
		*	La position de l'attribut dans le tampon.
		*/
		Attribute( vk::VertexLayout & layout
			, uint32_t location
			, uint32_t offset );

	private:
		vk::VertexAttribute const & m_attribute;
	};
}

#include "Attribute.inl"
