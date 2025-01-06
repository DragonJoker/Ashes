/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "Miscellaneous/XBoxDeviceMemory.hpp"

#include "Core/XBoxDevice.hpp"
#include "Core/XBoxInstance.hpp"
#include "Core/XBoxPhysicalDevice.hpp"

#include <ashes/common/Exception.hpp>

#include "ashesxbox_api.hpp"

#include <cstdlib>

#pragma warning( push )
#pragma warning( disable: 5262 )
#include <iostream>
#pragma warning( pop )

namespace ashes::xbox
{
	//*********************************************************************************************

	namespace mem
	{
		static VkMemoryPropertyFlags getMemoryProperties( uint32_t memoryTypeIndex )
		{
			assert( memoryTypeIndex < Instance::getMemoryProperties().memoryTypeCount
				&& "Wrong deduced memory type" );
			return Instance::getMemoryProperties().memoryTypes[memoryTypeIndex].propertyFlags;
		}

		static UINT getBindFlags( VkImageCreateInfo const & createInfo
			, VkMemoryPropertyFlags propertyFlags )
		{
			return ( ( isSampled( createInfo.usage )
					? D3D11_BIND_SHADER_RESOURCE
					: 0u )
				| ( isRenderable( createInfo.usage, createInfo.format, createInfo.mipLevels )
					? ( isHostVisible( propertyFlags )
						? 0u
						: ( isDepthOrStencilFormat( createInfo.format )
							? D3D11_BIND_DEPTH_STENCIL
							: D3D11_BIND_RENDER_TARGET ) )
					: 0u )
				| ( isStorage( createInfo.usage )
					? D3D11_BIND_UNORDERED_ACCESS
					: 0 ) );
		}

		static UINT getMiscFlags( VkImageCreateInfo const & createInfo )
		{
			return 0u
				| ( checkFlag( createInfo.flags, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT )
					? D3D11_RESOURCE_MISC_TEXTURECUBE
					: 0u );
		}
	}

	//*********************************************************************************************

	ObjectMemory::ObjectMemory( VkDevice device
		, VkDeviceMemory deviceMemory
		, ID3D11Resource * resource
		, VkDeviceSize offset
		, VkMemoryAllocateInfo allocateInfo
		, D3D11_MAP mapFlags )
		: device{ device }
		, deviceMemory{ deviceMemory }
		, resource{ resource }
		, offset{ offset }
		, allocateInfo{ std::move( allocateInfo ) }
		, mapFlags{ mapFlags }
	{
	}

	ObjectMemory::ObjectMemory( ObjectMemory && rhs )noexcept
		: device{ rhs.device }
		, deviceMemory{ rhs.deviceMemory }
		, resource{ rhs.resource }
		, offset{ rhs.offset }
		, allocateInfo{ std::move( rhs.allocateInfo ) }
		, mapFlags{ rhs.mapFlags }
		, subresources{ std::move( rhs.subresources ) }
	{
		rhs.device = nullptr;
		rhs.deviceMemory = nullptr;
		rhs.resource = nullptr;
		rhs.offset = 0;
		rhs.mapFlags = D3D11_MAP( 0 );
	}

	ObjectMemory & ObjectMemory::operator=( ObjectMemory && rhs )noexcept
	{
		device = rhs.device;
		deviceMemory = rhs.deviceMemory;
		resource = rhs.resource;
		offset = rhs.offset;
		allocateInfo = std::move( rhs.allocateInfo );
		mapFlags = rhs.mapFlags;
		subresources = std::move( rhs.subresources );

		rhs.device = nullptr;
		rhs.deviceMemory = nullptr;
		rhs.resource = nullptr;
		rhs.offset = 0;
		rhs.mapFlags = D3D11_MAP( 0 );

		return *this;
	}

	ObjectMemory::~ObjectMemory()noexcept
	{
		safeRelease( resource );
	}

