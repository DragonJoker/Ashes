#pragma once

#include "Prerequisites.hpp"

#include <NodesRenderer.hpp>

namespace vkapp
{
	class GeometryPass
		: public common::NodesRenderer
	{
	public:
		GeometryPass( ashes::Device const & device
			, ashes::CommandPool const & commandPool
			, ashes::Queue const & transferQueue
			, std::string const & fragmentShaderFile
			, GeometryPassResult const & gbuffer
			, ashes::Format depthFormat
			, ashes::UniformBuffer< common::SceneData > const & sceneUbo
			, ashes::UniformBuffer< common::ObjectData > const & objectUbo );
		void update( common::RenderTarget const & target )override;

	private:
		void doFillObjectDescriptorLayoutBindings( ashes::DescriptorSetLayoutBindingArray & bindings )override;
		void doFillObjectDescriptorSet( ashes::DescriptorSetLayout & descriptorLayout
			, ashes::DescriptorSet & descriptorSet )override;

	private:
		ashes::UniformBuffer< common::SceneData > const & m_sceneUbo;
		ashes::UniformBuffer< common::ObjectData > const & m_objectUbo;
	};
}
