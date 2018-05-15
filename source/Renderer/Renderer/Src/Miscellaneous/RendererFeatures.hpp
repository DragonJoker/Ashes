/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_RendererFeatures_HPP___
#define ___Renderer_RendererFeatures_HPP___
#pragma once

#include "RendererPrerequisites.hpp"

namespace renderer
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
	};
}

#endif