	VkResult ObjectMemory::lock( ID3D11DeviceContext * context
		, UINT subresource
		, D3D11_MAPPED_SUBRESOURCE & data )const
	{
		if ( HRESULT hr = context->Map( resource
				, subresource
				, mapFlags
				, 0
				, &data );
			!checkError( device, hr, "LockMemory" ) )
		{
			return VK_ERROR_MEMORY_MAP_FAILED;
		}

		return VK_SUCCESS;
	}

	void ObjectMemory::unlock( ID3D11DeviceContext * context
		, UINT subresource )const
	{
		context->Unmap( resource, subresource );
	}

	void ObjectMemory::upload( uint8_t const * data
		, UINT subresource
		, VkDeviceSize poffset
		, VkDeviceSize psize )const
	{
		assert( subresource < subresources.size() );
		auto & subresourceLayout = subresources[subresource];
		auto maxOffset = std::max( offset, poffset );
		auto objectOffset = maxOffset - offset;
		auto copySize = VkDeviceSize( psize - std::abs( int64_t( poffset ) - int64_t( offset ) ) );

		if ( psize == WholeSize )
		{
			assert( poffset == 0ull );
			copySize = allocateInfo.allocationSize;
		}
		else if ( copySize > allocateInfo.allocationSize )
		{
			copySize = allocateInfo.allocationSize - objectOffset;
		}

		auto context{ get( device )->getImmediateContext() };
		D3D11_MAPPED_SUBRESOURCE mapped{};

		if ( lock( *context, subresource, mapped ) == VK_SUCCESS )
		{
			assert( mapped.pData != nullptr );
			auto dstData = static_cast< uint8_t * >( mapped.pData ) + objectOffset;
			assert( dstData != nullptr );

			if ( mapped.DepthPitch == subresourceLayout.SysMemSlicePitch
				|| mapped.RowPitch == subresourceLayout.SysMemPitch )
			{
				copySize = std::min( copySize, VkDeviceSize( mapped.DepthPitch ) );
				std::memcpy( dstData, data + maxOffset, copySize );
			}
			else
			{
				auto mappedSteps = mapped.DepthPitch / mapped.RowPitch;
				auto systemSteps = subresourceLayout.SysMemSlicePitch / subresourceLayout.SysMemPitch;
				auto rowSize = std::min( mapped.RowPitch, subresourceLayout.SysMemPitch );
				assert( mappedSteps == systemSteps );
				auto src = data + maxOffset;
				auto dst = dstData;

				for ( UINT step = 0u; step < std::min( mappedSteps, systemSteps ); ++step )
				{
					std::memcpy( dst, src, rowSize );
					src += subresourceLayout.SysMemPitch;
					dst += mapped.RowPitch;
				}
			}

			unlock( *context, subresource );
		}
	}

	void ObjectMemory::download( uint8_t * data
		, UINT subresource
		, VkDeviceSize poffset
		, VkDeviceSize psize )const
	{
		assert( subresource < subresources.size() );
		auto & subresourceLayout = subresources[subresource];
		auto maxOffset = std::max( this->offset, poffset );
		auto objectOffset = maxOffset - this->offset;
		auto copySize = VkDeviceSize( psize - std::abs( int64_t( poffset ) - int64_t( this->offset ) ) );

		if ( psize == WholeSize )
		{
			assert( poffset == 0ull );
			objectOffset = 0u;
			copySize = allocateInfo.allocationSize;
		}
		else if ( copySize > allocateInfo.allocationSize )
		{
			copySize = allocateInfo.allocationSize - objectOffset;
		}

		auto context{ get( device )->getImmediateContext() };
		D3D11_MAPPED_SUBRESOURCE mapped{};

		if ( lock( *context, subresource, mapped ) == VK_SUCCESS )
		{
			assert( mapped.pData != nullptr );
			auto srcData = static_cast< uint8_t const * >( mapped.pData ) + objectOffset;
			assert( srcData != nullptr );

			if ( mapped.RowPitch != subresourceLayout.SysMemPitch )
			{
				auto mappedSteps = mapped.DepthPitch / mapped.RowPitch;
				auto systemSteps = subresourceLayout.SysMemSlicePitch / subresourceLayout.SysMemPitch;
				auto rowSize = std::min( mapped.RowPitch, subresourceLayout.SysMemPitch );
				assert( mappedSteps == systemSteps );
				auto src = srcData;
				auto dst = data + maxOffset;

				for ( UINT step = 0u; step < std::min( mappedSteps, systemSteps ); ++step )
				{
					std::memcpy( dst, src, rowSize );
					src += mapped.RowPitch;
					dst += subresourceLayout.SysMemPitch;
				}
			}
			else
			{
				copySize = std::min( copySize, VkDeviceSize( mapped.DepthPitch ) );
				std::memcpy( data + maxOffset, srcData, copySize );
			}

			unlock( *context, subresource );
		}
	}

