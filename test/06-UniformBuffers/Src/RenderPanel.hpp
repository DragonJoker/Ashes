#pragma once

#include "Prerequisites.hpp"

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
			, utils::Instance const & instance );
		~RenderPanel();

	private:
		/**
		*\name
		*	Initialisation.
		*/
		/**@{*/
		void doCleanup();
		ashes::SurfacePtr doCreateSurface( utils::Instance const & instance );
		void doCreateDevice( utils::Instance const & instance
			, ashes::Surface const & surface );
		void doCreateSwapChain( ashes::SurfacePtr surface );
		void doCreateTexture();
		void doCreateUniformBuffer();
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
		utils::DevicePtr m_device;
		ashes::QueuePtr m_graphicsQueue;
		ashes::QueuePtr m_presentQueue;
		ashes::CommandPoolPtr m_commandPool;
		utils::SwapChainPtr m_swapChain;
		ashes::ClearColorValue m_clearColour;
		ashes::RenderPassPtr m_renderPass;
		ashes::PipelineLayoutPtr m_pipelineLayout;
		ashes::PipelinePtr m_pipeline;
		ashes::VertexBufferPtr< TexturedVertexData > m_vertexBuffer;
		ashes::VertexLayoutPtr m_vertexLayout;
		ashes::StagingBufferPtr m_stagingBuffer;
		ashes::ImagePtr m_texture;
		ashes::ImageViewPtr m_view;
		ashes::SamplerPtr m_sampler;
		ashes::DescriptorSetLayoutPtr m_descriptorLayout;
		ashes::DescriptorSetPoolPtr m_descriptorPool;
		ashes::DescriptorSetPtr m_descriptorSet;
		ashes::UniformBufferPtr< utils::Mat4 > m_uniformBuffer;
		ashes::QueryPoolPtr m_queryPool;
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
