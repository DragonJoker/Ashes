/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "Miscellaneous/D3D11DeviceMemory.hpp"

#include "Core/D3D11Device.hpp"
#include "Core/D3D11Instance.hpp"
#include "Core/D3D11PhysicalDevice.hpp"

#include <ashes/common/Exception.hpp>

#include <iostream>

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	//*********************************************************************************************

	VkMemoryPropertyFlags getFlags( uint32_t memoryTypeIndex )
	{
		assert( memoryTypeIndex < Instance::getMemoryProperties().memoryTypeCount
			&& "Wrong deduced memory type" );
		return Instance::getMemoryProperties().memoryTypes[memoryTypeIndex].propertyFlags;
	}

	//*********************************************************************************************

	class BufferDeviceMemory
		:  public DeviceMemory::DeviceMemoryImpl
	{
	public:
		BufferDeviceMemory( VkDevice device
			, VkMemoryAllocateInfo allocateInfo
			, VkBufferUsageFlags targets )
			: DeviceMemory::DeviceMemoryImpl{ device, std::move( allocateInfo ) }
		{
			auto d3ddevice = get( m_device )->getDevice();
			UINT size = UINT( m_allocateInfo.allocationSize );
			m_bufferTargets = targets;

			if ( size > 0 )
			{
				D3D11_BUFFER_DESC desc{};
				desc.ByteWidth = size;
				desc.Usage = getBufferUsage( m_propertyFlags, m_bufferTargets );
				desc.BindFlags = getBindFlags( m_bufferTargets );
				desc.CPUAccessFlags = getCpuBufferAccessFlags( m_propertyFlags, m_bufferTargets );
				desc.MiscFlags = ( ( checkFlag( m_bufferTargets, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT )
						|| checkFlag( m_bufferTargets, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT ) )
					? D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS
					: 0u );
				desc.StructureByteStride = 0;

				auto hr = d3ddevice->CreateBuffer( &desc, nullptr, &m_buffer );

				if ( checkError( m_device, hr, "CreateBuffer" ) )
				{
					dxDebugName( m_buffer, Buffer );
				}
			}
		}

		~BufferDeviceMemory()noexcept
		{
			safeRelease( m_buffer );
		}

		VkResult lock( ID3D11DeviceContext * context
			, UINT subresource
			, D3D11_MAPPED_SUBRESOURCE & data )const override
		{
			HRESULT hr = context->Map( m_buffer
				, subresource
				, getBufferMapFlags( m_propertyFlags, m_bufferTargets )
				, 0
				, &data );
			return checkError( m_device, hr, "LockBuffer" )
				? VK_SUCCESS
				: VK_ERROR_MEMORY_MAP_FAILED;
		}

		void unlock( ID3D11DeviceContext * context
			, UINT subresource )const override
		{
			context->Unmap( m_buffer, subresource );
		}

		inline ID3D11Buffer * getBuffer()const
		{
			return m_buffer;
		}

	private:
		VkBufferUsageFlags m_bufferTargets;
		ID3D11Buffer * m_buffer{ nullptr };
	};
	
	//*********************************************************************************************

	UINT getBindFlags( VkImageCreateInfo const & createInfo
		, VkMemoryPropertyFlags propertyFlags )
	{
		return ( ( isSampled( createInfo.usage )
				? D3D11_BIND_SHADER_RESOURCE
				: 0 )
			| ( isRenderable( createInfo.usage, createInfo.format, createInfo.mipLevels )
				? ( isHostVisible( propertyFlags )
					? 0
					: ( isDepthOrStencilFormat( createInfo.format )
						? D3D11_BIND_DEPTH_STENCIL
						: D3D11_BIND_RENDER_TARGET ) )
				: 0 )
			| ( isStorage( createInfo.usage )
				? D3D11_BIND_UNORDERED_ACCESS
				: 0 ) );
	}

	UINT getMiscFlags( VkImageCreateInfo const & createInfo )
	{
		return 0u
			//| ( isMipmapped( createInfo.usage, createInfo.format, createInfo.mipLevels )
			//	? D3D11_RESOURCE_MISC_GENERATE_MIPS
			//	: 0 )
			| ( checkFlag( createInfo.flags, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT )
				? D3D11_RESOURCE_MISC_TEXTURECUBE
				: 0u );
	}

	//*********************************************************************************************

	class Texture1DDeviceMemory
		: public DeviceMemory::DeviceMemoryImpl
	{
	public:
		Texture1DDeviceMemory( VkDevice device
			, VkMemoryAllocateInfo allocateInfo
			, VkImageCreateInfo const & createInfo )
			: DeviceMemory::DeviceMemoryImpl{ device, std::move( allocateInfo ) }
			, m_usage{ createInfo.usage }
		{
			auto d3ddevice = get( m_device )->getDevice();
			D3D11_TEXTURE1D_DESC desc{};
			doInitTexDesc( createInfo, desc );
			HRESULT hr = d3ddevice->CreateTexture1D( &desc
				, nullptr
				, &m_texture );

			if ( checkError( m_device, hr, "CreateTexture1D" ) )
			{
				dxDebugName( m_texture, Texture1D );
			}
		}

		~Texture1DDeviceMemory()noexcept
		{
		}

		VkResult lock( ID3D11DeviceContext * context
			, UINT subresource
			, D3D11_MAPPED_SUBRESOURCE & data )const override
		{
			HRESULT hr = context->Map( m_texture
				, subresource
				, getImageMapFlags( m_propertyFlags, m_usage )
				, 0
				, &data );
			return checkError( m_device, hr, "LockTexture1D" )
				? VK_SUCCESS
				: VK_ERROR_MEMORY_MAP_FAILED;
		}

		void unlock( ID3D11DeviceContext * context
			, UINT subresource )const override
		{
			context->Unmap( m_texture, subresource );
		}

		inline ID3D11Texture1D * getImage()const
		{
			return m_texture;
		}

	private:
		void doInitTexDesc( VkImageCreateInfo const & createInfo
			, D3D11_TEXTURE1D_DESC & desc )
		{
			auto device = get( m_device )->getDevice();
			desc.Width = createInfo.extent.width;
			desc.Format = getTextureFormat( createInfo.format );
			desc.ArraySize = createInfo.arrayLayers;
			desc.Usage = getImageUsage( m_propertyFlags, m_usage );
			desc.CPUAccessFlags = getCpuImageAccessFlags( m_propertyFlags, m_usage );
			desc.MipLevels = createInfo.mipLevels;
			desc.BindFlags = getBindFlags( createInfo, m_propertyFlags );
			desc.MiscFlags = getMiscFlags( createInfo );

			if ( get( m_device )->getFeatureLevel() < D3D_FEATURE_LEVEL_11_0 )
			{
				desc.BindFlags &= ~( D3D11_BIND_UNORDERED_ACCESS );
			}
		}

	private:
		VkImageUsageFlags m_usage;
		ID3D11Texture1D * m_texture{ nullptr };
	};

	//*********************************************************************************************

	class Texture2DDeviceMemory
		: public DeviceMemory::DeviceMemoryImpl
	{
	public:
		Texture2DDeviceMemory( VkDevice device
			, VkMemoryAllocateInfo allocateInfo
			, VkImageCreateInfo const & createInfo )
			: DeviceMemory::DeviceMemoryImpl{ device, std::move( allocateInfo ) }
			, m_usage{ createInfo.usage }
		{
			auto d3ddevice = get( m_device )->getDevice();
			D3D11_TEXTURE2D_DESC desc{};
			doInitTexDesc( createInfo, desc );
			HRESULT hr = d3ddevice->CreateTexture2D( &desc
				, nullptr
				, &m_texture );

			if ( checkError( m_device, hr, "CreateTexture2D" ) )
			{
				dxDebugName( m_texture, Texture2D );
			}
		}

		~Texture2DDeviceMemory()noexcept
		{
		}

		VkResult lock( ID3D11DeviceContext * context
			, UINT subresource
			, D3D11_MAPPED_SUBRESOURCE & data )const override
		{
			HRESULT hr = context->Map( m_texture
				, subresource
				, getImageMapFlags( m_propertyFlags, m_usage )
				, 0
				, &data );
			return checkError( m_device, hr, "LockTexture2D" )
				? VK_SUCCESS
				: VK_ERROR_MEMORY_MAP_FAILED;
		}

		void unlock( ID3D11DeviceContext * context
			, UINT subresource )const override
		{
			context->Unmap( m_texture, subresource );
		}

		inline ID3D11Texture2D * getImage()const
		{
			return m_texture;
		}

	private:
		void doInitTexDesc( VkImageCreateInfo const & createInfo
			, D3D11_TEXTURE2D_DESC & desc )
		{
			auto device = get( m_device )->getDevice();
			desc.Width = createInfo.extent.width;
			desc.Height = createInfo.extent.height;
			desc.Format = getTextureFormat( createInfo.format );
			desc.ArraySize = createInfo.arrayLayers;
			desc.Usage = getImageUsage( m_propertyFlags, m_usage );
			desc.CPUAccessFlags = getCpuImageAccessFlags( m_propertyFlags, m_usage );
			desc.MipLevels = createInfo.mipLevels;
			desc.SampleDesc.Count = UINT( createInfo.samples );
			desc.SampleDesc.Quality = 0;
			desc.BindFlags = getBindFlags( createInfo, m_propertyFlags );
			desc.MiscFlags = getMiscFlags( createInfo );

			if ( desc.SampleDesc.Count > 1 )
			{
				HRESULT hr = device->CheckMultisampleQualityLevels( desc.Format
					, desc.SampleDesc.Count
					, &desc.SampleDesc.Quality );

				if ( hr == S_OK && desc.SampleDesc.Quality > 0 )
				{
					desc.SampleDesc.Quality--;
				}
			}

			if ( get( m_device )->getFeatureLevel() < D3D_FEATURE_LEVEL_11_0 )
			{
				desc.BindFlags &= ~( D3D11_BIND_UNORDERED_ACCESS );
			}
		}

	private:
		VkImageUsageFlags m_usage;
		ID3D11Texture2D * m_texture{ nullptr };
	};
	
	//*********************************************************************************************

	class Texture3DDeviceMemory
		: public DeviceMemory::DeviceMemoryImpl
	{
	public:
		Texture3DDeviceMemory( VkDevice device
			, VkMemoryAllocateInfo allocateInfo
			, VkImageCreateInfo const & createInfo )
			: DeviceMemory::DeviceMemoryImpl{ device, std::move( allocateInfo ) }
			, m_usage{ createInfo.usage }
		{
			auto d3ddevice = get( m_device )->getDevice();
			D3D11_TEXTURE3D_DESC desc{};
			doInitTexDesc( createInfo, desc );
			HRESULT hr = d3ddevice->CreateTexture3D( &desc
				, nullptr
				, &m_texture );

			if ( checkError( m_device, hr, "CreateTexture3D" ) )
			{
				dxDebugName( m_texture, Texture3D );
			}
		}

		~Texture3DDeviceMemory()noexcept
		{
		}

		VkResult lock( ID3D11DeviceContext * context
			, UINT subresource
			, D3D11_MAPPED_SUBRESOURCE & data )const override
		{
			HRESULT hr = context->Map( m_texture
				, subresource
				, getImageMapFlags( m_propertyFlags, m_usage )
				, 0
				, &data );
			return checkError( m_device, hr, "LockTexture3D" )
				? VK_SUCCESS
				: VK_ERROR_MEMORY_MAP_FAILED;
		}

		void unlock( ID3D11DeviceContext * context
			, UINT subresource )const override
		{
			context->Unmap( m_texture, subresource );
		}

		inline ID3D11Texture3D * getImage()const
		{
			return m_texture;
		}

	private:
		void doInitTexDesc( VkImageCreateInfo const & createInfo
			, D3D11_TEXTURE3D_DESC & desc )
		{
			auto device = get( m_device )->getDevice();
			desc.Width = createInfo.extent.width;
			desc.Height = createInfo.extent.height;
			desc.Depth = createInfo.extent.depth;
			desc.Format = getTextureFormat( createInfo.format );
			desc.Usage = getImageUsage( m_propertyFlags, m_usage );
			desc.CPUAccessFlags = getCpuImageAccessFlags( m_propertyFlags, m_usage );
			desc.MipLevels = createInfo.mipLevels;
			desc.BindFlags = getBindFlags( createInfo, m_propertyFlags );
			desc.MiscFlags = getMiscFlags( createInfo );

			if ( get( m_device )->getFeatureLevel() < D3D_FEATURE_LEVEL_11_0 )
			{
				desc.BindFlags &= ~( D3D11_BIND_UNORDERED_ACCESS );
			}
		}

	private:
		VkImageUsageFlags m_usage;
		ID3D11Texture3D * m_texture{ nullptr };
	};

	//*********************************************************************************************

	DeviceMemory::DeviceMemoryImpl::DeviceMemoryImpl( VkDevice device
		, VkMemoryAllocateInfo allocateInfo )
		: m_device{ device }
		, m_allocateInfo{ std::move( allocateInfo ) }
		, m_propertyFlags{ getFlags( m_allocateInfo.memoryTypeIndex ) }
	{
	}

	void DeviceMemory::DeviceMemoryImpl::upload( ByteArray const & data
		, UINT subresource
		, VkDeviceSize offset
		, VkDeviceSize size )const
	{
		if ( size == WholeSize )
		{
			assert( offset == 0ull );
			size = m_allocateInfo.allocationSize;
		}

		ID3D11DeviceContext * context;
		get( m_device )->getDevice()->GetImmediateContext( &context );
		D3D11_MAPPED_SUBRESOURCE mapped{};

		if ( lock( context, subresource, mapped ) == VK_SUCCESS )
		{
			uint8_t * dst = static_cast< uint8_t * >( mapped.pData ) + offset;
			std::memcpy( dst, data.data() + offset, size );
			unlock( context, subresource );
		}

		safeRelease( context );
	}

	void DeviceMemory::DeviceMemoryImpl::download( ByteArray & data
		, UINT subresource
		, VkDeviceSize offset
		, VkDeviceSize size )const
	{
		if ( size == WholeSize )
		{
			assert( offset == 0ull );
			size = m_allocateInfo.allocationSize;
		}

		ID3D11DeviceContext * context;
		get( m_device )->getDevice()->GetImmediateContext( &context );
		D3D11_MAPPED_SUBRESOURCE mapped{};

		if ( lock( context, subresource, mapped ) == VK_SUCCESS )
		{
			uint8_t * dst = static_cast< uint8_t * >( mapped.pData ) + offset;
			std::memcpy( data.data() + offset, dst, size );
			unlock( context, subresource );
		}

		safeRelease( context );
	}

	//*********************************************************************************************

	DeviceMemory::DeviceMemory( VkDevice device
		, VkMemoryAllocateInfo allocateInfo )
		: m_device{ device }
		, m_allocateInfo{ std::move( allocateInfo ) }
		, m_propertyFlags{ getFlags( m_allocateInfo.memoryTypeIndex ) }
	{
		thread_local uint8_t count = 10u;

		if ( ashes::checkFlag( m_propertyFlags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) )
		{
			m_data.resize( allocateInfo.allocationSize, count++ );
		}
	}

	DeviceMemory::~DeviceMemory()
	{
		if ( m_impl )
		{
			onDestroy( get( this ) );
		}
	}

	VkResult DeviceMemory::bindToBuffer( VkBuffer buffer
		, VkDeviceSize memoryOffset
		, ID3D11Buffer *& retBuffer )
	{
		VkResult result = VK_ERROR_INITIALIZATION_FAILED;

		try
		{
			auto impl = std::make_unique< BufferDeviceMemory >( m_device
				, m_allocateInfo
				, get( buffer )->getUsage() );
			m_buffer = impl->getBuffer();
			retBuffer = m_buffer;
			m_impl = std::move( impl );
			upload( 0u, m_data.size() );
			result = VK_SUCCESS;
		}
		catch ( Exception & exc )
		{
			std::cerr << exc.what() << std::endl;
			result = exc.getResult();
		}
		catch ( std::exception & exc )
		{
			std::cerr << exc.what() << std::endl;
		}
		catch ( ... )
		{
			std::cerr << "Unknown error" << std::endl;
		}

		return result;
	}

	VkResult DeviceMemory::bindToImage( VkImage image
		, VkDeviceSize memoryOffset
		, ID3D11Texture1D *& texture )
	{
		VkResult result = VK_ERROR_INITIALIZATION_FAILED;

		try
		{
			auto impl = std::make_unique< Texture1DDeviceMemory >( m_device
				, m_allocateInfo
				, get( image )->getCreateInfo() );
			texture = impl->getImage();
			m_impl = std::move( impl );
			upload();
			result = VK_SUCCESS;
		}
		catch ( Exception & exc )
		{
			std::cerr << exc.what() << std::endl;
			result = exc.getResult();
		}
		catch ( std::exception & exc )
		{
			std::cerr << exc.what() << std::endl;
		}
		catch ( ... )
		{
			std::cerr << "Unknown error" << std::endl;
		}

		return result;
	}

	VkResult DeviceMemory::bindToImage( VkImage image
		, VkDeviceSize memoryOffset
		, ID3D11Texture2D *& texture )
	{
		VkResult result = VK_ERROR_INITIALIZATION_FAILED;

		try
		{
			auto impl = std::make_unique< Texture2DDeviceMemory >( m_device
				, m_allocateInfo
				, get( image )->getCreateInfo() );
			texture = impl->getImage();
			m_impl = std::move( impl );
			upload();
			result = VK_SUCCESS;
		}
		catch ( Exception & exc )
		{
			std::cerr << exc.what() << std::endl;
			result = exc.getResult();
		}
		catch ( std::exception & exc )
		{
			std::cerr << exc.what() << std::endl;
		}
		catch ( ... )
		{
			std::cerr << "Unknown error" << std::endl;
		}

		return result;
	}

	VkResult DeviceMemory::bindToImage( VkImage image
		, VkDeviceSize memoryOffset
		, ID3D11Texture3D *& texture )
	{
		VkResult result = VK_ERROR_INITIALIZATION_FAILED;

		try
		{
			auto impl = std::make_unique< Texture3DDeviceMemory >( m_device
				, m_allocateInfo
				, get( image )->getCreateInfo() );
			texture = impl->getImage();
			m_impl = std::move( impl );
			upload();
			result = VK_SUCCESS;
		}
		catch ( Exception & exc )
		{
			std::cerr << exc.what() << std::endl;
			result = exc.getResult();
		}
		catch ( std::exception & exc )
		{
			std::cerr << exc.what() << std::endl;
		}
		catch ( ... )
		{
			std::cerr << "Unknown error" << std::endl;
		}

		return result;
	}

	void DeviceMemory::updateUpload( VkDeviceSize offset
		, VkDeviceSize size
		, UINT subresource )const
	{
		if ( !m_data.empty() && m_impl )
		{
			m_impl->upload( m_data, subresource, offset, size );
			m_dirty = false;
		}
	}

	void DeviceMemory::updateDownload( VkDeviceSize offset
		, VkDeviceSize size
		, UINT subresource )const
	{
		if ( !m_data.empty() && m_impl )
		{
			m_impl->download( m_data, subresource, offset, size );
			m_dirty = false;
		}
	}

	void DeviceMemory::updateData( VkDeviceMemory src
		, VkDeviceSize srcOffset
		, VkDeviceSize dstOffset
		, VkDeviceSize size )
	{
		if ( !m_data.empty()
			&& !get( src )->m_data.empty() )
		{
			std::memcpy( &m_data[dstOffset]
				, &get( src )->m_data[srcOffset]
				, size );
		}
	}

	VkResult DeviceMemory::lock( VkDeviceSize offset
		, VkDeviceSize size
		, VkMemoryMapFlags flags
		, void ** data )const
	{
		assert( !m_mapped && "VkDeviceMemory should not be mapped" );
		assert( !m_data.empty() && "VkDeviceMemory should be mappable" );
		size = (size == ~( 0ull )
			? m_allocateInfo.allocationSize
			: size);

		if ( m_data.size() <= offset 
			|| m_data.size() < ( offset + size ) )
		{
			return VK_ERROR_OUT_OF_DEVICE_MEMORY;
		}

		*data = m_data.data() + offset;
		m_mappedOffset = offset;
		m_mappedSubresource = flags;
		m_mappedSize = size;
		m_mapped = *data != nullptr;
		m_dirty = true;
		return VK_SUCCESS;
	}

	VkResult DeviceMemory::flush( VkDeviceSize offset
		, VkDeviceSize size )const
	{
		assert( m_mapped && "VkDeviceMemory should be mapped" );
		assert( !m_data.empty() && "VkDeviceMemory should be mappable" );
		size = ( size == ~( 0ull )
			? m_allocateInfo.allocationSize
			: size );

		if ( m_data.size() <= offset
			|| m_data.size() < ( offset + size ) )
		{
			return VK_ERROR_OUT_OF_DEVICE_MEMORY;
		}

		updateUpload( offset, size, m_mappedSubresource );
		return VK_SUCCESS;
	}

	VkResult DeviceMemory::invalidate( VkDeviceSize offset
		, VkDeviceSize size )const
	{
		assert( m_mapped && "VkDeviceMemory should be mapped" );
		assert( !m_data.empty() && "VkDeviceMemory should be mappable" );
		size = ( size == ~( 0ull )
			? m_allocateInfo.allocationSize
			: size );

		if ( m_data.size() <= offset
			|| m_data.size() < ( offset + size ) )
		{
			return VK_ERROR_OUT_OF_DEVICE_MEMORY;
		}

		m_dirty = true;
		return VK_SUCCESS;
	}

	void DeviceMemory::unlock()const
	{
		assert( m_mapped && "VkDeviceMemory should be mapped" );
		assert( !m_data.empty() && "VkDeviceMemory should be mappable" );
		m_mapped = false;
		updateUpload( m_mappedOffset, m_mappedSize, m_mappedSubresource );
	}

	void DeviceMemory::upload( VkDeviceSize offset
		, VkDeviceSize size
		, UINT subresource )const
	{
		updateUpload( offset, size, subresource );
	}

	void DeviceMemory::download( VkDeviceSize offset
		, VkDeviceSize size
		, UINT subresource )const
	{
		updateDownload( offset, size, subresource );
	}

	//*********************************************************************************************
}
