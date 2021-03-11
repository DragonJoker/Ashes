/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "util/UtilsDevice.hpp"

#include <ashespp/Buffer/Buffer.hpp>
#include <ashespp/Buffer/UniformBuffer.hpp>
#include <ashespp/Core/Device.hpp>
#include <ashespp/Core/DeviceCreateInfo.hpp>
#include <ashespp/Core/Instance.hpp>
#include <ashespp/Image/Image.hpp>

#include <ashes/common/Exception.hpp>

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
				{ VK_KHR_SWAPCHAIN_EXTENSION_NAME },
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
		, VkBufferUsageFlags usage
		, VkMemoryPropertyFlags flags )const
	{
		auto result = m_device->createBuffer( size, usage );
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

	ashes::ImagePtr Device::createImage( ashes::ImageCreateInfo const & createInfo
		, VkMemoryPropertyFlags flags
		, std::string const & debugName )const
	{
		auto result = m_device->createImage( debugName, createInfo );
		auto requirements = result->getMemoryRequirements();
		uint32_t deduced = deduceMemoryType( requirements.memoryTypeBits
			, flags );
		result->bindMemory( m_device->allocateMemory( debugName, { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, nullptr, requirements.size, deduced } ) );
		return result;
	}

	ashes::UniformBufferPtr Device::createUniformBuffer( uint32_t count
		, uint32_t size
		, VkBufferUsageFlags usage
		, VkMemoryPropertyFlags memoryFlags )const
	{
		return makeUniformBuffer( *this
			, count
			, size
			, usage
			, memoryFlags );
	}

	uint32_t Device::deduceMemoryType( VkMemoryPropertyFlags typeBits
		, VkMemoryPropertyFlags requirements )const
	{
		return m_device->deduceMemoryType( typeBits, requirements );
	}
}