	//*********************************************************************************************

	class BufferDeviceMemory
	{
	public:
		BufferDeviceMemory( VkDevice device
			, VkDeviceMemory parent
			, VkDeviceSize offset
			, VkMemoryAllocateInfo allocateInfo
			, VkBufferUsageFlags targets )
			: m_bufferTargets{ targets }
			, m_propertyFlags{ mem::getMemoryProperties( allocateInfo.memoryTypeIndex ) }
			, m_buffer{ doCreateBuffer( device, allocateInfo ) }
			, memory{ device, parent, m_buffer, offset, std::move( allocateInfo ), getBufferMapFlags( m_propertyFlags, m_bufferTargets ) }
		{
			memory.subresources.push_back( { nullptr, UINT( memory.allocateInfo.allocationSize ), UINT( memory.allocateInfo.allocationSize ) } );
		}

		inline ID3D11Buffer * getBuffer()const
		{
			return m_buffer;
		}

	private:
		ID3D11Buffer * doCreateBuffer( VkDevice device
			, VkMemoryAllocateInfo const & allocateInfo )const
		{
			ID3D11Buffer * result{ nullptr };
			auto d3ddevice = get( device )->getDevice();

			if ( auto size = UINT( allocateInfo.allocationSize );
				size > 0 )
			{
				D3D11_BUFFER_DESC desc{};
				desc.ByteWidth = size;
				desc.Usage = getBufferUsage( m_propertyFlags, m_bufferTargets );
				desc.BindFlags = getBindFlags( m_bufferTargets );
				desc.CPUAccessFlags = getCpuBufferAccessFlags( m_propertyFlags, m_bufferTargets );
				desc.MiscFlags = ( checkFlag( m_bufferTargets, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT )
					? D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS
					: 0u );
				desc.StructureByteStride = 0;

				auto hr = d3ddevice->CreateBuffer( &desc, nullptr, &result );

				if ( checkError( device, hr, "CreateBuffer" ) )
				{
					dxDebugName( result, Buffer );
				}
			}

			return result;
		}

	private:
		VkBufferUsageFlags m_bufferTargets;
		VkMemoryPropertyFlags m_propertyFlags;
		ID3D11Buffer * m_buffer{ nullptr };

	public:
		ObjectMemory memory;
	};
	
	//*********************************************************************************************

	class Texture1DDeviceMemory
	{
	public:
		Texture1DDeviceMemory( VkDevice device
			, VkDeviceMemory parent
			, VkDeviceSize offset
			, VkMemoryAllocateInfo allocateInfo
			, VkImageCreateInfo const & createInfo )
			: m_usage{ createInfo.usage }
			, m_propertyFlags{ mem::getMemoryProperties( allocateInfo.memoryTypeIndex ) }
			, m_texture{ doCreateTexture( device, createInfo ) }
			, memory{ device, parent, m_texture, offset, std::move( allocateInfo ), getImageMapFlags( m_propertyFlags, m_usage ) }
		{
			auto extent = getTexelBlockExtent( createInfo.format );
			auto byteSize = getTexelBlockByteSize( extent, createInfo.format );
			memory.subresources.resize( size_t( createInfo.arrayLayers ) * createInfo.mipLevels );

			for ( uint32_t layer = 0u; layer < createInfo.arrayLayers; ++layer )
			{
				for ( uint32_t level = 0u; level < createInfo.mipLevels; ++level )
				{
					auto mipWidth = getSubresourceValue( createInfo.extent.width, level );
					auto mipHeight = getSubresourceValue( createInfo.extent.height, level );
					auto rowPitch = byteSize * mipWidth / ( extent.width * extent.height * extent.depth );
					auto depthPitch = rowPitch * mipHeight * extent.height / ( extent.width * extent.depth );
					memory.subresources[D3D11CalcSubresource( level, layer, createInfo.mipLevels )] = { nullptr, rowPitch, depthPitch };
				}
			}
		}

