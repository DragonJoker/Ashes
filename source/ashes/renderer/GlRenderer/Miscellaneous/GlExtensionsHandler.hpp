/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <string>

namespace ashes::gl
{
#define makeGlExtension( name, major, minor )\
	static const VkExtensionProperties name = { "GL_"#name, makeVersion( major, minor, 0 ) }

	makeGlExtension( AMDX_debug_output, 1, 1 );
	makeGlExtension( ARB_base_instance, 4, 1 );
	makeGlExtension( ARB_blend_func_extended, 3, 2 );
	makeGlExtension( ARB_buffer_storage, 4, 3 );
	makeGlExtension( ARB_clear_texture, 4, 3 );
	makeGlExtension( ARB_clip_control, 4, 4 );
	makeGlExtension( ARB_compute_shader, 4, 2 );
	makeGlExtension( ARB_copy_image, 4, 2 );
	makeGlExtension( ARB_cull_distance, 4, 4 );
	makeGlExtension( ARB_debug_output, 4, 0 );
	makeGlExtension( ARB_depth_clamp, 3, 1 );
	makeGlExtension( ARB_draw_buffers_blend, 2, 0 );
	makeGlExtension( ARB_draw_indirect, 3, 2 );
	makeGlExtension( ARB_draw_instanced, 3, 0 );
	makeGlExtension( ARB_geometry_shader4, 2, 0 );
	makeGlExtension( ARB_get_program_binary, 3, 0 );
	makeGlExtension( ARB_gl_spirv, 4, 5 );
	makeGlExtension( ARB_gpu_shader5, 3, 2 );
	makeGlExtension( ARB_gpu_shader_int64, 4, 5 );
	makeGlExtension( ARB_gpu_shader_fp64, 3, 2 );
	makeGlExtension( ARB_internalformat_query, 4, 1 );
	makeGlExtension( ARB_internalformat_query2, 4, 2 );
	makeGlExtension( ARB_invalidate_subdata, 3, 2 );
	makeGlExtension( ARB_multi_draw_indirect, 4, 1 );
	makeGlExtension( ARB_pipeline_statistics_query, 4, 4 );
	makeGlExtension( ARB_polygon_offset_clamp, 4, 5 );
	makeGlExtension( ARB_query_buffer_object, 4, 3 );
	makeGlExtension( ARB_sample_shading, 2, 0 );
	makeGlExtension( ARB_separate_shader_objects, 4, 0 );
	makeGlExtension( ARB_shader_atomic_counters, 4, 1 );
	makeGlExtension( ARB_shader_image_load_store, 3, 2 );
	makeGlExtension( ARB_shader_storage_buffer_object, 4, 2 );
	makeGlExtension( ARB_sparse_buffer, 4, 4 );
	makeGlExtension( ARB_sparse_texture2, 4, 5 );
	makeGlExtension( ARB_sparse_texture_clamp, 4, 5 );
	makeGlExtension( ARB_tessellation_shader, 3, 2 );
	makeGlExtension( ARB_texture_buffer_range, 3, 1 );
	makeGlExtension( ARB_texture_compression, 1, 2 );
	makeGlExtension( ARB_texture_cube_map_array, 3, 0 );
	makeGlExtension( ARB_texture_filter_anisotropic, 4, 5 );
	makeGlExtension( ARB_texture_gather, 3, 0 );
	makeGlExtension( ARB_texture_storage, 3, 2 );
	makeGlExtension( ARB_texture_storage_multisample, 4, 2 );
	makeGlExtension( ARB_texture_view, 4, 2 );
	makeGlExtension( ARB_viewport_array, 3, 2 );
	makeGlExtension( ARB_ES2_compatibility, 4, 0 );
	makeGlExtension( ARB_ES3_compatibility, 4, 2 );
	makeGlExtension( EXT_draw_buffers2, 2, 0 );
	makeGlExtension( EXT_polygon_offset_clamp, 3, 3 );
	makeGlExtension( EXT_shader_image_load_formatted, 4, 3 );
	makeGlExtension( EXT_texture_compression_s3tc, 2, 0 );
	makeGlExtension( EXT_texture_sRGB, 2, 0 );
	makeGlExtension( KHR_debug, 4, 2 );
	makeGlExtension( KHR_robustness, 3, 2 );
	makeGlExtension( KHR_texture_compression_astc_ldr, 3, 2 );
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
		int m_major{ 0 };
		int m_minor{ 0 };
		uint32_t m_version{ 0u };
		int m_shaderVersion{ 0 };
		bool m_spirvSupported;
	};
}
