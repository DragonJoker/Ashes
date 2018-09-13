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
		OpaqueRendering( std::unique_ptr< GeometryPass > && renderer
			, common::Scene const & scene
			, ashes::StagingBuffer & stagingBuffer
			, GeometryPassResult const & gbuffer
			, ashes::TextureViewCRefArray const & views
			, common::TextureNodePtrArray const & textureNodes
			, ashes::UniformBuffer< common::SceneData > const & sceneUbo
			, ashes::UniformBuffer< common::LightsData > const & lightsUbo );
		void update( common::RenderTarget const & target )override;
		void draw( std::chrono::nanoseconds & gpu )const override;

	private:
		ashes::UniformBuffer< common::SceneData > const & m_sceneUbo;
		ashes::UniformBuffer< common::LightsData > const & m_lightsUbo;
		ashes::StagingBuffer & m_stagingBuffer;
		LightingPass m_lightingPass;
	};
}