		inline ID3D11Texture1D * getImage()const
		{
			return m_texture;
		}

	private:
		ID3D11Texture1D * doCreateTexture( VkDevice device
			, VkImageCreateInfo const & createInfo )const
		{
			ID3D11Texture1D * result{};
			getImageMapFlags( m_propertyFlags, m_usage );
			auto d3ddevice = get( device )->getDevice();
			D3D11_TEXTURE1D_DESC desc{};
			doInitTexDesc( device, createInfo, desc );

			if ( HRESULT hr = d3ddevice->CreateTexture1D( &desc
					, nullptr
					, &result );
				checkError( device, hr, "CreateTexture1D" ) )
			{
				dxDebugName( result, Texture1D );
			}

			return result;
		}

		void doInitTexDesc( VkDevice device
			, VkImageCreateInfo const & createInfo
			, D3D11_TEXTURE1D_DESC & desc )const
		{
			desc.Width = createInfo.extent.width;
			desc.Format = getDxgiFormatGroup( getTextureFormat( createInfo.format ) );
			desc.ArraySize = createInfo.arrayLayers;
			desc.Usage = getImageUsage( m_propertyFlags, m_usage );
			desc.CPUAccessFlags = getCpuImageAccessFlags( m_propertyFlags, m_usage );
			desc.MipLevels = createInfo.mipLevels;
			desc.BindFlags = mem::getBindFlags( createInfo, m_propertyFlags );
			desc.MiscFlags = mem::getMiscFlags( createInfo );

			if ( get( device )->getFeatureLevel() < D3D_FEATURE_LEVEL_11_0 )
			{
				desc.BindFlags &= ~D3D11_BIND_UNORDERED_ACCESS;
			}
		}

	private:
		VkImageUsageFlags m_usage;
		VkMemoryPropertyFlags m_propertyFlags;
		ID3D11Texture1D * m_texture{ nullptr };

	public:
		ObjectMemory memory;
	};

	//*********************************************************************************************

	class Texture2DDeviceMemory
	{
	public:
		Texture2DDeviceMemory( VkDevice device
			, VkDeviceMemory parent
			, VkDeviceSize offset
			, VkMemoryAllocateInfo allocateInfo
			, VkImageCreateInfo const & createInfo )
			: m_usage{ createInfo.usage }
			, m_propertyFlags{ mem::getMemoryProperties( allocateInfo.memoryTypeIndex ) }
			, m_texture{ doCreateTexture( device, createInfo ) }
			, memory{ device, parent, m_texture, offset, std::move( allocateInfo ), getImageMapFlags( m_propertyFlags, m_usage ) }
		{
			auto extent = getTexelBlockExtent( createInfo.format );
			auto byteSize = getTexelBlockByteSize( extent, createInfo.format );
			memory.subresources.resize( size_t( createInfo.arrayLayers ) * createInfo.mipLevels );

			for ( uint32_t layer = 0u; layer < createInfo.arrayLayers; ++layer )
			{
				for ( uint32_t level = 0u; level < createInfo.mipLevels; ++level )
				{
					auto mipWidth = getSubresourceValue( createInfo.extent.width, level );
					auto mipHeight = getSubresourceValue( createInfo.extent.height, level );
					auto rowPitch = byteSize * mipWidth / ( extent.width * extent.height * extent.depth );
					auto depthPitch = rowPitch * mipHeight * extent.height / ( extent.width * extent.depth );
					memory.subresources[D3D11CalcSubresource( level, layer, createInfo.mipLevels )] = { nullptr, rowPitch, depthPitch };
				}
			}
		}

		inline ID3D11Texture2D * getImage()const
		{
			return m_texture;
		}

