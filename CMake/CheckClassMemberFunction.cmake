include(CheckCXXSourceCompiles)

macro(CHECK_CLASS_MEMBER_FUNCTION _CLASS _MEMBER _HEADER _RESULT)
	set(_INCLUDE_FILES)
	foreach(it ${_HEADER})
		set(_INCLUDE_FILES "${_INCLUDE_FILES}#include <${it}>\n")
	endforeach()

	set(_CHECK_STRUCT_MEMBER_SOURCE_CODE "
${_INCLUDE_FILES}
#include <cstdint>

template <typename T>
class hasFunction
{
private:
	typedef uint8_t one;
	typedef uint16_t two;

	template <typename C> static one test(decltype(&C::${_MEMBER}));
	template <typename C> static two test(...);

public:
	enum { value = sizeof(test<T>(0)) == sizeof(uint8_t) };
};

int main()
{
	static_assert(hasFunction<${_CLASS}>::value, \"Function doesn't exist.\");
	return 0;
}
")

	check_cxx_source_compiles("${_CHECK_STRUCT_MEMBER_SOURCE_CODE}" ${_RESULT})
endmacro()
