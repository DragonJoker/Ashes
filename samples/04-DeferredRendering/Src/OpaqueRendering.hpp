#pragma once

#include "GeometryPass.hpp"
#include "LightingPass.hpp"

#include <OpaqueRendering.hpp>

namespace vkapp
{
	class OpaqueRendering
		: public common::OpaqueRendering
	{
	public:
		OpaqueRendering( std::unique_ptr< GeometryPass > renderer
			, common::Scene const & scene
			, ashes::StagingBuffer & stagingBuffer
			, GeometryPassResult const & gbuffer
			, ashes::ImageViewArray const & views
			, common::TextureNodePtrArray const & textureNodes
			, std::vector< common::SceneData > const & sceneData
			, ashes::UniformBuffer const & lightsUbo );
		void update( common::RenderTarget const & target )override;
		void draw( ashes::Queue const & queue
			, std::chrono::nanoseconds & gpu )const override;

	private:
		std::vector< common::SceneData > const & m_sceneData;
		ashes::UniformBuffer const & m_lightsUbo;
		ashes::StagingBuffer & m_stagingBuffer;
		LightingPass m_lightingPass;
	};
}
