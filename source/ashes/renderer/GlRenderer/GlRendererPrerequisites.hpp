/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#pragma once

#ifndef VK_NO_PROTOTYPES
#	define VK_NO_PROTOTYPES
#endif

#include <ashes/ashes.h>

#include "renderer/GlRenderer/Core/GlAutoIdIcdObject.hpp"
#include "renderer/GlRenderer/Miscellaneous/GlDebug.hpp"
#include "renderer/GlRenderer/Miscellaneous/OpenGLDefines.hpp"
#include "renderer/GlRenderer/Core/GlContextState.hpp"

#include <map>
#include <vector>
#include <unordered_set>

#include <cassert>
#include <functional>
#include <memory>
#include <string>

#pragma warning( push )
#pragma warning( disable: 5262 )
#include <sstream>
#pragma warning( pop )

#include <ashes/common/Format.hpp>

#include <renderer/RendererCommon/AshesRendererPrerequisites.hpp>
#include <renderer/RendererCommon/Helper/ConstantFormat.hpp>

#if defined( _WIN32 ) && !defined( GlRenderer_STATIC )
#	ifdef GlRenderer_EXPORTS
#		define GlRenderer_API __declspec( dllexport )
#	else
#		define GlRenderer_API __declspec( dllimport )
#	endif
#else
#	define GlRenderer_API
#endif

namespace ashes::gl
{
#if VK_EXT_debug_utils

	struct DebugUtilsMessengerData
	{
		VkDebugUtilsMessengerEXT debugMessenger;
		PFNGLDEBUGPROC callback;
		void * userParam;
	};

	struct DebugUtilsAMDMessengerData
	{
		VkDebugUtilsMessengerEXT debugMessenger;
		PFNGLDEBUGAMDPROC callback;
		void * userParam;
	};

#endif
#if VK_EXT_debug_report

	struct DebugReportCallbackData
	{
		VkDebugReportCallbackEXT debugReport;
		PFNGLDEBUGPROC callback;
		void * userParam;
	};
	
	struct DebugReportAMDCallbackData
	{
		VkDebugReportCallbackEXT debugReport;
		PFNGLDEBUGAMDPROC callback;
		void * userParam;
	};

#endif

	class ExtensionNotPresentException
		: public ashes::Exception
	{
	public:
		explicit ExtensionNotPresentException( std::string const & name )
			: Exception{ VK_ERROR_EXTENSION_NOT_PRESENT, name }
		{
		}
	};

	class Buffer;
	class BufferView;
	class CommandBuffer;
	class CommandPool;
	class DebugReportCallbackEXT;
	class DescriptorPool;
	class DescriptorSet;
	class DescriptorSetLayout;
	class Device;
	class DeviceMemory;
	class DeviceMemoryBinding;
	class Event;
	class Fence;
	class Framebuffer;
	class Image;
	class ImageView;
	class Instance;
	class PhysicalDevice;
	class Pipeline;
	class PipelineCache;
	class PipelineLayout;
	class QueryPool;
	class Queue;
	class RenderPass;
	class RenderWindow;
	class Sampler;
	class Semaphore;
	class ShaderModule;
	class SurfaceKHR;
	class SwapchainKHR;

	class SamplerYcbcrConversion;
	class DescriptorUpdateTemplate;
	class DisplayKHR;
	class DisplayModeKHR;
	class ObjectTableNVX;
	class IndirectCommandsLayoutNVX;
	class DebugUtilsMessengerEXT;
	class ValidationCacheEXT;

	struct ContextState;

	class CommandBase;
	class Context;
	class ContextImpl;
	class ContextLock;
	class ContextStateStack;
	class ExtensionsHandler;
	class FrameBufferAttachment;
	class GeometryBuffers;
	class ShaderProgram;

	using ContextPtr = std::unique_ptr< Context >;
	using CommandPtr = std::unique_ptr< CommandBase >;
	using ContextImplPtr = std::unique_ptr< ContextImpl >;
	using DeviceMemoryBindingPtr = std::unique_ptr< DeviceMemoryBinding >;

	using CommandArray = std::vector< CommandPtr >;
	using ContextStateArray = std::vector< ContextState >;

	using ShaderProgramPtr = std::unique_ptr< ShaderProgram >;
	
	using GeometryBuffersRef = std::reference_wrapper< GeometryBuffers >;
	using GeometryBuffersPtr = std::unique_ptr< GeometryBuffers >;
	using GeometryBuffersRefArray = std::vector< GeometryBuffersRef >;
	using VkDeviceMemorySet = std::unordered_set< VkDeviceMemory >;

	struct GlPhysicalDeviceFeatures
	{
		VkBool32 has420PackExtensions;
		VkBool32 hasCopyImage;
		VkBool32 hasImmutableStorage;
		VkBool32 hasProgramPipelines;
		VkBool32 hasTextureStorage;
		VkBool32 hasTextureViews;
		VkBool32 hasViewportArrays;
		VkBool32 hasProgramInterfaceQuery;
	};

	struct AttachmentDescription
	{
		uint32_t index;
		std::reference_wrapper< VkAttachmentDescription const > attach;
	};

