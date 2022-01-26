/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_DeviceMemory_HPP___
#define ___AshesPP_DeviceMemory_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

#include "ashespp/Core/Device.hpp"

namespace ashes
{
	/**
	*\brief
	*	A device memory object.
	*/
	class DeviceMemory
		: public VkObject
	{
	public:
		DeviceMemory( Device const & device
			, VkMemoryAllocateInfo allocateInfo );
		DeviceMemory( Device const & device
			, std::string const & debugName
			, VkMemoryAllocateInfo allocateInfo );
		~DeviceMemory();
		/**
		*\brief
		*	Maps a range of the memory in RAM.
		*\param[in] offset
		*	The range beginning offset.
		*\param[in] size
		*	The range size.
		*\param[in] flags
		*	The mapping flags.
		*\return
		*	\p nullptr if mapping failed.
		*/
		uint8_t * lock( VkDeviceSize offset
			, VkDeviceSize size
			, VkMemoryMapFlags flags )const;
		/**
		*\brief
		*	Invalidates the memory content.
		*\param[in] offset
		*	The mapped memory starting offset.
		*\param[in] size
		*	The range size.
		*/
		void invalidate( VkDeviceSize offset
			, VkDeviceSize size )const;
		/**
		*\brief
		*	Updates the VRAM.
		*\param[in] offset
		*	The mapped memory starting offset.
		*\param[in] size
		*	The range size.
		*/
		void flush( VkDeviceSize offset
			, VkDeviceSize size )const;
		/**
		*\brief
		*	Unmaps the memory from RAM.
		*/
		void unlock()const;

#if VK_KHR_buffer_device_address

		/**
		*\return
		*	An opaque capture address of the memory object.
		*/
		uint64_t getOpaqueCaptureAddress()const;

#endif
		/**
		*\brief
		*	Conversion implicite vers VkDeviceMemory.
		*/
		inline operator VkDeviceMemory const & ()const
		{
			return m_internal;
		}

	protected:
		Device const & m_device;
		VkMemoryAllocateInfo m_allocateInfo;
		VkDeviceMemory m_internal{};
	};
}

#endif
