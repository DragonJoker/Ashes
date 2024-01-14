#include "OpaqueRendering.hpp"

#include "RenderTarget.hpp"

#include <ashespp/Buffer/Buffer.hpp>
#include <ashespp/Buffer/StagingBuffer.hpp>
#include <ashespp/Buffer/VertexBuffer.hpp>
#include <ashespp/Command/CommandBuffer.hpp>
#include <ashespp/Command/CommandPool.hpp>
#include <ashespp/Core/Device.hpp>
#include <ashespp/Descriptor/DescriptorSet.hpp>
#include <ashespp/Descriptor/DescriptorSetLayout.hpp>
#include <ashespp/Descriptor/DescriptorSetPool.hpp>
#include <ashespp/Image/Image.hpp>
#include <ashespp/Image/ImageView.hpp>
#include <ashespp/Pipeline/GraphicsPipeline.hpp>
#include <ashespp/Pipeline/PipelineLayout.hpp>
#include <ashespp/RenderPass/RenderPass.hpp>

#include <util/GlslToSpv.hpp>

#include <FileUtils.hpp>

#include <algorithm>

namespace vkapp
{
	namespace
	{
		ashes::ImageViewArray doGetViews( GeometryPassResult const & gbuffer
			, ashes::ImageViewArray views )
		{
			ashes::ImageViewArray result
			{
				views[0]
			};

			for ( auto & texture : gbuffer )
			{
				result.emplace_back( texture.view );
			}

			return result;
		}
	}

	OpaqueRendering::OpaqueRendering( std::unique_ptr< GeometryPass > renderer
		, common::Scene const & scene
		, ashes::StagingBuffer & stagingBuffer
		, GeometryPassResult const & gbuffer
		, ashes::ImageViewArray const & views
		, common::TextureNodePtrArray const & textureNodes
		, std::vector< common::SceneData > const & sceneData
		, ashes::UniformBuffer const & lightsUbo )
		: common::OpaqueRendering{ std::move( renderer )
			, scene
			, stagingBuffer
			, doGetViews( gbuffer, views )
			, textureNodes }
		, m_sceneData{ sceneData}
		, m_lightsUbo{ lightsUbo }
		, m_stagingBuffer{ stagingBuffer }
		, m_lightingPass{ getInstance().getDevice()
			, getCommandPool()
			, getTransferQueue()
			, lightsUbo
			, stagingBuffer
			, views }
	{
		m_lightingPass.update( m_sceneData[0]
			, m_stagingBuffer
			, views
			, gbuffer );
	}

	void OpaqueRendering::update( common::RenderTarget const & target )
	{
		getInstance().update( target );
		m_lightingPass.update( m_sceneData[0]
			, m_stagingBuffer
			, { target.getDepthView(), target.getColourView() }
			, static_cast< RenderTarget const & >( target ).getGBuffer() );
	}

	void OpaqueRendering::draw( ashes::Queue const & queue
		, std::chrono::nanoseconds & gpu )const
	{
		getInstance().draw( queue, gpu );
		m_lightingPass.draw( queue, gpu );
	}
}
