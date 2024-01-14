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
		~RenderPanel()noexcept override;

	private:
		/**
		*\name
		*	Initialisation.
		*/
		/**@{*/
		void doCleanup()noexcept;
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
		void doCreateMainDescriptorSet();
		void doCreateMainRenderPass();
		void doCreateMainVertexBuffer();
		void doCreateMainPipelineLayout();
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
		wxTimer m_timer;
		std::array< utils::Mat4, 2u > m_rotate;
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
		ashes::ImagePtr m_renderTargetColour;
		ashes::ImagePtr m_renderTargetDepth;
		ashes::ImageView m_renderTargetColourView;
		ashes::ImageView m_renderTargetDepthView;
		ashes::FrameBufferPtr m_frameBuffer;
		ashes::UniformBufferPtr m_matrixUbo;
		utils::Mat4 m_matrixData;
		ashes::UniformBufferPtr m_objectUbo;
		std::array< utils::Mat4, 2 > m_objectData;
		std::array< ashes::PushConstantsBuffer< utils::Vec4 >, 2u > m_objectPcbs{
			ashes::PushConstantsBuffer< utils::Vec4 >{ VK_SHADER_STAGE_FRAGMENT_BIT, ashes::PushConstantArray{ { 0u, VK_FORMAT_R32G32B32A32_SFLOAT } } },
			ashes::PushConstantsBuffer< utils::Vec4 >{ VK_SHADER_STAGE_FRAGMENT_BIT, ashes::PushConstantArray{ { 0u, VK_FORMAT_R32G32B32A32_SFLOAT } } }
		};
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
		std::array< ashes::DescriptorSetPtr, 2u > m_offscreenDescriptorSets;
		std::vector< TexturedVertexData > m_offscreenVertexData
		{
			// Front
			{ { -1.0, -1.0, +1.0, 1.0 }, { 0.0, 0.0 } },
			{ { -1.0, +1.0, +1.0, 1.0 }, { 0.0, 1.0 } },
			{ { +1.0, -1.0, +1.0, 1.0 }, { 1.0, 0.0 } },
			{ { +1.0, +1.0, +1.0, 1.0 }, { 1.0, 1.0 } },
			// Top
			{ { -1.0, +1.0, +1.0, 1.0 }, { 0.0, 0.0 } },
			{ { -1.0, +1.0, -1.0, 1.0 }, { 0.0, 1.0 } },
			{ { +1.0, +1.0, +1.0, 1.0 }, { 1.0, 0.0 } },
			{ { +1.0, +1.0, -1.0, 1.0 }, { 1.0, 1.0 } },
			// Back
			{ { -1.0, +1.0, -1.0, 1.0 }, { 1.0, 1.0 } },
			{ { -1.0, -1.0, -1.0, 1.0 }, { 1.0, 0.0 } },
			{ { +1.0, +1.0, -1.0, 1.0 }, { 0.0, 1.0 } },
			{ { +1.0, -1.0, -1.0, 1.0 }, { 0.0, 0.0 } },
			// Bottom
			{ { -1.0, -1.0, -1.0, 1.0 }, { 1.0, 1.0 } },
			{ { -1.0, -1.0, +1.0, 1.0 }, { 1.0, 0.0 } },
			{ { +1.0, -1.0, -1.0, 1.0 }, { 0.0, 1.0 } },
			{ { +1.0, -1.0, +1.0, 1.0 }, { 0.0, 0.0 } },
			// Right
			{ { +1.0, -1.0, +1.0, 1.0 }, { 0.0, 0.0 } },
			{ { +1.0, +1.0, +1.0, 1.0 }, { 0.0, 1.0 } },
			{ { +1.0, -1.0, -1.0, 1.0 }, { 1.0, 0.0 } },
			{ { +1.0, +1.0, -1.0, 1.0 }, { 1.0, 1.0 } },
			// Left
			{ { -1.0, -1.0, -1.0, 1.0 }, { 0.0, 0.0 } },
			{ { -1.0, +1.0, -1.0, 1.0 }, { 0.0, 1.0 } },
			{ { -1.0, -1.0, +1.0, 1.0 }, { 1.0, 0.0 } },
			{ { -1.0, +1.0, +1.0, 1.0 }, { 1.0, 1.0 } },
		};
		ashes::UInt16Array m_offscreenIndexData
		{
			// Front
			0, 1, 2, 2, 1, 3,
			// Top
			4, 5, 6, 6, 5, 7,
			// Back
			8, 9, 10, 10, 9, 11,
			// Bottom
			12, 13, 14, 14, 13, 15,
			// Right
			16, 17, 18, 18, 17, 19,
			// Left
			20, 21, 22, 22, 21, 23,
		};
		ashes::QueryPoolPtr m_queryPool;
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
		std::vector< TexturedVertexData > m_mainVertexData
		{
			{ { -1.0, -1.0, 0.0, 1.0 }, { 0.0, 0.0 } },
			{ { -1.0, +1.0, 0.0, 1.0 }, { 0.0, 1.0 } },
			{ { +1.0, -1.0, 0.0, 1.0 }, { 1.0, 0.0 } },
			{ { +1.0, +1.0, 0.0, 1.0 }, { 1.0, 1.0 } },
		};
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
