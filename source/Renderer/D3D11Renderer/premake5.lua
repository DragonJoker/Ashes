project "D3D11Renderer"

currentSourceDir = path.join( sourceDir, "Renderer", "D3D11Renderer" )
currentBinaryDir = path.join( binaryDir, "Renderer", "D3D11Renderer" )

kind( "SharedLib" )
targetdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", sharedLibDir ) )
implibdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", staticLibDir ) )
location( currentBinaryDir )
defines( "D3D11Renderer_EXPORTS" )

includedirs{
	path.join( sourceDir, "Renderer", "Renderer", "Src" ),
	path.join( binaryDir, "Renderer", "Renderer", "Src" ),
	path.join( currentBinaryDir, "Src" ),
	path.join( currentSourceDir, "Src" )
}

links{
	"Renderer",
	binaryLinks
}

local configFile = path.join( currentBinaryDir, "Src", "D3D11RendererConfig.hpp" )
local content = io.readfile( "./Src/D3D11RendererConfig.hpp.in" )
content = string.gsub( content, "${D3D11Renderer_VERSION_MAJOR}", 0 )
content = string.gsub( content, "${D3D11Renderer_VERSION_MINOR}", 1 )
content = string.gsub( content, "${D3D11Renderer_VERSION_BUILD}", 0 )
io.writefile( configFile, content )

forceincludes( "D3D11RendererPch.hpp" )

pchheader( "D3D11RendererPch.hpp" )
pchsource( "./Src/D3D11RendererPch.cpp" )

files{ "./Src/**.hpp",
	"./Src/**.inl",
	"./Src/**.cpp",
	configFile
}

vpaths{ ["Header Files"] = "**.hpp" }
vpaths{ ["Header Files"] = configFile }
vpaths{ ["Header Files"] = "**.inl" }
vpaths{ ["Source Files"] = "**.cpp" }
