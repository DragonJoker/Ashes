/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "UtilsPrerequisites.hpp"

#include <AshesPP/Buffer/Buffer.hpp>
#include <AshesPP/Buffer/UniformBuffer.hpp>
#include <AshesPP/Buffer/VertexBuffer.hpp>
#include <AshesPP/Core/Device.hpp>
#include <AshesPP/Image/ImageCreateInfo.hpp>

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
			, ashes::Surface const & surface );
		/**
		*\~english
		*\brief
		*	Creates a GPU buffer.
		*\remarks
		*	This version will also create the DeviceMemory and bind it to the buffer.
		*\param[in] size
		*	The buffer size.
		*\param[in] target
		*	The buffer usage flags.
		*\param[in] flags
		*	The memory property flags for the DeviceMemory object.
		*\~french
		*\brief
		*	Crée un tampon GPU.
		*\remarks
		*	Cette version va aussi créer le DeviceMemory et le lier au tampon.
		*\param[in] size
		*	La taille du tampon.
		*\param[in] target
		*	Les indicateurs d'utilisation du tampon.
		*\param[in] flags
		*	Les indicateurs de propriétés de mémoire pour l'objet DeviceMemory.
		*/
		ashes::BufferBasePtr createBuffer( uint32_t size
			, VkBufferUsageFlags target
			, VkMemoryPropertyFlags flags )const;
		/**
		*\~english
		*\brief
		*	Creates a uniform buffer.
		*\param[in] count
		*	The buffer elements count.
		*\param[in] size
		*	The size of one element in the buffer.
		*\param[in] target
		*	The buffer usage flags.
		*\param[in] memoryFlags
		*	The buffer memory flags.
		*\~french
		*\brief
		*	Crée un tampon d'uniformes.
		*\param[in] count
		*	Le nombre d'éléments du tampon.
		*\param[in] size
		*	La taille d'un élément.
		*\param[in] target
		*	Les indicateurs d'utilisation du tampon.
		*\param[in] memoryFlags
		*	Les indicateurs de mémoire du tampon.
		*/
		ashes::UniformBufferBasePtr createUniformBuffer( uint32_t count
			, uint32_t size
			, VkBufferUsageFlags target
			, VkMemoryPropertyFlags memoryFlags )const;
		/**
		*\~english
		*\brief
		*	Creates a texture.
		*\remarks
		*	This version will also create the DeviceMemory and bind it to the texture.
		*\param[in] createInfo
		*	The creation informations.
		*\param[in] flags
		*	The memory property flags for the DeviceMemory object.
		*\~french
		*\brief
		*	Crée une texture.
		*\remarks
		*	Cette version va aussi créer le DeviceMemory et le lier à la texture.
		*\param[in] createInfo
		*	Les informations de création.
		*\param[in] flags
		*	Les indicateurs de propriétés de mémoire pour l'objet DeviceMemory.
		*/
		ashes::ImagePtr createImage( ashes::ImageCreateInfo const & createInfo
			, VkMemoryPropertyFlags flags )const;

		uint32_t deduceMemoryType( VkMemoryPropertyFlags typeBits
			, VkMemoryPropertyFlags requirements )const;

		inline ashes::Device const * operator->()const
		{
			return &getDevice();
		}

		inline ashes::Device const & getDevice()const
		{
			return *m_device;
		}

		inline operator ashes::Device const &()const
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
		ashes::PhysicalDevice const & m_gpu;
		uint32_t m_presentQueueFamilyIndex;
		uint32_t m_graphicsQueueFamilyIndex;
		uint32_t m_computeQueueFamilyIndex;
		VkPhysicalDeviceMemoryProperties m_memoryProperties;
		ashes::DeviceCreateInfo m_createInfos;
		ashes::DevicePtr m_device;
	};

	using DevicePtr = std::unique_ptr< Device >;

	template< typename T >
	ashes::BufferPtr< T > makeBuffer( Device const & device
		, uint32_t count
		, VkBufferUsageFlags target
		, VkMemoryPropertyFlags flags )
	{
		auto result = ashes::makeBuffer< T >( device.getDevice()
			, count
			, target );
		auto requirements = result->getMemoryRequirements();
		auto deduced = device.deduceMemoryType( requirements.memoryTypeBits, flags );
		result->bindMemory( device.getDevice().allocateMemory( { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, 0u, requirements.size, deduced } ) );
		return result;
	}

	template< typename T >
	ashes::VertexBufferPtr< T > makeVertexBuffer( Device const & device
		, uint32_t count
		, VkBufferUsageFlags target
		, VkMemoryPropertyFlags flags )
	{
		auto result = ashes::makeVertexBuffer< T >( device.getDevice()
			, count
			, target );
		auto requirements = result->getMemoryRequirements();
		auto deduced = device.deduceMemoryType( requirements.memoryTypeBits, flags );
		result->bindMemory( device.getDevice().allocateMemory( { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, 0u, requirements.size, deduced } ) );
		return result;
	}

	template< typename T >
	ashes::UniformBufferPtr< T > makeUniformBuffer( Device const & device
		, uint32_t count
		, VkBufferUsageFlags target
		, VkMemoryPropertyFlags flags )
	{
		auto result = ashes::makeUniformBuffer< T >( device.getDevice()
			, count
			, target );
		auto requirements = result->getMemoryRequirements();
		auto deduced = device.deduceMemoryType( requirements.memoryTypeBits, flags );
		result->bindMemory( device.getDevice().allocateMemory( { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, 0u, requirements.size, deduced } ) );
		return result;
	}
}

