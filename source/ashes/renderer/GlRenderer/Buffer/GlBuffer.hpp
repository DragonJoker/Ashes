/**
*\file
*	RenderingResources.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Buffer_HPP___
#define ___GlRenderer_Buffer_HPP___
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	class Buffer
		: public IcdObject
	{
		friend class DeviceMemory;

	public:
		Buffer( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkBufferCreateInfo createInfo );
		~Buffer()noexcept;

		VkMemoryRequirements getMemoryRequirements()const noexcept;
		bool isMapped()const noexcept;
		VkDeviceSize getOffset()const noexcept;

		GlBufferTarget getTarget()const noexcept
		{
			return m_target;
		}

		void setMemoryBinding( DeviceMemoryBinding const * binding )noexcept
		{
			m_binding = binding;
		}

		DeviceMemoryBinding const & getMemoryBinding()const noexcept
		{
			return *m_binding;
		}

		VkDevice getDevice()const noexcept
		{
			return m_device;
		}

	private:
		void setInternal( uint32_t v )noexcept
		{
			m_internal = v;
		}

	private:
		VkDevice m_device;
		UInt32Array m_queueFamilyIndices;
		VkBufferCreateInfo m_createInfo;
		GlBufferTarget m_target{};
		DeviceMemoryBinding const * m_binding{ nullptr };
	};
}

#endif
