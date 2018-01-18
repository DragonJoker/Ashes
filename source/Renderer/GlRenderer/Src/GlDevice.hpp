/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <Core/Device.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Classe contenant les informations liées au GPU logique.
	*/
	class Device
		: public renderer::Device
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] renderer
		*	L'instance de Renderer.
		*\param[in] connection
		*	La connection à l'application.
		*/
		Device( renderer::Renderer const & renderer
			, renderer::ConnectionPtr && connection );
		/**
		*\copydoc		renderer::Device::createRenderPass
		*/
		renderer::RenderPassPtr createRenderPass( std::vector< renderer::PixelFormat > const & formats
			, renderer::RenderSubpassPtrArray const & subpasses
			, renderer::RenderPassState const & initialState
			, renderer::RenderPassState const & finalState
			, bool clear
			, renderer::SampleCountFlag samplesCount )const override;
		/**
		*\copydoc		renderer::Device::createRenderSubpass
		*/
		renderer::RenderSubpassPtr createRenderSubpass( std::vector< renderer::PixelFormat > const & formats
			, renderer::RenderSubpassState const & neededState )const override;
		/**
		*\copydoc		renderer::Device::createVertexLayout
		*/
		renderer::VertexLayoutPtr createVertexLayout( uint32_t bindingSlot
			, uint32_t stride )const override;
		/**
		*\copydoc		renderer::Device::createGeometryBuffers
		*/
		renderer::GeometryBuffersPtr createGeometryBuffers( renderer::VertexBufferBase const & vbo
			, uint64_t vboOffset
			, renderer::VertexLayout const & layout )const override;
		/**
		*\copydoc		renderer::Device::createGeometryBuffers
		*/
		renderer::GeometryBuffersPtr createGeometryBuffers( renderer::VertexBufferBase const & vbo
			, uint64_t vboOffset
			, renderer::VertexLayout const & layout
			, renderer::BufferBase const & ibo
			, uint64_t iboOffset
			, renderer::IndexType type )const override;
		/**
		*\copydoc		renderer::Device::createGeometryBuffers
		*/
		renderer::GeometryBuffersPtr createGeometryBuffers( renderer::VertexBufferCRefArray const & vbos
			, std::vector< uint64_t > vboOffsets
			, renderer::VertexLayoutCRefArray const & layouts )const override;
		/**
		*\copydoc		renderer::Device::createGeometryBuffers
		*/
		renderer::GeometryBuffersPtr createGeometryBuffers( renderer::VertexBufferCRefArray const & vbos
			, std::vector< uint64_t > vboOffsets
			, renderer::VertexLayoutCRefArray const & layouts
			, renderer::BufferBase const & ibo
			, uint64_t iboOffset
			, renderer::IndexType type )const override;
		/**
		*\copydoc		renderer::Device::createPipelineLayout
		*/
		renderer::PipelineLayoutPtr createPipelineLayout()const override;
		/**
		*\copydoc		renderer::Device::createPipelineLayout
		*/
		renderer::PipelineLayoutPtr createPipelineLayout( renderer::DescriptorSetLayout const & layout )const override;
		/**
		*\copydoc		renderer::Device::createPipeline
		*/
		renderer::PipelinePtr createPipeline( renderer::PipelineLayout const & layout
			, renderer::ShaderProgram const & program
			, renderer::VertexLayoutCRefArray const & vertexLayouts
			, renderer::RenderPass const & renderPass
			, renderer::PrimitiveTopology topology
			, renderer::RasterisationState const & rasterisationState
			, renderer::ColourBlendState const & colourBlendState )const override;
		/**
		*\copydoc		renderer::Device::createDescriptorSetLayout
		*/
		renderer::DescriptorSetLayoutPtr createDescriptorSetLayout( renderer::DescriptorSetLayoutBindingArray && bindings )const override;
		/**
		*\copydoc		renderer::Device::createTexture
		*/
		renderer::TexturePtr createTexture()const override;
		/**
		*\copydoc		renderer::Device::createTextureView
		*/
		renderer::TextureViewPtr createTextureView( renderer::Texture const & texture
			, renderer::PixelFormat format
			, uint32_t baseMipLevel
			, uint32_t levelCount
			, uint32_t baseArrayLayer
			, uint32_t layerCount )const override;
		/**
		*\copydoc		renderer::Device::createSampler
		*/
		renderer::SamplerPtr createSampler( renderer::WrapMode wrapS
			, renderer::WrapMode wrapT
			, renderer::WrapMode wrapR
			, renderer::Filter minFilter
			, renderer::Filter magFilter
			, renderer::MipmapMode mipFilter
			, float minLod
			, float maxLod
			, float lodBias
			, renderer::BorderColour borderColour
			, float maxAnisotropy
			, renderer::CompareOp compareOp )const override;
		/**
		*\copydoc		renderer::Device::createBuffer
		*/
		renderer::BufferBasePtr createBuffer( uint32_t size
			, renderer::BufferTargets target
			, renderer::MemoryPropertyFlags memoryFlags )const override;
		/**
		*\copydoc		renderer::Device::createBufferView
		*/
		renderer::BufferViewPtr createBufferView( renderer::BufferBase const & buffer
			, renderer::PixelFormat format
			, uint32_t offset
			, uint32_t range )const override;
		/**
		*\copydoc		renderer::Device::createUniformBuffer
		*/
		renderer::UniformBufferBasePtr createUniformBuffer( uint32_t count
			, uint32_t size
			, renderer::BufferTargets target
			, renderer::MemoryPropertyFlags memoryFlags )const override;
		/**
		*\copydoc		renderer::Device::createSwapChain
		*/
		renderer::SwapChainPtr createSwapChain( renderer::IVec2 const & size )const override;
		/**
		*\copydoc		renderer::Device::createSemaphore
		*/
		renderer::SemaphorePtr createSemaphore()const override;
		/**
		*\copydoc		renderer::Device::createCommandPool
		*/
		renderer::CommandPoolPtr createCommandPool( uint32_t queueFamilyIndex
			, renderer::CommandPoolCreateFlags const & flags )const override;
		/**
		*\copydoc		renderer::Device::createShaderProgram
		*/
		virtual renderer::ShaderProgramPtr createShaderProgram()const override;
		/**
		*\brief
		*	Attend que le périphérique soit inactif.
		*/
		void waitIdle()const;
		/**
		*\brief
		*	Echange les tampons.
		*/
		void swapBuffers()const;
		/**
		*\brief
		*	Le numéro de version.
		*/
		inline std::string const & getVersion()const
		{
			return m_version;
		}

	private:
		ContextPtr m_context;
		std::string m_version;
	};
}
