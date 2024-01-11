/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "UtilsPrerequisites.hpp"

#include <ashespp/Buffer/Buffer.hpp>
#include <ashespp/Buffer/UniformBuffer.hpp>
#include <ashespp/Buffer/VertexBuffer.hpp>
#include <ashespp/Core/Device.hpp>
#include <ashespp/Image/ImageCreateInfo.hpp>

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
		*\param[in] usage
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
		*\param[in] usage
		*	Les indicateurs d'utilisation du tampon.
		*\param[in] flags
		*	Les indicateurs de propriétés de mémoire pour l'objet DeviceMemory.
		*/
		ashes::BufferBasePtr createBuffer( uint32_t size
			, VkBufferUsageFlags usage
			, VkMemoryPropertyFlags flags )const;
		/**
		*\~english
		*\brief
		*	Creates a uniform buffer.
		*\param[in] count
		*	The buffer elements count.
		*\param[in] size
		*	The size of one element in the buffer.
		*\param[in] usage
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
		*\param[in] usage
		*	Les indicateurs d'utilisation du tampon.
		*\param[in] memoryFlags
		*	Les indicateurs de mémoire du tampon.
		*/
		ashes::UniformBufferPtr createUniformBuffer( uint32_t count
			, uint32_t size
			, VkBufferUsageFlags usage
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
		ashes::ImagePtr createImage( ashes::ImageCreateInfo const & createInfo
			, VkMemoryPropertyFlags flags
			, std::string const & debugName )const;

		uint32_t deduceMemoryType( VkMemoryPropertyFlags typeBits
			, VkMemoryPropertyFlags requirements )const;

		ashes::Device const * operator->()const noexcept
		{
			return &getDevice();
		}

		ashes::Device const & getDevice()const noexcept
		{
			return *m_device;
		}

		operator ashes::Device const &()const noexcept
		{
			return *m_device;
		}

		uint32_t getPresentQueueFamily()const noexcept
		{
			return m_presentQueueFamilyIndex;
		}

		uint32_t getGraphicsQueueFamily()const noexcept
		{
			return m_graphicsQueueFamilyIndex;
		}

		uint32_t getComputeQueueFamily()const noexcept
		{
			return m_computeQueueFamilyIndex;
		}

		ashes::PhysicalDevice const & getPhysicalDevice()const noexcept
		{
			return m_gpu;
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
	inline ashes::BufferPtr< T > makeBuffer( Device const & device
		, uint32_t count
		, VkBufferUsageFlags usage
		, VkMemoryPropertyFlags flags )
	{
		auto result = ashes::makeBuffer< T >( device.getDevice()
			, count
			, usage );
		auto requirements = result->getMemoryRequirements();
		auto deduced = device.deduceMemoryType( requirements.memoryTypeBits, flags );
		result->bindMemory( device.getDevice().allocateMemory( { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, nullptr, requirements.size, deduced } ) );
		return result;
	}

	template< typename T >
	inline ashes::VertexBufferPtr< T > makeVertexBuffer( Device const & device
		, uint32_t count
		, VkBufferUsageFlags usage
		, VkMemoryPropertyFlags flags )
	{
		auto result = ashes::makeVertexBuffer< T >( device.getDevice()
			, count
			, usage );
		auto requirements = result->getMemoryRequirements();
		auto deduced = device.deduceMemoryType( requirements.memoryTypeBits, flags );
		result->bindMemory( device.getDevice().allocateMemory( { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, nullptr, requirements.size, deduced } ) );
		return result;
	}

	inline ashes::UniformBufferPtr makeUniformBuffer( Device const & device
		, uint32_t elemCount
		, uint32_t elemSize
		, VkBufferUsageFlags usage
		, VkMemoryPropertyFlags flags )
	{
		auto result = ashes::makeUniformBuffer( device.getDevice()
			, elemCount
			, elemSize
			, usage );
		auto requirements = result->getMemoryRequirements();
		auto deduced = device.deduceMemoryType( requirements.memoryTypeBits, flags );
		result->bindMemory( device.getDevice().allocateMemory( { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, nullptr, requirements.size, deduced } ) );
		return result;
	}
}