	private:
		ID3D11Texture2D * doCreateTexture( VkDevice device
			, VkImageCreateInfo const & createInfo )const
		{
			ID3D11Texture2D * result{};
			auto d3ddevice = get( device )->getDevice();
			D3D11_TEXTURE2D_DESC desc{};
			doInitTexDesc( device, createInfo, desc );

			if ( HRESULT hr = d3ddevice->CreateTexture2D( &desc
					, nullptr
					, &result );
				checkError( device, hr, "CreateTexture2D" ) )
			{
				dxDebugName( result, Texture2D );
			}

			return result;
		}

		void doInitTexDesc( VkDevice device
			, VkImageCreateInfo const & createInfo
			, D3D11_TEXTURE2D_DESC & desc )const
		{
			auto d3ddevice = get( device )->getDevice();
			desc.Width = createInfo.extent.width;
			desc.Height = createInfo.extent.height;
			desc.Format = getDxgiFormatGroup( getTextureFormat( createInfo.format ) );
			desc.ArraySize = createInfo.arrayLayers;
			desc.Usage = getImageUsage( m_propertyFlags, m_usage );
			desc.CPUAccessFlags = getCpuImageAccessFlags( m_propertyFlags, m_usage );
			desc.MipLevels = createInfo.mipLevels;
			desc.SampleDesc.Count = UINT( createInfo.samples );
			desc.SampleDesc.Quality = 0;
			desc.BindFlags = mem::getBindFlags( createInfo, m_propertyFlags );
			desc.MiscFlags = mem::getMiscFlags( createInfo );

			if ( desc.SampleDesc.Count > 1 )
			{
				HRESULT hr = d3ddevice->CheckMultisampleQualityLevels( desc.Format
					, desc.SampleDesc.Count
					, &desc.SampleDesc.Quality );

				if ( hr == S_OK && desc.SampleDesc.Quality > 0 )
				{
					desc.SampleDesc.Quality--;
				}
			}

			if ( get( device )->getFeatureLevel() < D3D_FEATURE_LEVEL_11_0 )
			{
				desc.BindFlags &= ~D3D11_BIND_UNORDERED_ACCESS;
			}
		}

	private:
		VkImageUsageFlags m_usage;
		VkMemoryPropertyFlags m_propertyFlags;
		ID3D11Texture2D * m_texture{ nullptr };

	public:
		ObjectMemory memory;
	};
	
	//*********************************************************************************************

	class Texture3DDeviceMemory
	{
	public:
		Texture3DDeviceMemory( VkDevice device
			, VkDeviceMemory parent
			, VkDeviceSize offset
			, VkMemoryAllocateInfo allocateInfo
			, VkImageCreateInfo const & createInfo )
			: m_usage{ createInfo.usage }
			, m_propertyFlags{ mem::getMemoryProperties( allocateInfo.memoryTypeIndex ) }
			, m_texture{ doCreateTexture( device, createInfo ) }
			, memory{ device, parent, m_texture, offset, std::move( allocateInfo ), getImageMapFlags( m_propertyFlags, m_usage ) }
		{
			auto extent = getTexelBlockExtent( createInfo.format );
			auto byteSize = getTexelBlockByteSize( extent, createInfo.format );
			memory.subresources.resize( size_t( createInfo.arrayLayers ) * createInfo.mipLevels );

			for ( uint32_t layer = 0u; layer < createInfo.arrayLayers; ++layer )
			{
				for ( uint32_t level = 0u; level < createInfo.mipLevels; ++level )
				{
					auto mipWidth = getSubresourceValue( createInfo.extent.width, level );
					auto mipHeight = getSubresourceValue( createInfo.extent.height, level );
					auto rowPitch = byteSize * mipWidth / ( extent.width * extent.height * extent.depth );
					auto depthPitch = rowPitch * mipHeight * extent.height / ( extent.width * extent.depth );
					memory.subresources[D3D11CalcSubresource( level, layer, createInfo.mipLevels )] = { nullptr, rowPitch, depthPitch };
				}
			}
		}

