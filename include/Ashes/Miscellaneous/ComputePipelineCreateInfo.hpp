/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ComputePipelineCreateInfo_HPP___
#define ___Ashes_ComputePipelineCreateInfo_HPP___
#pragma once

#include "Ashes/Pipeline/ShaderStageState.hpp"
#include "Ashes/Pipeline/SpecialisationInfo.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Holds the data used to create a compute pipeline.
	*\~french
	*\brief
	*	Contient les données utilisées pour créer un pipeline de calcul.
	*/
	struct ComputePipelineCreateInfo
	{
		ShaderStageState stage;
	};
}

#endif
