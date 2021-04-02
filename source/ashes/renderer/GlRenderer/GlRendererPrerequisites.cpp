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
			return VkExtent3D{ uint32_t( src.width << mipDiff )
				, uint32_t( src.height << mipDiff )
				, uint32_t( src.depth << mipDiff ) };
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

	void FboAttachment::bind( uint32_t mipLevel
		, GlFrameBufferTarget fboTarget
		, CmdList & list )const
	{
		bindIndex( mipLevel, fboTarget, 0u, list );
	}

	void FboAttachment::bindIndex( uint32_t mipLevel
		, GlFrameBufferTarget fboTarget
		, uint32_t index
		, CmdList & list )const
	{
		if ( target == GL_TEXTURE_1D )
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture1D >( fboTarget
				, GlAttachmentPoint( point + index )
				, target
				, object
				, mipLevel ) );
		}
		else if ( target == GL_TEXTURE_2D
			|| target == GL_TEXTURE_2D_MULTISAMPLE )
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture2D >( fboTarget
				, GlAttachmentPoint( point + index )
				, target
				, object
				, mipLevel ) );
		}
		else
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture >( fboTarget
				, GlAttachmentPoint( point + index )
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
		bindIndex( mipLevel, layer, fboTarget, 0u, list );
	}

	void FboAttachment::bindIndex( uint32_t mipLevel
		, uint32_t layer
		, GlFrameBufferTarget fboTarget
		, uint32_t index
		, CmdList & list )const
	{
		if ( target == GL_TEXTURE_1D )
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture1D >( fboTarget
				, GlAttachmentPoint( point + index )
				, target
				, object
				, mipLevel ) );
		}
		else if ( target == GL_TEXTURE_2D
			|| target == GL_TEXTURE_2D_MULTISAMPLE )
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture2D >( fboTarget
				, GlAttachmentPoint( point + index )
				, target
				, object
				, mipLevel ) );
		}
		else if ( target == GL_TEXTURE_3D )
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture3D >( fboTarget
				, GlAttachmentPoint( point + index )
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
				, GlAttachmentPoint( point + index )
				, object
				, mipLevel
				, layer ) );
		}
		else
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture >( fboTarget
				, GlAttachmentPoint( point + index )
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
		, VkImage dstImage )
		: region{ origRegion }
		, src{ initialiseAttachment( device, origRegion.srcSubresource, srcImage ) }
		, dst{ initialiseAttachment( device, origRegion.dstSubresource, dstImage ) }
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
