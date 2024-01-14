#pragma once

#include "Camera.hpp"
#include "RenderTarget.hpp"
#include "Gui.hpp"

#include <ashespp/Core/Surface.hpp>
#include <ashespp/Core/Device.hpp>
#include <ashespp/Pipeline/GraphicsPipeline.hpp>
#include <ashespp/Pipeline/PipelineLayout.hpp>
#include <ashespp/Image/Sampler.hpp>
#include <ashespp/Core/SwapChain.hpp>

#include <ashes/common/Signal.hpp>

#include <wx/panel.h>

#include <array>

namespace common
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
			, std::string const & rndName
			, std::string const & appName
			, std::string const & appDesc );
		~RenderPanel()noexcept override;
		void initialise( utils::Instance const & instance );
		void cleanup()noexcept;
		void update();
		void draw();

	protected:
		RenderTarget & getRenderTarget()noexcept
		{
			assert( m_renderTarget );
			return *m_renderTarget;
		}

		void setRenderTarget( std::unique_ptr< RenderTarget > v )noexcept
		{
			m_renderTarget = std::move( v );
		}

		MouseState const & getMouse()const noexcept
		{
			return m_mouse;
		}

		ashes::Queue & getGraphicsQueue()const noexcept
		{
			assert( m_graphicsQueue );
			return *m_graphicsQueue;
		}

		ashes::Queue & getPresentQueue()const noexcept
		{
			assert( m_presentQueue );
			return *m_presentQueue;
		}

		ashes::CommandPool const & getCommandPool()const noexcept
		{
			assert( m_commandPool );
			return *m_commandPool;
		}

		void setMouse( MouseState const & v )noexcept
		{
			m_mouse = v;
		}

	private:
		virtual void doInitialise( utils::Device const & device
			, VkExtent2D const & size ) = 0;
		virtual void doUpdateOverlays( Gui const & gui ) = 0;

		virtual void doUpdate()
		{
		}

		ashes::SurfacePtr doCreateSurface( utils::Instance const & instance )const;
		void doCreateDevice( utils::Instance const & instance
			, ashes::Surface const & surface );
		void doCreateSwapChain( ashes::SurfacePtr surface );
		void doCreatePipelineLayout();
		void doCreateDescriptorSet();
		void doCreateRenderPass();
		void doCreateVertexBuffer();
		void doCreatePipeline();
		void doPrepareFrames();
		void doUpdateGui( std::chrono::microseconds const & duration );
		void onSize( wxSizeEvent & event );
		void onMouseLDown( wxMouseEvent & event );
		void onMouseLUp( wxMouseEvent & event );
		void onMouseLDClick( wxMouseEvent & event );
		void onMouseRDown( wxMouseEvent & event );
		void onMouseRUp( wxMouseEvent & event );
		void onMouseRDClick( wxMouseEvent & event );
		void onMouseMove( wxMouseEvent & event );

	private:
		static size_t constexpr FrameSamplesCount = 1000;

		std::unique_ptr< RenderTarget > m_renderTarget{};
		MouseState m_mouse{};
		ashes::QueuePtr m_graphicsQueue{};
		ashes::QueuePtr m_presentQueue{};
		ashes::CommandPoolPtr m_commandPool{};
		std::string m_rndName{};
		std::string m_appName{};
		std::string m_appDesc{};
		bool m_ready{ false };
		std::chrono::microseconds m_frameTime{};
		std::array< std::chrono::microseconds, FrameSamplesCount > m_framesTimes{};
		uint32_t m_frameIndex{ 0 };
		size_t m_frameCount{ 0 };
		std::vector< TexturedVertexData > m_vertexData
		{
			{ { -1.0, -1.0, 0.0, 1.0 }, { 0.0, 0.0 } },
			{ { -1.0, +1.0, 0.0, 1.0 }, { 0.0, 1.0 } },
			{ { +1.0, -1.0, 0.0, 1.0 }, { 1.0, 0.0 } },
			{ { +1.0, +1.0, 0.0, 1.0 }, { 1.0, 1.0 } },
		};
		std::unique_ptr< Gui > m_gui{};

		utils::DevicePtr m_device{};
		utils::SwapChainPtr m_swapChain{};
		VkClearColorValue m_clearColour{};
		ashes::StagingBufferPtr m_stagingBuffer{};

		ashes::SamplerPtr m_sampler{};
		ashes::RenderPassPtr m_renderPass{};
		ashes::VertexBufferPtr< TexturedVertexData > m_vertexBuffer{};
		ashes::DescriptorSetLayoutPtr m_descriptorLayout{};
		ashes::DescriptorSetPoolPtr m_descriptorPool{};
		ashes::DescriptorSetPtr m_descriptorSet{};
		ashes::PipelineLayoutPtr m_pipelineLayout{};
		ashes::GraphicsPipelinePtr m_pipeline{};

		std::vector< ashes::FrameBufferPtr > m_frameBuffers{};
		std::vector< ashes::CommandBufferPtr > m_commandBuffers{};
		ashes::SignalConnection< utils::SwapChain::OnReset > m_swapChainReset{};
	};
}
