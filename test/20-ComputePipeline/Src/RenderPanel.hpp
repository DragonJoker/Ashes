#pragma once

#include "Prerequisites.hpp"

#include <ashespp/Buffer/PushConstantsBuffer.hpp>
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
		~RenderPanel();

	private:
		/**
		*\name
		*	Initialisation.
		*/
		/**@{*/
		void doCleanup();
		void doUpdateProjection();
		ashes::SurfacePtr doCreateSurface( utils::Instance const & instance );
		void doCreateDevice( utils::Instance const & instance
			, ashes::Surface const & surface );
		void doCreateSwapChain( ashes::SurfacePtr surface );
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
		utils::DevicePtr m_device;
		ashes::QueuePtr m_graphicsQueue;
		ashes::QueuePtr m_presentQueue;
		ashes::QueuePtr m_computeQueue;
		ashes::CommandPoolPtr m_commandPool;
		ashes::CommandPoolPtr m_computeCommandPool;
		utils::SwapChainPtr m_swapChain;
		VkClearColorValue m_clearColour;
		ashes::StagingBufferPtr m_stagingBuffer;
		ashes::ImagePtr m_texture;
		ashes::ImageView m_view;
		ashes::SamplerPtr m_sampler;
		ashes::ImagePtr m_renderTargetColour;
		ashes::ImagePtr m_renderTargetDepth;
		ashes::ImageView m_renderTargetColourView;
		ashes::ImageView m_renderTargetDepthView;
		ashes::FrameBufferPtr m_frameBuffer;
		ashes::UniformBufferPtr m_matrixUbo;
		utils::Mat4 m_matrixData;
		ashes::UniformBufferPtr m_objectUbo;
		std::array< utils::Mat4, 2u > m_objectData;
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
		ashes::GraphicsPipelinePtr m_offscreenPipeline;
		ashes::VertexBufferPtr< TexturedVertexData > m_offscreenVertexBuffer;
		ashes::BufferPtr< uint16_t > m_offscreenIndexBuffer;
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
		ashes::UniformBufferPtr m_computeUbo;
		Configuration m_computeData;
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
		ashes::GraphicsPipelinePtr m_mainPipeline;
		ashes::VertexBufferPtr< TexturedVertexData > m_mainVertexBuffer;
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
