#pragma once

#include "Prerequisites.hpp"

#include <AshesPP/Core/Surface.hpp>
#include <AshesPP/Core/Device.hpp>
#include <AshesPP/Pipeline/GraphicsPipeline.hpp>
#include <AshesPP/Pipeline/PipelineLayout.hpp>
#include <AshesPP/Image/Sampler.hpp>
#include <AshesPP/Core/SwapChain.hpp>

#include <common/Signal.hpp>

#include <ObjLoader.hpp>

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
		void doCreateUniformBuffer();
		void doCreateStagingBuffer();
		void doCreateOffscreenDescriptorSet();
		void doCreateOffscreenRenderPass();
		void doCreateFrameBuffer();
		void doCreateOffscreenVertexBuffer();
		void doCreateOffscreenPipeline();
		void doPrepareOffscreenFrame();
		void doPrepareHiPass();
		void doPrepareBlurXPass();
		void doPrepareBlurYPass();
		void doPrepareCombinePass();
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
		wxTimer * m_timer{ nullptr };
		utils::Mat4 m_rotate;
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
		ashes::ImageViewPtr m_view;
		ashes::SamplerPtr m_sampler;
		ashes::ImagePtr m_renderTargetColour;
		ashes::ImageViewPtr m_renderTargetColourView;
		ashes::FrameBufferPtr m_frameBuffer;
		ashes::UniformBufferPtr< utils::Mat4 > m_matrixUbo;
		ashes::UniformBufferPtr< utils::Mat4 > m_objectUbo;
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
		ashes::VertexBufferPtr< NonTexturedVertexData > m_offscreenVertexBuffer;
		ashes::BufferPtr< uint16_t > m_offscreenIndexBuffer;
		ashes::DescriptorSetLayoutPtr m_offscreenDescriptorLayout;
		ashes::DescriptorSetPoolPtr m_offscreenDescriptorPool;
		ashes::DescriptorSetPtr m_offscreenDescriptorSet;
		std::vector< NonTexturedVertexData > m_offscreenVertexData;
		ashes::UInt16Array m_offscreenIndexData;
		ashes::QueryPoolPtr m_queryPool;
		ashes::SemaphorePtr m_semaphore;
		/**@}*/
		/**
		*\name
		*	Bloom passes.
		*/
		/**@{*/
		struct Pass
		{
			ashes::CommandBufferPtr commandBuffer;
			ashes::DescriptorSetLayoutPtr descriptorLayout;
			ashes::DescriptorSetPoolPtr descriptorPool;
			ashes::DescriptorSetPtr descriptorSet;
			ashes::PipelineLayoutPtr pipelineLayout;
			ashes::RenderPassPtr renderPass;
			ashes::FrameBufferPtr frameBuffer;
			ashes::GraphicsPipelinePtr pipeline;
			ashes::ImagePtr image;
			ashes::ImageViewPtrArray views;
			ashes::SemaphorePtr semaphore;
		};
		struct
		{
			Pass hi;
			std::array< Pass, 4u > blurX;
			std::array< Pass, 4u > blurY;
			Pass combine;
		} m_passes;
		ashes::ImageViewPtr m_blurMipView;
		ashes::SamplerPtr m_mipSampler;
		std::array< ashes::SamplerPtr, 4 > m_blurSamplers;
		struct Configuration
		{
			utils::Vec2 textureSize;
			uint32_t coefficientsCount;
			uint32_t dump;
			std::array< utils::Vec4, 15u > coefficients;
		};
		ashes::UniformBufferPtr< Configuration > m_blurConfiguration;
		ashes::UniformBufferPtr< int > m_blurDirection;
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
