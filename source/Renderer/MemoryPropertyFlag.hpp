/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#pragma once

#include <VkLib/FlagCombination.hpp>

namespace renderer
{
	/**
	*@~french
	*@brief
	*	Masques de bits décrivant les propriétés pour un type de mémoire.
	*@~english
	*@brief
	*	Bimask specifying properties for a memory type.
	*/
	enum class MemoryPropertyFlag
		: uint32_t
	{
		//!@~french		Non cached on host.
		//!@~english	Non cachée sur l'hôte.
		eDeviceLocal = 0x00000001,
		//!@~french		Host can map memory allocated with this flag.
		//!@~english	L'hôte peut mapper la mémoire allouée avec cet indicateur.
		eHostVisible = 0x00000002,
		//!@~french		Cache commands to flush or invalidate the host memory changes are not necessary anymore, with this flag.
		//!@~english	Les commandes de cache pour ignorer ou valider les changements mémoire par l'hôte ne sont plus nécessaires, avec cet indicateur.
		eHostCoherent = 0x00000004,
		//!@~french		The memory allocated with this flag is cached on the host (hence changes will need flush or invalidate commands).
		//!@~english	La mémoire allouée avec cet indicateur est cachée sur l'hôte (ains, les changements auront besoin d'être invalidés ou ignorés).
		eHostCached = 0x00000008,
		//!@~french		Non visible to host, moreover object's memory may be provided lazily, by implementation.
		//!@~english	Non visible par l'hôte, de plus la mémoire de l'objet peut être fournie à la demande, par l'implémentation.
		eLazilyAllocated = 0x00000010,
	};
	VkLib_ImplementFlag( MemoryPropertyFlag )
}
