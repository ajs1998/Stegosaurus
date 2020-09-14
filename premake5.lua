workspace "Stegosaurus"
	architecture "x86"

	configurations {
		"Debug",
		"Release"
	}

OutDir = "%{cfg.buildcfg}/%{cfg.system}-%{cfg.architecture}"

AdditionalIncludeDirs = {}
AdditionalIncludeDirs["lodepng"] = "StegosaurusEngine/vendor/lodepng"

project "StegosaurusEngine"
	location "StegosaurusEngine"
    staticruntime "On"
	kind "StaticLib"
	language "C++"
    cppdialect "C++17"

	targetdir ("bin/" .. OutDir .. "/%{prj.name}")
	objdir ("bin-obj/" .. OutDir .. "/%{prj.name}")

	pchheader "stegosaurus_pch.h"
	pchsource "StegosaurusEngine/src/stegosaurus_pch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/src",
        "%{AdditionalIncludeDirs.lodepng}"
	}

	links {
        "lodepng"
	}
    
    defines {
        "STEG_BUILD_DLL"
    }

    postbuildcommands {
        ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. OutDir .. "/Stegosaurus")
    }
    
	filter "configurations:Debug"
		defines "STEGOSAURUS_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "STEGOSAURUS_RELEASE"
		optimize "On"

project "Stegosaurus"
	location "Stegosaurus"
    staticruntime "On"
	kind "ConsoleApp"
	language "C++"
    cppdialect "C++17"

	targetdir ("bin/" .. OutDir .. "/%{prj.name}")
	objdir ("bin-obj/" .. OutDir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/src",
		"StegosaurusEngine/src",
        "%{AdditionalIncludeDirs.lodepng}"
	}

	links {
		"StegosaurusEngine"
	}

	filter "configurations:Debug"
		defines "STEGOSAURUS_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "STEGOSAURUS_RELEASE"
		optimize "On"
