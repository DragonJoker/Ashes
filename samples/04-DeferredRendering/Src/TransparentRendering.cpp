#include "TransparentRendering.hpp"

namespace vkapp
{
	TransparentRendering::TransparentRendering( utils::Device const & device
		, ashes::CommandPool const & commandPool
		, ashes::Queue const & transferQueue
		, std::string const & fragmentShaderFile
		, std::vector< VkFormat > const & formats
		, ashes::UniformBuffer< common::SceneData > const & sceneUbo
		, ashes::UniformBuffer< common::ObjectData > const & objectUbo
		, ashes::UniformBuffer< common::LightsData > const & lightsUbo )
		: common::NodesRenderer{ device
			, commandPool
			, transferQueue
			, fragmentShaderFile
			, formats
			, false
			, false }
		, m_sceneUbo{ sceneUbo }
		, m_objectUbo{ objectUbo }
		, m_lightsUbo{ lightsUbo }
	{
	}

	void TransparentRendering::doFillObjectDescriptorLayoutBindings( ashes::VkDescriptorSetLayoutBindingArray & bindings )
	{
		bindings.emplace_back( 1u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT );
		bindings.emplace_back( 2u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT );
		bindings.emplace_back( 3u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT );
	}

	void TransparentRendering::doFillObjectDescriptorSet( ashes::DescriptorSetLayout & descriptorLayout
		, ashes::DescriptorSet & descriptorSet )
	{
		descriptorSet.createBinding( descriptorLayout.getBinding( 1u )
			, m_sceneUbo
			, 0u
			, 1u );
		descriptorSet.createBinding( descriptorLayout.getBinding( 2u )
			, m_objectUbo
			, 0u
			, 1u );
		descriptorSet.createBinding( descriptorLayout.getBinding( 3u )
			, m_lightsUbo
			, 0u
			, 1u );
	}
}
