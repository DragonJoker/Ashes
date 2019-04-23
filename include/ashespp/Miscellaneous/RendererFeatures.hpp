/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_RendererFeatures_HPP___
#define ___AshesPP_RendererFeatures_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	The renderer features.
	*\~french
	*\brief
	*	Les fonctionnalités du renderer.
	*/
	struct RendererFeatures
	{
		bool hasTexBufferRange;
		bool hasImageTexture;
		bool hasBaseInstance;
		bool hasClearTexImage;
		bool hasComputeShaders;
		bool hasStorageBuffers;
		bool supportsPersistentMapping;
	};
}

#endif
