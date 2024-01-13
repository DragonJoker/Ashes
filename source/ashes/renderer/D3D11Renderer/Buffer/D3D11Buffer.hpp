/**
*\file
*	RenderingResources.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_Buffer_HPP___
#define ___D3D11Renderer_Buffer_HPP___
#pragma once

#include "renderer/D3D11Renderer/Miscellaneous/D3D11DeviceMemory.hpp"

namespace ashes::d3d11
{
	class Buffer
		: public NonCopyable
	{
	public:
		Buffer( VkDevice device
			, VkBufferCreateInfo createInfo );
		~Buffer()noexcept;
		VkResult bindMemory( VkDeviceMemory memory
			, VkDeviceSize memoryOffset );

		VkMemoryRequirements getMemoryRequirements()const;
		bool isMapped()const;
		void copyFrom( ID3D11DeviceContext * context
			, VkBuffer src
			, D3D11_BOX const & srcBox
			, UINT dstOffset )const;

		void setDebugName( std::string name )
		{
			m_debugName = std::move( name );
		}

		VkDeviceMemory getMemory()const
		{
			assert( m_memory != nullptr );
			return m_memory;
		}

		VkDeviceSize getMemoryOffset()const
		{
			return m_memoryOffset;
		}

		ObjectMemory const & getObjectMemory()const
		{
			assert( m_objectMemory != nullptr );
			return *m_objectMemory;
		}

		ID3D11UnorderedAccessView * getUnorderedAccessView()const
		{
			return m_unorderedAccessView;
		}

		ID3D11Resource * getResource()const
		{
			return getObjectMemory().resource;
		}

		ID3D11Buffer * getBuffer()const
		{
			return &static_cast< ID3D11Buffer & >( *getResource() );
		}

		VkBufferUsageFlags getUsage()const
		{
			return m_createInfo.usage;
		}

		VkDeviceSize getSize()const
		{
			return m_createInfo.size;
		}

		VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkBufferCreateInfo m_createInfo;
		VkDeviceMemory m_memory{};
		VkDeviceSize m_memoryOffset{ 0u };
		ObjectMemory * m_objectMemory{ nullptr };
		ID3D11UnorderedAccessView * m_unorderedAccessView{ nullptr };
		std::string m_debugName;
	};
}

#endif
