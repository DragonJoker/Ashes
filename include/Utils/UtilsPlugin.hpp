#pragma once

#include "Utils/UtilsPrerequisites.hpp"

#include <AshesPP/Core/Instance.hpp>

#include <AshesCommon/DynamicLibrary.hpp>

#include <ashes/ashes.h>

namespace utils
{
	class Plugin
	{
	public:
		Plugin( Plugin const & ) = delete;
		Plugin( Plugin && ) = default;
		Plugin & operator=( Plugin const & ) = delete;
		Plugin & operator=( Plugin && ) = default;

		Plugin( AshPluginDescription desc );

		std::string getShortName()
		{
			return m_desc.name;
		}

		std::string getFullName()
		{
			return m_desc.description;
		}

		PFN_vkGetInstanceProcAddr getInstanceProcAddrFunc()
		{
			return m_desc.getInstanceProcAddr;
		}

	private:
		AshPluginDescription m_desc;
	};
}