	using AttachmentDescriptionArray = std::vector< AttachmentDescription >;

	struct BufferObjectBinding
	{
		GLuint bo;
		uint64_t offset;
		VkBuffer buffer;
	};

	using VboBindings = std::map< uint32_t, BufferObjectBinding >;
	using IboBinding = Optional< BufferObjectBinding >;

	using DeviceMemoryDestroyFunc = std::function< void( GLuint ) >;
	using DeviceMemoryDestroySignal = Signal< DeviceMemoryDestroyFunc >;
	using DeviceMemoryDestroyConnection = SignalConnection< DeviceMemoryDestroySignal >;

	using CmdBuffer = UInt32Array;
	using CmdList = std::vector< CmdBuffer >;
	using PreExecuteAction = std::function< void( CmdList &, ContextStateStack const & ) >;
	using PreExecuteActions = std::vector< PreExecuteAction >;

	struct FboAttachment
	{
		uint32_t referenceIndex{};
		GlAttachmentPoint point{};
		GLuint object{};
		GLuint originalObject{};
		GlAttachmentType type{};
		uint32_t index{};
		uint32_t imgLayerCount{};
		uint32_t viewLayerCount{};
		GlTextureType target{};
		uint32_t baseArrayLayer{};
		GLuint originalMipLevel{};
		uint32_t mipLevel{};
		bool isSrgb{};

		FboAttachment() = default;
		FboAttachment( VkDevice device
			, uint32_t referenceIndex
			, VkImageView view
			, uint32_t index
			, bool & multisampled );
		FboAttachment( VkDevice device
			, VkImageSubresourceLayers const & subresource
			, VkImage image );

		void bind( uint32_t mipLevel
			, GlFrameBufferTarget fboTarget
			, CmdList & list )const;
		void bindIndex( uint32_t mipLevel
			, GlFrameBufferTarget fboTarget
			, uint32_t index
			, CmdList & list )const;
		void bindRead( ContextStateStack & stack
			, uint32_t mipLevel
			, GlFrameBufferTarget fboTarget
			, CmdList & list )const;
		void bindDraw( ContextStateStack & stack
			, uint32_t mipLevel
			, GlFrameBufferTarget fboTarget
			, CmdList & list )const;
		void bind( uint32_t mipLevel
			, uint32_t layer
			, GlFrameBufferTarget fboTarget
			, CmdList & list )const;
		void bindIndex( uint32_t mipLevel
			, uint32_t layer
			, GlFrameBufferTarget fboTarget
			, uint32_t index
			, CmdList & list )const;
		void bindRead( ContextStateStack & stack
			, uint32_t mipLevel
			, uint32_t layer
			, GlFrameBufferTarget fboTarget
			, CmdList & list )const;
		void bindDraw( ContextStateStack & stack
			, uint32_t mipLevel
			, uint32_t layer
			, GlFrameBufferTarget fboTarget
			, CmdList & list )const;
		void bind( uint32_t mipLevel
			, uint32_t layer
			, uint32_t slice
			, GlFrameBufferTarget fboTarget
			, CmdList & list )const;
		void bindIndex( uint32_t mipLevel
			, uint32_t layer
			, uint32_t slice
			, GlFrameBufferTarget fboTarget
			, uint32_t index
			, CmdList & list )const;
		void bindRead( ContextStateStack & stack
			, uint32_t mipLevel
			, uint32_t layer
			, uint32_t slice
			, GlFrameBufferTarget fboTarget
			, CmdList & list )const;
		void bindDraw( ContextStateStack & stack
			, uint32_t mipLevel
			, uint32_t layer
			, uint32_t slice
			, GlFrameBufferTarget fboTarget
			, CmdList & list )const;
		void read( ContextStateStack & stack
			, CmdList & list )const;
		void draw( ContextStateStack & stack
			, CmdList & list )const;

		bool isDepthOrStencil()const
		{
			return point == GL_ATTACHMENT_POINT_DEPTH_STENCIL
				|| point == GL_ATTACHMENT_POINT_DEPTH
				|| point == GL_ATTACHMENT_POINT_STENCIL;
		}
	};

	using FboAttachmentArray = std::vector< FboAttachment >;

	struct LayerCopy
	{
		LayerCopy( VkDevice device
			, VkImageBlit origRegion
			, VkImage srcImage
			, VkImage dstImage );
		LayerCopy( VkDevice device
			, VkImageCopy const & origRegion
			, VkImage srcImage
			, VkImage dstImage );

		void bindSrc( ContextStateStack & stack
			, GlFrameBufferTarget fboTarget
			, CmdList & list )const
		{
			src.bindRead( stack
				, region.srcSubresource.mipLevel
				, fboTarget
				, list );
		}

		void bindDst( ContextStateStack & stack
			, GlFrameBufferTarget fboTarget
			, CmdList & list )const
		{
			dst.bindDraw( stack
				, region.dstSubresource.mipLevel
				, fboTarget
				, list );
		}

		void bindSrc( ContextStateStack & stack
			, uint32_t layer
			, GlFrameBufferTarget fboTarget
			, CmdList & list )const
		{
			src.bindRead( stack
				, region.srcSubresource.mipLevel
				, layer
				, fboTarget
				, list );
		}

