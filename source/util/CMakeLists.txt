set( _PROJECT_NAME util )
project( ashes_${_PROJECT_NAME} )

set( ${PROJECT_NAME}_SRC_FILES
	${Ashes_SOURCE_DIR}/source/util/GlslToSpv.cpp
	${Ashes_SOURCE_DIR}/source/util/PlaneEquation.cpp
	${Ashes_SOURCE_DIR}/source/util/RenderingResources.cpp
	${Ashes_SOURCE_DIR}/source/util/StringUtils.cpp
	${Ashes_SOURCE_DIR}/source/util/UtilsDebug.cpp
	${Ashes_SOURCE_DIR}/source/util/UtilsDevice.cpp
	${Ashes_SOURCE_DIR}/source/util/UtilsFile.cpp
	${Ashes_SOURCE_DIR}/source/util/UtilsInstance.cpp
	${Ashes_SOURCE_DIR}/source/util/UtilsPrerequisites.cpp
	${Ashes_SOURCE_DIR}/source/util/UtilsSwapChain.cpp
)
set( ${PROJECT_NAME}_HDR_FILES
	${Ashes_SOURCE_DIR}/include/util/Angle.hpp
	${Ashes_SOURCE_DIR}/include/util/Converter.hpp
	${Ashes_SOURCE_DIR}/include/util/Converter.inl
	${Ashes_SOURCE_DIR}/include/util/Degrees.hpp
	${Ashes_SOURCE_DIR}/include/util/ExponentialRange.hpp
	${Ashes_SOURCE_DIR}/include/util/GlslToSpv.hpp
	${Ashes_SOURCE_DIR}/include/util/Logarithm.hpp
	${Ashes_SOURCE_DIR}/include/util/LogarithmicRange.hpp
	${Ashes_SOURCE_DIR}/include/util/LogUtils.hpp
	${Ashes_SOURCE_DIR}/include/util/Mat4.hpp
	${Ashes_SOURCE_DIR}/include/util/Mat4.inl
	${Ashes_SOURCE_DIR}/include/util/PlaneEquation.hpp
	${Ashes_SOURCE_DIR}/include/util/Quaternion.hpp
	${Ashes_SOURCE_DIR}/include/util/Quaternion.inl
	${Ashes_SOURCE_DIR}/include/util/Radians.hpp
	${Ashes_SOURCE_DIR}/include/util/Range.hpp
	${Ashes_SOURCE_DIR}/include/util/RangedValue.hpp
	${Ashes_SOURCE_DIR}/include/util/RenderingResources.hpp
	${Ashes_SOURCE_DIR}/include/util/StringUtils.hpp
	${Ashes_SOURCE_DIR}/include/util/Transform.hpp
	${Ashes_SOURCE_DIR}/include/util/Transform.inl
	${Ashes_SOURCE_DIR}/include/util/Transform.hpp
	${Ashes_SOURCE_DIR}/include/util/util.hpp
	${Ashes_SOURCE_DIR}/include/util/UtilsDebug.hpp
	${Ashes_SOURCE_DIR}/include/util/UtilsDevice.hpp
	${Ashes_SOURCE_DIR}/include/util/UtilsFile.hpp
	${Ashes_SOURCE_DIR}/include/util/UtilsInstance.hpp
	${Ashes_SOURCE_DIR}/include/util/UtilsPrerequisites.hpp
	${Ashes_SOURCE_DIR}/include/util/UtilsPrerequisites.inl
	${Ashes_SOURCE_DIR}/include/util/UtilsSwapChain.hpp
	${Ashes_SOURCE_DIR}/include/util/Vec2.hpp
	${Ashes_SOURCE_DIR}/include/util/Vec2.inl
	${Ashes_SOURCE_DIR}/include/util/Vec3.hpp
	${Ashes_SOURCE_DIR}/include/util/Vec3.inl
	${Ashes_SOURCE_DIR}/include/util/Vec4.hpp
	${Ashes_SOURCE_DIR}/include/util/Vec4.inl
	${Ashes_SOURCE_DIR}/include/util/Vectorisation.hpp
	${Ashes_SOURCE_DIR}/include/util/Vectorisation.inl
)
source_group( "Header Files" FILES ${${PROJECT_NAME}_HDR_FILES} )
source_group( "Source Files" FILES ${${PROJECT_NAME}_SRC_FILES} )

if ( MSVC )
	set( BinLibraries
		${Ashes_BINARY_LIBRARIES}
		${glslang_LIBRARIES}
		PARENT_SCOPE
	)
endif ()

add_library( ${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
)
add_library( ashes::${_PROJECT_NAME}
	ALIAS
	${PROJECT_NAME}
)
target_include_directories( ${PROJECT_NAME}
	PRIVATE
		${Vulkan_INCLUDE_DIR}
		${ASHES_CONFIG_DIR}/include
		${Ashes_SOURCE_DIR}/include
		${Ashes_SOURCE_DIR}/source
		${ASHES_CONFIG_DIR}/source
		${ASHES_CONFIG_DIR}/include/${_PROJECT_NAME}
		${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}
		${ASHES_CONFIG_DIR}/source/${_PROJECT_NAME}
		${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}
		${glslang_INCLUDE_DIR}
)
target_link_libraries( ${PROJECT_NAME}
	PRIVATE
		ashes::common
	PUBLIC
		ashes::ashes
		ashes::ashespp
		${glslang_LIBRARIES}
)
target_compile_definitions( ${PROJECT_NAME} PRIVATE
	${TARGET_CXX_DEFINITIONS}
)
target_compile_options( ${PROJECT_NAME} PRIVATE
	${TARGET_CXX_OPTIONS}
)
set_target_properties( ${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	FOLDER "${Ashes_BASE_DIR}/Core"
)
