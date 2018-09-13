project "TestRenderer"

currentSourceDir = path.join( sourceDir, "Ashes", "TestRenderer" )
currentBinaryDir = path.join( binaryDir, "Ashes", "TestRenderer" )

kind( "SharedLib" )
targetdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", sharedLibDir ) )
implibdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", staticLibDir ) )
location( currentBinaryDir )
defines( "TestRenderer_EXPORTS" )

includedirs{
	path.join( sourceDir, "Ashes", "Ashes", "Src" ),
	path.join( binaryDir, "Ashes", "Ashes", "Src" ),
	path.join( currentBinaryDir, "Src" ),
	path.join( currentSourceDir, "Src" )
}

links{
	"Ashes",
	binaryLinks
}

local configFile = path.join( currentBinaryDir, "Src", "TestRendererConfig.hpp" )
local content = io.readfile( "./Src/TestRendererConfig.hpp.in" )
content = string.gsub( content, "${TestRenderer_VERSION_MAJOR}", 0 )
content = string.gsub( content, "${TestRenderer_VERSION_MINOR}", 1 )
content = string.gsub( content, "${TestRenderer_VERSION_BUILD}", 0 )
io.writefile( configFile, content )

forceincludes( "TestRendererPch.hpp" )

pchheader( "TestRendererPch.hpp" )
pchsource( "./Src/TestRendererPch.cpp" )

files{ "./Src/**.hpp",
	"./Src/**.inl",
	"./Src/**.cpp",
	configFile
}

vpaths{ ["Header Files"] = "**.hpp" }
vpaths{ ["Header Files"] = configFile }
vpaths{ ["Header Files"] = "**.inl" }
vpaths{ ["Source Files"] = "**.cpp" }
