/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_UniformBuffer_HPP___
#define ___AshesPP_UniformBuffer_HPP___
#pragma once

#include "ashespp/Buffer/Buffer.hpp"

namespace ashes
{
	/**
	*\brief
	*	A uniform variables buffer.
	*/
	class UniformBuffer
	{
	public:
		/**
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] count
		*	The number of instances of the data.
		*\param[in] size
		*	The size of an instance, in bytes.
		*\param[in] usage
		*	The buffer usage flags.
		*/
		UniformBuffer( Device const & device
			, VkDeviceSize count
			, VkDeviceSize size
			, VkBufferUsageFlags usage
			, QueueShare const & sharingMode = {} );
		/**
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] count
		*	The number of instances of the data.
		*\param[in] size
		*	The size of an instance, in bytes.
		*\param[in] usage
		*	The buffer usage flags.
		*/
		UniformBuffer( Device const & device
			, std::string const & debugName
			, VkDeviceSize count
			, VkDeviceSize size
			, VkBufferUsageFlags usage
			, QueueShare const & sharingMode = {} );
		/**
		*\brief
		*	Binds this buffer to given device memory object.
		*\param[in] memory
		*	The memory object.
		*/
		void bindMemory( DeviceMemoryPtr memory );
		/**
		*\return
		*	The memory requirements for this buffer.
		*/
		VkMemoryRequirements getMemoryRequirements()const;
		/**
		*\brief
		*	Retrieves the aligned size for an element.
		*\param[in] size
		*	The size of an element.
		*\return
		*	The aligned size.
		*/
		VkDeviceSize getAlignedSize( VkDeviceSize size )const;
		/**
		*\return
		*	The aligned size for an element.
		*/
		VkDeviceSize getAlignedSize()const
		{
			return getAlignedSize( getElementSize() );
		}
		/**
		*\return
		*	The GPU buffer.
		*/
		BufferBase const & getBuffer()const
		{
			return *m_buffer;
		}
		/**
		*\return
		*	The GPU buffer.
		*/
		BufferBase & getBuffer()
		{
			return *m_buffer;
		}
		/**
		*\return
		*	The size of one element in the buffer.
		*/
		VkDeviceSize getElementSize()const
		{
			return m_size;
		}
		/**
		*\brief
		*	Conversion implicite vers VkBuffer.
		*/
		operator VkBuffer const & ()const
		{
			return *m_buffer;
		}

	private:
		Device const & m_device;
		VkDeviceSize m_size{};
		BufferBasePtr m_buffer{};
	};
	/**
	*\brief
	*	UniformBuffer creation helper function.
	*\param[in] device
	*	The logical device.
	*\param[in] count
	*	The number of instances of the data.
	*\param[in] size
	*	The size of an instance, in bytes.
	*\param[in] usage
	*	The buffer usage flags.
	*\return
	*	The created buffer.
	*/
	inline UniformBufferPtr makeUniformBuffer( Device const & device
		, VkDeviceSize count
		, VkDeviceSize size
		, VkBufferUsageFlags usage
		, QueueShare sharingMode = {} )
	{
		return std::make_unique< UniformBuffer >( device
			, count
			, size
			, usage
			, std::move( sharingMode ) );
	}
	/**
	*\brief
	*	UniformBuffer creation helper function.
	*\param[in] device
	*	The logical device.
	*\param[in] count
	*	The number of instances of the data.
	*\param[in] size
	*	The size of an instance, in bytes.
	*\param[in] usage
	*	The buffer usage flags.
	*\return
	*	The created buffer.
	*/
	inline UniformBufferPtr makeUniformBuffer( Device const & device
		, std::string const & debugName
		, VkDeviceSize count
		, VkDeviceSize size
		, VkBufferUsageFlags usage
		, QueueShare sharingMode = {} )
	{
		return std::make_unique< UniformBuffer >( device
			, debugName
			, count
			, size
			, usage
			, std::move( sharingMode ) );
	}
}

#endif
