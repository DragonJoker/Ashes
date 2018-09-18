/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "Miscellaneous/D3D11DeviceMemory.hpp"

#include "Core/D3D11Device.hpp"
#include "Core/D3D11PhysicalDevice.hpp"

namespace d3d11_renderer
{
	//*********************************************************************************************

	class BufferDeviceMemory
		:  public DeviceMemory::DeviceMemoryImpl
	{
	public:
		BufferDeviceMemory( Device const & device
			, ashes::MemoryRequirements const & requirements
			, ashes::MemoryPropertyFlags flags
			, ashes::BufferTargets targets )
			: DeviceMemory::DeviceMemoryImpl{ device, requirements, flags }
		{
			auto d3ddevice = m_device.getDevice();
			UINT size = UINT( m_requirements.size );
			m_bufferTargets = targets;

			if ( size > 0 )
			{
				D3D11_BUFFER_DESC desc{};
				desc.ByteWidth = size;
				desc.Usage = getUsage( m_flags, m_bufferTargets );
				desc.BindFlags = convert( m_bufferTargets );
				desc.CPUAccessFlags = getCpuAccessFlags( m_flags, m_bufferTargets );
				desc.MiscFlags = 0;
				desc.StructureByteStride = 0;

				auto hr = d3ddevice->CreateBuffer( &desc, nullptr, &m_buffer );

				if ( dxCheckError( hr, "CreateBuffer" ) )
				{
					dxDebugName( m_buffer, Buffer );
				}
			}
		}

		~BufferDeviceMemory()noexcept
		{
			safeRelease( m_buffer );
		}

		uint8_t * lock( uint32_t offset
			, uint32_t size
			, ashes::MemoryMapFlags flags )const override
		{
			ID3D11DeviceContext * context;
			m_device.getDevice()->GetImmediateContext( &context );
			D3D11_MAPPED_SUBRESOURCE mappedResource;

			HRESULT hr = context->Map( m_buffer
				, 0
				, convert( flags, m_flags, m_bufferTargets )
				, 0
				, &mappedResource );
			safeRelease( context );
			uint8_t * result{ nullptr };

			if ( dxCheckError( hr, "ID3D11Buffer::Lock" ) )
			{
				result = static_cast< uint8_t * >( mappedResource.pData );
			}

			return result;
		}

		void flush( uint32_t offset
			, uint32_t size )const override
		{
		}

		void invalidate( uint32_t offset
			, uint32_t size )const override
		{
		}

		void unlock()const override
		{
			ID3D11DeviceContext * context;
			m_device.getDevice()->GetImmediateContext( &context );
			context->Unmap( m_buffer, 0 );
			safeRelease( context );
		}

		inline ID3D11Buffer * getBuffer()const
		{
			return m_buffer;
		}

	private:
		ashes::BufferTargets m_bufferTargets;
		ID3D11Buffer * m_buffer{ nullptr };
	};
	
	//*********************************************************************************************

	UINT getBindFlags( ashes::ImageCreateInfo const & createInfo )
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

	UINT getMiscFlags( ashes::ImageCreateInfo const & createInfo )
	{
		return ( isMipmapped( createInfo.usage, createInfo.format, createInfo.mipLevels )
				? D3D11_RESOURCE_MISC_GENERATE_MIPS
				: 0 )
			| ( checkFlag( createInfo.flags, ashes::ImageCreateFlag::eCubeCompatible )
				? D3D11_RESOURCE_MISC_TEXTURECUBE
				: 0u );
	}

	//*********************************************************************************************

	class Texture1DDeviceMemory
		: public DeviceMemory::DeviceMemoryImpl
	{
	public:
		Texture1DDeviceMemory( Device const & device
			, ashes::MemoryRequirements const & requirements
			, ashes::MemoryPropertyFlags flags
			, ashes::ImageCreateInfo const & createInfo )
			: DeviceMemory::DeviceMemoryImpl{ device, requirements, flags }
			, m_usage{ createInfo.usage }
		{
			auto d3ddevice = m_device.getDevice();
			D3D11_TEXTURE1D_DESC desc{};
			doInitTexDesc( createInfo, desc );
			HRESULT hr = d3ddevice->CreateTexture1D( &desc
				, nullptr
				, &m_texture );

			if ( dxCheckError( hr, "CreateTexture1D" ) )
			{
				dxDebugName( m_texture, Texture1D );
			}
		}

		~Texture1DDeviceMemory()noexcept
		{
		}

		uint8_t * lock( uint32_t offset
			, uint32_t size
			, ashes::MemoryMapFlags flags )const override
		{
			ID3D11DeviceContext * context;
			m_device.getDevice()->GetImmediateContext( &context );
			D3D11_MAPPED_SUBRESOURCE mappedResource;

			HRESULT hr = context->Map( m_texture
				, 0
				, convert( flags, m_flags, m_usage )
				, 0
				, &mappedResource );
			safeRelease( context );
			uint8_t * result{ nullptr };

			if ( dxCheckError( hr, "LockTexture1D" ) )
			{
				result = static_cast< uint8_t * >( mappedResource.pData );
			}

			return result;
		}

		void flush( uint32_t offset
			, uint32_t size )const override
		{
		}

		void invalidate( uint32_t offset
			, uint32_t size )const override
		{
		}

		void unlock()const override
		{
			ID3D11DeviceContext * context;
			m_device.getDevice()->GetImmediateContext( &context );
			context->Unmap( m_texture, 0 );
			safeRelease( context );
		}

		inline ID3D11Texture1D * getTexture()const
		{
			return m_texture;
		}

	private:
		void doInitTexDesc( ashes::ImageCreateInfo const & createInfo
			, D3D11_TEXTURE1D_DESC & desc )
		{
			auto device = m_device.getDevice();
			desc.Width = createInfo.extent.width;
			desc.Format = getTextureFormat( createInfo.format );
			desc.ArraySize = createInfo.arrayLayers;
			desc.Usage = getUsage( m_flags, m_usage );
			desc.CPUAccessFlags = getCpuAccessFlags( m_flags, m_usage );
			desc.MipLevels = createInfo.mipLevels;
			desc.BindFlags = getBindFlags( createInfo );
			desc.MiscFlags = getMiscFlags( createInfo );
		}

	private:
		ashes::ImageUsageFlags m_usage;
		ID3D11Texture1D * m_texture{ nullptr };
	};

