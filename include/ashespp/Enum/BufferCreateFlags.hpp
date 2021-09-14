/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_BufferCreateFlags_HPP___
#define ___AshesPP_BufferCreateFlags_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Gets the name of the given element type.
	*\param[in] value
	*	The element type.
	*\return
	*	The name.
	*/
	inline std::string getName( VkBufferCreateFlagBits value )
	{
		switch ( value )
		{
		case VK_BUFFER_CREATE_SPARSE_BINDING_BIT:
			return "sparse_binding";
		case VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT:
			return "sparse_residency";
		case VK_BUFFER_CREATE_SPARSE_ALIASED_BIT:
			return "sparse_aliased";
		case VK_BUFFER_CREATE_PROTECTED_BIT:
			return "protected";
#if defined( VK_API_VERSION_1_2 )
		case VK_BUFFER_CREATE_DEVICE_ADDRESS_CAPTURE_REPLAY_BIT:
			return "device_address_capture_replay";
#elif defined( VK_KHR_buffer_device_address )
		case VK_BUFFER_CREATE_DEVICE_ADDRESS_CAPTURE_REPLAY_BIT_KHR:
			return "device_address_capture_replay";
#elif defined( VK_EXT_buffer_device_address )
		case VK_BUFFER_CREATE_DEVICE_ADDRESS_CAPTURE_REPLAY_BIT_EXT:
			return "device_address_capture_replay";
#endif
		default:
			assert( false && "Unsupported VkBufferCreateFlagBits." );
			return "Unsupported VkBufferCreateFlagBits";
		}
	}
}

#endif
