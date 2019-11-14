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

#include <Camera.hpp>

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
		void doCreateDescriptorSet();
		void doCreateRenderPass();
		void doCreateVertexBuffer();
		void doCreatePipeline();
		void doCreateDepthStencil();
		void doPrepareFrames();
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
		void onMouseLDoubleClick( wxMouseEvent & event );
		void onMouseLDown( wxMouseEvent & event );
		void onMouseLUp( wxMouseEvent & event );
		void onMouseMove( wxMouseEvent & event );
		/**@}*/

	private:
		wxTimer * m_timer{ nullptr };
		bool m_moveCamera{ false };
		utils::IVec2 m_previousMousePosition;
		utils::Mat4 m_projection;
		Camera m_camera;
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
		ashes::UniformBufferPtr m_matrixUbo;
		utils::Mat4 m_matrixData;
		/**@}*/
		/**
		*\name
		*	Offscreen.
		*/
		/**@{*/
		ashes::RenderPassPtr m_renderPass;
		ashes::PipelineLayoutPtr m_pipelineLayout;
		ashes::GraphicsPipelinePtr m_pipeline;
		ashes::VertexBufferPtr< TexturedVertexData > m_vertexBuffer;
		ashes::BufferPtr< uint16_t > m_indexBuffer;
		ashes::VertexBufferPtr< utils::Mat4 > m_matrixBuffer;
		ashes::DescriptorSetLayoutPtr m_descriptorLayout;
		ashes::DescriptorSetPoolPtr m_descriptorPool;
		ashes::DescriptorSetPtr m_descriptorSet;
		ashes::QueryPoolPtr m_queryPool;
		/**@}*/
		/**
		*\name
		*	Swapchain.
		*/
		/**@{*/
		ashes::ImagePtr m_depthStencil;
		std::vector< ashes::FrameBufferPtr > m_frameBuffers;
		std::vector< ashes::CommandBufferPtr > m_commandBuffers;
		ashes::SignalConnection< utils::SwapChain::OnReset > m_swapChainReset;
		/**@}*/
	};
}
