#pragma once

#include "Prerequisites.hpp"

#include <NodesRenderer.hpp>

namespace vkapp
{
	class GeometryPass
		: public common::NodesRenderer
	{
	public:
		GeometryPass( utils::Device const & device
			, ashes::CommandPool const & commandPool
			, ashes::Queue const & transferQueue
			, std::string const & fragmentShaderFile
			, GeometryPassResult const & gbuffer
			, VkFormat depthFormat
			, ashes::UniformBuffer const & sceneUbo
			, ashes::UniformBuffer const & objectUbo );
		void update( common::RenderTarget const & target )override;

	private:
		void doFillObjectDescriptorLayoutBindings( ashes::VkDescriptorSetLayoutBindingArray & bindings )override;
		void doFillObjectDescriptorSet( ashes::DescriptorSetLayout & descriptorLayout
			, ashes::DescriptorSet & descriptorSet )override;

	private:
		ashes::UniformBuffer const & m_sceneUbo;
		ashes::UniformBuffer const & m_objectUbo;
	};
}