		inline ID3D11Texture3D * getImage()const
		{
			return m_texture;
		}

	private:
		ID3D11Texture3D * doCreateTexture( VkDevice device
			, VkImageCreateInfo const & createInfo )const
		{
			ID3D11Texture3D * result{};
			auto d3ddevice = get( device )->getDevice();
			D3D11_TEXTURE3D_DESC desc{};
			doInitTexDesc( device, createInfo, desc );

			if ( HRESULT hr = d3ddevice->CreateTexture3D( &desc
					, nullptr
					, &result );
				checkError( device, hr, "CreateTexture3D" ) )
			{
				dxDebugName( result, Texture3D );
			}

			return result;
		}

		void doInitTexDesc( VkDevice device
			, VkImageCreateInfo const & createInfo
			, D3D11_TEXTURE3D_DESC & desc )const
		{
			desc.Width = createInfo.extent.width;
			desc.Height = createInfo.extent.height;
			desc.Depth = createInfo.extent.depth;
			desc.Format = getDxgiFormatGroup( getTextureFormat( createInfo.format ) );
			desc.Usage = getImageUsage( m_propertyFlags, m_usage );
			desc.CPUAccessFlags = getCpuImageAccessFlags( m_propertyFlags, m_usage );
			desc.MipLevels = createInfo.mipLevels;
			desc.BindFlags = mem::getBindFlags( createInfo, m_propertyFlags );
			desc.MiscFlags = mem::getMiscFlags( createInfo );

			if ( get( device )->getFeatureLevel() < D3D_FEATURE_LEVEL_11_0 )
			{
				desc.BindFlags &= ~D3D11_BIND_UNORDERED_ACCESS;
			}
		}

	private:
		VkImageUsageFlags m_usage;
		VkMemoryPropertyFlags m_propertyFlags;
		ID3D11Texture3D * m_texture{ nullptr };

	public:
		ObjectMemory memory;
	};

	//*********************************************************************************************

	DeviceMemory::DeviceMemory( VkDevice device
		, VkMemoryAllocateInfo allocateInfo )
		: m_device{ device }
		, m_allocateInfo{ std::move( allocateInfo ) }
		, m_propertyFlags{ mem::getMemoryProperties( m_allocateInfo.memoryTypeIndex ) }
	{
		thread_local uint8_t defaultInitValue = 10u;

		if ( ashes::checkFlag( m_propertyFlags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) )
		{
			m_data.resize( allocateInfo.allocationSize, defaultInitValue );
			++defaultInitValue;
		}
	}

	DeviceMemory::~DeviceMemory()noexcept
	{
		if ( !m_objects.empty() )
		{
			onDestroy( get( this ) );
		}
	}

