/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <string>

namespace ashes::gl
{
	static int32_t constexpr NotInCore = 9;

#define makeGlExtension( coreMajor, coreMinor, name )\
	static const VkExtensionProperties name = { "GL_"#name, makeVersion( coreMajor, coreMinor, 0 ) }

	// Core since OpenGL 2.0
	makeGlExtension( 2, 0, ARB_texture_compression );
	// Core since OpenGL 3.0
	makeGlExtension( 3, 0, EXT_draw_buffers2 );
	// Core since OpenGL 3.1
	makeGlExtension( 3, 1, ARB_draw_instanced );
	makeGlExtension( 3, 1, ARB_texture_multisample );
	// Core since OpenGL 3.2
	makeGlExtension( 3, 2, ARB_blend_func_extended );
	makeGlExtension( 3, 2, KHR_robustness );
	// Core since OpenGL 3.3
	makeGlExtension( 3, 3, ARB_depth_clamp );
	// Core since OpenGL 4.0
	makeGlExtension( 4, 0, ARB_draw_buffers_blend );
	makeGlExtension( 4, 0, ARB_draw_indirect );
	makeGlExtension( 4, 0, ARB_sample_shading );
	makeGlExtension( 4, 0, ARB_tessellation_shader );
	makeGlExtension( 4, 0, ARB_texture_cube_map_array );
	makeGlExtension( 4, 0, ARB_texture_gather );
	// Core since OpenGL 4.1
	makeGlExtension( 4, 1, ARB_geometry_shader4 );
	makeGlExtension( 4, 1, ARB_get_program_binary );
	makeGlExtension( 4, 1, ARB_separate_shader_objects );
	makeGlExtension( 4, 1, ARB_viewport_array );
	makeGlExtension( 4, 1, ARB_ES2_compatibility );
	// Core since OpenGL 4.2
	makeGlExtension( 4, 2, ARB_base_instance );
	makeGlExtension( 4, 2, ARB_gpu_shader5 );
	makeGlExtension( 4, 2, ARB_shader_atomic_counters );
	makeGlExtension( 4, 2, ARB_shader_image_load_store );
	makeGlExtension( 4, 2, ARB_shading_language_420pack );
	makeGlExtension( 4, 2, ARB_texture_storage );
	makeGlExtension( 4, 2, ARB_ES3_compatibility );
	// Core since OpenGL 4.3
	makeGlExtension( 4, 3, ARB_compute_shader );
	makeGlExtension( 4, 3, ARB_copy_image );
	makeGlExtension( 4, 3, ARB_internalformat_query );
	makeGlExtension( 4, 3, ARB_internalformat_query2 );
	makeGlExtension( 4, 3, ARB_invalidate_subdata );
	makeGlExtension( 4, 3, ARB_multi_draw_indirect );
	makeGlExtension( 4, 3, ARB_program_interface_query );
	makeGlExtension( 4, 3, ARB_shader_storage_buffer_object );
	makeGlExtension( 4, 3, ARB_texture_buffer_range );
	makeGlExtension( 4, 3, ARB_texture_storage_multisample );
	makeGlExtension( 4, 3, ARB_texture_view );
	makeGlExtension( 4, 3, KHR_debug );
	// Core since OpenGL 4.4
	makeGlExtension( 4, 4, ARB_buffer_storage );
	makeGlExtension( 4, 4, ARB_clear_texture );
	// Core since OpenGL 4.5
	makeGlExtension( 4, 5, ARB_clip_control );
	makeGlExtension( 4, 5, ARB_gl_spirv );
	makeGlExtension( 4, 5, ARB_query_buffer_object );
	// Core since OpenGL 4.6
	makeGlExtension( 4, 6, ARB_polygon_offset_clamp );
	makeGlExtension( 4, 6, ARB_texture_filter_anisotropic );
	// Not in core
	makeGlExtension( NotInCore, NotInCore, AMDX_debug_output );
	makeGlExtension( NotInCore, NotInCore, ARB_cull_distance );
	makeGlExtension( NotInCore, NotInCore, ARB_debug_output );
	makeGlExtension( NotInCore, NotInCore, ARB_gpu_shader_int64 );
	makeGlExtension( NotInCore, NotInCore, ARB_gpu_shader_fp64 );
	makeGlExtension( NotInCore, NotInCore, ARB_pipeline_statistics_query );
	makeGlExtension( NotInCore, NotInCore, ARB_sparse_buffer );
	makeGlExtension( NotInCore, NotInCore, ARB_sparse_texture2 );
	makeGlExtension( NotInCore, NotInCore, ARB_sparse_texture_clamp );
	makeGlExtension( NotInCore, NotInCore, EXT_polygon_offset_clamp );
	makeGlExtension( NotInCore, NotInCore, EXT_shader_image_load_formatted );
	makeGlExtension( NotInCore, NotInCore, EXT_texture_compression_s3tc );
	makeGlExtension( NotInCore, NotInCore, EXT_texture_sRGB );
	makeGlExtension( NotInCore, NotInCore, KHR_texture_compression_astc_ldr );
	makeGlExtension( NotInCore, NotInCore, KHR_texture_compression_astc_hdr );
#undef makeGlExtension

	class ExtensionsHandler
	{
	public:
		void initialise();

		bool find( VkExtensionProperties const & name )const;
		bool findAny( VkExtensionPropertiesArray const & names )const;
		bool findAll( VkExtensionPropertiesArray const & names )const;
		bool hasSPIRVShaderBinaryFormat()const;

		inline int isSPIRVSupported()const
		{
			return m_spirvSupported;
		}

		inline int getMajor()const
		{
			return m_major;
		}

		inline int getMinor()const
		{
			return m_minor;
		}

		inline int getShaderVersion()const
		{
			return m_shaderVersion;
		}

		inline AshPluginFeatures const & getFeatures()const
		{
			return m_features;
		}

	private:
		StringArray m_deviceExtensionNames;
		StringArray m_deviceSPIRVExtensionNames;
		std::vector< uint32_t > m_shaderBinaryFormats;
		AshPluginFeatures m_features;
		uint32_t m_major{ 0 };
		uint32_t m_minor{ 0 };
		uint32_t m_version{ 0 };
		uint32_t m_shaderVersion{ 0 };
		bool m_spirvSupported;
	};
}
