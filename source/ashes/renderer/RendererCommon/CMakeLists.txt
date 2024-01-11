set( _PROJECT_NAME RendererCommon )
project( ashes${_PROJECT_NAME} )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( PROJECT_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" )
set( PROJECT_SOVERSION "${${PROJECT_NAME}_VERSION_BUILD}" )

if ( ASHES_BUILD_SPIRV_CROSS )
	set( ${PROJECT_NAME}_SPIRV_CROSS_SRC_FILES
		${SPIRV_CROSS_DIR}/spirv_cross.cpp
		${SPIRV_CROSS_DIR}/spirv_cross_parsed_ir.cpp
		${SPIRV_CROSS_DIR}/spirv_cfg.cpp
		${SPIRV_CROSS_DIR}/spirv_glsl.cpp
		${SPIRV_CROSS_DIR}/spirv_parser.cpp
	)
	set( ${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES
		${SPIRV_CROSS_DIR}/GLSL.std.450.h
		${SPIRV_CROSS_DIR}/spirv_common.hpp
		${SPIRV_CROSS_DIR}/spirv.hpp
		${SPIRV_CROSS_DIR}/spirv_cross.hpp
		${SPIRV_CROSS_DIR}/spirv_cross_parsed_ir.hpp
		${SPIRV_CROSS_DIR}/spirv_cfg.hpp
		${SPIRV_CROSS_DIR}/spirv_glsl.hpp
		${SPIRV_CROSS_DIR}/spirv_parser.hpp
	)
	if ( ASHES_BUILD_RENDERER_D3D11 )
		set( ${PROJECT_NAME}_SPIRV_CROSS_SRC_FILES
			${${PROJECT_NAME}_SPIRV_CROSS_SRC_FILES}
			${SPIRV_CROSS_DIR}/spirv_hlsl.cpp
		)
		set( ${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES
			${${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES}
			${SPIRV_CROSS_DIR}/spirv_hlsl.hpp
		)
	endif ()
	add_library( ashesSpirVCross STATIC
		${${PROJECT_NAME}_SPIRV_CROSS_SRC_FILES}
		${${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES}
	)
	add_library( ashes::SpirVCross
		ALIAS
		ashesSpirVCross
	)
	target_include_directories( ashesSpirVCross
		PUBLIC
			${SPIRV_CROSS_DIR}
	)
	set( SPIRV_CROSS_LIBRARIES ashes::SpirVCross )
	set_target_properties( ashesSpirVCross
		PROPERTIES
			CXX_STANDARD 17
			CXX_EXTENSIONS OFF
			FOLDER "${Ashes_BASE_DIR}/Renderer"
	)
endif ()

set( ${PROJECT_NAME}_SRC_FILES
	AshesRendererPrerequisites.cpp
	IcdObject.cpp
	InlineUniformBlocks.cpp
	ShaderBindings.cpp
)
set( ${PROJECT_NAME}_HDR_FILES
	AshesRendererPrerequisites.hpp
	IcdObject.hpp
	InlineUniformBlocks.hpp
	ShaderBindings.hpp
)
source_group( "Header Files" FILES ${${PROJECT_NAME}_HDR_FILES} )
source_group( "Source Files" FILES ${${PROJECT_NAME}_SRC_FILES} )

set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Helper/ColourBlendState.hpp
	Helper/ColourBlendStateAttachment.hpp
	Helper/ConstantFormat.hpp
	Helper/DepthStencilState.hpp
	Helper/DynamicState.hpp
	Helper/InputAssemblyState.hpp
	Helper/MultisampleState.hpp
	Helper/RasterisationState.hpp
	Helper/ShaderStageState.hpp
	Helper/StencilOpState.hpp
	Helper/TessellationState.hpp
	Helper/VertexInputState.hpp
	Helper/ViewportState.hpp
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
source_group( "Header Files\\Helper" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )

set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	Layer/Layer.cpp
	Layer/ValidationLayer.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Layer/Layer.hpp
	Layer/ValidationLayer.hpp
)
set( ${PROJECT_NAME}_SRC_FILES
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_FOLDER_SRC_FILES}
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
source_group( "Header Files\\Layer" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
source_group( "Source Files\\Layer" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

add_library( ${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
)
add_library( ashes::${_PROJECT_NAME}
	ALIAS
	${PROJECT_NAME}
)
target_sources( ${PROJECT_NAME} 
	PRIVATE
		${Ashes_SOURCE_DIR}/.editorconfig
)
target_link_libraries( ${PROJECT_NAME}
	PUBLIC
		ashes::ashes
		${Ashes_BINARY_LIBRARIES}
		${SPIRV_CROSS_LIBRARIES}
		${RendererDependencies}
)
target_include_directories( ${PROJECT_NAME}
	PUBLIC
		${Ashes_SOURCE_DIR}/include/ashes
		${Ashes_SOURCE_DIR}/source/ashes
		${Ashes_SOURCE_DIR}/source/ashes/renderer
		${Ashes_BINARY_DIR}/ashes
		${Ashes_BINARY_DIR}/ashes/renderer
	PRIVATE
		${Ashes_BINARY_DIR}/ashes/renderer/${_PROJECT_NAME}
		${Ashes_SOURCE_DIR}/source/ashes/renderer/${_PROJECT_NAME}
)
target_include_directories( ${PROJECT_NAME}
	SYSTEM PUBLIC
		${Vulkan_INCLUDE_DIR}
		${SPIRV_CROSS_DIR}
		${SPIRV_CROSS_INCLUDE_DIR}
		${TARGET_INCLUDE_DIRS}
)
target_compile_definitions( ${PROJECT_NAME} PUBLIC
	${TARGET_CXX_DEFINITIONS}
)
target_compile_options( ${PROJECT_NAME} PUBLIC
	${TARGET_CXX_OPTIONS}
)
target_add_compilation_flags( ${PROJECT_NAME} )
set_target_properties( ${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	FOLDER "${Ashes_BASE_DIR}/Renderer"
	DEBUG_POSTFIX "d"
)

parse_subdir_files_ex( "${Ashes_SOURCE_DIR}" ashes/renderer Helper "Helper" )
parse_subdir_files_ex( "${Ashes_SOURCE_DIR}" ashes/renderer Layer "Layer" )
parse_subdir_files_ex( "${Ashes_SOURCE_DIR}" ashes/renderer Linux "Linux" )
parse_subdir_files_ex( "${Ashes_SOURCE_DIR}" ashes/renderer Platform "Platform" )
parse_subdir_files_ex( "${Ashes_SOURCE_DIR}" ashes/renderer Util "Util" )
parse_subdir_files_ex( "${Ashes_SOURCE_DIR}" ashes/renderer Win32 "Win32" )

source_group( "Header Files\\SPIRV-Cross" FILES ${${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES} )
source_group( "Source Files\\SPIRV-Cross" FILES ${${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES} )
