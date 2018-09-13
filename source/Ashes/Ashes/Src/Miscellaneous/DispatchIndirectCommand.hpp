/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DispatchIndirectCommand_HPP___
#define ___Ashes_DispatchIndirectCommand_HPP___
#pragma once

#include "AshesPrerequisites.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Structure specifying a dispatch indirect command.
	*\~french
	*\brief
	*	Structure décrivant une commande de dispatch indirect.
	*/
	struct DispatchIndirectCommand
	{
		/**
		*\~english
		*\brief
		*	The number of local workgroups to dispatch to the X, Y, and Z dimensions.
		*\~french
		*\brief
		*	Le nombre de groupes de travail locaux à distribuer dans les dimensions X, Y, et Z.
		*/
		uint32_t x;
		uint32_t y;
		uint32_t z;
	};
}

#endif
