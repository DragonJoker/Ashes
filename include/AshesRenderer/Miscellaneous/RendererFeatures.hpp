/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_RendererFeatures_HPP___
#define ___Ashes_RendererFeatures_HPP___
#pragma once

#include "AshesRenderer/AshesRendererPrerequisites.hpp"

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
