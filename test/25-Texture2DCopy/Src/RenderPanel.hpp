#pragma once

#include "Gui.hpp"

#include <ashespp/Descriptor/DescriptorSetLayout.hpp>
#include <ashespp/Descriptor/DescriptorSetPool.hpp>
#include <ashespp/Descriptor/DescriptorSet.hpp>
#include <ashespp/RenderPass/FrameBuffer.hpp>
#include <ashespp/Image/Image.hpp>
#include <ashespp/RenderPass/RenderPass.hpp>
#include <ashespp/Image/ImageView.hpp>
#include <ashespp/Image/Sampler.hpp>
#include <ashespp/Buffer/StagingBuffer.hpp>
#include <ashespp/Core/SwapChain.hpp>

namespace vkapp
{
	class RenderPanel
		: public wxPanel
	{
	private:
		struct MouseState
		{
			VkOffset2D position;
			bool left{ false };
			bool right{ false };
		};

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
		void doCreateTextureDst( size_t index );
		void doCreateDescriptorSet();
		void doCreateRenderPass();
		void doCreateVertexBuffer();
		void doCreateStagingBuffer();
		void doCreatePipeline();
		void doPrepareFrames();
		void doPrepareCommandBuffers();
		/**@}*/
		/**
		*\name
		*	Rendering.
		*/
		/**@{*/
		void doUpdateGui();
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
		void onMouseLDown( wxMouseEvent & event );
		void onMouseLUp( wxMouseEvent & event );
		void onMouseLDClick( wxMouseEvent & event );
		void onMouseRDown( wxMouseEvent & event );
		void onMouseRUp( wxMouseEvent & event );
		void onMouseRDClick( wxMouseEvent & event );
		void onMouseMove( wxMouseEvent & event );
		void onKeyUp( wxKeyEvent & event );
		/**@}*/

	private:
		static size_t constexpr FrameSamplesCount = 1000;
		wxTimer * m_timer{ nullptr };
		std::vector< TexturedVertexData > m_vertexData;
		MouseState m_mouse;
		std::chrono::microseconds m_frameTime;
		std::array< std::chrono::microseconds, FrameSamplesCount > m_framesTimes;
		uint32_t m_frameIndex{ 0 };
		size_t m_frameCount{ 0 };
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
		ashes::RenderPassPtr m_renderPass;
		ashes::PipelineLayoutPtr m_pipelineLayout;
		ashes::GraphicsPipelinePtr m_pipeline;
		ashes::VertexBufferPtr< TexturedVertexData > m_vertexBuffer;
		ashes::StagingBufferPtr m_stagingBuffer;
		ashes::ImagePtr m_srcTexture;
		ashes::ImageView m_srcView;
		std::map< VkFormat, ashes::ImagePtr > m_dstTextures;
		std::map< VkFormat, ashes::ImageView > m_dstViews;
		size_t m_curIndex;
		ashes::SamplerPtr m_sampler;
		ashes::DescriptorSetLayoutPtr m_descriptorLayout;
		ashes::DescriptorSetPoolPtr m_descriptorPool;
		std::map< VkFormat, ashes::DescriptorSetPtr > m_descriptorSets;
		ashes::QueryPoolPtr m_queryPool;
		std::unique_ptr< Gui > m_gui;
		std::vector< std::string > m_choices;
		std::vector< VkFormat > m_choicesIndex;
		/**@}*/
		/**
		*\name
		*	Swapchain.
		*/
		/**@{*/
		std::vector< ashes::FrameBufferPtr > m_frameBuffers;
		ashes::CommandBufferPtrArray m_commandBuffers;
		ashes::SignalConnection< utils::SwapChain::OnReset > m_swapChainReset;
		/**@}*/
	};
}
