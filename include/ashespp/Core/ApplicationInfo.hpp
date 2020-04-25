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

		ApplicationInfo( std::string applicationName
			, uint32_t applicationVersion
			, std::string engineName
			, uint32_t engineVersion
			, uint32_t apiVersion )
			: applicationName{ applicationName }
			, engineName{ engineName }
			, vk
			{
				VK_STRUCTURE_TYPE_APPLICATION_INFO,
				nullptr,
				this->applicationName.c_str(),
				applicationVersion,
				this->engineName.c_str(),
				engineVersion,
				apiVersion,
			}
		{
		}

		ApplicationInfo( ApplicationInfo && application )noexcept
			: applicationName{ std::move( application.applicationName ) }
			, engineName{ std::move( application.engineName ) }
			, vk
			{
				VK_STRUCTURE_TYPE_APPLICATION_INFO,
				nullptr,
				this->applicationName.c_str(),
				application.vk.applicationVersion,
				this->engineName.c_str(),
				application.vk.engineVersion,
				application.vk.apiVersion,
			}
		{
		}

		ApplicationInfo & operator=( ApplicationInfo && application )noexcept
		{
			applicationName = std::move( application.applicationName );
			engineName = std::move( application.engineName );
			vk =
			{
				VK_STRUCTURE_TYPE_APPLICATION_INFO,
				nullptr,
				applicationName.c_str(),
				application.vk.applicationVersion,
				engineName.c_str(),
				application.vk.engineVersion,
				application.vk.apiVersion,
			};

			return *this;
		}

		inline operator VkApplicationInfo const &()const
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
