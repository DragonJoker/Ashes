/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Utils/UtilsDevice.hpp"

#include <AshesPP/Buffer/Buffer.hpp>
#include <AshesPP/Buffer/UniformBuffer.hpp>
#include <AshesPP/Core/Device.hpp>
#include <AshesPP/Core/DeviceCreateInfo.hpp>
#include <AshesPP/Core/Instance.hpp>
#include <AshesPP/Image/Image.hpp>

#include <AshesRenderer/Util/Exception.hpp>

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
					if ( ashes::checkFlag( queueProps[i].queueFlags, VK_QUEUE_GRAPHICS_BIT ) )
					{
						// Tout d'abord on choisit une file graphique
						if ( graphicsQueueFamilyIndex == std::numeric_limits< uint32_t >::max() )
						{
							graphicsQueueFamilyIndex = i;
						}

						// Si la file supporte aussi les calculs, on la choisit en compute queue
						if ( ashes::checkFlag( queueProps[i].queueFlags, VK_QUEUE_COMPUTE_BIT )
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
					else if ( ashes::checkFlag( queueProps[i].queueFlags, VK_QUEUE_COMPUTE_BIT )
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
				throw ashes::Exception{ VK_ERROR_INITIALIZATION_FAILED
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
			ashes::DeviceQueueCreateInfoArray queueCreateInfos;

			if ( graphicsQueueFamilyIndex != uint32_t( ~( 0u ) ) )
			{
				queueCreateInfos.push_back(
					{
						0u,
						graphicsQueueFamilyIndex,
						queuePriorities,
					} );
			}

			if ( presentQueueFamilyIndex != graphicsQueueFamilyIndex )
			{
				queueCreateInfos.push_back(
					{
						0u,
						presentQueueFamilyIndex,
						queuePriorities,
					} );
			}

			if ( computeQueueFamilyIndex != graphicsQueueFamilyIndex )
			{
				queueCreateInfos.push_back(
					{
						0u,
						computeQueueFamilyIndex,
						queuePriorities,
					} );
			}

			return ashes::DeviceCreateInfo
			{
				0u,
				std::move( queueCreateInfos ),
				instance.getEnabledLayerNames(),
				{ 1u, ashes::KHR_PLATFORM_SURFACE_EXTENSION_NAME },
				gpu.getFeatures()
			};
		}
	}

	Device::Device( ashes::Instance const & instance
		, ashes::Surface const & surface )
		: m_gpu{ surface.getGpu() }
		, m_createInfos{ doGetDeviceCreateInfo( instance
			, surface
			, m_gpu
			, m_presentQueueFamilyIndex
			, m_graphicsQueueFamilyIndex
			, m_computeQueueFamilyIndex ) }
		, m_device{ instance.createDevice( surface.getGpu()
			, std::move( m_createInfos ) ) }
		, m_memoryProperties{ m_gpu.getMemoryProperties() }
	{
	}

	ashes::BufferBasePtr Device::createBuffer( uint32_t size
		, VkBufferUsageFlags target
		, VkMemoryPropertyFlags flags )const
	{
		auto result = m_device->createBuffer( size, target );
		auto requirements = result->getMemoryRequirements();
		uint32_t deduced = deduceMemoryType( requirements.memoryTypeBits
			, flags );
		result->bindMemory( m_device->allocateMemory( { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, nullptr, requirements.size, deduced } ) );
		return result;
	}

	ashes::ImagePtr Device::createImage( ashes::ImageCreateInfo const & createInfo
		, VkMemoryPropertyFlags flags )const
	{
		auto result = m_device->createImage( createInfo );
		auto requirements = result->getMemoryRequirements();
		uint32_t deduced = deduceMemoryType( requirements.memoryTypeBits
			, flags );
		result->bindMemory( m_device->allocateMemory( { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, nullptr, requirements.size, deduced } ) );
		return result;
	}

	ashes::UniformBufferBasePtr Device::createUniformBuffer( uint32_t count
		, uint32_t size
		, VkBufferUsageFlags target
		, VkMemoryPropertyFlags memoryFlags )const
	{
		auto result = std::make_unique< ashes::UniformBufferBase >( *m_device
			, count
			, size
			, target );
		auto requirements = result->getMemoryRequirements();
		uint32_t deduced = deduceMemoryType( requirements.memoryTypeBits
			, memoryFlags );
		result->bindMemory( m_device->allocateMemory( { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, nullptr, requirements.size, deduced } ) );
		return result;
	}

	uint32_t Device::deduceMemoryType( VkMemoryPropertyFlags typeBits
		, VkMemoryPropertyFlags requirements )const
	{
		uint32_t result = 0xFFFFFFFFu;
		bool found{ false };

		// Recherche parmi les types de mémoire la première ayant les propriétés voulues.
		uint32_t i{ 0 };

		while ( i < m_memoryProperties.memoryTypeCount && !found )
		{
			if ( ( ashes::checkFlag( typeBits, 1u ) ) == 1 )
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
			throw ashes::Exception{ VK_ERROR_VALIDATION_FAILED_EXT, "Could not deduce memory type" };
		}

		return result;
	}
}
