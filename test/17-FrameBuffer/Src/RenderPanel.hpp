#pragma once

#include "Prerequisites.hpp"

#include <ashespp/Core/Surface.hpp>
#include <ashespp/Core/Device.hpp>
#include <ashespp/Pipeline/GraphicsPipeline.hpp>
#include <ashespp/Pipeline/PipelineLayout.hpp>
#include <ashespp/Image/ImageView.hpp>
#include <ashespp/Image/Sampler.hpp>
#include <ashespp/Core/SwapChain.hpp>

#include <ashes/common/Signal.hpp>

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
			, utils::Instance const & instance );
		~RenderPanel()noexcept override;

	private:
		/**
		*\name
		*	Initialisation.
		*/
		/**@{*/
		void doCleanup()noexcept;
		void doUpdateProjection();
		ashes::SurfacePtr doCreateSurface( utils::Instance const & instance );
		void doCreateDevice( utils::Instance const & instance
			, ashes::Surface const & surface );
		void doCreateSwapChain( ashes::SurfacePtr surface );
		void doCreateTexture();
		void doCreateUniformBuffer();
		void doCreateStagingBuffer();
		void doCreateOffscreenDescriptorSet();
		void doCreateOffscreenRenderPass();
		void doCreateFrameBuffer();
		void doCreateOffscreenVertexBuffer();
		void doCreateOffscreenPipeline();
		void doPrepareOffscreenFrame();
		void doCreateMainPipelineLayout();
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
		wxTimer m_timer;
		std::vector< TexturedVertexData > m_offscreenVertexData
		{
			{
				{ -200.0f, -200.0f, 0.0f, 1.0f },
				{ -0.1f, -0.1f },
			},
			{
				{ -200.0f, 200.0f, 0.0f, 1.0f },
				{ -0.1f, 1.1f },
			},
			{
				{ 200.0f, -200.0f, 0.0f, 1.0f },
				{ 1.1f, -0.1f },
			},
			{
				{ 200.0f, 200.0f, 0.0f, 1.0f },
				{ 1.1f, 1.1f },
			},
		};
		std::vector< TexturedVertexData > m_mainVertexData
		{
			{ { -1.0, -1.0, 0.0, 1.0 }, { 0.0, 0.0 } },
			{ { -1.0, +1.0, 0.0, 1.0 }, { 0.0, 1.0 } },
			{ { +1.0, -1.0, 0.0, 1.0 }, { 1.0, 0.0 } },
			{ { +1.0, +1.0, 0.0, 1.0 }, { 1.0, 1.0 } },
		};
		/**
		*\name
		*	Global.
		*/
		/**@{*/
		utils::DevicePtr m_device;
		ashes::QueuePtr m_graphicsQueue;
		ashes::QueuePtr m_presentQueue;
		ashes::CommandPoolPtr m_commandPool;
		utils::SwapChainPtr m_swapChain;
		VkClearColorValue m_clearColour;
		ashes::StagingBufferPtr m_stagingBuffer;
		ashes::ImagePtr m_texture;
		ashes::ImageView m_view;
		ashes::SamplerPtr m_sampler;
		ashes::ImagePtr m_renderTargetColour;
		ashes::ImageView m_renderTargetColourView;
		ashes::FrameBufferPtr m_frameBuffer;
		ashes::UniformBufferPtr m_matrixUbo;
		utils::Mat4 m_matrixData;
		/**@}*/
		/**
		*\name
		*	Offscreen.
		*/
		/**@{*/
		ashes::CommandBufferPtr m_commandBuffer;
		ashes::RenderPassPtr m_offscreenRenderPass;
		ashes::PipelineLayoutPtr m_offscreenPipelineLayout;
		ashes::GraphicsPipelinePtr m_offscreenPipeline;
		ashes::VertexBufferPtr< TexturedVertexData > m_offscreenVertexBuffer;
		ashes::BufferPtr< uint16_t > m_offscreenIndexBuffer;
		ashes::DescriptorSetLayoutPtr m_offscreenDescriptorLayout;
		ashes::DescriptorSetPoolPtr m_offscreenDescriptorPool;
		ashes::DescriptorSetPtr m_offscreenDescriptorSet;
		ashes::UInt16Array m_offscreenIndexData
		{
			0, 1, 2, 2, 1, 3,
		};
		ashes::QueryPoolPtr m_queryPool;
		ashes::SemaphorePtr m_offscreenFinished;
		/**@}*/
		/**
		*\name
		*	Main.
		*/
		/**@{*/
		ashes::RenderPassPtr m_mainRenderPass;
		ashes::PipelineLayoutPtr m_mainPipelineLayout;
		ashes::GraphicsPipelinePtr m_mainPipeline;
		ashes::VertexBufferPtr< TexturedVertexData > m_mainVertexBuffer;
		ashes::DescriptorSetLayoutPtr m_mainDescriptorLayout;
		ashes::DescriptorSetPoolPtr m_mainDescriptorPool;
		ashes::DescriptorSetPtr m_mainDescriptorSet;
		/**@}*/
		/**
		*\name
		*	Swapchain.
		*/
		/**@{*/
		std::vector< ashes::FrameBufferPtr > m_frameBuffers;
		std::vector< ashes::CommandBufferPtr > m_commandBuffers;
		ashes::SignalConnection< utils::SwapChain::OnReset > m_swapChainReset;
		/**@}*/
	};
}
