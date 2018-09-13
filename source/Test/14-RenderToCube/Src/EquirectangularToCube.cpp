#include "EquirectangularToCube.hpp"

#include <Buffer/StagingBuffer.hpp>
#include <Buffer/UniformBuffer.hpp>
#include <Buffer/VertexBuffer.hpp>
#include <Command/Queue.hpp>
#include <Core/Device.hpp>
#include <Descriptor/DescriptorSet.hpp>
#include <Descriptor/DescriptorSetLayout.hpp>
#include <Descriptor/DescriptorSetLayoutBinding.hpp>
#include <Descriptor/DescriptorSetPool.hpp>
#include <Image/StagingTexture.hpp>
#include <Image/Texture.hpp>
#include <Image/TextureView.hpp>
#include <Pipeline/DepthStencilState.hpp>
#include <Pipeline/InputAssemblyState.hpp>
#include <Pipeline/MultisampleState.hpp>
#include <Pipeline/Scissor.hpp>
#include <Pipeline/VertexLayout.hpp>
#include <Pipeline/Viewport.hpp>
#include <RenderPass/FrameBuffer.hpp>
#include <RenderPass/RenderPassCreateInfo.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>
#include <Shader/ShaderProgram.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

#include <Transform.hpp>

#include <FileUtils.hpp>

namespace vkapp
{
	using utils::Vec3;
	using utils::Vec4;
	using utils::Mat4;

	namespace
	{
		ashes::TexturePtr doCreateTexture( ashes::Device & device
			, ashes::CommandBuffer const & commandBuffer
			, common::ImageData const & image )
		{
			auto result = device.createTexture(
				{
					0u,
					ashes::TextureType::e2D,
					image.format,
					ashes::Extent3D{ image.size.width, image.size.height, 1u },
					1u,
					1u,
					ashes::SampleCountFlag::e1,
					ashes::ImageTiling::eOptimal,
					ashes::ImageUsageFlag::eSampled | ashes::ImageUsageFlag::eTransferDst
				}
				, ashes::MemoryPropertyFlag::eDeviceLocal );
			auto view = result->createView( ashes::TextureViewType::e2D
				, image.format );

			auto staging = device.createStagingTexture( image.format
				, { image.size.width, image.size.height, 1u } );
			staging->uploadTextureData( commandBuffer
				, image.format
				, image.data
				, *view );
			return result;
		}

		ashes::SamplerPtr doCreateSampler( ashes::Device & device )
		{
			return device.createSampler( ashes::WrapMode::eClampToEdge
				, ashes::WrapMode::eClampToEdge
				, ashes::WrapMode::eClampToEdge
				, ashes::Filter::eLinear
				, ashes::Filter::eLinear );
		}

