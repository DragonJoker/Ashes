#include "EquirectangularToCube.hpp"

#include <Buffer/GeometryBuffers.hpp>
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
		renderer::TextureViewPtr doCreateTexture( renderer::Device & device
			, renderer::CommandBuffer const & commandBuffer
			, renderer::StagingBuffer & stagingBuffer
			, common::ImageData const & image
			, renderer::Texture & texture
			, std::string const & filePath )
		{
			texture.setImage( image.format, renderer::UIVec2{ image.size[0], image.size[1] } );

			auto result = texture.createView( renderer::TextureType::e2D
				, image.format );

			stagingBuffer.uploadTextureData( commandBuffer
				, image.data.data()
				, image.data.size()
				, *result );

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

		renderer::ShaderProgramPtr doCreateProgram( renderer::Device & device )
		{
			std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";
			auto result = device.createShaderProgram();

			if ( !wxFileExists( shadersFolder / "equirectangular.vert" )
				|| !wxFileExists( shadersFolder / "equirectangular.frag" ) )
			{
				throw std::runtime_error{ "Shader files are missing" };
			}

			result->createModule( common::parseShaderFile( device, shadersFolder / "equirectangular.vert" )
				, renderer::ShaderStageFlag::eVertex );
			result->createModule( common::parseShaderFile( device, shadersFolder / "equirectangular.frag" )
				, renderer::ShaderStageFlag::eFragment );

			return result;
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
			auto result = device.createVertexLayout( 0u, uint32_t( sizeof( VertexData ) ) );
			result->createAttribute< Vec4 >( 0u, 0u );
			return result;
		}

		renderer::GeometryBuffersPtr doCreateGeometryBuffer( renderer::Device & device
			, renderer::VertexBufferBase const & vbo
			, renderer::VertexLayout const & layout )
		{
			return device.createGeometryBuffers( vbo, 0u, layout );
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
		, m_texture{ device.createTexture() }
		, m_view{ doCreateTexture( m_device, *m_commandBuffer, m_stagingBuffer, m_image, *m_texture, filePath ) }
		, m_sampler{ doCreateSampler( m_device ) }
		, m_matrixUbo{ doCreateMatrixUbo( m_device, *m_commandBuffer, m_stagingBuffer ) }
		, m_program{ doCreateProgram( m_device ) }
		, m_vertexBuffer{ doCreateVertexBuffer( m_device, *m_commandBuffer, m_stagingBuffer ) }
		, m_vertexLayout{ doCreateVertexLayout( m_device ) }
		, m_geometryBuffers{ doCreateGeometryBuffer( m_device, *m_vertexBuffer, *m_vertexLayout ) }
		, m_descriptorLayout{ doCreateDescriptorSetLayout( m_device ) }
		, m_descriptorPool{ m_descriptorLayout->createPool( 6u ) }
		, m_pipelineLayout{ m_device.createPipelineLayout( *m_descriptorLayout ) }
	{
		auto size = renderer::UIVec2{ texture.getDimensions()[0], texture.getDimensions()[1] };
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
				*m_program,
				*facePipeline.renderPass,
				{ *m_vertexLayout },
				renderer::InputAssemblyState{ renderer::PrimitiveTopology::eTriangleList },
				renderer::RasterisationState{ 1.0f },
				renderer::MultisampleState{},
				renderer::ColourBlendState::createDefault(),
				renderer::DepthStencilState{ 0u, false, false },
				renderer::TessellationState{},
				renderer::Viewport{ size[0], size[1], 0, 0 },
				renderer::Scissor{ 0, 0, size[0], size[1] }
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
			commandBuffer.bindGeometryBuffers( *m_geometryBuffers );
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
