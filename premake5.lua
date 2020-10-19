workspace "Stegosaurus"
	architecture "x86"

	configurations {
		"Debug",
		"Release"
	}

OutDir = "%{cfg.buildcfg}/%{cfg.system}-%{cfg.architecture}"

AdditionalIncludeDirs = {}
AdditionalIncludeDirs["lodepng"] = "StegosaurusEngine/vendor/lodepng"
AdditionalIncludeDirs["tinyAES"] = "StegosaurusEngine/vendor/tinyAES"
AdditionalIncludeDirs["argon2"] = "StegosaurusEngine/vendor/argon2"

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
        "%{AdditionalIncludeDirs.lodepng}",
        "%{AdditionalIncludeDirs.tinyAES}",
        "%{AdditionalIncludeDirs.argon2}"
	}

    links {
        "lodepng",
        "tinyAES",
        "argon2"
    }

    defines {
        "STEG_BUILD_DLL"
    }

    postbuildcommands {
        ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. OutDir .. "/StegosaurusApplication")
    }

	filter "configurations:Debug"
		defines "STEGOSAURUS_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "STEGOSAURUS_RELEASE"
		optimize "On"

project "StegosaurusApplication"
	location "StegosaurusApplication"
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
		"StegosaurusEngine/src"
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

project "lodepng"
    location "StegosaurusEngine/vendor/lodepng"
    staticruntime "On"
    kind "StaticLib"
    language "C++"
    
	targetdir ("bin/" .. OutDir .. "/%{prj.name}")
	objdir ("bin-obj/" .. OutDir .. "/%{prj.name}")

	files {
		"StegosaurusEngine/vendor/%{prj.name}/lodepng.h",
		"StegosaurusEngine/vendor/%{prj.name}/lodepng.cpp"
    }
    
	includedirs {
		"StegosaurusEngine/vendor/%{prj.name}/lodepng.h",
		"StegosaurusEngine/vendor/%{prj.name}/lodepng.cpp"
	}

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"

project "tinyAES"
    location "StegosaurusEngine/vendor/tinyAES"
    staticruntime "On"
    kind "StaticLib"
    language "C"
    
	targetdir ("bin/" .. OutDir .. "/%{prj.name}")
	objdir ("bin-obj/" .. OutDir .. "/%{prj.name}")

	files {
		"StegosaurusEngine/vendor/%{prj.name}/aes.h",
		"StegosaurusEngine/vendor/%{prj.name}/aes.hpp",
		"StegosaurusEngine/vendor/%{prj.name}/aes.c"
    }
    
	includedirs {
		"StegosaurusEngine/vendor/%{prj.name}/aes.h",
		"StegosaurusEngine/vendor/%{prj.name}/aes.hpp",
		"StegosaurusEngine/vendor/%{prj.name}/aes.c"
	}

    defines {
        "CBC=1",
        "ECB=1",
        "CTR=1"
    }

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"

project "argon2"
    location "StegosaurusEngine/vendor/argon2"
    staticruntime "On"
    kind "StaticLib"
    language "C"
    
	targetdir ("bin/" .. OutDir .. "/%{prj.name}")
	objdir ("bin-obj/" .. OutDir .. "/%{prj.name}")

	files {
		"StegosaurusEngine/vendor/%{prj.name}/include/argon2.h",
		"StegosaurusEngine/vendor/%{prj.name}/src/**.h",
		"StegosaurusEngine/vendor/%{prj.name}/src/**.c"
    }
    
	includedirs {
		"StegosaurusEngine/vendor/%{prj.name}/include",
		"StegosaurusEngine/vendor/%{prj.name}/src/**.h",
		"StegosaurusEngine/vendor/%{prj.name}/src/**.c"
	}

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"
