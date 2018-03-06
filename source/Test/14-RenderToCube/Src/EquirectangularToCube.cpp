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
#include <Image/Texture.hpp>
#include <Image/TextureView.hpp>
#include <Pipeline/DepthStencilState.hpp>
#include <Pipeline/InputAssemblyState.hpp>
#include <Pipeline/MultisampleState.hpp>
#include <Pipeline/Scissor.hpp>
#include <Pipeline/VertexLayout.hpp>
#include <Pipeline/Viewport.hpp>
#include <RenderPass/FrameBuffer.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>
#include <Shader/ShaderProgram.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

#include <Utils/Transform.hpp>

#include <FileUtils.hpp>

namespace vkapp
{
	using renderer::Vec3;
	using renderer::Vec4;
	using renderer::Mat4;

	namespace
	{
		renderer::TexturePtr doCreateTexture( renderer::Device & device
			, renderer::CommandBuffer const & commandBuffer
			, common::ImageData const & image )
		{
			auto result = device.createTexture(
				{
					0u,
					renderer::TextureType::e2D,
					image.format,
					renderer::Extent3D{ image.size.width, image.size.height, 1u },
					1u,
					1u,
					renderer::SampleCountFlag::e1,
					renderer::ImageTiling::eOptimal,
					renderer::ImageUsageFlag::eSampled | renderer::ImageUsageFlag::eTransferDst
				}
				, renderer::MemoryPropertyFlag::eDeviceLocal );

			renderer::BufferBasePtr staging = device.createBuffer( uint32_t( image.data.size() )
				, renderer::BufferTarget::eTransferSrc
				, renderer::MemoryPropertyFlag::eHostVisible | renderer::MemoryPropertyFlag::eHostCoherent );

			if ( auto * buffer = staging->lock( 0u, uint32_t( image.data.size() ), renderer::MemoryMapFlag::eWrite ) )
			{
				std::memcpy( buffer, image.data.data(), image.data.size() );
				staging->unlock();
			}

			renderer::BufferImageCopy bufferCopyRegion{};
			bufferCopyRegion.imageSubresource.aspectMask = renderer::ImageAspectFlag::eColour;
			bufferCopyRegion.imageSubresource.mipLevel = 0u;
			bufferCopyRegion.imageSubresource.baseArrayLayer = 0u;
			bufferCopyRegion.imageSubresource.layerCount = 1u;
			bufferCopyRegion.imageExtent.width = image.size.width;
			bufferCopyRegion.imageExtent.height = image.size.height;
			bufferCopyRegion.imageExtent.depth = 1;
			bufferCopyRegion.bufferOffset = 0u;
			bufferCopyRegion.levelSize = uint32_t( image.data.size() );
			renderer::ImageSubresourceRange subresourceRange
			{
				renderer::ImageAspectFlag::eColour,
				0u,
				1u,
				0u,
				1u,
			};
			commandBuffer.begin();
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eAllCommands
				, renderer::PipelineStageFlag::eAllCommands
				, renderer::ImageMemoryBarrier{ 0u
					, renderer::AccessFlag::eTransferWrite
					, renderer::ImageLayout::eUndefined
					, renderer::ImageLayout::eTransferDstOptimal
					, ~( 0u )
					, ~( 0u )
					, *result
					, subresourceRange } );
			commandBuffer.copyToImage( bufferCopyRegion
				, *staging
				, *result );
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eAllCommands
				, renderer::PipelineStageFlag::eAllCommands
				, renderer::ImageMemoryBarrier{ renderer::AccessFlag::eTransferWrite
					, renderer::AccessFlag::eShaderRead
					, renderer::ImageLayout::eTransferDstOptimal
					, renderer::ImageLayout::eShaderReadOnlyOptimal
					, ~( 0u )
					, ~( 0u )
					, *result
				, subresourceRange } );

			commandBuffer.end();
			auto fence = device.createFence();
			device.getGraphicsQueue().submit( commandBuffer, fence.get() );
			fence->wait( renderer::FenceTimeout );

