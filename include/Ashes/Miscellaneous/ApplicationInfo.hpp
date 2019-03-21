/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ApplicationInfo_HPP___
#define ___Ashes_ApplicationInfo_HPP___
#pragma once

#include <cstdint>
#include <string>

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Holds the application informations.
	*\~french
	*\brief
	*	Contient les informations de l'application.
	*/
	struct ApplicationInfo
	{
		std::string applicationName;
		uint32_t applicationVersion;
		std::string engineName;
		uint32_t engineVersion;
		uint32_t apiVersion;
	};
}

#endif
