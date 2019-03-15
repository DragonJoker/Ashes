#pragma once

#include "Prerequisites.hpp"

#include <RenderPass/FrameBuffer.hpp>
#include <RenderPass/RenderPass.hpp>
#include <Core/SwapChain.hpp>

#include <Utils/Signal.hpp>

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
		ashes::ConnectionPtr doCreateSurface( ashes::Instance const & instance );
		void doInitialiseQueues( ashes::Instance const & instance
			, ashes::Connection const & surface );
		void doCreateDevice( ashes::Instance const & instance
			, ashes::ConnectionPtr surface );
		void doCreateSwapChain();
		void doCreateRenderPass();
		void doCreateVertexBuffer();
		void doCreatePipeline();
		bool doPrepareFrames();
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
		/**
		*\name
		*	Global.
		/**@{*/
		uint32_t m_graphicsQueueFamilyIndex;
		uint32_t m_presentQueueFamilyIndex;
		ashes::DevicePtr m_device;
		ashes::QueuePtr m_graphicsQueue;
		ashes::QueuePtr m_presentQueue;
		ashes::CommandPoolPtr m_commandPool;
		ashes::SwapChainPtr m_swapChain;
		ashes::RenderPassPtr m_renderPass;
		ashes::PipelineLayoutPtr m_pipelineLayout;
		ashes::PipelinePtr m_pipeline;
		ashes::VertexBufferPtr< VertexData > m_vertexBuffer;
		ashes::VertexLayoutPtr m_vertexLayout;
		ashes::QueryPoolPtr m_queryPool;
		/**@}*/
		/**
		*\name
		*	Swapchain.
		*/
		/**@{*/
		std::vector< ashes::FrameBufferPtr > m_frameBuffers;
		ashes::CommandBufferPtrArray m_commandBuffers;
		ashes::SignalConnection< ashes::SwapChain::OnReset > m_swapChainReset;
		/**@}*/
	};
}
