#include "Core/GlContext.hpp"

#include "Core/GlContextLock.hpp"

#if VK_USE_PLATFORM_WIN32_KHR
#	include "Core/GlMswContext.hpp"
#elif VK_USE_PLATFORM_XLIB_KHR
#	include "Core/GlX11Context.hpp"
#else
#	error "Unsupported surface type"
#endif

#include "ashesgl4_api.hpp"

#include <iostream>
#include <locale>

#if Ashes_LogContextLocking

#define logContextLock()\
	std::stringstream stream;\
	stream.imbue( std::locale{ "C" } );\
	stream << "Context [0x" << std::setfill( '0' ) << std::setw( 8 ) << std::hex << this << "] locked by thread [" << std::this_thread::get_id() << "]";\
	get( m_instance )->reportMessage( VK_DEBUG_REPORT_DEBUG_BIT_EXT\
		, VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT\
		, uint64_t( m_instance )\
		, 0u\
		, 0u\
		, "CONTEXT"\
		, stream.str().c_str() )

#define logContextUnlock()\
	std::stringstream stream;\
	stream.imbue( std::locale{ "C" } );\
	stream << "Context [0x" << std::setfill( '0' ) << std::setw( 8 ) << std::hex << this << "] unlocked by thread [" << std::this_thread::get_id() << "]";\
	get( m_instance )->reportMessage( VK_DEBUG_REPORT_DEBUG_BIT_EXT\
		, VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT\
		, uint64_t( m_instance )\
		, 0u\
		, 0u\
		, "CONTEXT"\
		, stream.str().c_str() )

#else

#define logContextLock()
#define logContextUnlock()

#endif

namespace ashes::gl4
{
	//*************************************************************************

	using ashes::operator==;
	using ashes::operator!=;

	Context const & getContext( ContextLock const & lock )
	{
		return lock.getContext();
	}

	//*************************************************************************

	namespace
	{
		void doApply( ContextLock const & context
			, VkPipelineColorBlendStateCreateInfo const & state )
		{
			if ( state.logicOpEnable )
			{
				glLogCall( context
					, glLogicOp
					, convert( state.logicOp ) );
			}

			auto & blendConstants = state.blendConstants;
			glLogCall( context
				, glBlendColor
				, blendConstants[0]
				, blendConstants[1]
				, blendConstants[2]
				, blendConstants[3] );

			bool blend = false;
			GLuint buf = 0u;

			for ( auto it = state.pAttachments;
				it != state.pAttachments + state.attachmentCount;
				++it )
			{
				auto & blendState = *it;

				if ( blendState.blendEnable )
				{
					blend = true;
					glLogCall( context
						, glBlendEquationSeparatei
						, buf
						, convert( blendState.colorBlendOp )
						, convert( blendState.alphaBlendOp ) );
					glLogCall( context
						, glBlendFuncSeparatei
						, buf
						, convert( blendState.srcColorBlendFactor )
						, convert( blendState.dstColorBlendFactor )
						, convert( blendState.srcAlphaBlendFactor )
						, convert( blendState.dstAlphaBlendFactor ) );
				}

				++buf;
			}

			if ( blend )
			{
				glLogCall( context
					, glEnable
					, GlTweak( GL_BLEND ) );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GlTweak( GL_BLEND ) );
			}
		}

		void doApply( ContextLock const & context
			, VkPipelineRasterizationStateCreateInfo const & state )
		{
			if ( state.cullMode != VK_CULL_MODE_NONE )
			{
				glLogCall( context
					, glEnable
					, GlTweak( GL_CULL_FACE ) );
				glLogCall( context
					, glCullFace
					, convertCullModeFlags( state.cullMode ) );
				glLogCall( context
					, glFrontFace
					, convert( state.frontFace ) );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GlTweak( GL_CULL_FACE ) );
			}

			glLogCall( context
				, glPolygonMode
				, GL_CULL_MODE_FRONT_AND_BACK
				, convert( state.polygonMode ) );

