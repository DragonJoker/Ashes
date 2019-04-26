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
				desc.Usage = getBufferUsage( m_flags, m_bufferTargets );
				desc.BindFlags = getBindFlags( m_bufferTargets );
				desc.CPUAccessFlags = getCpuBufferAccessFlags( m_flags, m_bufferTargets );
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

		VkResult lock( VkDeviceSize offset
			, VkDeviceSize size
			, void ** data )const override
		{
			ID3D11DeviceContext * context;
			get( m_device )->getDevice()->GetImmediateContext( &context );
			D3D11_MAPPED_SUBRESOURCE mappedResource;

			HRESULT hr = context->Map( m_buffer
				, 0
				, getBufferMapFlags( m_flags, m_bufferTargets )
				, 0
				, &mappedResource );
			safeRelease( context );
			VkResult result = VK_ERROR_MEMORY_MAP_FAILED;

			if ( checkError( m_device, hr, "ID3D11Buffer::Lock" ) )
			{
				*data = static_cast< uint8_t * >( mappedResource.pData ) + offset;
				result = VK_SUCCESS;
			}

			return result;
		}

		VkResult flush( VkDeviceSize offset
			, VkDeviceSize size )const override
		{
			return VK_SUCCESS;
		}

		VkResult invalidate( VkDeviceSize offset
			, VkDeviceSize size )const override
		{
			return VK_SUCCESS;
		}

		void unlock()const override
		{
			ID3D11DeviceContext * context;
			get( m_device )->getDevice()->GetImmediateContext( &context );
			context->Unmap( m_buffer, 0 );
			safeRelease( context );
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

	UINT getBindFlags( VkImageCreateInfo const & createInfo )
	{
		return ( ( isSampled( createInfo.usage )
				? D3D11_BIND_SHADER_RESOURCE
				: 0 )
			| ( isRenderable( createInfo.usage, createInfo.format, createInfo.mipLevels )
				? ( isDepthOrStencilFormat( createInfo.format )
					? D3D11_BIND_DEPTH_STENCIL
					: D3D11_BIND_RENDER_TARGET )
				: 0 )
			| ( isStorage( createInfo.usage )
				? D3D11_BIND_UNORDERED_ACCESS
				: 0 ) );
	}

	UINT getMiscFlags( VkImageCreateInfo const & createInfo )
	{
		return ( isMipmapped( createInfo.usage, createInfo.format, createInfo.mipLevels )
				? D3D11_RESOURCE_MISC_GENERATE_MIPS
				: 0 )
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

		VkResult lock( VkDeviceSize offset
			, VkDeviceSize size
			, void ** data )const override
		{
			ID3D11DeviceContext * context;
			get( m_device )->getDevice()->GetImmediateContext( &context );
			D3D11_MAPPED_SUBRESOURCE mappedResource;

			HRESULT hr = context->Map( m_texture
				, 0
				, getImageMapFlags( m_flags, m_usage )
				, 0
				, &mappedResource );
			safeRelease( context );
			VkResult result = VK_ERROR_MEMORY_MAP_FAILED;

			if ( checkError( m_device, hr, "LockTexture1D" ) )
			{
				*data = mappedResource.pData;
				result = VK_SUCCESS;
			}

			return result;
		}

		VkResult flush( VkDeviceSize offset
			, VkDeviceSize size )const override
		{
			return VK_SUCCESS;
		}

		VkResult invalidate( VkDeviceSize offset
			, VkDeviceSize size )const override
		{
			return VK_SUCCESS;
		}

		void unlock()const override
		{
			ID3D11DeviceContext * context;
			get( m_device )->getDevice()->GetImmediateContext( &context );
			context->Unmap( m_texture, 0 );
			safeRelease( context );
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
			desc.Usage = getImageUsage( m_flags, m_usage );
			desc.CPUAccessFlags = getCpuImageAccessFlags( m_flags, m_usage );
			desc.MipLevels = createInfo.mipLevels;
			desc.BindFlags = getBindFlags( createInfo );
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

		VkResult lock( VkDeviceSize offset
			, VkDeviceSize size
			, void ** data )const override
		{
			ID3D11DeviceContext * context;
			get( m_device )->getDevice()->GetImmediateContext( &context );
			D3D11_MAPPED_SUBRESOURCE mappedResource;

			HRESULT hr = context->Map( m_texture
				, 0
				, getImageMapFlags( m_flags, m_usage )
				, 0
				, &mappedResource );
			safeRelease( context );
			VkResult result = VK_ERROR_MEMORY_MAP_FAILED;

			if ( checkError( m_device, hr, "LockTexture2D" ) )
			{
				*data = mappedResource.pData;
				result = VK_SUCCESS;
			}

			return result;
		}

		VkResult flush( VkDeviceSize offset
			, VkDeviceSize size )const override
		{
			return VK_SUCCESS;
		}

		VkResult invalidate( VkDeviceSize offset
			, VkDeviceSize size )const override
		{
			return VK_SUCCESS;
		}

		void unlock()const override
		{
			ID3D11DeviceContext * context;
			get( m_device )->getDevice()->GetImmediateContext( &context );
			context->Unmap( m_texture, 0 );
			safeRelease( context );
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
			desc.Usage = getImageUsage( m_flags, m_usage );
			desc.CPUAccessFlags = getCpuImageAccessFlags( m_flags, m_usage );
			desc.MipLevels = createInfo.mipLevels;
			desc.SampleDesc.Count = UINT( createInfo.samples );
			desc.SampleDesc.Quality = 0;
			desc.BindFlags = getBindFlags( createInfo );
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

		VkResult lock( VkDeviceSize offset
			, VkDeviceSize size
			, void ** data )const override
		{
			ID3D11DeviceContext * context;
			get( m_device )->getDevice()->GetImmediateContext( &context );
			D3D11_MAPPED_SUBRESOURCE mappedResource;

			HRESULT hr = context->Map( m_texture
				, 0
				, getImageMapFlags( m_flags, m_usage )
				, 0
				, &mappedResource );
			safeRelease( context );
			VkResult result = VK_ERROR_MEMORY_MAP_FAILED;

			if ( checkError( m_device, hr, "LockTexture3D" ) )
			{
				*data = mappedResource.pData;
				result = VK_SUCCESS;
			}

			return result;
		}

		VkResult flush( VkDeviceSize offset
			, VkDeviceSize size )const override
		{
			return VK_SUCCESS;
		}

		VkResult invalidate( VkDeviceSize offset
			, VkDeviceSize size )const override
		{
			return VK_SUCCESS;
		}

		void unlock()const override
		{
			ID3D11DeviceContext * context;
			get( m_device )->getDevice()->GetImmediateContext( &context );
			context->Unmap( m_texture, 0 );
			safeRelease( context );
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
			desc.Usage = getImageUsage( m_flags, m_usage );
			desc.CPUAccessFlags = getCpuImageAccessFlags( m_flags, m_usage );
			desc.MipLevels = createInfo.mipLevels;
			desc.BindFlags = getBindFlags( createInfo );
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
		, m_flags{ getFlags( m_allocateInfo.memoryTypeIndex ) }
	{
	}

	void DeviceMemory::DeviceMemoryImpl::upload( ByteArray const & data
		, VkDeviceSize offset
		, VkDeviceSize size )const
	{
		if ( size == WholeSize )
		{
			assert( offset == 0ull );
			size = m_allocateInfo.allocationSize;
		}

		void * dst{ nullptr };

		if ( lock( offset, size, &dst ) == VK_SUCCESS )
		{
			std::memcpy( dst, data.data() + offset, size );
			unlock();
		}
	}

	void DeviceMemory::DeviceMemoryImpl::download( ByteArray & data
		, VkDeviceSize offset
		, VkDeviceSize size )const
	{
		if ( size == WholeSize )
		{
			assert( offset == 0ull );
			size = m_allocateInfo.allocationSize;
		}

		void * src{ nullptr };

		if ( lock( offset, size, &src ) == VK_SUCCESS )
		{
			std::memcpy( data.data() + offset, src, size );
			unlock();
		}
	}

	//*********************************************************************************************

	DeviceMemory::DeviceMemory( VkDevice device
		, VkMemoryAllocateInfo allocateInfo )
		: m_device{ device }
		, m_allocateInfo{ std::move( allocateInfo ) }
		, m_flags{ getFlags( m_allocateInfo.memoryTypeIndex ) }
	{
		if ( ashes::checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) )
		{
			m_data.resize( allocateInfo.allocationSize );
		}
	}

	DeviceMemory::~DeviceMemory()
	{
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
			retBuffer = impl->getBuffer();
			m_impl = std::move( impl );
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

	void DeviceMemory::upload( VkDeviceSize offset
		, VkDeviceSize size )const
	{
		if ( m_dirty )
		{
			m_impl->upload( m_data, offset, size );
			m_dirty = false;
		}
	}

	void DeviceMemory::download( VkDeviceSize offset
		, VkDeviceSize size )const
	{
		if ( m_dirty )
		{
			m_impl->download( m_data, offset, size );
			m_dirty = false;
		}
	}

	VkResult DeviceMemory::lock( VkDeviceSize offset
		, VkDeviceSize size
		, VkMemoryMapFlags flags
		, void ** data )const
	{
		assert( !m_mapped && "VkDeviceMemory should not be mapped" );
		*data = m_data.data() + offset;
		m_mappedOffset = offset;
		m_mappedSize = size == ~( 0ull )
			? m_allocateInfo.allocationSize
			: size;
		m_mapped = *data != nullptr;
		m_dirty = true;
		return VK_SUCCESS;
	}

	VkResult DeviceMemory::flush( VkDeviceSize offset
		, VkDeviceSize size )const
	{
		assert( m_mapped && "VkDeviceMemory should be mapped" );
		upload( offset, size );
		return VK_SUCCESS;
	}

	VkResult DeviceMemory::invalidate( VkDeviceSize offset
		, VkDeviceSize size )const
	{
		assert( m_mapped && "VkDeviceMemory should be mapped" );
		m_dirty = true;
		return VK_SUCCESS;
	}

	void DeviceMemory::unlock()const
	{
		assert( m_mapped && "VkDeviceMemory should be mapped" );
		m_mapped = false;
		upload( m_mappedOffset, m_mappedSize );
	}

	//*********************************************************************************************
}
