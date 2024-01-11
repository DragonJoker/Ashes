/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ComputePipeline_HPP___
#define ___AshesPP_ComputePipeline_HPP___
#pragma once

#include "ComputePipelineCreateInfo.hpp"
#include "Pipeline.hpp"

namespace ashes
{
	/**
	*\brief
	*	A compute pipeline.
	*/
	class ComputePipeline
		: public Pipeline
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent logical device.
		*\param[in] createInfo
		*	The pipeline creation informations.
		*/
		ComputePipeline( Device const & device
			, ComputePipelineCreateInfo createInfo );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent logical device.
		*\param[in] createInfo
		*	The pipeline creation informations.
		*/
		ComputePipeline( Device const & device
			, std::string const & debugName
			, ComputePipelineCreateInfo createInfo );

	private:
		ComputePipelineCreateInfo m_createInfo;
	};
}

#endif
