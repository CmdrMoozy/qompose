function(qomposeEnableWarnings)
	if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
		set(F_WARN "-pedantic -Wall -Wextra -Wcast-align -Wcast-qual")
		set(F_WARN "${F_WARN} -Wctor-dtor-privacy -Wlogical-op")
		set(F_WARN "${F_WARN} -Wformat=2 -Winit-self -Wlogical-op")
		set(F_WARN "${F_WARN} -Wmissing-include-dirs -Wnoexcept")
		set(F_WARN "${F_WARN} -Wold-style-cast -Weffc++ -Wshadow")
		set(F_WARN "${F_WARN} -Woverloaded-virtual -Wredundant-decls")
		set(F_WARN "${F_WARN} -Wsign-conversion -Wsign-promo -Wundef")
		set(F_WARN "${F_WARN} -Wstrict-null-sentinel -Wswitch-default")
		set(F_WARN "${F_WARN} -Wdisabled-optimization")
		set(F_WARN "${F_WARN} -Wzero-as-null-pointer-constant")
		set(F_WARN "${F_WARN} -Wstrict-overflow=5 -Wswitch-default")

		set(F_DBG "${F_WARN} -std=c++11 -O0")
		set(F_REL "${F_WARN} -std=c++11 -fomit-frame-pointer")

		set(CMAKE_CXX_FLAGS_DEBUG
			"${CMAKE_CXX_FLAGS_DEBUG} ${F_DBG}"
			CACHE INTERNAL "CMAKE_CXX_FLAGS_RELEASE")
		set(CMAKE_CXX_FLAGS_RELEASE
			"${CMAKE_CXX_FLAGS_RELEASE} ${F_REL}"
			CACHE INTERNAL "CMAKE_CXX_FLAGS_RELEASE")
	elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang)
		set(F_WARN "-pedantic -Wall -Wextra -Wcast-align -Wcast-qual")
		set(F_WARN "${F_WARN} -Wdisabled-optimization -Wformat=2")
		set(F_WARN "${F_WARN} -Winit-self -Wstrict-overflows=5")
		set(F_WARN "${F_WARN} -Wmissing-include-dirs -Wold-style-cast")
		set(F_WARN "${F_WARN} -Wshadow -Wswitch-default -Weffc++")
		set(F_WARN "${F_WARN} -Woverloaded-virtual -Wredundant-decls")
		set(F_WARN "${F_WARN} -Wsign-conversion -Wsign-promo -Wundef")
		set(F_WARN "${F_WARN} -Wint-to-void-pointer-cast")
		set(F_WARN "${F_WARN} -Wctor-dtor-privacy")

		set(F_DBG "${F_WARN} -std=c++11 -O0")
		set(F_REL "${F_WARN} -std=c++11 -fomit-frame-pointer")

		set(CMAKE_CXX_FLAGS_DEBUG
			"${CMAKE_CXX_FLAGS_DEBUG} ${F_DBG}"
			CACHE INTERNAL "CMAKE_CXX_FLAGS_RELEASE")
		set(CMAKE_CXX_FLAGS_RELEASE
			"${CMAKE_CXX_FLAGS_RELEASE} ${F_REL}"
			CACHE INTERNAL "CMAKE_CXX_FLAGS_RELEASE")
	elseif(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
		set(F_DBG "/W4 /WX /FIQomposeMSVCWarnings.h")

		set(F_REL "/W4 /WX /FIQomposeMSVCWarnings.h")

		set(CMAKE_CXX_FLAGS_DEBUG
			"${CMAKE_CXX_FLAGS_DEBUG} ${F_DBG}"
			CACHE INTERNAL "CMAKE_CXX_FLAGS_RELEASE")
		set(CMAKE_CXX_FLAGS_RELEASE
			"${CMAKE_CXX_FLAGS_RELEASE} ${F_REL}"
			CACHE INTERNAL "CMAKE_CXX_FLAGS_RELEASE")
	endif()
endfunction()
