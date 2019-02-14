#pragma once

#include "Prerequisites.hpp"

#include <Descriptor/DescriptorSetLayout.hpp>
#include <Descriptor/DescriptorSetPool.hpp>
#include <Descriptor/DescriptorSet.hpp>
#include <RenderPass/FrameBuffer.hpp>
#include <Image/Texture.hpp>
#include <RenderPass/RenderPass.hpp>
#include <Image/Sampler.hpp>
#include <Buffer/StagingBuffer.hpp>
#include <Core/SwapChain.hpp>

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
		void doCreateDevice( ashes::Renderer const & renderer );
		void doCreateSwapChain();
		void doCreateTexture();
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
		ashes::DevicePtr m_device;
		ashes::SwapChainPtr m_swapChain;
		ashes::RenderPassPtr m_renderPass;
		ashes::PipelineLayoutPtr m_pipelineLayout;
		ashes::VertexLayoutPtr m_vertexLayout;
		ashes::PipelinePtr m_pipeline;
		ashes::VertexBufferPtr< TexturedVertexData > m_vertexBuffer;
		ashes::StagingBufferPtr m_stagingBuffer;
		ashes::BufferPtr< utils::Vec4 > m_textureBuffer;
		ashes::BufferViewPtr m_bufferView;
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
		std::vector< ashes::FrameBufferPtr > m_frameBuffers;
		ashes::CommandBufferPtrArray m_commandBuffers;
		ashes::SignalConnection< ashes::SwapChain::OnReset > m_swapChainReset;
		/**@}*/
	};
}
