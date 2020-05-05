/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_BufferView_HPP___
#define ___AshesPP_BufferView_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	/**
	*\brief
	*	A view on a buffer.
	*/
	class BufferView
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] buffer
		*	The buffer from which the view is created.
		*\param[in] format
		*	The view's pixels format.
		*\param[in] offset
		*	The offset in the buffer.
		*\param[in] range
		*	The number of elements from the buffer.
		*/
		BufferView( Device const & device
			, BufferBase const & buffer
			, VkFormat format
			, VkDeviceSize offset
			, VkDeviceSize range );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] buffer
		*	The buffer from which the view is created.
		*\param[in] format
		*	The view's pixels format.
		*\param[in] offset
		*	The offset in the buffer.
		*\param[in] range
		*	The number of elements from the buffer.
		*/
		BufferView( Device const & device
			, std::string const & debugName
			, BufferBase const & buffer
			, VkFormat format
			, VkDeviceSize offset
			, VkDeviceSize range );
		/**
		*\brief
		*	Destructor.
		*/
		~BufferView();
		/**
		*\return
		*	The view's pixels format.
		*/
		inline VkFormat getFormat()const
		{
			return m_format;
		}
		/**
		*\return
		*	The view's buffer.
		*/
		inline BufferBase const & getBuffer()const
		{
			return m_buffer;
		}
		/**
		*\return
		*	The offset in the buffer.
		*/
		inline VkDeviceSize getOffset()const
		{
			return m_offset;
		}
		/**
		*\return
		*	The number of elements from the buffer.
		*/
		inline VkDeviceSize getRange()const
		{
			return m_range;
		}
		/**
		*\brief
		*	VkBufferView implicit cast operator.
		*/
		inline operator VkBufferView const &()const
		{
			return m_internal;
		}

	private:
		Device const & m_device;
		BufferBase const & m_buffer;
		VkFormat m_format;
		VkDeviceSize m_offset;
		VkDeviceSize m_range;
		VkBufferView m_internal{ VK_NULL_HANDLE };
	};
}

#endif
