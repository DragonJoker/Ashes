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
		/**
		*\~english
		*\brief
		*	Tells if SPIR-V is supported.
		*\~french
		*\brief
		*	Dit si le SPIR-V est supporté.
		*/
		bool spirvSupported;
		/**
		*\~english
		*\brief
		*	Tells if GLSL is supported.
		*\~french
		*\brief
		*	Dit si le GLSL est supporté.
		*/
		bool glslSupported;
		/**
		*\~english
		*\brief
		*	Tells if raw constants (single uniforms or specialisation constants) are supported.
		*\~french
		*\brief
		*	Dit si les constantes brutes (uniform simples ou constantes de spécialisation) sont supportées.
		*/
		bool rawConstantsSupported;
	};
}

#endif
