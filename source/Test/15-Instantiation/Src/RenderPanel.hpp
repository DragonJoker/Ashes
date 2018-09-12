#pragma once

#include "Prerequisites.hpp"

#include <Core/Connection.hpp>
#include <Core/Device.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <Image/Sampler.hpp>
#include <Core/SwapChain.hpp>

#include <Utils/Signal.hpp>

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
		void doCreateDescriptorSet();
		void doCreateRenderPass();
		void doCreateVertexBuffer();
		void doCreatePipeline();
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
		renderer::DevicePtr m_device;
		renderer::SwapChainPtr m_swapChain;
		renderer::StagingBufferPtr m_stagingBuffer;
		renderer::TexturePtr m_texture;
		renderer::TextureViewPtr m_view;
		renderer::SamplerPtr m_sampler;
		renderer::UniformBufferPtr< utils::Mat4 > m_matrixUbo;
		renderer::CommandBufferPtr m_updateCommandBuffer;
		/**@}*/
		/**
		*\name
		*	Offscreen.
		*/
		/**@{*/
		renderer::RenderPassPtr m_renderPass;
		renderer::PipelineLayoutPtr m_pipelineLayout;
		renderer::PipelinePtr m_pipeline;
		renderer::VertexBufferPtr< TexturedVertexData > m_vertexBuffer;
		renderer::BufferPtr< uint16_t > m_indexBuffer;
		renderer::VertexLayoutPtr m_vertexLayout;
		renderer::VertexBufferPtr< utils::Mat4 > m_matrixBuffer;
		renderer::VertexLayoutPtr m_matrixLayout;
		renderer::DescriptorSetLayoutPtr m_descriptorLayout;
		renderer::DescriptorSetPoolPtr m_descriptorPool;
		renderer::DescriptorSetPtr m_descriptorSet;
		renderer::QueryPoolPtr m_queryPool;
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
