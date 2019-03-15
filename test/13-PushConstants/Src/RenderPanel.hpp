#pragma once

#include "Prerequisites.hpp"

#include <Buffer/PushConstantsBuffer.hpp>
#include <Core/Connection.hpp>
#include <Core/Device.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <Image/Sampler.hpp>
#include <Core/SwapChain.hpp>

#include <Utils/Signal.hpp>

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
		ashes::ConnectionPtr doCreateSurface( ashes::Instance const & instance );
		void doInitialiseQueues( ashes::Instance const & instance
			, ashes::Connection const & surface );
		void doCreateDevice( ashes::Instance const & instance
			, ashes::ConnectionPtr surface );
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
		utils::Mat4 m_rotate[2];
		/**
		*\name
		*	Global.
		*/
		/**@{*/
		uint32_t m_graphicsQueueFamilyIndex;
		uint32_t m_presentQueueFamilyIndex;
		ashes::DevicePtr m_device;
		ashes::QueuePtr m_graphicsQueue;
		ashes::QueuePtr m_presentQueue;
		ashes::CommandPoolPtr m_commandPool;
		ashes::SwapChainPtr m_swapChain;
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
		ashes::PushConstantsBuffer< utils::Vec4 > m_objectPcbs[2];
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
		ashes::QueryPoolPtr m_queryPool;
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
		ashes::SignalConnection< ashes::SwapChain::OnReset > m_swapChainReset;
		/**@}*/
	};
}