	//*********************************************************************************************

	class Texture2DDeviceMemory
		: public DeviceMemory::DeviceMemoryImpl
	{
	public:
		Texture2DDeviceMemory( Device const & device
			, ashes::MemoryRequirements const & requirements
			, ashes::MemoryPropertyFlags flags
			, ashes::ImageCreateInfo const & createInfo )
			: DeviceMemory::DeviceMemoryImpl{ device, requirements, flags }
			, m_usage{ createInfo.usage }
		{
			auto d3ddevice = m_device.getDevice();
			D3D11_TEXTURE2D_DESC desc{};
			doInitTexDesc( createInfo, desc );
			HRESULT hr = d3ddevice->CreateTexture2D( &desc
				, nullptr
				, &m_texture );

			if ( dxCheckError( hr, "CreateTexture2D" ) )
			{
				dxDebugName( m_texture, Texture2D );
			}
		}

		~Texture2DDeviceMemory()noexcept
		{
		}

		uint8_t * lock( uint32_t offset
			, uint32_t size
			, ashes::MemoryMapFlags flags )const override
		{
			ID3D11DeviceContext * context;
			m_device.getDevice()->GetImmediateContext( &context );
			D3D11_MAPPED_SUBRESOURCE mappedResource;

			HRESULT hr = context->Map( m_texture
				, 0
				, convert( flags, m_flags, m_usage )
				, 0
				, &mappedResource );
			safeRelease( context );
			uint8_t * result{ nullptr };

			if ( dxCheckError( hr, "LockTexture2D" ) )
			{
				result = static_cast< uint8_t * >( mappedResource.pData );
			}

			return result;
		}

		void flush( uint32_t offset
			, uint32_t size )const override
		{
		}

		void invalidate( uint32_t offset
			, uint32_t size )const override
		{
		}

		void unlock()const override
		{
			ID3D11DeviceContext * context;
			m_device.getDevice()->GetImmediateContext( &context );
			context->Unmap( m_texture, 0 );
			safeRelease( context );
		}

		inline ID3D11Texture2D * getTexture()const
		{
			return m_texture;
		}

	private:
		void doInitTexDesc( ashes::ImageCreateInfo const & createInfo
			, D3D11_TEXTURE2D_DESC & desc )
		{
			auto device = m_device.getDevice();
			desc.Width = createInfo.extent.width;
			desc.Height = createInfo.extent.height;
			desc.Format = getTextureFormat( createInfo.format );
			desc.ArraySize = createInfo.arrayLayers;
			desc.Usage = getUsage( m_flags, m_usage );
			desc.CPUAccessFlags = getCpuAccessFlags( m_flags, m_usage );
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
		}

	private:
		ashes::ImageUsageFlags m_usage;
		ID3D11Texture2D * m_texture{ nullptr };
	};
	
	//*********************************************************************************************