			if ( state.depthBiasEnable )
			{
				switch ( state.polygonMode )
				{
				case VK_POLYGON_MODE_FILL:
					glLogCall( context
						, glEnable
						, GlTweak( GL_POLYGON_OFFSET_FILL ) );
					break;

				case VK_POLYGON_MODE_LINE:
					glLogCall( context
						, glEnable
						, GlTweak( GL_POLYGON_OFFSET_LINE ) );
					break;

				case VK_POLYGON_MODE_POINT:
					glLogCall( context
						, glEnable
						, GlTweak( GL_POLYGON_OFFSET_POINT ) );
					break;
				}

				glLogCall( context
					, glPolygonOffsetClampEXT
					, state.depthBiasConstantFactor
					, state.depthBiasSlopeFactor
					, state.depthBiasClamp );
			}
			else
			{
				switch ( state.polygonMode )
				{
				case VK_POLYGON_MODE_FILL:
					glLogCall( context
						, glDisable
						, GlTweak( GL_POLYGON_OFFSET_FILL ) );
					break;

				case VK_POLYGON_MODE_LINE:
					glLogCall( context
						, glDisable
						, GlTweak( GL_POLYGON_OFFSET_LINE ) );
					break;

				case VK_POLYGON_MODE_POINT:
					glLogCall( context
						, glDisable
						, GlTweak( GL_POLYGON_OFFSET_POINT ) );
					break;
				}
			}

			if ( state.depthClampEnable )
			{
				glLogCall( context
					, glEnable
					, GL_DEPTH_CLAMP );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_DEPTH_CLAMP );
			}

			if ( state.rasterizerDiscardEnable )
			{
				glLogCall( context
					, glEnable
					, GL_RASTERIZER_DISCARD );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_RASTERIZER_DISCARD );
			}

			glLogCall( context
				, glLineWidth
				, state.lineWidth );
		}

		void doApply( ContextLock const & context
			, VkPipelineMultisampleStateCreateInfo const & state )
		{
			if ( state.rasterizationSamples != VK_SAMPLE_COUNT_1_BIT )
			{
				glLogCall( context
					, glEnable
					, GL_MULTISAMPLE );

				if ( state.alphaToCoverageEnable )
				{
					glLogCall( context
						, glEnable
						, GL_SAMPLE_ALPHA_TO_COVERAGE );
				}
				else
				{
					glLogCall( context
						, glDisable
						, GL_SAMPLE_ALPHA_TO_COVERAGE );
				}

				if ( state.alphaToOneEnable )
				{
					glLogCall( context, glEnable, GL_SAMPLE_ALPHA_TO_ONE );
				}
				else
				{
					glLogCall( context, glDisable, GL_SAMPLE_ALPHA_TO_ONE );
				}
			}
			else
			{
				glLogCall( context, glDisable, GL_MULTISAMPLE );
			}
		}

		void doApply( ContextLock const & context
			, VkPipelineDepthStencilStateCreateInfo const & state )
		{
			if ( state.depthWriteEnable )
			{
				glLogCall( context
					, glDepthMask
					, GL_TRUE );
			}
			else
			{
				glLogCall( context
					, glDepthMask
					, GL_FALSE );
			}

			if ( state.depthTestEnable )
			{
				glLogCall( context
					, glEnable
					, GlTweak( GL_DEPTH_TEST ) );
				glLogCall( context
					, glDepthFunc
					, convert( state.depthCompareOp ) );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GlTweak( GL_DEPTH_TEST ) );
			}

			if ( state.stencilTestEnable )
			{
				glLogCall( context
					, glEnable
					, GlTweak( GL_STENCIL_TEST ) );
				glLogCall( context
					, glStencilMaskSeparate
					, GL_CULL_MODE_BACK
					, state.back.writeMask );
				glLogCall( context
					, glStencilFuncSeparate
					, GL_CULL_MODE_BACK
					, convert( state.back.compareOp )
					, state.back.reference
					, state.back.compareMask );
				glLogCall( context
					, glStencilOpSeparate
					, GL_CULL_MODE_BACK
					, convert( state.back.failOp )
					, convert( state.back.depthFailOp )
					, convert( state.back.passOp ) );
				glLogCall( context
					, glStencilMaskSeparate
					, GL_CULL_MODE_FRONT
					, state.front.writeMask );
				glLogCall( context
					, glStencilFuncSeparate
					, GL_CULL_MODE_FRONT
					, convert( state.front.compareOp )
					, state.front.reference
					, state.front.compareMask );
				glLogCall( context
					, glStencilOpSeparate
					, GL_CULL_MODE_FRONT
					, convert( state.front.failOp )
					, convert( state.front.depthFailOp )
					, convert( state.front.passOp ) );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GlTweak( GL_STENCIL_TEST ) );
			}

			if ( state.depthBoundsTestEnable )
			{
				glLogCall( context
					, glEnable
					, GL_DEPTH_CLAMP );
				glLogCall( context
					, glDepthRange
					, state.minDepthBounds
					, state.maxDepthBounds );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_DEPTH_CLAMP );
			}
		}

		void doApply( ContextLock const & context
			, VkPipelineTessellationStateCreateInfo const & state )
		{
			if ( state.patchControlPoints )
			{
				glLogCall( context
					, glPatchParameteri
					, GL_PATCH_VERTICES
					, int( state.patchControlPoints ) );
			}
		}

		void doApply( ContextLock const & context
			, VkPipelineInputAssemblyStateCreateInfo const & state )
		{
			if ( state.topology == VK_PRIMITIVE_TOPOLOGY_POINT_LIST )
			{
				glLogCall( context
					, glEnable
					, GL_PROGRAM_POINT_SIZE );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_PROGRAM_POINT_SIZE );
			}

			if ( state.primitiveRestartEnable )
			{
				glLogCall( context
					, glEnable
					, GL_PRIMITIVE_RESTART );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_PRIMITIVE_RESTART );
			}
		}
	}

	//*************************************************************************

	Context::Context( std::unique_ptr< ContextImpl > impl )
		: m_impl{ std::move( impl ) }
		, m_instance{ m_impl->instance }
		, createInfo{ m_impl->createInfo }
		, m_state{}
	{
		m_impl->initialise( *this );
		ContextLock lock{ *this };
		glLogCall( lock
			, glEnable
			, GL_TEXTURE_CUBE_MAP_SEAMLESS );

		doApply( lock, m_state.cbState );
		doApply( lock, m_state.dsState );
		doApply( lock, m_state.msState );
		doApply( lock, m_state.rsState );
		doApply( lock, m_state.iaState );
	}

	Context::~Context()
	{
	}

	void Context::apply( ContextLock const & context
		, Device const & device
		, ContextState const & state )
	{
		apply( context, state.cbState );
		apply( context, state.dsState );
		apply( context, state.msState );
		apply( context, state.rsState, state.dyState );
		apply( context, state.iaState );

		if ( device.getEnabledFeatures().tessellationShader )
		{
			apply( context, state.tsState );
		}
	}