		void bindDst( ContextStateStack & stack
			, uint32_t layer
			, GlFrameBufferTarget fboTarget
			, CmdList & list )const
		{
			dst.bindDraw( stack
				, region.dstSubresource.mipLevel
				, layer
				, fboTarget
				, list );
		}

		void bindSrc( ContextStateStack & stack
			, uint32_t layer
			, uint32_t slice
			, GlFrameBufferTarget fboTarget
			, CmdList & list )const
		{
			src.bindRead( stack
				, region.srcSubresource.mipLevel
				, layer
				, slice
				, fboTarget
				, list );
		}

		void bindDst( ContextStateStack & stack
			, uint32_t layer
			, uint32_t slice
			, GlFrameBufferTarget fboTarget
			, CmdList & list )const
		{
			dst.bindDraw( stack
				, region.dstSubresource.mipLevel
				, layer
				, slice
				, fboTarget
				, list );
		}

		GlAttachmentPoint getSrcPoint()const
		{
			return src.point;
		}

		GlAttachmentPoint getDstPoint()const
		{
			return dst.point;
		}

		VkImageBlit const & getRegion()const
		{
			return region;
		}

		bool isSrcDepthOrStencil()const
		{
			return src.isDepthOrStencil();
		}

		bool isDstDepthOrStencil()const
		{
			return dst.isDepthOrStencil();
		}

	private:
		VkImageBlit region;
		FboAttachment src;
		FboAttachment dst;
	};
	PFN_vkVoidFunction getFunction( char const * const name );

	namespace details
	{
		inline std::ostream & operator<<( std::ostream & stream, VkExtensionProperties const & value )
		{
			stream << value.extensionName << " [core since " << ashes::getMajor( value.specVersion ) << "." << ashes::getMinor( value.specVersion ) << "]";
			return stream;
		}

		template< typename FuncT >
		inline bool getFunctionRec( bool tryCore
			, std::string const & name
			, FuncT & function
			, std::stringstream & errStream )
		{
			if ( !tryCore )
			{
				return false;
			}

#pragma warning( push )
#pragma warning( disable: 4191 )
			function = FuncT( getFunction( name.c_str() ) );
#pragma warning( pop )

			if ( function )
			{
				return true;
			}

			errStream << "\n  Tried: " << name;
			return false;
		}

		template< typename FuncT, typename ParamT >
		inline bool getFunctionRec( [[maybe_unused]] bool tryCore
			, std::string const & name
			, FuncT & function
			, std::stringstream & errStream
			, char const * const lastShort
			, VkExtensionProperties const & lastExtension )
		{
#pragma warning( push )
#pragma warning( disable: 4191 )
			function = FuncT( getFunction( ( name + lastShort ).c_str() ) );
#pragma warning( pop )

			if ( function )
			{
				return true;
			}

			errStream << "\n  Tried: " << name << lastShort << " (" << lastExtension << ")";
			return false;
		}

		template< typename FuncT, typename ... ParamsT >
		inline bool getFunctionRec( bool tryCore
			, std::string const & name
			, FuncT & function
			, std::stringstream & errStream
			, char const * const currentShort
			, VkExtensionProperties const & currentExtension
			, ParamsT ... params )
		{
#pragma warning( push )
#pragma warning( disable: 4191 )
			function = FuncT( getFunction( ( name + currentShort ).c_str() ) );
#pragma warning( pop )

			if ( function )
			{
				return true;
			}

			errStream << "\n  Tried: " << name << currentShort << " (" << currentExtension << ")";
			return getFunctionRec( tryCore
				, name
				, function
				, errStream
				, params... );
		}
	}

	template< typename FuncT, typename ... ParamsT >
	inline bool getFunction( char const * const name
		, FuncT & function
		, std::stringstream & errStream
		, ParamsT ... params )
	{
		return details::getFunctionRec( true
			, std::string{ name ? name : "" }
			, function
			, errStream
			, params... );
	}

	template< typename FuncT, typename ... ParamsT >
	inline bool getFunction( std::string const & name
		, FuncT & function
		, std::stringstream & errStream
		, ParamsT ... params )
	{
		return details::getFunctionRec( true
			, name
			, function
			, errStream
			, params... );
	}

	template< typename FuncT, typename ... ParamsT >
	inline bool getExtFunction( std::string const & name
		, FuncT & function
		, std::stringstream & errStream
		, ParamsT ... params )
	{
		return details::getFunctionRec( false
			, name
			, function
			, errStream
			, params... );
	}

	inline void * getBufferOffset( intptr_t value )
	{
		using BytePtr = uint8_t *;
		return reinterpret_cast< void * >( BytePtr{} + value );
	}

	uint32_t deduceMemoryType( VkDevice device
		, uint32_t typeBits
		, VkMemoryPropertyFlags requirements );
	uint32_t getScreenIndex( VkDisplayModeKHR displayMode );
	VkDisplayModeParametersKHR getDisplayModeParameters( VkDisplayModeKHR displayMode );
	VkExtent2D getDisplayResolution( VkDisplayModeKHR displayMode );
}