	class Texture3DDeviceMemory
		: public DeviceMemory::DeviceMemoryImpl
	{
	public:
		Texture3DDeviceMemory( Device const & device
			, ashes::MemoryRequirements const & requirements
			, ashes::MemoryPropertyFlags flags
			, ashes::ImageCreateInfo const & createInfo )
			: DeviceMemory::DeviceMemoryImpl{ device, requirements, flags }
			, m_usage{ createInfo.usage }
		{
			auto d3ddevice = m_device.getDevice();
			D3D11_TEXTURE3D_DESC desc{};
			doInitTexDesc( createInfo, desc );
			HRESULT hr = d3ddevice->CreateTexture3D( &desc
				, nullptr
				, &m_texture );

			if ( dxCheckError( hr, "CreateTexture3D" ) )
			{
				dxDebugName( m_texture, Texture3D );
			}
		}

		~Texture3DDeviceMemory()noexcept
		{
		}

		uint8_t * lock( uint32_t offset
			, uint32_t size
			, ashes::MemoryMapFlags flags )const override
		{
			ID3D11DeviceContext * context;
			m_device.getDevice()->GetImmediateContext( &context );
			D3D11_MAPPED_SUBRESOURCE mappedResource;

			HRESULT hr = context->Map( m_texture
				, 0
				, convert( flags, m_flags, m_usage )
				, 0
				, &mappedResource );
			safeRelease( context );
			uint8_t * result{ nullptr };

			if ( dxCheckError( hr, "LockTexture3D" ) )
			{
				result = static_cast< uint8_t * >( mappedResource.pData );
			}

			return result;
		}

		void flush( uint32_t offset
			, uint32_t size )const override
		{
		}

		void invalidate( uint32_t offset
			, uint32_t size )const override
		{
		}

		void unlock()const override
		{
			ID3D11DeviceContext * context;
			m_device.getDevice()->GetImmediateContext( &context );
			context->Unmap( m_texture, 0 );
			safeRelease( context );
		}

		inline ID3D11Texture3D * getTexture()const
		{
			return m_texture;
		}

	private:
		void doInitTexDesc( ashes::ImageCreateInfo const & createInfo
			, D3D11_TEXTURE3D_DESC & desc )
		{
			auto device = m_device.getDevice();
			desc.Width = createInfo.extent.width;
			desc.Height = createInfo.extent.height;
			desc.Depth = createInfo.extent.depth;
			desc.Format = getTextureFormat( createInfo.format );
			desc.Usage = getUsage( m_flags, m_usage );
			desc.CPUAccessFlags = getCpuAccessFlags( m_flags, m_usage );
			desc.MipLevels = createInfo.mipLevels;
			desc.BindFlags = getBindFlags( createInfo );
			desc.MiscFlags = getMiscFlags( createInfo );
		}

	private:
		ashes::ImageUsageFlags m_usage;
		ID3D11Texture3D * m_texture{ nullptr };
	};

	//*********************************************************************************************

	DeviceMemory::DeviceMemoryImpl::DeviceMemoryImpl( Device const & device
		, ashes::MemoryRequirements const & requirements
		, ashes::MemoryPropertyFlags flags )
		: m_device{ device }
		, m_requirements{ requirements }
		, m_flags{ flags }
	{
	}

	//*********************************************************************************************

	DeviceMemory::DeviceMemory( Device const & device
		, ashes::MemoryRequirements const & requirements
		, ashes::MemoryPropertyFlags flags )
		: ashes::DeviceMemory{ device, flags }
		, m_device{ device }
		, m_requirements{ requirements }
	{
	}

	DeviceMemory::~DeviceMemory()
	{
	}

	uint8_t * DeviceMemory::lock( uint32_t offset
		, uint32_t size
		, ashes::MemoryMapFlags flags )const
	{
		return m_impl->lock( offset, size, flags );
	}

	void DeviceMemory::flush( uint32_t offset
		, uint32_t size )const
	{
		m_impl->flush( offset, size );
	}

	void DeviceMemory::invalidate( uint32_t offset
		, uint32_t size )const
	{
		m_impl->invalidate( offset, size );
	}

	void DeviceMemory::unlock()const
	{
		m_impl->unlock();
	}

	ID3D11Buffer * DeviceMemory::bindToBuffer( ashes::BufferTargets targets )
	{
		auto impl = std::make_unique< BufferDeviceMemory >( m_device
			, m_requirements
			, m_flags
			, targets );
		ID3D11Buffer * result = impl->getBuffer();
		m_impl = std::move( impl );
		return result;
	}

	ID3D11Texture1D * DeviceMemory::bindToTexture1D( ashes::ImageCreateInfo const & createInfo )
	{
		auto impl = std::make_unique< Texture1DDeviceMemory >( m_device
			, m_requirements
			, m_flags
			, createInfo );
		ID3D11Texture1D * result = impl->getTexture();
		m_impl = std::move( impl );
		return result;
	}

	ID3D11Texture2D * DeviceMemory::bindToTexture2D( ashes::ImageCreateInfo const & createInfo )
	{
		auto impl = std::make_unique< Texture2DDeviceMemory >( m_device
			, m_requirements
			, m_flags
			, createInfo );
		ID3D11Texture2D * result = impl->getTexture();
		m_impl = std::move( impl );
		return result;
	}

	ID3D11Texture3D * DeviceMemory::bindToTexture3D( ashes::ImageCreateInfo const & createInfo )
	{
		auto impl = std::make_unique< Texture3DDeviceMemory >( m_device
			, m_requirements
			, m_flags
			, createInfo );
		ID3D11Texture3D * result = impl->getTexture();
		m_impl = std::move( impl );
		return result;
	}

	//*********************************************************************************************
}
