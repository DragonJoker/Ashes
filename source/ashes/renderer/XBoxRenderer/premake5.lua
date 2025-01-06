project "XBoxRenderer"

currentSourceDir = path.join( sourceDir, "Ashes", "XBoxRenderer" )
currentBinaryDir = path.join( binaryDir, "Ashes", "XBoxRenderer" )

kind( "SharedLib" )
targetdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", sharedLibDir ) )
implibdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", staticLibDir ) )
location( currentBinaryDir )
defines( "XBoxRenderer_EXPORTS" )

includedirs{
	path.join( sourceDir, "Ashes", "Ashes", "Src" ),
	path.join( binaryDir, "Ashes", "Ashes", "Src" ),
	path.join( currentBinaryDir, "Src" ),
	path.join( currentSourceDir, "Src" )
}

links{
	"Ashes",
	binaryLinks,
	"d3d11",
	"dxgi",
	"d3dcompiler",
}

local configFile = path.join( currentBinaryDir, "Src", "XBoxRendererConfig.hpp" )
local content = io.readfile( "./Src/XBoxRendererConfig.hpp.in" )
content = string.gsub( content, "${XBoxRenderer_VERSION_MAJOR}", 0 )
content = string.gsub( content, "${XBoxRenderer_VERSION_MINOR}", 1 )
content = string.gsub( content, "${XBoxRenderer_VERSION_BUILD}", 0 )
io.writefile( configFile, content )

forceincludes( "XBoxRendererPch.hpp" )

pchheader( "XBoxRendererPch.hpp" )
pchsource( "./Src/XBoxRendererPch.cpp" )

files{ "./Src/**.hpp",
	"./Src/**.inl",
	"./Src/**.cpp",
	configFile
}

vpaths{ ["Header Files"] = "**.hpp" }
vpaths{ ["Header Files"] = configFile }
vpaths{ ["Header Files"] = "**.inl" }
vpaths{ ["Source Files"] = "**.cpp" }
