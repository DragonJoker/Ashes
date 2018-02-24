/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_SpecialisationMapEntry_HPP___
#define ___Renderer_SpecialisationMapEntry_HPP___
#pragma once

#include "RendererPrerequisites.hpp"

namespace renderer
{
	/**
	*\~english
	*\brief
	*	Specifies a push constant range.
	*\~french
	*\brief
	*	Définit un intervalle de push constants.
	*/
	struct SpecialisationMapEntry
	{
		/**
		*\~english
		*\brief
		*	The ID of the specialization constant in SPIR-V.
		*\~french
		*\brief
		*	L'ID de la constante de spécialsation en SPIR-V.
		*/
		uint32_t constantID;
		/**
		*\~english
		*\brief
		*	The byte offset of the specialization constant value within the supplied data buffer.
		*\~french
		*\brief
		*	Le décalage de la valeur de la constante de spécialisation dans le tampon de données.
		*/
		uint32_t offset;
		/**
		*\~english
		*\brief
		*	The byte size of the specialization constant value within the supplied data buffer.
		*\~french
		*\brief
		*	La taille de la valeur de la constante de spécialisation dans le tampon de données.
		*/
		uint32_t size;
	};
}

#endif
