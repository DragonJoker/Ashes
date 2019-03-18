#pragma once

#include "Prerequisites.hpp"

#include <Buffer/PushConstantsBuffer.hpp>
#include <Core/Surface.hpp>
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
			, ashes::Instance const & instance );
		~RenderPanel();

	private:
		/**
		*\name
		*	Initialisation.
		*/
		/**@{*/
		void doCleanup();
		void doUpdateProjection();
		ashes::SurfacePtr doCreateSurface( ashes::Instance const & instance );
		void doInitialiseQueues( ashes::Instance const & instance
			, ashes::Surface const & surface );
		void doCreateDevice( ashes::Instance const & instance
			, ashes::SurfacePtr surface );
		void doCreateSwapChain();
		void doCreateTexture();
		void doCreateUniformBuffers();
		void doCreateStagingBuffer();
		void doCreateOffscreenDescriptorSet();
		void doCreateOffscreenRenderPass();
		void doCreateFrameBuffer();
		void doCreateOffscreenVertexBuffer();
		void doCreateOffscreenPipeline();
		void doPrepareOffscreenFrame();
		void doCreateComputeDescriptorSet();
		void doCreateComputePipeline();
		void doPrepareCompute();
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
		void doUpdate();
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
		struct Configuration
		{
			utils::IVec2 textureSize;
			float roll;
		};

		wxTimer * m_timer{ nullptr };
		utils::Mat4 m_rotate[2];
		uint32_t m_frame{ 0u };
		/**
		*\name
		*	Global.
		*/
		/**@{*/
		uint32_t m_graphicsQueueFamilyIndex;
		uint32_t m_presentQueueFamilyIndex;
		uint32_t m_computeQueueFamilyIndex;
		ashes::DevicePtr m_device;
		ashes::QueuePtr m_graphicsQueue;
		ashes::QueuePtr m_presentQueue;
		ashes::QueuePtr m_computeQueue;
		ashes::CommandPoolPtr m_commandPool;
		ashes::CommandPoolPtr m_computeCommandPool;
		utils::SwapChainPtr m_swapChain;
		ashes::ClearColorValue m_clearColour;
		ashes::StagingBufferPtr m_stagingBuffer;
		ashes::TexturePtr m_texture;
		ashes::TextureViewPtr m_view;
		ashes::SamplerPtr m_sampler;
		ashes::TexturePtr m_renderTargetColour;
		ashes::TextureViewPtr m_renderTargetColourView;
		ashes::TexturePtr m_renderTargetDepth;
		ashes::TextureViewPtr m_renderTargetDepthView;
		ashes::FrameBufferPtr m_frameBuffer;
		ashes::UniformBufferPtr< utils::Mat4 > m_matrixUbo;
		ashes::UniformBufferPtr< utils::Mat4 > m_objectUbo;
		ashes::PushConstantsBufferTPtr< utils::Vec4 > m_objectPcbs[2];
		/**@}*/
		/**
		*\name
		*	Offscreen.
		*/
		/**@{*/
		ashes::CommandBufferPtr m_commandBuffer;
		ashes::RenderPassPtr m_offscreenRenderPass;
		ashes::PipelineLayoutPtr m_offscreenPipelineLayout;
		ashes::PipelinePtr m_offscreenPipeline;
		ashes::VertexBufferPtr< TexturedVertexData > m_offscreenVertexBuffer;
		ashes::BufferPtr< uint16_t > m_offscreenIndexBuffer;
		ashes::VertexLayoutPtr m_offscreenVertexLayout;
		ashes::DescriptorSetLayoutPtr m_offscreenDescriptorLayout;
		ashes::DescriptorSetPoolPtr m_offscreenDescriptorPool;
		ashes::DescriptorSetPtr m_offscreenDescriptorSets[2];
		std::vector< TexturedVertexData > m_offscreenVertexData;
		ashes::UInt16Array m_offscreenIndexData;
		ashes::QueryPoolPtr m_offscreenQueryPool;
		/**
		*\name
		*	Compute.
		*/
		/**@{*/
		ashes::UniformBufferPtr< Configuration > m_computeUbo;
		ashes::FencePtr m_computeFence;
		ashes::CommandBufferPtr m_computeCommandBuffer;
		ashes::DescriptorSetLayoutPtr m_computeDescriptorLayout;
		ashes::DescriptorSetPoolPtr m_computeDescriptorPool;
		ashes::DescriptorSetPtr m_computeDescriptorSet;
		ashes::PipelineLayoutPtr m_computePipelineLayout;
		ashes::ComputePipelinePtr m_computePipeline;
		ashes::QueryPoolPtr m_computeQueryPool;
		/**@}*/
		/**
		*\name
		*	Main.
		*/
		/**@{*/
		ashes::RenderPassPtr m_mainRenderPass;
		ashes::PipelineLayoutPtr m_mainPipelineLayout;
		ashes::PipelinePtr m_mainPipeline;
		ashes::VertexBufferPtr< TexturedVertexData > m_mainVertexBuffer;
		ashes::VertexLayoutPtr m_mainVertexLayout;
		ashes::DescriptorSetLayoutPtr m_mainDescriptorLayout;
		ashes::DescriptorSetPoolPtr m_mainDescriptorPool;
		ashes::DescriptorSetPtr m_mainDescriptorSet;
		std::vector< TexturedVertexData > m_mainVertexData;
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
