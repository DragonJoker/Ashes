/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Ashes/Descriptor/DescriptorSet.hpp>

#include <vector>

namespace test_renderer
{
	/**
	*\brief
	*	Classe wrappant un TestDescriptorSet.
	*/
	class DescriptorSet
		: public ashes::DescriptorSet
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] pool
		*	Le pool parent.
		*\param[in] bindingPoint
		*	Le point d'attache du set.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical connection to the GPU.
		*\param[in] pool
		*	The parent pool.
		*\param[in] bindingPoint
		*	The binding point for the set.
		*/
		DescriptorSet( Device const & device
			, DescriptorPool const & pool
			, DescriptorSetLayout const & layout
			, uint32_t bindingPoint );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~DescriptorSet();
		/**
		*\copydoc	ashes::DescriptorSet::update
		*/
		void update()const override;

	private:
		DescriptorPool const & m_pool;
		DescriptorSetLayout const & m_layout;
	};
}
