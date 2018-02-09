#include "OpaqueRendering.hpp"

#include <Buffer/Buffer.hpp>
#include <Buffer/GeometryBuffers.hpp>
#include <Buffer/StagingBuffer.hpp>
#include <Buffer/VertexBuffer.hpp>
#include <Command/CommandBuffer.hpp>
#include <Command/CommandPool.hpp>
#include <Core/Device.hpp>
#include <Descriptor/DescriptorSet.hpp>
#include <Descriptor/DescriptorSetLayout.hpp>
#include <Descriptor/DescriptorSetLayoutBinding.hpp>
#include <Descriptor/DescriptorSetPool.hpp>
#include <Image/Texture.hpp>
#include <Image/TextureView.hpp>
#include <Pipeline/DepthStencilState.hpp>
#include <Pipeline/MultisampleState.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <Pipeline/Scissor.hpp>
#include <Pipeline/VertexLayout.hpp>
#include <Pipeline/Viewport.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderPassState.hpp>
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>
#include <RenderPass/FrameBufferAttachment.hpp>
#include <Shader/ShaderProgram.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

#include <FileUtils.hpp>

#include <algorithm>

namespace vkapp
{
	namespace
	{
		renderer::ShaderProgramPtr doCreateProgram( renderer::Device const & device )
		{
			renderer::ShaderProgramPtr result = device.createShaderProgram();
			std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";

			if ( false && result->isSPIRVSupported() )
			{
				if ( !wxFileExists( shadersFolder / "opaque_vert.spv" )
					|| !wxFileExists( shadersFolder / "opaque_frag.spv" ) )
				{
					throw std::runtime_error{ "Shader files are missing" };
				}

				result->createModule( common::dumpBinaryFile( shadersFolder / "opaque_vert.spv" )
					, renderer::ShaderStageFlag::eVertex );
				result->createModule( common::dumpBinaryFile( shadersFolder / "opaque_frag.spv" )
					, renderer::ShaderStageFlag::eFragment );
			}
			else
			{
				if ( !wxFileExists( shadersFolder / "opaque.vert" )
					|| !wxFileExists( shadersFolder / "opaque.frag" ) )
				{
					throw std::runtime_error{ "Shader files are missing" };
				}

				result->createModule( common::dumpTextFile( shadersFolder / "opaque.vert" )
					, renderer::ShaderStageFlag::eVertex );
				result->createModule( common::dumpTextFile( shadersFolder / "opaque.frag" )
					, renderer::ShaderStageFlag::eFragment );
			}

			return result;
		}

		std::vector< renderer::PixelFormat > doGetFormats( renderer::TextureView const & colourView
			, renderer::TextureView const & depthView )
		{
			return
			{
				colourView.getFormat(),
				depthView.getFormat()
			};
		}

		renderer::RenderPassAttachmentArray doGetAttaches( renderer::TextureView const & depthView
			, renderer::TextureView const & colourView )
		{
			renderer::RenderPassAttachmentArray result;
			auto formats = doGetFormats( depthView, colourView );

			for ( auto format : formats )
			{
				result.push_back( { format, false } );
			}

			return result;
		}

