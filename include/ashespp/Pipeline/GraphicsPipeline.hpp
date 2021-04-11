/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_GraphicsPipeline_HPP___
#define ___AshesPP_GraphicsPipeline_HPP___
#pragma once

#include "Pipeline.hpp"

namespace ashes
{
	/**
	*\brief
	*	A rendering pipeline.
	*/
	class GraphicsPipeline
		: public Pipeline
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent device.
		*\param[in] layout
		*	The pipeline layout.
		*\param[in] createInfo
		*	The creation informations.
		*/
		GraphicsPipeline( Device const & device
			, GraphicsPipelineCreateInfo createInfo );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent device.
		*\param[in] layout
		*	The pipeline layout.
		*\param[in] createInfo
		*	The creation informations.
		*/
		GraphicsPipeline( Device const & device
			, std::string const & debugName
			, GraphicsPipelineCreateInfo createInfo );

	protected:
		GraphicsPipelineCreateInfo m_createInfo;
	};
}

#endif
