/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Utils/UtilsDevice.hpp"

#include <Ashes/Core/Device.hpp>
#include <Ashes/Core/Exception.hpp>
#include <Ashes/Core/Instance.hpp>

namespace utils
{
	namespace
	{
		void doInitialiseQueueFamilies( ashes::Instance const & instance
			, ashes::Surface const & surface
			, uint32_t & presentQueueFamilyIndex
			, uint32_t & graphicsQueueFamilyIndex
			, uint32_t & computeQueueFamilyIndex )
		{
			auto & gpu = instance.getPhysicalDevice( 0u );
			// Parcours des propriétés des files, pour vérifier leur support de la présentation.
			std::vector< bool > supportsPresent( static_cast< uint32_t >( gpu.getQueueProperties().size() ) );
			uint32_t i{ 0u };
			graphicsQueueFamilyIndex = std::numeric_limits< uint32_t >::max();
			presentQueueFamilyIndex = std::numeric_limits< uint32_t >::max();
			computeQueueFamilyIndex = std::numeric_limits< uint32_t >::max();

			for ( auto & present : supportsPresent )
			{
				auto present = surface.getSupport( i );

				if ( gpu.getQueueProperties()[i].queueCount > 0 )
				{
					if ( gpu.getQueueProperties()[i].queueFlags & ashes::QueueFlag::eGraphics )
					{
						// Tout d'abord on choisit une file graphique
						if ( graphicsQueueFamilyIndex == std::numeric_limits< uint32_t >::max() )
						{
							graphicsQueueFamilyIndex = i;
						}

						// Si la file supporte aussi les calculs, on la choisit en compute queue
						if ( gpu.getQueueProperties()[i].queueFlags & ashes::QueueFlag::eCompute
							&& computeQueueFamilyIndex == std::numeric_limits< uint32_t >::max() )
						{
							computeQueueFamilyIndex = i;
						}

						// Si une file supporte les graphismes et la présentation, on la préfère.
						if ( present )
						{
							graphicsQueueFamilyIndex = i;
							presentQueueFamilyIndex = i;
							break;
						}
					}
					else if ( gpu.getQueueProperties()[i].queueFlags & ashes::QueueFlag::eCompute
						&& computeQueueFamilyIndex == std::numeric_limits< uint32_t >::max() )
					{
						computeQueueFamilyIndex = i;
					}

				}

				++i;
			}

			if ( presentQueueFamilyIndex == std::numeric_limits< uint32_t >::max() )
			{
				// Pas de file supportant les deux, on a donc 2 files distinctes.
				for ( size_t i = 0; i < gpu.getQueueProperties().size(); ++i )
				{
					if ( supportsPresent[i] )
					{
						presentQueueFamilyIndex = static_cast< uint32_t >( i );
						break;
					}
				}
			}

			// Si on n'en a pas trouvé, on génère une erreur.
			if ( graphicsQueueFamilyIndex == std::numeric_limits< uint32_t >::max()
				|| presentQueueFamilyIndex == std::numeric_limits< uint32_t >::max()
				|| computeQueueFamilyIndex == std::numeric_limits< uint32_t >::max() )
			{
				throw ashes::Exception{ ashes::Result::eErrorInitializationFailed
					, "Queue families retrieval" };
			}
		}

		ashes::DeviceCreateInfo doGetDeviceCreateInfo( ashes::Instance const & instance
			, ashes::Surface const & surface
			, uint32_t & presentQueueFamilyIndex
			, uint32_t & graphicsQueueFamilyIndex
			, uint32_t & computeQueueFamilyIndex )
		{
			doInitialiseQueueFamilies( instance
				, surface
				, presentQueueFamilyIndex
				, graphicsQueueFamilyIndex
				, computeQueueFamilyIndex );
			std::vector< float > queuePriorities = { 1.0f };
			ashes::DeviceCreateInfo result;

			if ( graphicsQueueFamilyIndex != uint32_t( ~( 0u ) ) )
			{
				result.queueCreateInfos.push_back(
					{
						ashes::DeviceQueueCreateFlag::eNone,
						graphicsQueueFamilyIndex,
						queuePriorities,
					} );
			}

			if ( presentQueueFamilyIndex != graphicsQueueFamilyIndex )
			{
				result.queueCreateInfos.push_back(
					{
						ashes::DeviceQueueCreateFlag::eNone,
						presentQueueFamilyIndex,
						queuePriorities,
					} );
			}

			if ( computeQueueFamilyIndex != graphicsQueueFamilyIndex )
			{
				result.queueCreateInfos.push_back(
					{
						ashes::DeviceQueueCreateFlag::eNone,
						computeQueueFamilyIndex,
						queuePriorities,
					} );
			}

			result.enabledFeatures = surface.getGpu().getFeatures();
			result.enabledLayerNames = instance.getLayerNames();
			result.enabledExtensionNames = instance.getExtensionNames();
			return result;
		}
	}

	Device::Device( ashes::Instance const & instance
		, ashes::SurfacePtr surface )
		: m_createInfos{ doGetDeviceCreateInfo( instance
			, *surface
			, m_presentQueueFamilyIndex
			, m_graphicsQueueFamilyIndex
			, m_computeQueueFamilyIndex ) }
		, m_device{ instance.createDevice( std::move( surface )
			, std::move( m_createInfos ) ) }
	{
	}
}
