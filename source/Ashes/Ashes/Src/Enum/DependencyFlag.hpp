/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DependencyFlag_HPP___
#define ___Ashes_DependencyFlag_HPP___
#pragma once

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Bitmask specifying how execution and memory dependencies are formed.
	*\~french
	*\brief
	*	Masque de bits définissant comment l'exécution et les dépendances mémoire sont constituées.
	*/
	enum class DependencyFlag
		: uint32_t
	{
		eByRegion = 0x00000001
	};
	Renderer_ImplementFlag( DependencyFlag )
}

#endif
