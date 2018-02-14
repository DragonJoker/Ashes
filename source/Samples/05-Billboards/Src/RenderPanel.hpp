#pragma once

#include "Prerequisites.hpp"

#include <Core/Connection.hpp>
#include <Core/Device.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <Image/Sampler.hpp>
#include <Core/SwapChain.hpp>

#include <Utils/UtilsSignal.hpp>

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
		void onMouseLDoubleClick( wxMouseEvent & event );
		void onMouseLDown( wxMouseEvent & event );
		void onMouseLUp( wxMouseEvent & event );
		void onMouseMove( wxMouseEvent & event );
		/**@}*/

	private:
	private:
		wxTimer * m_timer{ nullptr };
		bool m_moveCamera{ false };
		renderer::IVec2 m_previousMousePosition;
		renderer::Mat4 m_projection;
		common::Camera m_camera;
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
		renderer::TexturePtr m_renderTargetColour;
		renderer::TextureViewPtr m_renderTargetColourView;
		renderer::TexturePtr m_renderTargetDepth;
		renderer::TextureViewPtr m_renderTargetDepthView;
		renderer::FrameBufferPtr m_frameBuffer;
		renderer::UniformBufferPtr< common::SceneData > m_sceneUbo;
		renderer::CommandBufferPtr m_updateCommandBuffer;
		/**@}*/
		/**
		*\name
		*	Offscreen.
		*/
		/**@{*/
		renderer::CommandBufferPtr m_commandBuffer;
		renderer::RenderPassPtr m_offscreenRenderPass;
		renderer::ShaderProgramPtr m_offscreenProgram;
		renderer::PipelineLayoutPtr m_offscreenPipelineLayout;
		renderer::PipelinePtr m_offscreenPipeline;
		renderer::VertexBufferPtr< common::TexturedVertexData > m_offscreenVertexBuffer;
		renderer::VertexLayoutPtr m_offscreenVertexLayout;
		renderer::VertexBufferPtr< common::BillboardInstanceData > m_offscreenBillboardBuffer;
		renderer::VertexLayoutPtr m_offscreenBillboardLayout;
		renderer::GeometryBuffersPtr m_offscreenGeometryBuffers;
		renderer::DescriptorSetLayoutPtr m_offscreenDescriptorLayout;
		renderer::DescriptorSetPoolPtr m_offscreenDescriptorPool;
		renderer::DescriptorSetPtr m_offscreenDescriptorSet;
		std::vector< common::TexturedVertexData > m_offscreenVertexData;
		renderer::QueryPoolPtr m_offscreenQueryPool;
		/**@}*/
		/**
		*\name
		*	Main.
		*/
		/**@{*/
		renderer::RenderPassPtr m_mainRenderPass;
		renderer::ShaderProgramPtr m_mainProgram;
		renderer::PipelineLayoutPtr m_mainPipelineLayout;
		renderer::PipelinePtr m_mainPipeline;
		renderer::VertexBufferPtr< common::TexturedVertexData > m_mainVertexBuffer;
		renderer::VertexLayoutPtr m_mainVertexLayout;
		renderer::GeometryBuffersPtr m_mainGeometryBuffers;
		renderer::DescriptorSetLayoutPtr m_mainDescriptorLayout;
		renderer::DescriptorSetPoolPtr m_mainDescriptorPool;
		renderer::DescriptorSetPtr m_mainDescriptorSet;
		std::vector< common::TexturedVertexData > m_mainVertexData;
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