		ashes::UniformBufferPtr< Mat4 > doCreateMatrixUbo( ashes::Device & device
			, ashes::CommandBuffer const & commandBuffer
			, ashes::StagingBuffer & stagingBuffer )
		{
			static Mat4 const projection = utils::Mat4{ device.perspective( float( utils::toRadians( 90.0_degrees ) ), 1.0f, 0.1f, 10.0f ) };
			static std::array< Mat4, 6u > const views = [&device]()
			{
				std::array< Mat4, 6u > result
				{
					utils::lookAt( Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ +1.0f, +0.0f, +0.0f }, Vec3{ 0.0f, -1.0f, +0.0f } ),
					utils::lookAt( Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ -1.0f, +0.0f, +0.0f }, Vec3{ 0.0f, -1.0f, +0.0f } ),
					utils::lookAt( Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ +0.0f, +1.0f, +0.0f }, Vec3{ 0.0f, +0.0f, +1.0f } ),
					utils::lookAt( Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ +0.0f, -1.0f, +0.0f }, Vec3{ 0.0f, +0.0f, -1.0f } ),
					utils::lookAt( Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ +0.0f, +0.0f, +1.0f }, Vec3{ 0.0f, -1.0f, +0.0f } ),
					utils::lookAt( Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ +0.0f, +0.0f, -1.0f }, Vec3{ 0.0f, -1.0f, +0.0f } )
				};

				if ( device.getRenderer().getName().find( "d3d" ) != std::string::npos )
				{
					std::swap( result[2], result[3] );
				}

				return result;
			}();

			auto result = ashes::makeUniformBuffer< utils::Mat4 >( device
				, 6u
				, ashes::BufferTarget::eTransferDst
				, ashes::MemoryPropertyFlag::eHostVisible );

			result->getData( 0u ) = projection * views[0];
			result->getData( 1u ) = projection * views[1];
			result->getData( 2u ) = projection * views[2];
			result->getData( 3u ) = projection * views[3];
			result->getData( 4u ) = projection * views[4];
			result->getData( 5u ) = projection * views[5];

			stagingBuffer.uploadUniformData( commandBuffer
				, result->getDatas()
				, *result
				, ashes::PipelineStageFlag::eVertexShader );
			return result;
		}

		std::vector< ashes::ShaderStageState > doCreateProgram( ashes::Device & device )
		{
			std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";
			std::vector< ashes::ShaderStageState > shaderStages;
			shaderStages.push_back( { device.createShaderModule( ashes::ShaderStageFlag::eVertex ) } );
			shaderStages.push_back( { device.createShaderModule( ashes::ShaderStageFlag::eFragment ) } );

			if ( device.getRenderer().isGLSLSupported()
				|| device.getRenderer().isSPIRVSupported() )
			{
				if ( !wxFileExists( shadersFolder / "equirectangular.vert" )
					|| !wxFileExists( shadersFolder / "equirectangular.frag" ) )
				{
					throw std::runtime_error{ "Shader files are missing" };
				}

				shaderStages[0].module->loadShader( common::parseShaderFile( device, shadersFolder / "equirectangular.vert" ) );
				shaderStages[1].module->loadShader( common::parseShaderFile( device, shadersFolder / "equirectangular.frag" ) );
			}
			else
			{
				if ( !wxFileExists( shadersFolder / "equirectangular.hvert" )
					|| !wxFileExists( shadersFolder / "equirectangular.hpix" ) )
				{
					throw std::runtime_error{ "Shader files are missing" };
				}

				shaderStages[0].module->loadShader( common::parseShaderFile( device, shadersFolder / "equirectangular.hvert" ) );
				shaderStages[0].entryPoint = "mainVx";
				shaderStages[1].module->loadShader( common::parseShaderFile( device, shadersFolder / "equirectangular.hpix" ) );
				shaderStages[1].entryPoint = "mainPx";
			}

			return shaderStages;
		}

		ashes::VertexBufferPtr< VertexData > doCreateVertexBuffer( ashes::Device & device
			, ashes::CommandBuffer const & commandBuffer
			, ashes::StagingBuffer & stagingBuffer )
		{
			std::vector< VertexData > vertexData
			{
				{
					{ Vec4{ -1, +1, -1, 1 } }, { Vec4{ +1, -1, -1, 1 } }, { Vec4{ -1, -1, -1, 1 } }, { Vec4{ +1, -1, -1, 1 } }, { Vec4{ -1, +1, -1, 1 } }, { Vec4{ +1, +1, -1, 1 } },// Back
					{ Vec4{ -1, -1, +1, 1 } }, { Vec4{ -1, +1, -1, 1 } }, { Vec4{ -1, -1, -1, 1 } }, { Vec4{ -1, +1, -1, 1 } }, { Vec4{ -1, -1, +1, 1 } }, { Vec4{ -1, +1, +1, 1 } },// Left
					{ Vec4{ +1, -1, -1, 1 } }, { Vec4{ +1, +1, +1, 1 } }, { Vec4{ +1, -1, +1, 1 } }, { Vec4{ +1, +1, +1, 1 } }, { Vec4{ +1, -1, -1, 1 } }, { Vec4{ +1, +1, -1, 1 } },// Right
					{ Vec4{ -1, -1, +1, 1 } }, { Vec4{ +1, +1, +1, 1 } }, { Vec4{ -1, +1, +1, 1 } }, { Vec4{ +1, +1, +1, 1 } }, { Vec4{ -1, -1, +1, 1 } }, { Vec4{ +1, -1, +1, 1 } },// Front
					{ Vec4{ -1, +1, -1, 1 } }, { Vec4{ +1, +1, +1, 1 } }, { Vec4{ +1, +1, -1, 1 } }, { Vec4{ +1, +1, +1, 1 } }, { Vec4{ -1, +1, -1, 1 } }, { Vec4{ -1, +1, +1, 1 } },// Top
					{ Vec4{ -1, -1, -1, 1 } }, { Vec4{ +1, -1, -1, 1 } }, { Vec4{ -1, -1, +1, 1 } }, { Vec4{ +1, -1, -1, 1 } }, { Vec4{ +1, -1, +1, 1 } }, { Vec4{ -1, -1, +1, 1 } },// Bottom
				}
			};
			auto result = ashes::makeVertexBuffer< VertexData >( device
				, 36u
				, ashes::BufferTarget::eTransferDst
				, ashes::MemoryPropertyFlag::eHostVisible );

			stagingBuffer.uploadVertexData( commandBuffer
				, vertexData
				, *result );

			return result;
		}

		ashes::VertexLayoutPtr doCreateVertexLayout( ashes::Device & device )
		{
			auto result = ashes::makeLayout< VertexData >( 0u );
			result->createAttribute( 0u
				, ashes::Format::eR32G32B32A32_SFLOAT
				, 0u
				, "POSITION"
				, 0u );
			return result;
		}

		ashes::DescriptorSetLayoutPtr doCreateDescriptorSetLayout( ashes::Device & device )
		{
			ashes::DescriptorSetLayoutBindingArray bindings
			{
				{ 0u, ashes::DescriptorType::eUniformBuffer, ashes::ShaderStageFlag::eVertex },
				{ 1u, ashes::DescriptorType::eCombinedImageSampler, ashes::ShaderStageFlag::eFragment },
			};
			return device.createDescriptorSetLayout( std::move( bindings ) );
		}

		ashes::RenderPassPtr doCreateRenderPass( ashes::Device const & device
			, ashes::Format format )
		{
			ashes::RenderPassCreateInfo renderPass;
			renderPass.flags = 0u;

			renderPass.attachments.resize( 1u );
			renderPass.attachments[0].format = format;
			renderPass.attachments[0].loadOp = ashes::AttachmentLoadOp::eClear;
			renderPass.attachments[0].storeOp = ashes::AttachmentStoreOp::eStore;
			renderPass.attachments[0].stencilLoadOp = ashes::AttachmentLoadOp::eDontCare;
			renderPass.attachments[0].stencilStoreOp = ashes::AttachmentStoreOp::eDontCare;
			renderPass.attachments[0].samples = ashes::SampleCountFlag::e1;
			renderPass.attachments[0].initialLayout = ashes::ImageLayout::eUndefined;
			renderPass.attachments[0].finalLayout = ashes::ImageLayout::eShaderReadOnlyOptimal;

			renderPass.subpasses.resize( 1u );
			renderPass.subpasses[0].flags = 0u;
			renderPass.subpasses[0].pipelineBindPoint = ashes::PipelineBindPoint::eGraphics;
			renderPass.subpasses[0].colorAttachments.push_back( { 0u, ashes::ImageLayout::eColourAttachmentOptimal } );

			renderPass.dependencies.resize( 2u );
			renderPass.dependencies[0].srcSubpass = ashes::ExternalSubpass;
			renderPass.dependencies[0].dstSubpass = 0u;
			renderPass.dependencies[0].srcStageMask = ashes::PipelineStageFlag::eBottomOfPipe;
			renderPass.dependencies[0].dstStageMask = ashes::PipelineStageFlag::eColourAttachmentOutput;
			renderPass.dependencies[0].srcAccessMask = 0u;
			renderPass.dependencies[0].dstAccessMask = ashes::AccessFlag::eColourAttachmentWrite;
			renderPass.dependencies[0].dependencyFlags = ashes::DependencyFlag::eByRegion;

			renderPass.dependencies[1].srcSubpass = 0u;
			renderPass.dependencies[1].dstSubpass = ashes::ExternalSubpass;
			renderPass.dependencies[1].srcStageMask = ashes::PipelineStageFlag::eColourAttachmentOutput;
			renderPass.dependencies[1].dstStageMask = ashes::PipelineStageFlag::eColourAttachmentOutput;
			renderPass.dependencies[1].srcAccessMask = ashes::AccessFlag::eColourAttachmentWrite;
			renderPass.dependencies[1].dstAccessMask = ashes::AccessFlag::eShaderRead;
			renderPass.dependencies[1].dependencyFlags = ashes::DependencyFlag::eByRegion;

			return device.createRenderPass( renderPass );
		}
	}

	EquirectangularToCube::EquirectangularToCube( std::string const & filePath
		, ashes::Device & device
		, ashes::Texture & texture )
		: m_device{ device }
		, m_commandBuffer{ device.getGraphicsCommandPool().createCommandBuffer() }
		, m_image{ common::loadImage( filePath ) }
		, m_stagingBuffer{ device, ashes::BufferTarget::eTransferSrc, uint32_t( m_image.data.size() ) }
		, m_texture{ doCreateTexture( m_device, *m_commandBuffer, m_image ) }
		, m_view{ m_texture->createView( ashes::TextureViewType::e2D, m_image.format ) }
		, m_sampler{ doCreateSampler( m_device ) }
		, m_matrixUbo{ doCreateMatrixUbo( m_device, *m_commandBuffer, m_stagingBuffer ) }
		, m_vertexBuffer{ doCreateVertexBuffer( m_device, *m_commandBuffer, m_stagingBuffer ) }
		, m_vertexLayout{ doCreateVertexLayout( m_device ) }
		, m_descriptorLayout{ doCreateDescriptorSetLayout( m_device ) }
		, m_descriptorPool{ m_descriptorLayout->createPool( 6u ) }
		, m_pipelineLayout{ m_device.createPipelineLayout( *m_descriptorLayout ) }
		, m_renderPass{ doCreateRenderPass( m_device, texture.getFormat() ) }
	{
		auto size = ashes::Extent2D{ texture.getDimensions().width, texture.getDimensions().height };
		uint32_t face = 0u;

		for ( auto & facePipeline : m_faces )
		{
			ashes::FrameBufferAttachmentArray attaches;
			facePipeline.view = texture.createView( ashes::TextureViewType::e2D, texture.getFormat(), 0u, 1u, face, 1u );
			attaches.emplace_back( *m_renderPass->getAttachments().begin(), *facePipeline.view );
			facePipeline.frameBuffer = m_renderPass->createFrameBuffer( size
				, std::move( attaches ) );

			facePipeline.pipeline = m_pipelineLayout->createPipeline( ashes::GraphicsPipelineCreateInfo
			{
				doCreateProgram( m_device ),
				*m_renderPass,
				ashes::VertexInputState::create( *m_vertexLayout ),
				ashes::InputAssemblyState{ ashes::PrimitiveTopology::eTriangleList },
				ashes::RasterisationState{ 0u, false, false, ashes::PolygonMode::eFill, ashes::CullModeFlag::eNone },
				ashes::MultisampleState{},
				ashes::ColourBlendState::createDefault(),
				{},
				ashes::DepthStencilState{ 0u, false, false },
				ashes::TessellationState{},
				ashes::Viewport{ size.width, size.height, 0, 0 },
				ashes::Scissor{ 0, 0, size.width, size.height }
			} );

			facePipeline.descriptorSet = m_descriptorPool->createDescriptorSet();
			facePipeline.descriptorSet->createBinding( m_descriptorLayout->getBinding( 0u )
				, *m_matrixUbo
				, face
				, 1u );
			facePipeline.descriptorSet->createBinding( m_descriptorLayout->getBinding( 1u )
				, *m_view
				, *m_sampler );
			facePipeline.descriptorSet->update();
			++face;
		}
	}

	void EquirectangularToCube::render( ashes::CommandBuffer & commandBuffer )
	{
		for ( auto & facePipeline : m_faces )
		{
			commandBuffer.memoryBarrier( ashes::PipelineStageFlag::eTransfer
				, ashes::PipelineStageFlag::eColourAttachmentOutput
				, facePipeline.view->makeColourAttachment( ashes::ImageLayout::eUndefined
					, 0u ) );
			commandBuffer.beginRenderPass( *m_renderPass
				, *facePipeline.frameBuffer
				, { ashes::ClearColorValue{ 0, 0, 0, 0 } }
			, ashes::SubpassContents::eInline );
			commandBuffer.bindPipeline( *facePipeline.pipeline );
			commandBuffer.bindDescriptorSet( *facePipeline.descriptorSet
				, *m_pipelineLayout );
			commandBuffer.bindVertexBuffer( 0u, m_vertexBuffer->getBuffer(), 0u );
			commandBuffer.draw( m_vertexBuffer->getCount() );
			commandBuffer.endRenderPass();
		}

	}

	void EquirectangularToCube::render()
	{
		m_commandBuffer->begin( ashes::CommandBufferUsageFlag::eOneTimeSubmit );
		render( *m_commandBuffer );
		m_commandBuffer->end();
		m_device.getGraphicsQueue().submit( *m_commandBuffer, nullptr );
		m_device.getGraphicsQueue().waitIdle();
	}
}
