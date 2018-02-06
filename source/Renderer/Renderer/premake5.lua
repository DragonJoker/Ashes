project "Renderer"

currentSourceDir = path.join( sourceDir, "Renderer", "Renderer" )
currentBinaryDir = path.join( binaryDir, "Renderer", "Renderer" )

kind( "StaticLib" )
targetdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", staticLibDir ) )
location( currentBinaryDir )

includedirs{
	path.join( sourceDir, "Core" ),
	path.join( currentBinaryDir, "Src" ),
	path.join( currentSourceDir, "Src" )
}

links{
	"Utils"
}

local configFile = path.join( currentBinaryDir, "Src", "RendererConfig.hpp" )
local content = io.readfile( "./Src/RendererConfig.hpp.in" )
content = string.gsub( content, "${RENDERLIB_WIN32}", os.istarget( "Windows" ) and 1 or 0 )
content = string.gsub( content, "${RENDERLIB_XLIB}", os.istarget( "Linux" ) and 1 or 0 )
content = string.gsub( content, "${RENDERLIB_XCB}", 0 )
content = string.gsub( content, "${RENDERLIB_ANDROID}",0 )
content = string.gsub( content, "${RENDERLIB_MIR}",0 )
content = string.gsub( content, "${RENDERLIB_WAYLAND}",0 )
io.writefile( configFile, content )

forceincludes( "RendererPch.hpp" )

pchheader( "RendererPch.hpp" )
pchsource( "./Src/RendererPch.cpp" )

files{ "./Src/**.hpp",
	"./Src/**.inl",
	"./Src/**.cpp",
	configFile
}

vpaths{ ["Header Files"] = "**.hpp" }
vpaths{ ["Header Files"] = configFile }
vpaths{ ["Header Files"] = "**.inl" }
vpaths{ ["Source Files"] = "**.cpp" }
