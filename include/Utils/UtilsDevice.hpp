/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "UtilsPrerequisites.hpp"

#include <Ashes/Core/Device.hpp>

namespace utils
{
	/**
	*\~english
	*\brief
	*	Helper class to handle logical device creation.
	*\~french
	*\brief
	*	Classe d'aide à la création du périphérique logique.
	*/
	class Device
	{
	public:
		Device( ashes::Instance const & instance
			, ashes::SurfacePtr surface );

		inline ashes::Device const & getDevice()const
		{
			return *m_device;
		}

		inline uint32_t getPresentQueueFamily()const
		{
			return m_presentQueueFamilyIndex;
		}

		inline uint32_t getGraphicsQueueFamily()const
		{
			return m_graphicsQueueFamilyIndex;
		}

		inline uint32_t getComputeQueueFamily()const
		{
			return m_computeQueueFamilyIndex;
		}

	private:
		uint32_t m_presentQueueFamilyIndex;
		uint32_t m_graphicsQueueFamilyIndex;
		uint32_t m_computeQueueFamilyIndex;
		ashes::DeviceCreateInfo m_createInfos;
		ashes::DevicePtr m_device;
	};

	using DevicePtr = std::unique_ptr< Device >;
}

