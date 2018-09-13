/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_VertexInputAttributeSemantic_HPP___
#define ___Ashes_VertexInputAttributeSemantic_HPP___
#pragma once

#include "AshesPrerequisites.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	The semantic used in the shader for a specific vertex input attribute.
	*\remarks
	*	Mandatory with D3D renderers, unused with others.
	*\~french
	*\brief
	*	La sémantique utilisée dans le shader pour un attribut de sommet.
	*\remarks
	*	Obligatoire avec les renderers D3D, inutilisé avec les autres.
	*/
	struct VertexInputAttributeSemantic
	{
		/**
		*\~english
		*	The attribute semantic name.
		*\~french
		*\brief
		*	Le nom sémantique de l'attribut.
		*/
		std::string name;
		/**
		*\~english
		*\brief
		*	The attribute semantic index.
		*\~french
		*\brief
		*	L'indice sémantique de l'attribut.
		*/
		uint32_t index;
	};
}

#endif
