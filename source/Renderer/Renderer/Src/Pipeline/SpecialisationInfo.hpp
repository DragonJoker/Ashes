/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_SpecialisationInfo_HPP___
#define ___Renderer_SpecialisationInfo_HPP___
#pragma once

#include "SpecialisationMapEntry.hpp"

namespace renderer
{
	/**
	*\~english
	*\brief
	*	Specifies a constants specialisation info.
	*\~french
	*\brief
	*	Définit les informations de spécialisation des constantes.
	*/
	struct SpecialisationInfo
	{
		/**
		*\~english
		*\brief
		*	The array of SpecializationMapEntry which maps constant IDs to offsets in \p data.
		*\~french
		*\brief
		*	Le tableau de SpecialisationMapEntry qui associe les ID de constantes à des offsets dans \p data.
		*/
		std::vector< SpecialisationMapEntry > mapEntries;
		/**
		*\~english
		*\brief
		*	The actual constant values to specialize with.
		*\~french
		*\brief
		*	Les valeurs réelles des constantes à spécialiser.
		*/
		ByteArray data;
	};
}

#endif
