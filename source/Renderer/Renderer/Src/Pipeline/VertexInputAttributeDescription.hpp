/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_VertexInputAttributeDescription_HPP___
#define ___Renderer_VertexInputAttributeDescription_HPP___
#pragma once

#include "VertexInputAttributeSemantic.hpp"

namespace renderer
{
	/**
	*\~english
	*\brief
	*	Structure specifying vertex input attribute description.
	*\~french
	*\brief
	*	Structure définissant la description d'un attribut d'entrée de sommet.
	*/
	struct VertexInputAttributeDescription
	{
		/**
		*\~english
		*	The shader binding location number for this attribute.
		*\remarks
		*	Unused with D3D renderers, mandatory with others.
		*\~french
		*\brief
		*	Le numéro de la position de l'attribut dans le shader.
		*\remarks
		*	Inutilisé avec les renderers D3D, obligatoire avec les autres.
		*/
		uint32_t location;
		/**
		*\~english
		*\brief
		*	The binding number which this attribute takes its data from.
		*\~french
		*\brief
		*	Le numéro d'attache depuis laquelle cet attribut récupère ses données.
		*/
		uint32_t binding;
		/**
		*\~english
		*	The type and size of the attribute data.
		*\~french
		*\brief
		*	Le type et la taille des données de l'attribut.
		*/
		Format format;
		/**
		*\~english
		*\brief
		*	A byte offset of this attribute relative to the start of an element in the vertex input binding.
		*\~french
		*\brief
		*	Un offset en octets pour cet attribut, relatif au début d'un élément dans l'attache d'entrée de sommets.
		*/
		uint32_t offset;
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
		VertexInputAttributeSemantic semantic;
	};
}

#endif
