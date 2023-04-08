#pragma once

#ifdef _MSC_VER
#pragma warning (disable:4267)
#pragma warning (disable:4101)
#define _CRT_NONSTDC_NO_DEPRECATE
#define strdup _strdup
#endif

#include <sstream>
#include <regex>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <string>
#include <cstring>
#include <chrono>
#include <string_view>
#include <iomanip>
#include <memory>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <random>
#include <initializer_list>
#include <variant>
#include <type_traits>

#define PK_VERSION				"0.9.7"

// debug macros
#define DEBUG_NO_BUILTIN_MODULES	0
#define DEBUG_EXTRA_CHECK			0
#define DEBUG_DIS_EXEC				0
#define DEBUG_DIS_EXEC_MIN			1
#define DEBUG_CEVAL_STEP			0
#define DEBUG_FULL_EXCEPTION		0
#define DEBUG_MEMORY_POOL			0
#define DEBUG_NO_MEMORY_POOL		0
#define DEBUG_NO_AUTO_GC			0
#define DEBUG_GC_STATS				0

#if (defined(__ANDROID__) && __ANDROID_API__ <= 22) || defined(__EMSCRIPTEN__)
#define PK_ENABLE_FILEIO 		0
#else
#define PK_ENABLE_FILEIO 		0	// TODO: refactor this
#endif

#if defined(__EMSCRIPTEN__) || defined(__arm__) || defined(__i386__)
typedef int32_t i64;
typedef float f64;
#define S_TO_INT(...) static_cast<i64>(std::stoi(__VA_ARGS__))
#define S_TO_FLOAT(...) static_cast<f64>(std::stof(__VA_ARGS__))
#else
typedef int64_t i64;
typedef double f64;
#define S_TO_INT(...) static_cast<i64>(std::stoll(__VA_ARGS__))
#define S_TO_FLOAT(...) static_cast<f64>(std::stod(__VA_ARGS__))
#endif

namespace pkpy{

namespace std = ::std;

struct Dummy { };
struct DummyInstance { };
struct DummyModule { };

struct Type {
	int index;
	Type(): index(-1) {}
	Type(int index): index(index) {}
	bool operator==(Type other) const noexcept { return this->index == other.index; }
	bool operator!=(Type other) const noexcept { return this->index != other.index; }
	operator int() const noexcept { return this->index; }
};

#define THREAD_LOCAL	// thread_local
#define CPP_LAMBDA(x) ([](VM* vm, Args& args) { return x; })
#define CPP_NOT_IMPLEMENTED() ([](VM* vm, Args& args) { vm->NotImplementedError(); return vm->None; })

#ifdef POCKETPY_H
#define UNREACHABLE() throw std::runtime_error( "L" + std::to_string(__LINE__) + " UNREACHABLE()!");
#else
#define UNREACHABLE() throw std::runtime_error( __FILE__ + std::string(":") + std::to_string(__LINE__) + " UNREACHABLE()!");
#endif

inline const float kLocalsLoadFactor = 0.67f;
inline const float kInstAttrLoadFactor = 0.67f;
inline const float kTypeAttrLoadFactor = 0.5f;

static_assert(sizeof(i64) == sizeof(int*));
static_assert(sizeof(f64) == sizeof(int*));
static_assert(std::numeric_limits<float>::is_iec559);
static_assert(std::numeric_limits<double>::is_iec559);

struct PyObject;
#define BITS(p) (reinterpret_cast<i64>(p))
inline bool is_tagged(PyObject* p) noexcept { return (BITS(p) & 0b11) != 0b00; }
inline bool is_int(PyObject* p) noexcept { return (BITS(p) & 0b11) == 0b01; }
inline bool is_float(PyObject* p) noexcept { return (BITS(p) & 0b11) == 0b10; }

inline bool is_both_int_or_float(PyObject* a, PyObject* b) noexcept {
    return is_tagged(a) && is_tagged(b);
}

inline bool is_both_int(PyObject* a, PyObject* b) noexcept {
    return is_int(a) && is_int(b);
}

struct Expr;
typedef std::unique_ptr<Expr> Expr_;

} // namespace pkpy