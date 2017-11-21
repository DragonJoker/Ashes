#include "DescriptorSet.hpp"

#include "DescriptorSetLayoutBinding.hpp"
#include "DescriptorSetPool.hpp"
#include "Sampler.hpp"
#include "Texture.hpp"

namespace renderer
{
	DescriptorSet::DescriptorSet( DescriptorSetPool const & pool )
		: m_descriptorSet{ pool.getPool().createDescriptorSet() }
	{
	}

	void DescriptorSet::createBinding( DescriptorSetLayoutBinding const & layoutBinding
		, Texture const & view
		, Sampler const & sampler )
	{
		m_descriptorSet->createBinding( layoutBinding.getBinding()
			, view.getImage().getView()
			, sampler.getSampler() );
	}

	void DescriptorSet::createBinding( DescriptorSetLayoutBinding const & layoutBinding
		, Texture const & view )
	{
		m_descriptorSet->createBinding( layoutBinding.getBinding()
			, view.getImage().getView() );
	}

	void DescriptorSet::update()const
	{
		m_descriptorSet->update();
	}
}
