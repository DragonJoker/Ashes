project "VkRenderer"

currentSourceDir = path.join( sourceDir, "Ashes", "VkRenderer" )
currentBinaryDir = path.join( binaryDir, "Ashes", "VkRenderer" )

kind( "SharedLib" )
targetdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", sharedLibDir ) )
implibdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", staticLibDir ) )
location( currentBinaryDir )
defines( "VkRenderer_EXPORTS" )

includedirs{
	path.join( sourceDir, "Ashes", "Ashes", "Src" ),
	path.join( binaryDir, "Ashes", "Ashes", "Src" ),
	path.join( currentBinaryDir, "Src" ),
	path.join( currentSourceDir, "Src" )
}

include( path.join( modulesDir, "FindGlslang.lua" ) )
include( path.join( modulesDir, "FindSPIRV.lua" ) )
include( path.join( modulesDir, "FindSPIRVTools.lua" ) )
include( path.join( modulesDir, "FindOGLCompiler.lua" ) )
include( path.join( modulesDir, "FindOSDependent.lua" ) )

local glslToSPIRV = 0

if ( SPIRVToolsEnabled
	and SPIRVEnabled 
	and OSDependentEnabled
	and OGLCompilerEnabled
	and GlslangEnabled )
then
	glslToSPIRV = 1
end

links{
	"Ashes",
	binaryLinks
}

local configFile = path.join( currentBinaryDir, "Src", "VkRendererConfig.hpp" )
local content = io.readfile( "./Src/VkRendererConfig.hpp.in" )
content = string.gsub( content, "${VkRenderer_VERSION_MAJOR}", 0 )
content = string.gsub( content, "${VkRenderer_VERSION_MINOR}", 1 )
content = string.gsub( content, "${VkRenderer_VERSION_BUILD}", 0 )
content = string.gsub( content, "${VKRENDERER_GLSL_TO_SPV}", glslToSPIRV )
io.writefile( configFile, content )

forceincludes( "VkRendererPch.hpp" )

pchheader( "VkRendererPch.hpp" )
pchsource( "./Src/VkRendererPch.cpp" )

files{ "./Src/**.hpp",
	"./Src/**.inl",
	"./Src/**.cpp",
	configFile
}

vpaths{ ["Header Files"] = "**.hpp" }
vpaths{ ["Header Files"] = configFile }
vpaths{ ["Header Files"] = "**.inl" }
vpaths{ ["Source Files"] = "**.cpp" }
