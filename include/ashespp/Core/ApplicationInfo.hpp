/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ApplicationInfo_HPP___
#define ___AshesPP_ApplicationInfo_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

#include <cstdint>
#include <string>

namespace ashes
{
	struct ApplicationInfo
	{
		ApplicationInfo( ApplicationInfo  const & ) = delete;
		ApplicationInfo& operator=( ApplicationInfo  const & ) = delete;

		ApplicationInfo( std::string papplicationName
			, uint32_t applicationVersion
			, std::string pengineName
			, uint32_t engineVersion
			, uint32_t apiVersion )
			: applicationName{ papplicationName }
			, engineName{ pengineName }
			, vk{ VK_STRUCTURE_TYPE_APPLICATION_INFO
				, nullptr
				, applicationName.c_str()
				, applicationVersion
				, engineName.c_str()
				, engineVersion
				, apiVersion }
		{
		}

		ApplicationInfo( ApplicationInfo && application )noexcept
			: applicationName{ std::move( application.applicationName ) }
			, engineName{ std::move( application.engineName ) }
			, vk{ VK_STRUCTURE_TYPE_APPLICATION_INFO
				, application.vk.pNext
				, applicationName.c_str()
				, application.vk.applicationVersion
				, engineName.c_str()
				, application.vk.engineVersion
				, application.vk.apiVersion }
		{
		}

		ApplicationInfo & operator=( ApplicationInfo && application )noexcept
		{
			applicationName = std::move( application.applicationName );
			engineName = std::move( application.engineName );
			vk = { VK_STRUCTURE_TYPE_APPLICATION_INFO
				, application.vk.pNext
				, applicationName.c_str()
				, application.vk.applicationVersion
				, engineName.c_str()
				, application.vk.engineVersion
				, application.vk.apiVersion };
			return *this;
		}

		operator VkApplicationInfo const &()const
		{
			return vk;
		}

	private:
		std::string applicationName;
		std::string engineName;
		VkApplicationInfo vk;
	};
}

#endif