	VkResult DeviceMemory::bindToBuffer( VkBuffer buffer
		, VkDeviceSize memoryOffset
		, ObjectMemory *& objectMemory )
	{
		VkResult result = VK_ERROR_INITIALIZATION_FAILED;

		try
		{
			BufferDeviceMemory impl
			{
				m_device,
				get( this ),
				memoryOffset,
				m_allocateInfo,
				get( buffer )->getUsage()
			};
			m_objects.emplace_back( std::make_unique< ObjectMemory >( std::move( impl.memory ) ) );
			objectMemory = m_objects.back().get();
			updateUpload( *objectMemory, 0ULL, WholeSize, 0u );
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

	VkResult DeviceMemory::bindToImage1D( VkImage image
		, VkDeviceSize memoryOffset
		, ObjectMemory *& objectMemory )
	{
		VkResult result = VK_ERROR_INITIALIZATION_FAILED;

		try
		{
			Texture1DDeviceMemory impl
			{
				m_device,
				get( this ),
				memoryOffset,
				m_allocateInfo,
				get( image )->getCreateInfo()
			};
			m_objects.emplace_back( std::make_unique< ObjectMemory >( std::move( impl.memory ) ) );
			objectMemory = m_objects.back().get();
			updateUpload( *objectMemory, 0ULL, WholeSize, 0u );
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

	VkResult DeviceMemory::bindToImage2D( VkImage image
		, VkDeviceSize memoryOffset
		, ObjectMemory *& objectMemory )
	{
		VkResult result = VK_ERROR_INITIALIZATION_FAILED;

		try
		{
			Texture2DDeviceMemory impl
			{
				m_device,
				get( this ),
				memoryOffset,
				m_allocateInfo,
				get( image )->getCreateInfo()
			};
			m_objects.emplace_back( std::make_unique< ObjectMemory >( std::move( impl.memory ) ) );
			objectMemory = m_objects.back().get();
			updateUpload( *objectMemory, 0ULL, WholeSize, 0u );
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

	VkResult DeviceMemory::bindToImage3D( VkImage image
		, VkDeviceSize memoryOffset
		, ObjectMemory *& objectMemory )
	{
		VkResult result = VK_ERROR_INITIALIZATION_FAILED;

		try
		{
			Texture3DDeviceMemory impl
			{
				m_device,
				get( this ),
				memoryOffset,
				m_allocateInfo,
				get( image )->getCreateInfo()
			};
			m_objects.emplace_back( std::make_unique< ObjectMemory >( std::move( impl.memory ) ) );
			objectMemory = m_objects.back().get();
			updateUpload( *objectMemory, 0ULL, WholeSize, 0u );
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

	void DeviceMemory::updateUpload( ObjectMemory const & memory
		, VkDeviceSize offset
		, VkDeviceSize size
		, UINT subresource )const
	{
		if ( !m_data.empty() )
		{
			memory.upload( m_data.data()
				, subresource
				, ( ( size == WholeSize && offset == 0u )
					? 0u
					: offset + memory.offset )
				, size );
		}
	}

	void DeviceMemory::updateDownload( ObjectMemory const & memory
		, VkDeviceSize offset
		, VkDeviceSize size
		, UINT subresource )const
	{
		if ( !m_data.empty() )
		{
			memory.download( m_data.data()
				, subresource
				, ( ( size == WholeSize && offset == 0u )
					? 0u
					: offset + memory.offset )
				, size );
		}
	}

	void DeviceMemory::updateUpload( VkDeviceSize offset
		, VkDeviceSize size
		, UINT subresource )const
	{
		if ( !m_data.empty() && !m_objects.empty() )
		{
			auto paramOffset = offset;
			auto paramSize = size;

			for ( auto & object : m_objects )
			{
				auto objectOffset = std::max( paramOffset, object->offset ) - object->offset;
				auto objectSize = std::min( paramSize
					, ( object->allocateInfo.allocationSize > objectOffset
						? object->allocateInfo.allocationSize - objectOffset
						: 0u ) );
				objectOffset = object->offset;

				if ( objectSize > 0u
					&& paramOffset + paramSize > objectOffset )
				{
					object->upload( m_data.data()
						, subresource
						, offset
						, size );
				}
			}

			m_dirty = false;
		}
	}

	void DeviceMemory::updateDownload( VkDeviceSize offset
		, VkDeviceSize size
		, UINT subresource )const
	{
		if ( !m_data.empty() && !m_objects.empty() )
		{
			auto paramOffset = offset;
			auto paramSize = size;

			for ( auto & object : m_objects )
			{
				auto objectOffset = std::max( paramOffset, object->offset ) - object->offset;
				auto objectSize = std::min( paramSize
					, ( object->allocateInfo.allocationSize > objectOffset
						? object->allocateInfo.allocationSize - objectOffset
						: 0u ) );
				objectOffset = object->offset;

				if ( objectSize > 0u
					&& paramOffset + paramSize > objectOffset )
				{
					object->download( m_data.data()
						, subresource
						, offset
						, size );
				}
			}

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
		size = (size == WholeSize
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
		size = ( size == WholeSize
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
		size = ( size == WholeSize
			? m_allocateInfo.allocationSize
			: size );

		if ( m_data.size() <= offset
			|| m_data.size() < ( offset + size ) )
		{
			return VK_ERROR_OUT_OF_DEVICE_MEMORY;
		}

		m_dirty = true;
		updateDownload( offset, size, m_mappedSubresource );
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
