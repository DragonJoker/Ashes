/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_Attribute_HPP___
#define ___Ashes_Attribute_HPP___
#pragma once

#include "Ashes/AshesPrerequisites.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Class wrapping the concept of a vertex attribute.
	*\~french
	*\brief
	*	Classe encapsulant le concept d'attribut de sommet.
	*/
	class Attribute
	{
	public:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] layout
		*	The parent vertex layout.
		*\param[in] format
		*	The attribute format, in the shader.
		*\param[in] location
		*	The attribute location in the shader.
		*\param[in] offset
		*	The attribute location in the buffer.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] layout
		*	Le layout de sommets parent.
		*\param[in] format
		*	Le format de l'attribut dans le tampon.
		*\param[in] location
		*	La position de l'attribut dans le shader.
		*\param[in] offset
		*	La position de l'attribut dans le tampon.
		*/
		Attribute( VertexLayout const & layout
			, Format format
			, uint32_t location
			, uint32_t offset );
		/**
		*\~english
		*\return
		*	The parent vertex layout.
		*\~french
		*\return
		*	Le layout de sommets parent.
		*/
		VertexLayout const & layout;
		/**
		*\~english
		*\return
		*	The format of the attribute in the buffer.
		*\~french
		*\return
		*	Le format de l'attribut dans le tampon.
		*/
		Format const format;
		/**
		*\~english
		*\return
		*	The attribute location in the shader.
		*\~french
		*\return
		*	La position de l'attribut dans le shader.
		*/
		uint32_t const location;
		/**
		*\~english
		*\return
		*	The attribute location in the buffer.
		*\~french
		*\return
		*	La position de l'attribut dans le tampon.
		*/
		uint32_t const offset;
	};
}

#endif
