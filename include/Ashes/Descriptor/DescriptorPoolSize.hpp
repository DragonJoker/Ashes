/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DescriptorPoolSize_HPP___
#define ___Ashes_DescriptorPoolSize_HPP___
#pragma once

#include "Ashes/AshesPrerequisites.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Specifies a descriptor pool size.
	*\~french
	*\brief
	*	Spécifie la taille d'un pool de descripteurs.
	*/
	struct DescriptorPoolSize
	{
		/**
		*\~english
		*\brief
		*	The descriptor type.
		*\~french
		*\brief
		*	Le type de descripteur.
		*/ 
		DescriptorType type;
		/**
		*\~english
		*\brief
		*	The number of descriptors of that type to allocate.
		*\~french
		*\brief
		*	Le nombre de descripteurs de ce type à allouer.
		*/
		uint32_t descriptorCount;
	};
}

#endif