#if defined( VK_USE_PLATFORM_ANDROID_KHR )

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

#elif defined( VK_USE_PLATFORM_FUCHSIA )

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

#elif defined( VK_USE_PLATFORM_IOS_MVK )

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

#elif defined( VK_USE_PLATFORM_MACOS_MVK )

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

#elif defined( VK_USE_PLATFORM_VI_NN )

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

#elif defined( VK_USE_PLATFORM_XCB_KHR )

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

#elif defined( VK_USE_PLATFORM_XLIB_KHR )

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		return std::unique_ptr< Context >( new Context
			{
				std::make_unique< X11Context >( instance
					, std::move( createInfo )
					, mainContext )
			} );
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		return std::unique_ptr< Context >( new Context
			{
				std::make_unique< X11Context >( instance
					, surface
					, mainContext )
			} );
	}

#elif defined( VK_USE_PLATFORM_WAYLAND_KHR )

ContextPtr Context::create( VkInstance instance
		, VkSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

#elif defined( VK_USE_PLATFORM_WIN32_KHR )

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		return std::unique_ptr< Context >( new Context
			{
				std::make_unique< MswContext >( instance
					, std::move( createInfo )
					, mainContext )
			} );
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		return std::unique_ptr< Context >( new Context
			{
				std::make_unique< MswContext >( instance
					, surface
					, mainContext )
			} );
	}