			return result;
		}

		renderer::SamplerPtr doCreateSampler( renderer::Device & device )
		{
			return device.createSampler( renderer::WrapMode::eClampToEdge
				, renderer::WrapMode::eClampToEdge
				, renderer::WrapMode::eClampToEdge
				, renderer::Filter::eLinear
				, renderer::Filter::eLinear );
		}

		renderer::UniformBufferPtr< Mat4 > doCreateMatrixUbo( renderer::Device & device
			, renderer::CommandBuffer const & commandBuffer
			, renderer::StagingBuffer & stagingBuffer )
		{
			static Mat4 const projection = device.perspective( utils::toRadians( 90.0_degrees ), 1.0f, 0.1f, 10.0f );
			static Mat4 const views[] =
			{
				utils::lookAt( Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ +1.0f, +0.0f, +0.0f }, Vec3{ 0.0f, -1.0f, +0.0f } ),
				utils::lookAt( Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ -1.0f, +0.0f, +0.0f }, Vec3{ 0.0f, -1.0f, +0.0f } ),
				utils::lookAt( Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ +0.0f, +1.0f, +0.0f }, Vec3{ 0.0f, +0.0f, +1.0f } ),
				utils::lookAt( Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ +0.0f, -1.0f, +0.0f }, Vec3{ 0.0f, +0.0f, -1.0f } ),
				utils::lookAt( Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ +0.0f, +0.0f, +1.0f }, Vec3{ 0.0f, -1.0f, +0.0f } ),
				utils::lookAt( Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ +0.0f, +0.0f, -1.0f }, Vec3{ 0.0f, -1.0f, +0.0f } )
			};

			auto result = renderer::makeUniformBuffer< renderer::Mat4 >( device
				, 6u
				, renderer::BufferTarget::eTransferDst
				, renderer::MemoryPropertyFlag::eHostVisible );

			if ( device.getClipDirection() == renderer::ClipDirection::eTopDown )
			{
				result->getData( 0u ) = projection * views[0];
				result->getData( 1u ) = projection * views[1];
				result->getData( 2u ) = projection * views[2];
				result->getData( 3u ) = projection * views[3];
				result->getData( 4u ) = projection * views[4];
				result->getData( 5u ) = projection * views[5];
			}
			else
			{
				result->getData( 0u ) = projection * views[0];
				result->getData( 1u ) = projection * views[1];
				result->getData( 2u ) = projection * views[3];
				result->getData( 3u ) = projection * views[2];
				result->getData( 4u ) = projection * views[4];
				result->getData( 5u ) = projection * views[5];
			}

			stagingBuffer.uploadUniformData( commandBuffer
				, result->getDatas()
				, *result
				, renderer::PipelineStageFlag::eVertexShader );
			return result;
		}

		std::vector< renderer::ShaderStageState > doCreateProgram( renderer::Device & device )
		{
			std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";

			if ( !wxFileExists( shadersFolder / "equirectangular.vert" )
				|| !wxFileExists( shadersFolder / "equirectangular.frag" ) )
			{
				throw std::runtime_error{ "Shader files are missing" };
			}

			std::vector< renderer::ShaderStageState > shaderStages;
			shaderStages.push_back( { device.createShaderModule( renderer::ShaderStageFlag::eVertex ) } );
			shaderStages.push_back( { device.createShaderModule( renderer::ShaderStageFlag::eFragment ) } );
			shaderStages[0].module->loadShader( common::parseShaderFile( device, shadersFolder / "equirectangular.vert" ) );
			shaderStages[1].module->loadShader( common::parseShaderFile( device, shadersFolder / "equirectangular.frag" ) );

			return shaderStages;
		}

		renderer::VertexBufferPtr< VertexData > doCreateVertexBuffer( renderer::Device & device
			, renderer::CommandBuffer const & commandBuffer
			, renderer::StagingBuffer & stagingBuffer )
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
			auto result = renderer::makeVertexBuffer< VertexData >( device
				, 36u
				, renderer::BufferTarget::eTransferDst
				, renderer::MemoryPropertyFlag::eHostVisible );

			stagingBuffer.uploadVertexData( commandBuffer
				, vertexData
				, *result
				, renderer::PipelineStageFlag::eVertexShader );

			return result;
		}

		renderer::VertexLayoutPtr doCreateVertexLayout( renderer::Device & device )
		{
			auto result = renderer::makeLayout< VertexData >( 0u );
			result->createAttribute< Vec4 >( 0u, 0u );
			return result;
		}

		renderer::DescriptorSetLayoutPtr doCreateDescriptorSetLayout( renderer::Device & device )
		{
			renderer::DescriptorSetLayoutBindingArray bindings
			{
				{ 0u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex },
				{ 1u, renderer::DescriptorType::eCombinedImageSampler, renderer::ShaderStageFlag::eFragment },
			};
			return device.createDescriptorSetLayout( std::move( bindings ) );
		}
	}

	EquirectangularToCube::EquirectangularToCube( std::string const & filePath
		, renderer::Device & device
		, renderer::Texture & texture )
		: m_device{ device }
		, m_target{ texture }
		, m_commandBuffer{ device.getGraphicsCommandPool().createCommandBuffer() }
		, m_image{ common::loadImage( filePath ) }
		, m_stagingBuffer{ device, renderer::BufferTarget::eTransferSrc, uint32_t( m_image.data.size() ) }
		, m_texture{ doCreateTexture( m_device, *m_commandBuffer, m_image ) }
		, m_view{ m_texture->createView( renderer::TextureType::e2D, m_image.format ) }
		, m_sampler{ doCreateSampler( m_device ) }
		, m_matrixUbo{ doCreateMatrixUbo( m_device, *m_commandBuffer, m_stagingBuffer ) }
		, m_vertexBuffer{ doCreateVertexBuffer( m_device, *m_commandBuffer, m_stagingBuffer ) }
		, m_vertexLayout{ doCreateVertexLayout( m_device ) }
		, m_descriptorLayout{ doCreateDescriptorSetLayout( m_device ) }
		, m_descriptorPool{ m_descriptorLayout->createPool( 6u ) }
		, m_pipelineLayout{ m_device.createPipelineLayout( *m_descriptorLayout ) }
	{
		auto size = renderer::Extent2D{ texture.getDimensions().width, texture.getDimensions().height };
		uint32_t face = 0u;
		std::vector< renderer::PixelFormat > formats{ 1u, m_target.getFormat() };
		renderer::RenderPassAttachmentArray rpAttaches
		{
			{
				0u,
				m_target.getFormat(),
				renderer::SampleCountFlag::e1,
				renderer::AttachmentLoadOp::eClear,
				renderer::AttachmentStoreOp::eStore,
				renderer::AttachmentLoadOp::eDontCare,
				renderer::AttachmentStoreOp::eDontCare,
				renderer::ImageLayout::eUndefined,
				renderer::ImageLayout::eShaderReadOnlyOptimal,
			}
		};
		renderer::RenderSubpassAttachmentArray subAttaches
		{
			{ 0u, renderer::ImageLayout::eColourAttachmentOptimal }
		};

		for ( auto & facePipeline : m_faces )
		{
			facePipeline.view = m_target.createView( renderer::TextureType::e2D
				, m_target.getFormat()
				, 0u
				, 1u
				, face
				, 1u );
			++face;
		}

		face = 0u;

		for ( auto & facePipeline : m_faces )
		{
			renderer::RenderSubpassPtrArray subpasses;
			subpasses.emplace_back( m_device.createRenderSubpass( renderer::PipelineBindPoint::eGraphics
				, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
					, renderer::AccessFlag::eColourAttachmentWrite }
				, subAttaches ) );
			facePipeline.renderPass = m_device.createRenderPass( rpAttaches
				, std::move( subpasses )
				, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
					, renderer::AccessFlag::eColourAttachmentWrite }
				, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
					, renderer::AccessFlag::eShaderRead } );

			renderer::FrameBufferAttachmentArray attaches;
			attaches.emplace_back( *facePipeline.renderPass->begin(), *facePipeline.view );
			facePipeline.frameBuffer = facePipeline.renderPass->createFrameBuffer( size
				, std::move( attaches ) );

			facePipeline.pipeline = m_pipelineLayout->createPipeline( renderer::GraphicsPipelineCreateInfo
			{
				doCreateProgram( m_device ),
				*facePipeline.renderPass,
				renderer::VertexInputState::create( *m_vertexLayout ),
				renderer::InputAssemblyState{ renderer::PrimitiveTopology::eTriangleList },
				renderer::RasterisationState{},
				renderer::MultisampleState{},
				renderer::ColourBlendState::createDefault(),
				{},
				renderer::DepthStencilState{ 0u, false, false },
				renderer::TessellationState{},
				renderer::Viewport{ size.width, size.height, 0, 0 },
				renderer::Scissor{ 0, 0, size.width, size.height }
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

	void EquirectangularToCube::render( renderer::CommandBuffer & commandBuffer )
	{
		for ( auto & facePipeline : m_faces )
		{
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eTransfer
				, renderer::PipelineStageFlag::eColourAttachmentOutput
				, facePipeline.view->makeColourAttachment( renderer::ImageLayout::eUndefined
					, 0u ) );
			commandBuffer.beginRenderPass( *facePipeline.renderPass
				, *facePipeline.frameBuffer
				, { renderer::RgbaColour{ 0, 0, 0, 0 } }
			, renderer::SubpassContents::eInline );
			commandBuffer.bindPipeline( *facePipeline.pipeline );
			commandBuffer.bindDescriptorSet( *facePipeline.descriptorSet
				, *m_pipelineLayout );
			commandBuffer.bindVertexBuffer( 0u, m_vertexBuffer->getBuffer(), 0u );
			commandBuffer.draw( 36u );
			commandBuffer.endRenderPass();
		}

	}

	void EquirectangularToCube::render()
	{
		if ( m_commandBuffer->begin( renderer::CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			render( *m_commandBuffer );
			m_commandBuffer->end();
			m_device.getGraphicsQueue().submit( *m_commandBuffer, nullptr );
			m_device.getGraphicsQueue().waitIdle();
		}
	}
}
