/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Utils/UtilsDevice.hpp"

#include <Ashes/Buffer/Buffer.hpp>
#include <Ashes/Buffer/UniformBuffer.hpp>
#include <Ashes/Core/Device.hpp>
#include <Ashes/Core/Exception.hpp>
#include <Ashes/Core/Instance.hpp>
#include <Ashes/Image/Image.hpp>
#include <Ashes/Miscellaneous/MemoryRequirements.hpp>

namespace utils
{
	namespace
	{
		void doInitialiseQueueFamilies( ashes::Instance const & instance
			, ashes::Surface const & surface
			, ashes::PhysicalDevice const & gpu
			, uint32_t & presentQueueFamilyIndex
			, uint32_t & graphicsQueueFamilyIndex
			, uint32_t & computeQueueFamilyIndex )
		{
			// Parcours des propriétés des files, pour vérifier leur support de la présentation.
			auto queueProps = gpu.getQueueFamilyProperties();
			std::vector< uint32_t > supportsPresent( static_cast< uint32_t >( queueProps.size() ) );
			uint32_t i{ 0u };
			graphicsQueueFamilyIndex = std::numeric_limits< uint32_t >::max();
			presentQueueFamilyIndex = std::numeric_limits< uint32_t >::max();
			computeQueueFamilyIndex = std::numeric_limits< uint32_t >::max();

			for ( auto & present : supportsPresent )
			{
				present = surface.getSupport( i );

				if ( queueProps[i].queueCount > 0 )
				{
					if ( queueProps[i].queueFlags & ashes::QueueFlag::eGraphics )
					{
						// Tout d'abord on choisit une file graphique
						if ( graphicsQueueFamilyIndex == std::numeric_limits< uint32_t >::max() )
						{
							graphicsQueueFamilyIndex = i;
						}

						// Si la file supporte aussi les calculs, on la choisit en compute queue
						if ( queueProps[i].queueFlags & ashes::QueueFlag::eCompute
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
					else if ( queueProps[i].queueFlags & ashes::QueueFlag::eCompute
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
				for ( size_t i = 0; i < queueProps.size(); ++i )
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
			, ashes::PhysicalDevice const & gpu
			, uint32_t & presentQueueFamilyIndex
			, uint32_t & graphicsQueueFamilyIndex
			, uint32_t & computeQueueFamilyIndex )
		{
			doInitialiseQueueFamilies( instance
				, surface
				, gpu
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

			result.enabledFeatures = gpu.getFeatures();
			result.enabledLayerNames = instance.getEnabledLayerNames();
			result.enabledExtensionNames.push_back( ashes::KHR_SWAPCHAIN_EXTENSION_NAME );
			return result;
		}
	}

	Device::Device( ashes::Instance const & instance
		, ashes::SurfacePtr surface )
		: m_gpu{ surface->getGpu() }
		, m_createInfos{ doGetDeviceCreateInfo( instance
			, *surface
			, m_gpu
			, m_presentQueueFamilyIndex
			, m_graphicsQueueFamilyIndex
			, m_computeQueueFamilyIndex ) }
		, m_device{ instance.createDevice( std::move( surface )
			, std::move( m_createInfos ) ) }
		, m_memoryProperties{ m_gpu.getMemoryProperties() }
	{
	}

	ashes::BufferBasePtr Device::createBuffer( uint32_t size
		, ashes::BufferTargets target
		, ashes::MemoryPropertyFlags flags )const
	{
		auto result = m_device->createBuffer( size, target );
		auto requirements = result->getMemoryRequirements();
		uint32_t deduced = deduceMemoryType( requirements.memoryTypeBits
			, flags );
		result->bindMemory( m_device->allocateMemory( { requirements.size, deduced } ) );
		return result;
	}

	ashes::ImagePtr Device::createImage( ashes::ImageCreateInfo const & createInfo
		, ashes::MemoryPropertyFlags flags )const
	{
		auto result = m_device->createImage( createInfo );
		auto requirements = result->getMemoryRequirements();
		uint32_t deduced = deduceMemoryType( requirements.memoryTypeBits
			, flags );
		result->bindMemory( m_device->allocateMemory( { requirements.size, deduced } ) );
		return result;
	}

	ashes::UniformBufferBasePtr Device::createUniformBuffer( uint32_t count
		, uint32_t size
		, ashes::BufferTargets target
		, ashes::MemoryPropertyFlags memoryFlags )const
	{
		auto result = std::make_unique< ashes::UniformBufferBase >( *m_device
			, count
			, size
			, target );
		auto requirements = result->getMemoryRequirements();
		uint32_t deduced = deduceMemoryType( requirements.memoryTypeBits
			, memoryFlags );
		result->bindMemory( m_device->allocateMemory( { requirements.size, deduced } ) );
		return result;
	}

	uint32_t Device::deduceMemoryType( ashes::MemoryPropertyFlags typeBits
		, ashes::MemoryPropertyFlags requirements )const
	{
		uint32_t result = 0xFFFFFFFFu;
		bool found{ false };

		// Recherche parmi les types de mémoire la première ayant les propriétés voulues.
		uint32_t i{ 0 };

		while ( i < m_memoryProperties.memoryTypes.size() && !found )
		{
			if ( ( checkFlag( typeBits, ashes::MemoryPropertyFlag( 1u ) ) ) == 1 )
			{
				// Le type de mémoire est disponible, a-t-il les propriétés demandées?
				if ( ( m_memoryProperties.memoryTypes[i].propertyFlags & requirements ) == requirements )
				{
					result = i;
					found = true;
				}
			}

			typeBits = typeBits >> 1;
			++i;
		}

		if ( !found )
		{
			throw ashes::Exception{ ashes::Result::eErrorRenderer, "Could not deduce memory type" };
		}

		return result;
	}
}
