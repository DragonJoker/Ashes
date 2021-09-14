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

		VkOffset3D operator+( VkOffset3D const & lhs, VkExtent3D const & rhs )
		{
			return VkOffset3D{ lhs.x + int32_t( rhs.width )
				, lhs.x + int32_t( rhs.height )
				, lhs.x + int32_t( rhs.depth ) };
		}

		VkExtent3D getExtent( VkExtent3D const & src
			, uint32_t mipDiff )
		{
			return VkExtent3D{ src.width << mipDiff
				, src.height << mipDiff
				, src.depth << mipDiff };
		}

		VkImageBlit convert( VkImageCopy const & src )
		{
			if ( src.dstSubresource.mipLevel == src.srcSubresource.mipLevel )
			{
				return VkImageBlit{ src.srcSubresource
					, { src.srcOffset, src.srcOffset + src.extent }
					, src.dstSubresource
					, { src.dstOffset, src.dstOffset + src.extent } };
			}

			if ( src.dstSubresource.mipLevel > src.srcSubresource.mipLevel )
			{
				return VkImageBlit{ src.srcSubresource
					, { src.srcOffset, src.srcOffset + getExtent( src.extent, src.dstSubresource.mipLevel - src.srcSubresource.mipLevel ) }
					, src.dstSubresource
					, { src.dstOffset, src.dstOffset + src.extent } };
			}

			return VkImageBlit{ src.srcSubresource
				, { src.srcOffset, src.srcOffset + src.extent }
				, src.dstSubresource
				, { src.dstOffset, src.dstOffset + getExtent( src.extent, src.srcSubresource.mipLevel - src.dstSubresource.mipLevel ) } };
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

	namespace
	{
		bool isCube( VkImage image )
		{
			return checkFlag( get( image )->getCreateFlags()
				, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT );
		}

		bool isCube( VkImageView view )
		{
			return isCube( get( view )->getImage() );
		}

		bool isMultisampled( VkImage image )
		{
			return get( image )->getSamples() > VK_SAMPLE_COUNT_1_BIT;
		}

		bool isMultisampled( VkImageView view )
		{
			return isMultisampled( get( view )->getImage() );
		}
	}

	FboAttachment::FboAttachment()
	{
	}

	FboAttachment::FboAttachment( VkDevice device
		, uint32_t referenceIndex
		, VkImageView view
		, uint32_t index
		, bool & multisampled )
		: referenceIndex{ referenceIndex }
		, point{ getAttachmentPoint( view ) }
		, object{ get( view )->getInternal() }
		, originalObject{ get( get( view )->getImage() )->getInternal() }
		, type{ getAttachmentType( view ) }
		, index{ index }
		, imgLayerCount{ std::max( get( get( view )->getImage() )->getArrayLayers()
			, get( get( view )->getImage() )->getDimensions().depth ) }
		, viewLayerCount{ ( ( get( view )->getType() == VK_IMAGE_VIEW_TYPE_3D )
			? imgLayerCount
			: get( view )->getSubresourceRange().layerCount ) }
		, target{ ( get( get( view )->getImage() )->getType() == VK_IMAGE_TYPE_3D
			? GL_TEXTURE_3D
			: ( get( get( view )->getImage() )->getType() == VK_IMAGE_TYPE_2D
				? ( viewLayerCount > 1u
					? ( isMultisampled( view )
						? GL_TEXTURE_2D_MULTISAMPLE_ARRAY
						: GL_TEXTURE_2D_ARRAY )
					: ( isMultisampled( view )
						? GL_TEXTURE_2D_MULTISAMPLE
						: GL_TEXTURE_2D ) )
				: ( viewLayerCount > 1u
					? GL_TEXTURE_1D_ARRAY
					: GL_TEXTURE_1D ) ) ) }
		, baseArrayLayer{ ( hasTextureViews( device )
			? 0
			: ( ( ( !isCube( view ) && imgLayerCount > 1u ) || ( isCube( view ) && imgLayerCount > 6u ) )
				? get( view )->getSubresourceRange().baseArrayLayer
				: 0u ) ) }
		, originalMipLevel{ get( view )->getSubresourceRange().baseMipLevel }
		, mipLevel{ originalMipLevel }
		, isSrgb{ isSRGBFormat( get( get( view )->getImage() )->getFormatVk() ) }
	{
		multisampled = isMultisampled( view );
	}

	FboAttachment::FboAttachment( VkDevice device
		, VkImageSubresourceLayers & subresource
		, VkImage image )
		: referenceIndex{ 0u }
		, point{ ( hasTextureViews( device )
			? getAttachmentPoint( subresource.aspectMask )
			: getAttachmentPoint( get( image )->getFormatVk() ) ) }
		, object{ get( image )->getInternal() }
		, originalObject{}
		, type{ ( hasTextureViews( device )
			? getAttachmentType( subresource.aspectMask )
			: getAttachmentType( get( image )->getFormatVk() ) ) }
		, index{ 0u }
		, imgLayerCount{ get( image )->getArrayLayers() }
		, viewLayerCount{ imgLayerCount }
		, target{ ( get( image )->getType() == VK_IMAGE_TYPE_3D
			? GL_TEXTURE_3D
			: ( get( image )->getType() == VK_IMAGE_TYPE_2D
				? ( viewLayerCount > 1u
					? ( isMultisampled( image )
						? GL_TEXTURE_2D_MULTISAMPLE_ARRAY
						: GL_TEXTURE_2D_ARRAY )
					: ( isMultisampled( image )
						? GL_TEXTURE_2D_MULTISAMPLE
						: GL_TEXTURE_2D ) )
				: ( viewLayerCount > 1u
					? GL_TEXTURE_1D_ARRAY
					: GL_TEXTURE_1D ) ) ) }
		, baseArrayLayer{ subresource.baseArrayLayer }
		, originalMipLevel{}
		, mipLevel{ ( get( image )->getArrayLayers() > 1u
			? 0u
			: subresource.mipLevel ) }
		, isSrgb{ isSRGBFormat( get( image )->getFormatVk() ) }
	{
	}

	void FboAttachment::bind( uint32_t pmipLevel
		, GlFrameBufferTarget fboTarget
		, CmdList & list )const
	{
		bindIndex( pmipLevel, fboTarget, 0u, list );
	}

	void FboAttachment::bindIndex( uint32_t pmipLevel
		, GlFrameBufferTarget fboTarget
		, uint32_t pindex
		, CmdList & list )const
	{
		if ( target == GL_TEXTURE_1D )
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture1D >( fboTarget
				, GlAttachmentPoint( point + pindex )
				, target
				, object
				, pmipLevel ) );
		}
		else if ( target == GL_TEXTURE_2D
			|| target == GL_TEXTURE_2D_MULTISAMPLE )
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture2D >( fboTarget
				, GlAttachmentPoint( point + pindex )
				, target
				, object
				, pmipLevel ) );
		}
		else
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture >( fboTarget
				, GlAttachmentPoint( point + pindex )
				, object
				, pmipLevel ) );
		}
	}

	void FboAttachment::bindRead( ContextStateStack & stack
		, uint32_t pmipLevel
		, GlFrameBufferTarget fboTarget
		, CmdList & list )const
	{
		bind( pmipLevel, fboTarget, list );
		read( stack, list );
	}

	void FboAttachment::bindDraw( ContextStateStack & stack
		, uint32_t pmipLevel
		, GlFrameBufferTarget fboTarget
		, CmdList & list )const
	{
		bind( pmipLevel, fboTarget, list );
		draw( stack, list );
	}

	void FboAttachment::bind( uint32_t pmipLevel
		, uint32_t layer
		, GlFrameBufferTarget fboTarget
		, CmdList & list )const
	{
		bindIndex( pmipLevel, layer, fboTarget, 0u, list );
	}

	void FboAttachment::bindIndex( uint32_t pmipLevel
		, uint32_t layer
		, GlFrameBufferTarget fboTarget
		, uint32_t pindex
		, CmdList & list )const
	{
		if ( target == GL_TEXTURE_1D )
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture1D >( fboTarget
				, GlAttachmentPoint( point + pindex )
				, target
				, object
				, pmipLevel ) );
		}
		else if ( target == GL_TEXTURE_2D
			|| target == GL_TEXTURE_2D_MULTISAMPLE )
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture2D >( fboTarget
				, GlAttachmentPoint( point + pindex )
				, target
				, object
				, pmipLevel ) );
		}
		else if ( target == GL_TEXTURE_3D )
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture3D >( fboTarget
				, GlAttachmentPoint( point + pindex )
				, target
				, object
				, pmipLevel
				, layer ) );
		}
		else if ( target == GL_TEXTURE_CUBE
			|| target == GL_TEXTURE_1D_ARRAY
			|| target == GL_TEXTURE_2D_ARRAY
			|| target == GL_TEXTURE_2D_MULTISAMPLE_ARRAY
			|| target == GL_TEXTURE_CUBE_ARRAY )
		{
			list.push_back( makeCmd< OpType::eFramebufferTextureLayer >( fboTarget
				, GlAttachmentPoint( point + pindex )
				, object
				, pmipLevel
				, layer ) );
		}
		else
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture >( fboTarget
				, GlAttachmentPoint( point + pindex )
				, object
				, pmipLevel ) );
		}
	}

	void FboAttachment::bindRead( ContextStateStack & stack
		, uint32_t pmipLevel
		, uint32_t layer
		, GlFrameBufferTarget fboTarget
		, CmdList & list )const
	{
		bind( pmipLevel, layer, fboTarget, list );
		read( stack, list );
	}

	void FboAttachment::bindDraw( ContextStateStack & stack
		, uint32_t pmipLevel
		, uint32_t layer
		, GlFrameBufferTarget fboTarget
		, CmdList & list )const
	{
		bind( pmipLevel, layer, fboTarget, list );
		draw( stack, list );
	}

	void FboAttachment::bind( uint32_t pmipLevel
		, uint32_t layer
		, uint32_t slice
		, GlFrameBufferTarget fboTarget
		, CmdList & list )const
	{
		bindIndex( pmipLevel, layer, slice, fboTarget, 0u, list );
	}

	void FboAttachment::bindIndex( uint32_t pmipLevel
		, uint32_t layer
		, uint32_t slice
		, GlFrameBufferTarget fboTarget
		, uint32_t pindex
		, CmdList & list )const
	{
		if ( target == GL_TEXTURE_1D )
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture1D >( fboTarget
				, GlAttachmentPoint( point + pindex )
				, target
				, object
				, pmipLevel ) );
		}
		else if ( target == GL_TEXTURE_2D
			|| target == GL_TEXTURE_2D_MULTISAMPLE )
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture2D >( fboTarget
				, GlAttachmentPoint( point + pindex )
				, target
				, object
				, pmipLevel ) );
		}
		else if ( target == GL_TEXTURE_3D )
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture3D >( fboTarget
				, GlAttachmentPoint( point + pindex )
				, target
				, object
				, pmipLevel
				, slice ) );
		}
		else if ( target == GL_TEXTURE_CUBE
			|| target == GL_TEXTURE_1D_ARRAY
			|| target == GL_TEXTURE_2D_ARRAY
			|| target == GL_TEXTURE_2D_MULTISAMPLE_ARRAY
			|| target == GL_TEXTURE_CUBE_ARRAY )
		{
			list.push_back( makeCmd< OpType::eFramebufferTextureLayer >( fboTarget
				, GlAttachmentPoint( point + pindex )
				, object
				, pmipLevel
				, layer ) );
		}
		else
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture >( fboTarget
				, GlAttachmentPoint( point + pindex )
				, object
				, pmipLevel ) );
		}
	}

	void FboAttachment::bindRead( ContextStateStack & stack
		, uint32_t pmipLevel
		, uint32_t layer
		, uint32_t slice
		, GlFrameBufferTarget fboTarget
		, CmdList & list )const
	{
		bind( pmipLevel, layer, slice, fboTarget, list );
		read( stack, list );
	}

	void FboAttachment::bindDraw( ContextStateStack & stack
		, uint32_t pmipLevel
		, uint32_t layer
		, uint32_t slice
		, GlFrameBufferTarget fboTarget
		, CmdList & list )const
	{
		bind( pmipLevel, layer, slice, fboTarget, list );
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
		, VkImage dstImage )
		: region{ origRegion }
		, src{ device, origRegion.srcSubresource, srcImage }
		, dst{ device, origRegion.dstSubresource, dstImage }
	{
	}

	LayerCopy::LayerCopy( VkDevice device
		, VkImageCopy origRegion
		, VkImage srcImage
		, VkImage dstImage )
		: LayerCopy{ device, convert( origRegion ), srcImage, dstImage }
	{
	}

	//*********************************************************************************************
}
