#pragma once

#include "Prerequisites.hpp"

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
			, ashes::Renderer const & renderer );
		~RenderPanel();

	private:
		/**
		*\name
		*	Initialisation.
		*/
		/**@{*/
		void doCleanup();
		void doUpdateProjection();
		void doCreateDevice( ashes::Renderer const & renderer );
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
		utils::Mat4 m_rotate;
		utils::Clock::time_point m_time;
		/**
		*\name
		*	Global.
		*/
		/**@{*/
		ashes::DevicePtr m_device;
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
		ashes::CommandBufferPtr m_updateCommandBuffer;
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
		ashes::VertexBufferPtr< UVWVertexData > m_offscreenVertexBuffer;
		ashes::BufferPtr< uint16_t > m_offscreenIndexBuffer;
		ashes::VertexLayoutPtr m_offscreenVertexLayout;
		ashes::DescriptorSetLayoutPtr m_offscreenDescriptorLayout;
		ashes::DescriptorSetPoolPtr m_offscreenDescriptorPool;
		ashes::DescriptorSetPtr m_offscreenDescriptorSet;
		std::vector< UVWVertexData > m_offscreenVertexData;
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
		ashes::VertexBufferPtr< UVVertexData > m_mainVertexBuffer;
		ashes::VertexLayoutPtr m_mainVertexLayout;
		ashes::DescriptorSetLayoutPtr m_mainDescriptorLayout;
		ashes::DescriptorSetPoolPtr m_mainDescriptorPool;
		ashes::DescriptorSetPtr m_mainDescriptorSet;
		std::vector< UVVertexData > m_mainVertexData;
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
