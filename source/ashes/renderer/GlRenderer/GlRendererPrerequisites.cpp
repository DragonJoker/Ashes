#include "GlRendererPrerequisites.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	//*********************************************************************************************

	namespace
	{
		template< typename VkObjectT >
		void assertReport( VkObjectT object
			, bool condition
			, char const * msg )
		{
			if ( !condition )
			{
				reportError( object
					, VK_ERROR_VALIDATION_FAILED_EXT
					, "Validation Error."
					, msg );
			}
		}

		VkOffset3D convert( VkExtent3D const & src )
		{
			return VkOffset3D{ int32_t( src.width )
				, int32_t( src.height )
				, int32_t( src.depth ) };
		}

		VkOffset3D convert( VkExtent3D const & src
			, uint32_t mipDiff )
		{
			return VkOffset3D{ int32_t( src.width << mipDiff )
				, int32_t( src.height << mipDiff )
				, int32_t( src.depth << mipDiff ) };
		}

		VkImageBlit convert( VkImageCopy const & src )
		{
			if ( src.dstSubresource.mipLevel == src.srcSubresource.mipLevel )
			{
				return VkImageBlit{ src.srcSubresource
					, { src.srcOffset, convert( src.extent ) }
					, src.dstSubresource
					, { src.dstOffset, convert( src.extent ) } };
			}

			if ( src.dstSubresource.mipLevel > src.srcSubresource.mipLevel )
			{
				return VkImageBlit{ src.srcSubresource
					, { src.srcOffset, convert( src.extent, src.dstSubresource.mipLevel - src.srcSubresource.mipLevel ) }
					, src.dstSubresource
					, { src.dstOffset, convert( src.extent ) } };
			}

			return VkImageBlit{ src.srcSubresource
				, { src.srcOffset, convert( src.extent ) }
				, src.dstSubresource
				, { src.dstOffset, convert( src.extent, src.srcSubresource.mipLevel - src.dstSubresource.mipLevel ) } };
		}
	}

	//*********************************************************************************************

	uint32_t deduceMemoryType( VkDevice device
		, uint32_t typeBits
		, VkMemoryPropertyFlags requirements )
	{
		return ashes::deduceMemoryType( typeBits
			, requirements
			, get( get( device )->getPhysicalDevice() )->getMemoryProperties() );
	}

	uint32_t getScreenIndex( VkDisplayModeKHR displayMode )
	{
		return get( get( displayMode )->getDisplay() )->getScreenIndex();
	}

	VkDisplayModeParametersKHR getDisplayModeParameters( VkDisplayModeKHR displayMode )
	{
		return get( displayMode )->getParameters();
	}

	VkExtent2D getDisplayResolution( VkDisplayModeKHR displayMode )
	{
		return get( get( displayMode )->getDisplay() )->getResolution();
	}

	//*********************************************************************************************

	void FboAttachment::bind( uint32_t mipLevel
		, GlFrameBufferTarget fboTarget
		, CmdList & list )const
	{
		if ( target == GL_TEXTURE_1D )
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture1D >( fboTarget
				, point
				, target
				, object
				, mipLevel ) );
		}
		else if ( target == GL_TEXTURE_2D
			|| target == GL_TEXTURE_2D_MULTISAMPLE )
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture2D >( fboTarget
				, point
				, target
				, object
				, mipLevel ) );
		}
		else
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture >( fboTarget
				, point
				, object
				, mipLevel ) );
		}
	}

	void FboAttachment::bindRead( ContextStateStack & stack
		, uint32_t mipLevel
		, GlFrameBufferTarget target
		, CmdList & list )const
	{
		bind( mipLevel, target, list );
		read( stack, list );
	}

	void FboAttachment::bindDraw( ContextStateStack & stack
		, uint32_t mipLevel
		, GlFrameBufferTarget target
		, CmdList & list )const
	{
		bind( mipLevel, target, list );
		draw( stack, list );
	}

	void FboAttachment::bind( uint32_t mipLevel
		, uint32_t layer
		, GlFrameBufferTarget fboTarget
		, CmdList & list )const
	{
		if ( target == GL_TEXTURE_1D )
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture1D >( fboTarget
				, point
				, target
				, object
				, mipLevel ) );
		}
		else if ( target == GL_TEXTURE_2D
			|| target == GL_TEXTURE_2D_MULTISAMPLE )
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture2D >( fboTarget
				, point
				, target
				, object
				, mipLevel ) );
		}
		else if ( target == GL_TEXTURE_3D )
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture3D >( fboTarget
				, point
				, target
				, object
				, mipLevel
				, layer ) );
		}
		else if ( target == GL_TEXTURE_CUBE
			|| target == GL_TEXTURE_1D_ARRAY
			|| target == GL_TEXTURE_2D_ARRAY
			|| target == GL_TEXTURE_2D_MULTISAMPLE_ARRAY
			|| target == GL_TEXTURE_CUBE_ARRAY )
		{
			list.push_back( makeCmd< OpType::eFramebufferTextureLayer >( fboTarget
				, point
				, object
				, mipLevel
				, layer ) );
		}
		else
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture >( fboTarget
				, point
				, object
				, mipLevel ) );
		}
	}

	void FboAttachment::bindRead( ContextStateStack & stack
		, uint32_t mipLevel
		, uint32_t layer
		, GlFrameBufferTarget target
		, CmdList & list )const
	{
		bind( mipLevel, layer, target, list );
		read( stack, list );
	}

	void FboAttachment::bindDraw( ContextStateStack & stack
		, uint32_t mipLevel
		, uint32_t layer
		, GlFrameBufferTarget target
		, CmdList & list )const
	{
		bind( mipLevel, layer, target, list );
		draw( stack, list );
	}

	void FboAttachment::read( ContextStateStack & stack
		, CmdList & list )const
	{
		if ( !isDepthOrStencil() )
		{
			if ( isSrgb )
			{
				stack.applySRGBStatus( list, isSrgb );
			}

			list.push_back( makeCmd< OpType::eReadBuffer >( point ) );
		}

#if !defined( NDEBUG )
		list.push_back( makeCmd< OpType::eCheckFramebuffer >() );
#endif
	}

	void FboAttachment::draw( ContextStateStack & stack
		, CmdList & list )const
	{
		if ( !isDepthOrStencil() )
		{
			if ( isSrgb )
			{
				stack.applySRGBStatus( list, isSrgb );
			}

			list.push_back( makeCmd< OpType::eDrawBuffers >( point ) );
		}

#if !defined( NDEBUG )
		list.push_back( makeCmd< OpType::eCheckFramebuffer >() );
#endif
	}

	//*********************************************************************************************

	LayerCopy::LayerCopy( VkDevice device
		, VkImageBlit origRegion
		, VkImage srcImage
		, VkImage dstImage
		, VkImageViewArray & views )
		: region{ origRegion }
		, src{ initialiseAttachment( device, origRegion.srcSubresource, srcImage, srcView ) }
		, dst{ initialiseAttachment( device, origRegion.dstSubresource, dstImage, dstView ) }
	{
		auto srcExtent = getSubresourceDimensions( get( srcImage )->getDimensions(), region.srcSubresource.mipLevel );
		auto dstExtent = getSubresourceDimensions( get( dstImage )->getDimensions(), region.dstSubresource.mipLevel );

		if ( srcView != VK_NULL_HANDLE )
		{
			views.push_back( srcView );
			region.srcSubresource.mipLevel = 0u;
		}

		if ( dstView != VK_NULL_HANDLE )
		{
			views.push_back( dstView );
			region.dstSubresource.mipLevel = 0u;
		}

		// Convert extent to rectangle bounds for src
		region.srcOffsets[1].x += region.srcOffsets[0].x;
		region.srcOffsets[1].y += region.srcOffsets[0].y;
		region.srcOffsets[1].z += region.srcOffsets[0].z;

		// Convert extent to rectangle bounds for dst
		region.dstOffsets[1].x += region.dstOffsets[0].x;
		region.dstOffsets[1].y += region.dstOffsets[0].y;
		region.dstOffsets[1].z += region.dstOffsets[0].z;
	}

	LayerCopy::LayerCopy( VkDevice device
		, VkImageCopy origRegion
		, VkImage srcImage
		, VkImage dstImage
		, VkImageViewArray & views )
		: LayerCopy{ device, convert( origRegion ), srcImage, dstImage, views }
	{
	}

	//*********************************************************************************************
}
