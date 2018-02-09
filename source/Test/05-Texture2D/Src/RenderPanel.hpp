#pragma once

#include "Prerequisites.hpp"

#include <Descriptor/DescriptorSetLayout.hpp>
#include <Descriptor/DescriptorSetPool.hpp>
#include <Descriptor/DescriptorSet.hpp>
#include <RenderPass/FrameBuffer.hpp>
#include <Image/Texture.hpp>
#include <RenderPass/RenderPass.hpp>
#include <Image/Sampler.hpp>
#include <Buffer/StagingBuffer.hpp>
#include <Core/SwapChain.hpp>

namespace vkapp
{
	class RenderPanel
		: public wxPanel
	{
	public:
		RenderPanel( wxWindow * parent
			, wxSize const & size
			, renderer::Renderer const & renderer );
		~RenderPanel();

	private:
		/**
		*\name
		*	Initialisation.
		*/
		/**@{*/
		void doCleanup();
		void doCreateDevice( renderer::Renderer const & renderer );
		void doCreateSwapChain();
		void doCreateTexture();
		void doCreateDescriptorSet();
		void doCreateRenderPass();
		void doCreateVertexBuffer();
		void doCreateStagingBuffer();
		void doCreatePipeline();
		void doPrepareFrames();
		/**@}*/
		/**
		*\name
		*	Rendering.
		*/
		/**@{*/
		void doDraw();
		void doResetSwapChain();
		/**@}*/
		/**
		*\name
		*	Events.
		*/
		/**@{*/
		void onTimer( wxTimerEvent & event );
		void onSize( wxSizeEvent & event );
		/**@}*/

	private:
		wxTimer * m_timer{ nullptr };
		std::vector< TexturedVertexData > m_vertexData;
		/**
		*\name
		*	Global.
		*/
		/**@{*/
		renderer::DevicePtr m_device;
		renderer::SwapChainPtr m_swapChain;
		renderer::RenderPassPtr m_renderPass;
		renderer::PipelineLayoutPtr m_pipelineLayout;
		renderer::ShaderProgramPtr m_program;
		renderer::VertexLayoutPtr m_vertexLayout;
		renderer::PipelinePtr m_pipeline;
		renderer::VertexBufferPtr< TexturedVertexData > m_vertexBuffer;
		renderer::GeometryBuffersPtr m_geometryBuffers;
		renderer::StagingBufferPtr m_stagingBuffer;
		renderer::TexturePtr m_texture;
		renderer::TextureViewPtr m_view;
		renderer::SamplerPtr m_sampler;
		renderer::DescriptorSetLayoutPtr m_descriptorLayout;
		renderer::DescriptorSetPoolPtr m_descriptorPool;
		renderer::DescriptorSetPtr m_descriptorSet;
		/**@}*/
		/**
		*\name
		*	Swapchain.
		*/
		/**@{*/
		std::vector< renderer::FrameBufferPtr > m_frameBuffers;
		renderer::CommandBufferPtrArray m_commandBuffers;
		renderer::SignalConnection< renderer::SwapChain::OnReset > m_swapChainReset;
		/**@}*/
	};
}
