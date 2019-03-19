#pragma once

#include "Prerequisites.hpp"

#include <Core/Device.hpp>
#include <Core/SwapChain.hpp>

#include <Utils/Signal.hpp>

#include <wx/panel.h>

#include <array>

namespace vkapp
{
	class RenderPanel
		: public wxPanel
	{
	private:
		struct RenderingResources
		{
			RenderingResources( ashes::SemaphorePtr imageAvailableSemaphore
				, ashes::SemaphorePtr finishedRenderingSemaphore
				, ashes::FencePtr fence
				, ashes::CommandBufferPtr commandBuffer
				, uint32_t imageIndex )
				: imageAvailableSemaphore{ std::move( imageAvailableSemaphore ) }
				, finishedRenderingSemaphore{ std::move( finishedRenderingSemaphore ) }
				, fence{ std::move( fence ) }
				, commandBuffer{ std::move( commandBuffer ) }
				, imageIndex{ imageIndex }
			{
			}

			ashes::SemaphorePtr imageAvailableSemaphore;
			ashes::SemaphorePtr finishedRenderingSemaphore;
			ashes::FencePtr fence;
			ashes::CommandBufferPtr commandBuffer;
			uint32_t imageIndex{ 0u };
		};
		using RenderingResourcesPtr = std::unique_ptr< RenderingResources >;
		using RenderingResourcesArray = std::vector< RenderingResourcesPtr >;

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
		ashes::SurfacePtr doCreateSurface( ashes::Instance const & instance );
		void doCreateDevice( ashes::Instance const & instance
			, ashes::SurfacePtr surface );
		void doCreateSwapChain();
		void doCreateRenderPass();
		bool doPrepareFrames();
		void doCreateRenderingResources();
		void doCreateFrameBuffers();
		void doCreateCommandBuffers();
		ashes::FrameBufferAttachmentArray doPrepareAttaches( uint32_t backBuffer )const;
		/**@}*/
		/**
		*\name
		*	Rendering.
		*/
		/**@{*/
		void doDraw();
		RenderingResources * getResources();
		void present( RenderingResources & resources );
		bool doCheckNeedReset( ashes::Result errCode
			, bool acquisition
			, char const * const action );
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
		utils::DevicePtr m_device;
		ashes::QueuePtr m_graphicsQueue;
		ashes::QueuePtr m_presentQueue;
		ashes::CommandPoolPtr m_commandPool;
		ashes::SwapChainPtr m_swapChain;
		ashes::ClearColorValue m_clearColour;
		ashes::RenderPassPtr m_renderPass;
		RenderingResourcesArray m_renderingResources;
		size_t m_resourceIndex{ 0u };
		/**@}*/
		/**
		*\name
		*	Swapchain.
		*/
		/**@{*/
		std::vector< ashes::FrameBufferPtr > m_frameBuffers;
		ashes::CommandBufferPtrArray m_commandBuffers;
		/**@}*/
	};
}