		renderer::RenderPassPtr doCreateRenderPass( renderer::Device const & device
			, renderer::TextureView const & colourView
			, renderer::TextureView const & depthView )
		{
			auto formats = doGetFormats( colourView, depthView );
			auto attaches = doGetAttaches( colourView, depthView );
			renderer::RenderSubpassPtrArray subpasses;
			subpasses.emplace_back( device.createRenderSubpass( formats
				, { renderer::PipelineStageFlag::eColourAttachmentOutput, renderer::AccessFlag::eColourAttachmentWrite } ) );
			return device.createRenderPass( attaches
				, subpasses
				, renderer::RenderPassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
					, renderer::AccessFlag::eColourAttachmentWrite
					, { renderer::ImageLayout::eColourAttachmentOptimal, renderer::ImageLayout::eDepthStencilAttachmentOptimal } }
				, renderer::RenderPassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
					, renderer::AccessFlag::eColourAttachmentWrite
					, { renderer::ImageLayout::eColourAttachmentOptimal, renderer::ImageLayout::eDepthStencilAttachmentOptimal } } );
		}

		renderer::FrameBufferPtr doCreateFrameBuffer( renderer::RenderPass const & renderPass
			, renderer::TextureView const & colourView
			, renderer::TextureView const & depthView )
		{
			auto formats = doGetFormats( colourView, depthView );
			renderer::FrameBufferAttachmentArray attaches;
			attaches.emplace_back( *( renderPass.begin() + 0u ), depthView );
			attaches.emplace_back( *( renderPass.begin() + 1u ), colourView );
			auto dimensions = colourView.getTexture().getDimensions();
			return renderPass.createFrameBuffer( renderer::UIVec2{ dimensions[0], dimensions[1] }
				, std::move( attaches ) );
		}

		renderer::UniformBufferPtr< common::MaterialData > doCreateMaterialsUbo( renderer::Device const & device
			, common::Object const & submeshes
			, uint32_t & count )
		{
			count = 0u;

			for ( auto & submesh : submeshes )
			{
				count += std::count_if( submesh.materials.begin()
					, submesh.materials.end()
					, []( common::Material const & lookup )
					{
						return !lookup.hasOpacity;
					} );
			}

			renderer::UniformBufferPtr< common::MaterialData > result;

			if ( count )
			{
				result = std::make_unique< renderer::UniformBuffer< common::MaterialData > >( device
					, count
					, renderer::BufferTarget::eTransferDst
					, renderer::MemoryPropertyFlag::eDeviceLocal );
			}

			return result;
		}

		renderer::DescriptorSetLayoutPtr doCreateDescriptorLayout( renderer::Device const & device )
		{
			std::vector< renderer::DescriptorSetLayoutBinding > bindings
			{
				renderer::DescriptorSetLayoutBinding{ 0u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex },
				renderer::DescriptorSetLayoutBinding{ 1u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex },
				renderer::DescriptorSetLayoutBinding{ 2u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eFragment },
				renderer::DescriptorSetLayoutBinding{ 3u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eFragment },
			};
			return device.createDescriptorSetLayout( std::move( bindings ) );
		}

		renderer::TexturePtr doCreateTexture( renderer::Device const & device
			, renderer::UIVec3 const & size )
		{
			auto result = device.createTexture();
			result->setImage( utils::PixelFormat::eR8G8B8A8
				, { size[0], size[1] }
				, renderer::ImageUsageFlag::eColourAttachment | renderer::ImageUsageFlag::eSampled );
			return result;
		}
	}

	OpaqueRendering::OpaqueRendering( renderer::Device const & device
		, common::Object const & submeshes
		, renderer::UniformBuffer< renderer::Mat4 > const & matrixUbo
		, renderer::UniformBuffer< renderer::Mat4 > const & objectUbo
		, renderer::UniformBuffer< common::LightsData > const & lightsUbo
		, renderer::StagingBuffer & stagingBuffer
		, renderer::TextureView const & colourView
		, renderer::TextureView const & depthView
		, common::TextureNodePtrArray const & textureNodes )
		: m_device{ device }
		, m_matrixUbo{ matrixUbo }
		, m_objectUbo{ objectUbo }
		, m_lightsUbo{ lightsUbo }
		, m_stagingBuffer{ stagingBuffer }
		, m_geometryPass{ device
			, submeshes
			, matrixUbo
			, objectUbo
			, stagingBuffer
			, depthView
			, textureNodes }
		, m_lightingPass{ device
			, lightsUbo
			, stagingBuffer
			, depthView
			, colourView }
	{
		update( colourView, depthView );
	}

	void OpaqueRendering::update( renderer::TextureView const & colourView
		, renderer::TextureView const & depthView )
	{
		m_colourView = &colourView;
		m_depthView = &depthView;
		m_geometryPass.update( depthView );
		m_lightingPass.update( m_matrixUbo.getData( 0u )
			, m_stagingBuffer
			, colourView
			, depthView
			, m_geometryPass.getResult() );
	}

	bool OpaqueRendering::draw()const
	{
		auto result = m_geometryPass.draw();
		result &= m_lightingPass.draw();
		return result;
	}
}
