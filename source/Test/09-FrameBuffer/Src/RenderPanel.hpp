#pragma once

#include "Prerequisites.hpp"

#include <Core/Connection.hpp>
#include <Core/Device.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <Image/Sampler.hpp>
#include <Core/SwapChain.hpp>

#include <Utils/Signal.hpp>

#include <wx/panel.h>

#include <array>

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
		void doUpdateProjection();
		void doCreateDevice( renderer::Renderer const & renderer );
		void doCreateSwapChain();
		void doCreateTexture();
		void doCreateUniformBuffer();
		void doCreateStagingBuffer();
		void doCreateOffscreenDescriptorSet();
		void doCreateOffscreenRenderPass();
		void doCreateFrameBuffer();
		void doCreateOffscreenVertexBuffer();
		void doCreateOffscreenPipeline();
		void doPrepareOffscreenFrame();
		void doCreateMainDescriptorSet();
		void doCreateMainRenderPass();
		void doCreateMainVertexBuffer();
		void doCreateMainPipeline();
		void doPrepareMainFrames();
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
		std::vector< TexturedVertexData > m_offscreenVertexData;
		std::vector< TexturedVertexData > m_mainVertexData;
		/**
		*\name
		*	Global.
		/**@{*/
		renderer::DevicePtr m_device;
		renderer::SwapChainPtr m_swapChain;
		renderer::StagingBufferPtr m_stagingBuffer;
		renderer::TexturePtr m_texture;
		renderer::TextureViewPtr m_view;
		renderer::SamplerPtr m_sampler;
		renderer::TexturePtr m_renderTargetColour;
		renderer::TextureViewPtr m_renderTargetColourView;
		renderer::FrameBufferPtr m_frameBuffer;
		renderer::UniformBufferPtr< renderer::Mat4 > m_matrixUbo;
		/**@}*/
		/**
		*\name
		*	Offscreen.
		*/
		/**@{*/
		renderer::CommandBufferPtr m_commandBuffer;
		renderer::CommandBufferPtr m_updateCommandBuffer;
		renderer::RenderPassPtr m_offscreenRenderPass;
		renderer::PipelineLayoutPtr m_offscreenPipelineLayout;
		renderer::PipelinePtr m_offscreenPipeline;
		renderer::VertexBufferPtr< TexturedVertexData > m_offscreenVertexBuffer;
		renderer::BufferPtr< uint16_t > m_offscreenIndexBuffer;
		renderer::VertexLayoutPtr m_offscreenVertexLayout;
		renderer::DescriptorSetLayoutPtr m_offscreenDescriptorLayout;
		renderer::DescriptorSetPoolPtr m_offscreenDescriptorPool;
		renderer::DescriptorSetPtr m_offscreenDescriptorSet;
		renderer::UInt16Array m_offscreenIndexData;
		renderer::QueryPoolPtr m_queryPool;
		renderer::SemaphorePtr m_offscreenFinished;
		/**@}*/
		/**
		*\name
		*	Main.
		*/
		/**@{*/
		renderer::RenderPassPtr m_mainRenderPass;
		renderer::PipelineLayoutPtr m_mainPipelineLayout;
		renderer::PipelinePtr m_mainPipeline;
		renderer::VertexBufferPtr< TexturedVertexData > m_mainVertexBuffer;
		renderer::VertexLayoutPtr m_mainVertexLayout;
		renderer::DescriptorSetLayoutPtr m_mainDescriptorLayout;
		renderer::DescriptorSetPoolPtr m_mainDescriptorPool;
		renderer::DescriptorSetPtr m_mainDescriptorSet;
		/**@}*/
		/**
		*\name
		*	Swapchain.
		*/
		/**@{*/
		std::vector< renderer::FrameBufferPtr > m_frameBuffers;
		std::vector< renderer::CommandBufferPtr > m_commandBuffers;
		renderer::SignalConnection< renderer::SwapChain::OnReset > m_swapChainReset;
		/**@}*/
	};
}