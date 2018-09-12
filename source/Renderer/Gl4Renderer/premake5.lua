project "Gl4Renderer"

currentSourceDir = path.join( sourceDir, "Renderer", "Gl4Renderer" )
currentBinaryDir = path.join( binaryDir, "Renderer", "Gl4Renderer" )

kind( "SharedLib" )
targetdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", sharedLibDir ) )
implibdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", staticLibDir ) )
location( currentBinaryDir )
defines( "Gl4Renderer_EXPORTS" )

includedirs{
	path.join( sourceDir, "Renderer", "Renderer", "Src" ),
	path.join( binaryDir, "Renderer", "Renderer", "Src" ),
	path.join( currentBinaryDir, "Src" ),
	path.join( currentSourceDir, "Src" )
}

links{
	"Renderer"
}

if ( os.istarget( "Windows" ) )
then
	links{
		"opengl32",
		binaryLinks
	}
else
	links{
		"GL",
		binaryLinks
	}
end

forceincludes( "GlRendererPch.hpp" )

pchheader( "GlRendererPch.hpp" )
pchsource( "./Src/GlRendererPch.cpp" )

files{ "./Src/**.hpp",
	"./Src/**.inl",
	"./Src/**.cpp"
}

vpaths{ ["Header Files"] = "**.hpp" }
vpaths{ ["Header Files"] = "**.inl" }
vpaths{ ["Source Files"] = "**.cpp" }
