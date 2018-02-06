project "GlRenderer"

currentSourceDir = path.join( sourceDir, "Renderer", "GlRenderer" )
currentBinaryDir = path.join( binaryDir, "Renderer", "GlRenderer" )

kind( "SharedLib" )
targetdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", sharedLibDir ) )
implibdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", staticLibDir ) )
location( currentBinaryDir )
defines( "GlRenderer_EXPORTS" )

includedirs{
	path.join( sourceDir, "Core" ),
	path.join( sourceDir, "Renderer", "Renderer", "Src" ),
	path.join( binaryDir, "Renderer", "Renderer", "Src" ),
	path.join( currentBinaryDir, "Src" ),
	path.join( currentSourceDir, "Src" )
}

links{
	"Utils",
	"Renderer",
	"opengl32"
}

if ( os.istarget( "Windows" ) )
then
	links{
		"opengl32"
	}
else
	links{
		"GL"
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
