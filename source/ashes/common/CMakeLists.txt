set( _PROJECT_NAME common )
project( ashesCommon )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( PROJECT_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" )
set( PROJECT_SOVERSION "${${PROJECT_NAME}_VERSION_BUILD}" )

set( ${PROJECT_NAME}_SRC_FILES
	Common.cpp
	Format.cpp
)
set( ${PROJECT_NAME}_NVS_FILES
	ashes_common.natvis
)

if ( ASHES_BUILD_UWP )
	set( ${PROJECT_NAME}_PLATFORM_SRC_FILES
		UWP/DynamicLibrary.cpp
		UWP/FileUtils.cpp
	)
elseif ( WIN32 )
	set( ${PROJECT_NAME}_PLATFORM_SRC_FILES
		Win32/DynamicLibrary.cpp
		Win32/FileUtils.cpp
	)
elseif ( APPLE )
	set( ${PROJECT_NAME}_PLATFORM_SRC_FILES
		MacOS/DynamicLibrary.cpp
		MacOS/FileUtils.cpp
	)
else ()
	set( ${PROJECT_NAME}_PLATFORM_SRC_FILES
		Linux/DynamicLibrary.cpp
		Linux/FileUtils.cpp
	)
endif ()

set( ${PROJECT_NAME}_HDR_FILES
	../../../include/ashes/common/ArrayView.hpp
	../../../include/ashes/common/BuddyAllocator.hpp
	../../../include/ashes/common/BuddyAllocator.inl
	../../../include/ashes/common/ConstantFormat.hpp
	../../../include/ashes/common/DeviceAllocator.hpp
	../../../include/ashes/common/DynamicLibrary.hpp
	../../../include/ashes/common/Exception.hpp
	../../../include/ashes/common/FileUtils.hpp
	../../../include/ashes/common/FlagCombination.hpp
	../../../include/ashes/common/Format.hpp
	../../../include/ashes/common/Hash.hpp
	../../../include/ashes/common/Optional.hpp
	../../../include/ashes/common/Signal.hpp
	../../../include/ashes/common/VkTypeTraits.hpp
)

add_library( ${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_NVS_FILES}
	${${PROJECT_NAME}_PLATFORM_SRC_FILES}
	${${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES}
	${${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES}
)
add_library( ashes::${_PROJECT_NAME}
	ALIAS
	${PROJECT_NAME}
)
target_sources( ${PROJECT_NAME} 
	PRIVATE
		../../../.editorconfig
)
target_link_libraries( ${PROJECT_NAME}
	PUBLIC
		${CMAKE_DL_LIBS}
)
target_include_directories( ${PROJECT_NAME}
	PRIVATE
		${Ashes_SOURCE_DIR}/include
		${ASHES_CONFIG_DIR}
		${Ashes_SOURCE_DIR}/include/ashes
		${Ashes_SOURCE_DIR}/include/ashes/${_PROJECT_NAME}
)
target_include_directories( ${PROJECT_NAME}
	SYSTEM PRIVATE
		${Vulkan_INCLUDE_DIR}
)
if ( NOT MSVC )
	target_compile_features( ${PROJECT_NAME}
		PUBLIC
			cxx_std_17
	)
endif ()
target_compile_definitions( ${PROJECT_NAME}
	PUBLIC
		${TARGET_COMPILE_DEFINITIONS}
)
if ( ASHES_BUILD_UWP )
	target_compile_definitions( ${PROJECT_NAME}
		PUBLIC
			Ashes_UWP
	)
endif ()
target_compile_options( ${PROJECT_NAME}
	PUBLIC
		${TARGET_COMPILE_OPTIONS}
)
target_add_compilation_flags( ${PROJECT_NAME} )
set_target_properties( ${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	FOLDER "${Ashes_BASE_DIR}/Core"
	DEBUG_POSTFIX "d"
)

source_group( "Source Files\\Platform" FILES ${${PROJECT_NAME}_PLATFORM_SRC_FILES} )

target_install_dir_headers_ex( ${PROJECT_NAME}
	Ashes
	${Ashes_SOURCE_DIR}/include/ashes/${_PROJECT_NAME}
	ashes/${_PROJECT_NAME}
)