#endif

	void Context::onBaseContextCreated()
	{
		loadBaseFunctions();
		loadDebugFunctions();
	}

	void Context::lock()
	{
		m_mutex.lock();
		m_enabled = true;
		m_activeThread = std::this_thread::get_id();
		m_impl->enable();
		logContextLock();
	}

	void Context::unlock()
	{
		logContextUnlock();
		m_impl->disable();
		m_enabled = false;
		m_mutex.unlock();
	}

	void Context::loadBaseFunctions()
	{
#define GL_LIB_BASE_FUNCTION( fun )\
		m_gl##fun = PFN_gl##fun( &::gl##fun );
#define GL_LIB_FUNCTION( fun )\
		if ( !( getFunction( "gl"#fun, m_gl##fun ) ) )\
		{\
			throw std::runtime_error{ std::string{ "Couldn't load function " } + "gl"#fun };\
		}
#define GL_LIB_FUNCTION_OPT( fun )\
		if ( !( getFunction( "gl"#fun, m_gl##fun ) ) )\
		{\
			std::cerr << "Couldn't load function " << "gl"#fun << std::endl;\
		}
#include "Miscellaneous/OpenGLFunctionsList.inl"
	}

	void Context::loadDebugFunctions()
	{
		auto & extensions = get( m_impl->instance )->getExtensions();

		if ( extensions.find( KHR_debug ) )
		{
			if ( !getFunction( "glDebugMessageCallback", m_glDebugMessageCallback ) )
			{
				if ( !getFunction( "glDebugMessageCallbackKHR", m_glDebugMessageCallback ) )
				{
					std::cerr << "Unable to retrieve function glDebugMessageCallback" << std::endl;
				}
			}
		}
		else if ( extensions.find( ARB_debug_output ) )
		{
			if ( !getFunction( "glDebugMessageCallback", m_glDebugMessageCallback ) )
			{
				if ( !getFunction( "glDebugMessageCallbackARB", m_glDebugMessageCallback ) )
				{
					std::cerr << "Unable to retrieve function glDebugMessageCallback" << std::endl;
				}
			}
		}
		else if ( extensions.find( AMDX_debug_output ) )
		{
			if ( !getFunction( "glDebugMessageCallbackAMD", m_glDebugMessageCallbackAMD ) )
			{
				std::cerr << "Unable to retrieve function glDebugMessageCallbackAMD" << std::endl;
			}
		}
	}

	void Context::apply( ContextLock const & context
		, VkPipelineInputAssemblyStateCreateInfo const & state )
	{
		auto & save = m_state.iaState;

		if ( state.topology != save.topology )
		{
			if ( state.topology == VK_PRIMITIVE_TOPOLOGY_POINT_LIST )
			{
				glLogCall( context
					, glEnable
					, GL_PROGRAM_POINT_SIZE );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_PROGRAM_POINT_SIZE );
			}
		}

		if ( state.primitiveRestartEnable != save.primitiveRestartEnable )
		{
			if ( state.primitiveRestartEnable )
			{
				glLogCall( context
					, glEnable
					, GL_PRIMITIVE_RESTART );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_PRIMITIVE_RESTART );
			}
		}
	}

	void Context::apply( ContextLock const & context
		, VkPipelineColorBlendStateCreateInfo const & state )
	{
		auto & save = m_state.cbState;
		static bool hadBlend = false;

		if ( state.logicOpEnable
			&& !save.logicOpEnable
			&& state.logicOp != save.logicOp )
		{
			glLogCall( context
				, glLogicOp
				, convert( state.logicOp ) );
		}

		if ( state.blendConstants != save.blendConstants )
		{
			auto & blendConstants = state.blendConstants;
			glLogCall( context
				, glBlendColor
				, blendConstants[0]
				, blendConstants[1]
				, blendConstants[2]
				, blendConstants[3] );
		}

		bool blend = false;
		GLuint buf = 0u;

		for ( auto it = state.pAttachments;
			it != state.pAttachments + state.attachmentCount;
			++it )
		{
			auto & blendState = *it;

			if ( blendState.blendEnable )
			{
				blend = true;
				glLogCall( context
					, glBlendEquationSeparatei
					, buf
					, convert( blendState.colorBlendOp )
					, convert( blendState.alphaBlendOp ) );
				glLogCall( context
					, glBlendFuncSeparatei
					, buf
					, convert( blendState.srcColorBlendFactor )
					, convert( blendState.dstColorBlendFactor )
					, convert( blendState.srcAlphaBlendFactor )
					, convert( blendState.dstAlphaBlendFactor ) );
			}

			++buf;
		}

		if ( hadBlend != blend )
		{
			if ( blend )
			{
				glLogCall( context
					, glEnable
					, GlTweak( GL_BLEND ) );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GlTweak( GL_BLEND ) );
			}
		}

		save = state;
		hadBlend = blend;
	}

	void Context::apply( ContextLock const & context
		, VkPipelineRasterizationStateCreateInfo const & state
		, VkPipelineDynamicStateCreateInfo newDyState )
	{
		auto & save = m_state.rsState;

		if ( state.cullMode != save.cullMode
			|| state.frontFace != save.frontFace )
		{
			if ( state.cullMode != VK_CULL_MODE_NONE )
			{
				if ( save.cullMode == VK_CULL_MODE_NONE )
				{
					glLogCall( context
						, glEnable
						, GlTweak( GL_CULL_FACE ) );
				}

				if ( state.cullMode != save.cullMode )
				{
					glLogCall( context
						, glCullFace
						, convertCullModeFlags( state.cullMode ) );
				}

				if ( state.frontFace != save.frontFace )
				{
					glLogCall( context
						, glFrontFace
						, convert( state.frontFace ) );
				}
			}
			else if ( save.cullMode != VK_CULL_MODE_NONE )
			{
				glLogCall( context
					, glDisable
					, GlTweak( GL_CULL_FACE ) );
			}
		}

		if ( state.polygonMode != save.polygonMode )
		{
			glLogCall( context
				, glPolygonMode
				, GL_CULL_MODE_FRONT_AND_BACK
				, convert( state.polygonMode ) );
		}

		if ( state.depthBiasEnable != save.depthBiasEnable
			|| state.polygonMode != save.polygonMode )
		{
			if ( state.depthBiasEnable )
			{
				switch ( state.polygonMode )
				{
				case VK_POLYGON_MODE_FILL:
					glLogCall( context
						, glEnable
						, GlTweak( GL_POLYGON_OFFSET_FILL ) );
					break;

				case VK_POLYGON_MODE_LINE:
					glLogCall( context
						, glEnable
						, GlTweak( GL_POLYGON_OFFSET_LINE ) );
					break;

				case VK_POLYGON_MODE_POINT:
					glLogCall( context
						, glEnable
						, GlTweak( GL_POLYGON_OFFSET_POINT ) );
					break;
				}

				if ( !hasDynamicState( newDyState.pDynamicStates
					, newDyState.pDynamicStates + newDyState.dynamicStateCount
					, VK_DYNAMIC_STATE_DEPTH_BIAS ) )
				{
					glLogCall( context
						, glPolygonOffsetClampEXT
						, state.depthBiasConstantFactor
						, state.depthBiasSlopeFactor
						, state.depthBiasClamp );
				}
			}
			else
			{
				switch ( state.polygonMode )
				{
				case VK_POLYGON_MODE_FILL:
					glLogCall( context
						, glDisable
						, GlTweak( GL_POLYGON_OFFSET_FILL ) );
					break;

				case VK_POLYGON_MODE_LINE:
					glLogCall( context
						, glDisable
						, GlTweak( GL_POLYGON_OFFSET_LINE ) );
					break;

				case VK_POLYGON_MODE_POINT:
					glLogCall( context
						, glDisable
						, GlTweak( GL_POLYGON_OFFSET_POINT ) );
					break;
				}
			}
		}

		if ( state.depthClampEnable != save.depthClampEnable )
		{
			if ( state.depthClampEnable )
			{
				glLogCall( context
					, glEnable
					, GL_DEPTH_CLAMP );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_DEPTH_CLAMP );
			}
		}

		if ( state.rasterizerDiscardEnable != save.rasterizerDiscardEnable )
		{
			if ( state.rasterizerDiscardEnable )
			{
				glLogCall( context
					, glEnable
					, GL_RASTERIZER_DISCARD );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_RASTERIZER_DISCARD );
			}
		}

		if ( state.polygonMode == VK_POLYGON_MODE_LINE
			&& !hasDynamicState( newDyState.pDynamicStates
				, newDyState.pDynamicStates + newDyState.dynamicStateCount
				, VK_DYNAMIC_STATE_LINE_WIDTH ) )
		{
			glLogCall( context
				, glLineWidth
				, state.lineWidth );
		}

		save = state;
	}

	void Context::apply( ContextLock const & context
		, VkPipelineMultisampleStateCreateInfo const & state )
	{
		auto & save = m_state.msState;

		if ( state.rasterizationSamples != VK_SAMPLE_COUNT_1_BIT )
		{
			glLogCall( context
				, glEnable
				, GL_MULTISAMPLE );

			if ( state.alphaToCoverageEnable != save.alphaToCoverageEnable )
			{
				if ( state.alphaToCoverageEnable )
				{
					glLogCall( context
						, glEnable
						, GL_SAMPLE_ALPHA_TO_COVERAGE );
				}
				else
				{
					glLogCall( context
						, glDisable
						, GL_SAMPLE_ALPHA_TO_COVERAGE );
				}
			}

			if ( state.alphaToOneEnable != save.alphaToOneEnable )
			{
				if ( state.alphaToOneEnable )
				{
					glLogCall( context
						, glEnable
						, GL_SAMPLE_ALPHA_TO_ONE );
				}
				else
				{
					glLogCall( context
						, glDisable
						, GL_SAMPLE_ALPHA_TO_ONE );
				}
			}

			if ( state.sampleShadingEnable )
			{
				glLogCall( context
					, glEnable
					, GL_SAMPLE_SHADING );
				glLogCall( context
					, glMinSampleShading
					, state.minSampleShading );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_SAMPLE_SHADING );
			}
		}
		else
		{
			glLogCall( context
				, glDisable
				, GL_MULTISAMPLE );
		}

		save = state;
	}

	void Context::apply( ContextLock const & context
		, VkPipelineDepthStencilStateCreateInfo const & state )
	{
		auto & save = m_state.dsState;

		if ( state.depthWriteEnable )
		{
			glLogCall( context
				, glDepthMask
				, GL_TRUE );
		}
		else
		{
			glLogCall( context
				, glDepthMask
				, GL_FALSE );
		}

		if ( state.depthTestEnable != save.depthTestEnable )
		{
			if ( state.depthTestEnable )
			{
				glLogCall( context
					, glEnable
					, GlTweak( GL_DEPTH_TEST ) );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GlTweak( GL_DEPTH_TEST ) );
			}
		}

		if ( state.depthTestEnable
			&& state.depthCompareOp != save.depthCompareOp )
		{
			glLogCall( context
				, glDepthFunc
				, convert( state.depthCompareOp ) );
		}

		if ( state.stencilTestEnable != save.stencilTestEnable
			|| ( state.stencilTestEnable
				&& ( state.back != save.back
					|| state.front != save.front ) ) )
		{
			if ( state.stencilTestEnable )
			{
				glLogCall( context
					, glEnable
					, GlTweak( GL_STENCIL_TEST ) );
				glLogCall( context
					, glStencilMaskSeparate
					, GL_CULL_MODE_BACK
					, state.back.writeMask );
				glLogCall( context
					, glStencilFuncSeparate
					, GL_CULL_MODE_BACK
					, convert( state.back.compareOp )
					, state.back.reference
					, state.back.compareMask );
				glLogCall( context
					, glStencilOpSeparate
					, GL_CULL_MODE_BACK
					, convert( state.back.failOp )
					, convert( state.back.depthFailOp )
					, convert( state.back.passOp ) );
				glLogCall( context
					, glStencilMaskSeparate
					, GL_CULL_MODE_FRONT
					, state.front.writeMask );
				glLogCall( context
					, glStencilFuncSeparate
					, GL_CULL_MODE_FRONT
					, convert( state.front.compareOp )
					, state.front.reference
					, state.front.compareMask );
				glLogCall( context
					, glStencilOpSeparate
					, GL_CULL_MODE_FRONT
					, convert( state.front.failOp )
					, convert( state.front.depthFailOp )
					, convert( state.front.passOp ) );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GlTweak( GL_STENCIL_TEST ) );
			}
		}

		if ( state.depthBoundsTestEnable != save.depthBoundsTestEnable
			|| ( state.depthBoundsTestEnable
				&& ( state.minDepthBounds != save.minDepthBounds
					|| state.maxDepthBounds != save.maxDepthBounds ) ) )
		{
			if ( state.depthBoundsTestEnable )
			{
				glLogCall( context
					, glEnable
					, GL_DEPTH_CLAMP );
				glLogCall( context
					, glDepthRange
					, state.minDepthBounds
					, state.maxDepthBounds );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_DEPTH_CLAMP );
			}
		}

		save = state;
	}

	void Context::apply( ContextLock const & context
		, VkPipelineTessellationStateCreateInfo const & state )
	{
		auto & save = m_state.tsState;

		if ( state.patchControlPoints != save.patchControlPoints
			&& state.patchControlPoints )
		{
			glLogCall( context
				, glPatchParameteri
				, GL_PATCH_VERTICES
				, int( state.patchControlPoints ) );
		}

		save = state;
	}

	void Context::apply( ContextLock const & context
		, VkPipelineViewportStateCreateInfo const & state )
	{
		auto & save = m_state.vpState;

		auto & viewport = *state.pViewports;
		glLogCall( context
			, glViewport
			, GLsizei( viewport.x )
			, GLsizei( viewport.y )
			, GLint( viewport.width )
			, GLint( viewport.height ) );
		glLogCall( context
			, glDepthRange
			, viewport.minDepth
			, viewport.maxDepth );

		auto & scissor = *state.pScissors;
		glLogCall( context
			, glScissor
			, scissor.offset.x
			, scissor.offset.y
			, scissor.extent.width
			, scissor.extent.height );

		save = state;
	}

	//*************************************************************************
}
