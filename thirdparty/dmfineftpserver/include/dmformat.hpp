#ifndef __DMFORMATALL_H__
#define __DMFORMATALL_H__


/*** Start of inlined file: dmchrono.h ***/
// Formatting library for C++ - chrono support
//
// Copyright (c) 2012 - present, Victor Zverovich
// All rights reserved.
//
// For the license information refer to format.h.

#ifndef FMT_CHRONO_H_
#define FMT_CHRONO_H_

#include <chrono>
#include <ctime>
#include <locale>
#include <sstream>


/*** Start of inlined file: dmformat.h ***/
#ifndef FMT_FORMAT_H_
#define FMT_FORMAT_H_

#define FMT_HEADER_ONLY
#include <algorithm>
#include <cerrno>
#include <cmath>
#include <cstdint>
#include <limits>
#include <memory>
#include <stdexcept>


/*** Start of inlined file: dmcore.h ***/
// Formatting library for C++ - the core API
//
// Copyright (c) 2012 - present, Victor Zverovich
// All rights reserved.
//
// For the license information refer to format.h.

#ifndef FMT_CORE_H_
#define FMT_CORE_H_

#include <cstdio>  // std::FILE
#include <cstring>
#include <functional>
#include <iterator>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

// The fmt library version in the form major * 10000 + minor * 100 + patch.
#define FMT_VERSION 70103

#ifdef __clang__
#  define FMT_CLANG_VERSION (__clang_major__ * 100 + __clang_minor__)
#else
#  define FMT_CLANG_VERSION 0
#endif

#if defined(__GNUC__) && !defined(__clang__)
#  define FMT_GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)
#else
#  define FMT_GCC_VERSION 0
#endif

#if defined(__INTEL_COMPILER)
#  define FMT_ICC_VERSION __INTEL_COMPILER
#else
#  define FMT_ICC_VERSION 0
#endif

#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
#  define FMT_HAS_GXX_CXX11 FMT_GCC_VERSION
#else
#  define FMT_HAS_GXX_CXX11 0
#endif

#ifdef __NVCC__
#  define FMT_NVCC __NVCC__
#else
#  define FMT_NVCC 0
#endif

#ifdef _MSC_VER
#  define FMT_MSC_VER _MSC_VER
#  define FMT_SUPPRESS_MSC_WARNING(n) __pragma(warning(suppress : n))
#else
#  define FMT_MSC_VER 0
#  define FMT_SUPPRESS_MSC_WARNING(n)
#endif

#ifdef __has_feature
#  define FMT_HAS_FEATURE(x) __has_feature(x)
#else
#  define FMT_HAS_FEATURE(x) 0
#endif

#if defined(__has_include) && !defined(__INTELLISENSE__) && \
	(!FMT_ICC_VERSION || FMT_ICC_VERSION >= 1600)
#  define FMT_HAS_INCLUDE(x) __has_include(x)
#else
#  define FMT_HAS_INCLUDE(x) 0
#endif

#ifdef __has_cpp_attribute
#  define FMT_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
#  define FMT_HAS_CPP_ATTRIBUTE(x) 0
#endif

#define FMT_HAS_CPP14_ATTRIBUTE(attribute) \
  (__cplusplus >= 201402L && FMT_HAS_CPP_ATTRIBUTE(attribute))

#define FMT_HAS_CPP17_ATTRIBUTE(attribute) \
  (__cplusplus >= 201703L && FMT_HAS_CPP_ATTRIBUTE(attribute))

// Check if relaxed C++14 constexpr is supported.
// GCC doesn't allow throw in constexpr until version 6 (bug 67371).
#ifndef FMT_USE_CONSTEXPR
#  define FMT_USE_CONSTEXPR                                           \
	(FMT_HAS_FEATURE(cxx_relaxed_constexpr) || FMT_MSC_VER >= 1910 || \
	 (FMT_GCC_VERSION >= 600 && __cplusplus >= 201402L)) &&           \
		!FMT_NVCC && !FMT_ICC_VERSION
#endif
#if FMT_USE_CONSTEXPR
#  define FMT_CONSTEXPR constexpr
#  define FMT_CONSTEXPR_DECL constexpr
#else
#  define FMT_CONSTEXPR inline
#  define FMT_CONSTEXPR_DECL
#endif

#ifndef FMT_OVERRIDE
#  if FMT_HAS_FEATURE(cxx_override_control) || \
	  (FMT_GCC_VERSION >= 408 && FMT_HAS_GXX_CXX11) || FMT_MSC_VER >= 1900
#    define FMT_OVERRIDE override
#  else
#    define FMT_OVERRIDE
#  endif
#endif

// Check if exceptions are disabled.
#ifndef FMT_EXCEPTIONS
#  if (defined(__GNUC__) && !defined(__EXCEPTIONS)) || \
	  FMT_MSC_VER && !_HAS_EXCEPTIONS
#    define FMT_EXCEPTIONS 0
#  else
#    define FMT_EXCEPTIONS 1
#  endif
#endif

// Define FMT_USE_NOEXCEPT to make fmt use noexcept (C++11 feature).
#ifndef FMT_USE_NOEXCEPT
#  define FMT_USE_NOEXCEPT 0
#endif

#if FMT_USE_NOEXCEPT || FMT_HAS_FEATURE(cxx_noexcept) || \
	(FMT_GCC_VERSION >= 408 && FMT_HAS_GXX_CXX11) || FMT_MSC_VER >= 1900
#  define FMT_DETECTED_NOEXCEPT noexcept
#  define FMT_HAS_CXX11_NOEXCEPT 1
#else
#  define FMT_DETECTED_NOEXCEPT throw()
#  define FMT_HAS_CXX11_NOEXCEPT 0
#endif

#ifndef FMT_NOEXCEPT
#  if FMT_EXCEPTIONS || FMT_HAS_CXX11_NOEXCEPT
#    define FMT_NOEXCEPT FMT_DETECTED_NOEXCEPT
#  else
#    define FMT_NOEXCEPT
#  endif
#endif

// [[noreturn]] is disabled on MSVC and NVCC because of bogus unreachable code
// warnings.
#if FMT_EXCEPTIONS && FMT_HAS_CPP_ATTRIBUTE(noreturn) && !FMT_MSC_VER && \
	!FMT_NVCC
#  define FMT_NORETURN [[noreturn]]
#else
#  define FMT_NORETURN
#endif

#ifndef FMT_DEPRECATED
#  if FMT_HAS_CPP14_ATTRIBUTE(deprecated) || FMT_MSC_VER >= 1900
#    define FMT_DEPRECATED [[deprecated]]
#  else
#    if (defined(__GNUC__) && !defined(__LCC__)) || defined(__clang__)
#      define FMT_DEPRECATED __attribute__((deprecated))
#    elif FMT_MSC_VER
#      define FMT_DEPRECATED __declspec(deprecated)
#    else
#      define FMT_DEPRECATED /* deprecated */
#    endif
#  endif
#endif

// Workaround broken [[deprecated]] in the Intel, PGI and NVCC compilers.
#if FMT_ICC_VERSION || defined(__PGI) || FMT_NVCC
#  define FMT_DEPRECATED_ALIAS
#else
#  define FMT_DEPRECATED_ALIAS FMT_DEPRECATED
#endif

#ifndef FMT_INLINE
#  if FMT_GCC_VERSION || FMT_CLANG_VERSION
#    define FMT_INLINE inline __attribute__((always_inline))
#  else
#    define FMT_INLINE inline
#  endif
#endif

#ifndef FMT_USE_INLINE_NAMESPACES
#  if FMT_HAS_FEATURE(cxx_inline_namespaces) || FMT_GCC_VERSION >= 404 || \
	  (FMT_MSC_VER >= 1900 && !_MANAGED)
#    define FMT_USE_INLINE_NAMESPACES 1
#  else
#    define FMT_USE_INLINE_NAMESPACES 0
#  endif
#endif

#ifndef FMT_BEGIN_NAMESPACE
#  if FMT_USE_INLINE_NAMESPACES
#    define FMT_INLINE_NAMESPACE inline namespace
#    define FMT_END_NAMESPACE \
	  }                       \
	  }
#  else
#    define FMT_INLINE_NAMESPACE namespace
#    define FMT_END_NAMESPACE \
	  }                       \
	  using namespace v7;     \
	  }
#  endif
#  define FMT_BEGIN_NAMESPACE \
	namespace fmt {           \
	FMT_INLINE_NAMESPACE v7 {
#endif

#if !defined(FMT_HEADER_ONLY) && defined(_WIN32)
#  define FMT_CLASS_API FMT_SUPPRESS_MSC_WARNING(4275)
#  ifdef FMT_EXPORT
#    define FMT_API __declspec(dllexport)
#    define FMT_EXTERN_TEMPLATE_API FMT_API
#    define FMT_EXPORTED
#  elif defined(FMT_SHARED)
#    define FMT_API __declspec(dllimport)
#    define FMT_EXTERN_TEMPLATE_API FMT_API
#  endif
#else
#  define FMT_CLASS_API
#endif
#ifndef FMT_API
#  define FMT_API
#endif
#ifndef FMT_EXTERN_TEMPLATE_API
#  define FMT_EXTERN_TEMPLATE_API
#endif
#ifndef FMT_INSTANTIATION_DEF_API
#  define FMT_INSTANTIATION_DEF_API FMT_API
#endif

#ifndef FMT_HEADER_ONLY
#  define FMT_EXTERN extern
#else
#  define FMT_EXTERN
#endif

// libc++ supports string_view in pre-c++17.
#if (FMT_HAS_INCLUDE(<string_view>) &&                       \
	 (__cplusplus > 201402L || defined(_LIBCPP_VERSION))) || \
	(defined(_MSVC_LANG) && _MSVC_LANG > 201402L && _MSC_VER >= 1910)
#  include <string_view>
#  define FMT_USE_STRING_VIEW
#elif FMT_HAS_INCLUDE("experimental/string_view") && __cplusplus >= 201402L
#  include <experimental/string_view>
#  define FMT_USE_EXPERIMENTAL_STRING_VIEW
#endif

#ifndef FMT_UNICODE
#  define FMT_UNICODE !FMT_MSC_VER
#endif
#if FMT_UNICODE && FMT_MSC_VER
#  pragma execution_character_set("utf-8")
#endif

FMT_BEGIN_NAMESPACE

// Implementations of enable_if_t and other metafunctions for older systems.
template <bool B, class T = void>
using enable_if_t = typename std::enable_if<B, T>::type;
template <bool B, class T, class F>
using conditional_t = typename std::conditional<B, T, F>::type;
template <bool B> using bool_constant = std::integral_constant<bool, B>;
template <typename T>
using remove_reference_t = typename std::remove_reference<T>::type;
template <typename T>
using remove_const_t = typename std::remove_const<T>::type;
template <typename T>
using remove_cvref_t = typename std::remove_cv<remove_reference_t<T>>::type;
template <typename T> struct type_identity { using type = T; };
template <typename T> using type_identity_t = typename type_identity<T>::type;

struct monostate {};

// An enable_if helper to be used in template parameters which results in much
// shorter symbols: https://godbolt.org/z/sWw4vP. Extra parentheses are needed
// to workaround a bug in MSVC 2019 (see #1140 and #1186).
#define FMT_ENABLE_IF(...) enable_if_t<(__VA_ARGS__), int> = 0

namespace detail {

// A helper function to suppress "conditional expression is constant" warnings.
template <typename T> constexpr T const_check(T value) { return value; }

FMT_NORETURN FMT_API void assert_fail(const char* file, int line,
									  const char* message);

#ifndef FMT_ASSERT
#  ifdef NDEBUG
// FMT_ASSERT is not empty to avoid -Werror=empty-body.
#    define FMT_ASSERT(condition, message) ((void)0)
#  else
#    define FMT_ASSERT(condition, message)                                    \
	  ((condition) /* void() fails with -Winvalid-constexpr on clang 4.0.1 */ \
		   ? (void)0                                                          \
		   : ::fmt::detail::assert_fail(__FILE__, __LINE__, (message)))
#  endif
#endif

#if defined(FMT_USE_STRING_VIEW)
template <typename Char> using std_string_view = std::basic_string_view<Char>;
#elif defined(FMT_USE_EXPERIMENTAL_STRING_VIEW)
template <typename Char>
using std_string_view = std::experimental::basic_string_view<Char>;
#else
template <typename T> struct std_string_view {};
#endif

#ifdef FMT_USE_INT128
// Do nothing.
#elif defined(__SIZEOF_INT128__) && !FMT_NVCC && \
	!(FMT_CLANG_VERSION && FMT_MSC_VER)
#  define FMT_USE_INT128 1
using int128_t = __int128_t;
using uint128_t = __uint128_t;
#else
#  define FMT_USE_INT128 0
#endif
#if !FMT_USE_INT128
struct int128_t {};
struct uint128_t {};
#endif

// Casts a nonnegative integer to unsigned.
template <typename Int>
FMT_CONSTEXPR typename std::make_unsigned<Int>::type to_unsigned(Int value) {
  FMT_ASSERT(value >= 0, "negative value");
  return static_cast<typename std::make_unsigned<Int>::type>(value);
}

FMT_SUPPRESS_MSC_WARNING(4566) constexpr unsigned char micro[] = "\u00B5";

template <typename Char> constexpr bool is_unicode() {
  return FMT_UNICODE || sizeof(Char) != 1 ||
		 (sizeof(micro) == 3 && micro[0] == 0xC2 && micro[1] == 0xB5);
}

#ifdef __cpp_char8_t
using char8_type = char8_t;
#else
enum char8_type : unsigned char {};
#endif
}  // namespace detail

#ifdef FMT_USE_INTERNAL
namespace internal = detail;  // DEPRECATED
#endif

/**
  An implementation of ``std::basic_string_view`` for pre-C++17. It provides a
  subset of the API. ``fmt::basic_string_view`` is used for format strings even
  if ``std::string_view`` is available to prevent issues when a library is
  compiled with a different ``-std`` option than the client code (which is not
  recommended).
 */
template <typename Char> class basic_string_view {
 private:
  const Char* data_;
  size_t size_;

 public:
  using value_type = Char;
  using iterator = const Char*;

  constexpr basic_string_view() FMT_NOEXCEPT : data_(nullptr), size_(0) {}

  /** Constructs a string reference object from a C string and a size. */
  constexpr basic_string_view(const Char* s, size_t count) FMT_NOEXCEPT
	  : data_(s),
		size_(count) {}

  /**
	\rst
	Constructs a string reference object from a C string computing
	the size with ``std::char_traits<Char>::length``.
	\endrst
   */
#if __cplusplus >= 201703L  // C++17's char_traits::length() is constexpr.
  FMT_CONSTEXPR
#endif
  basic_string_view(const Char* s)
	  : data_(s), size_(std::char_traits<Char>::length(s)) {}

  /** Constructs a string reference from a ``std::basic_string`` object. */
  template <typename Traits, typename Alloc>
  FMT_CONSTEXPR basic_string_view(
	  const std::basic_string<Char, Traits, Alloc>& s) FMT_NOEXCEPT
	  : data_(s.data()),
		size_(s.size()) {}

  template <typename S, FMT_ENABLE_IF(std::is_same<
									  S, detail::std_string_view<Char>>::value)>
  FMT_CONSTEXPR basic_string_view(S s) FMT_NOEXCEPT : data_(s.data()),
													  size_(s.size()) {}

  /** Returns a pointer to the string data. */
  constexpr const Char* data() const { return data_; }

  /** Returns the string size. */
  constexpr size_t size() const { return size_; }

  constexpr iterator begin() const { return data_; }
  constexpr iterator end() const { return data_ + size_; }

  constexpr const Char& operator[](size_t pos) const { return data_[pos]; }

  FMT_CONSTEXPR void remove_prefix(size_t n) {
	data_ += n;
	size_ -= n;
  }

  // Lexicographically compare this string reference to other.
  int compare(basic_string_view other) const {
	size_t str_size = size_ < other.size_ ? size_ : other.size_;
	int result = std::char_traits<Char>::compare(data_, other.data_, str_size);
	if (result == 0)
	  result = size_ == other.size_ ? 0 : (size_ < other.size_ ? -1 : 1);
	return result;
  }

  friend bool operator==(basic_string_view lhs, basic_string_view rhs) {
	return lhs.compare(rhs) == 0;
  }
  friend bool operator!=(basic_string_view lhs, basic_string_view rhs) {
	return lhs.compare(rhs) != 0;
  }
  friend bool operator<(basic_string_view lhs, basic_string_view rhs) {
	return lhs.compare(rhs) < 0;
  }
  friend bool operator<=(basic_string_view lhs, basic_string_view rhs) {
	return lhs.compare(rhs) <= 0;
  }
  friend bool operator>(basic_string_view lhs, basic_string_view rhs) {
	return lhs.compare(rhs) > 0;
  }
  friend bool operator>=(basic_string_view lhs, basic_string_view rhs) {
	return lhs.compare(rhs) >= 0;
  }
};

using string_view = basic_string_view<char>;
using wstring_view = basic_string_view<wchar_t>;

/** Specifies if ``T`` is a character type. Can be specialized by users. */
template <typename T> struct is_char : std::false_type {};
template <> struct is_char<char> : std::true_type {};
template <> struct is_char<wchar_t> : std::true_type {};
template <> struct is_char<detail::char8_type> : std::true_type {};
template <> struct is_char<char16_t> : std::true_type {};
template <> struct is_char<char32_t> : std::true_type {};

/**
  \rst
  Returns a string view of `s`. In order to add custom string type support to
  {fmt} provide an overload of `to_string_view` for it in the same namespace as
  the type for the argument-dependent lookup to work.

  **Example**::

	namespace my_ns {
	inline string_view to_string_view(const my_string& s) {
	  return {s.data(), s.length()};
	}
	}
	std::string message = fmt::format(my_string("The answer is {}"), 42);
  \endrst
 */
template <typename Char, FMT_ENABLE_IF(is_char<Char>::value)>
inline basic_string_view<Char> to_string_view(const Char* s) {
  return s;
}

template <typename Char, typename Traits, typename Alloc>
inline basic_string_view<Char> to_string_view(
	const std::basic_string<Char, Traits, Alloc>& s) {
  return s;
}

template <typename Char>
inline basic_string_view<Char> to_string_view(basic_string_view<Char> s) {
  return s;
}

template <typename Char,
		  FMT_ENABLE_IF(!std::is_empty<detail::std_string_view<Char>>::value)>
inline basic_string_view<Char> to_string_view(detail::std_string_view<Char> s) {
  return s;
}

// A base class for compile-time strings. It is defined in the fmt namespace to
// make formatting functions visible via ADL, e.g. format(FMT_STRING("{}"), 42).
struct compile_string {};

template <typename S>
struct is_compile_string : std::is_base_of<compile_string, S> {};

template <typename S, FMT_ENABLE_IF(is_compile_string<S>::value)>
constexpr basic_string_view<typename S::char_type> to_string_view(const S& s) {
  return s;
}

namespace detail {
void to_string_view(...);
using fmt::v7::to_string_view;

// Specifies whether S is a string type convertible to fmt::basic_string_view.
// It should be a constexpr function but MSVC 2017 fails to compile it in
// enable_if and MSVC 2015 fails to compile it as an alias template.
template <typename S>
struct is_string : std::is_class<decltype(to_string_view(std::declval<S>()))> {
};

template <typename S, typename = void> struct char_t_impl {};
template <typename S> struct char_t_impl<S, enable_if_t<is_string<S>::value>> {
  using result = decltype(to_string_view(std::declval<S>()));
  using type = typename result::value_type;
};

// Reports a compile-time error if S is not a valid format string.
template <typename..., typename S, FMT_ENABLE_IF(!is_compile_string<S>::value)>
FMT_INLINE void check_format_string(const S&) {
#ifdef FMT_ENFORCE_COMPILE_STRING
  static_assert(is_compile_string<S>::value,
				"FMT_ENFORCE_COMPILE_STRING requires all format strings to use "
				"FMT_STRING.");
#endif
}
template <typename..., typename S, FMT_ENABLE_IF(is_compile_string<S>::value)>
void check_format_string(S);

struct error_handler {
  constexpr error_handler() = default;
  constexpr error_handler(const error_handler&) = default;

  // This function is intentionally not constexpr to give a compile-time error.
  FMT_NORETURN FMT_API void on_error(const char* message);
};
}  // namespace detail

/** String's character type. */
template <typename S> using char_t = typename detail::char_t_impl<S>::type;

/**
  \rst
  Parsing context consisting of a format string range being parsed and an
  argument counter for automatic indexing.

  You can use one of the following type aliases for common character types:

  +-----------------------+-------------------------------------+
  | Type                  | Definition                          |
  +=======================+=====================================+
  | format_parse_context  | basic_format_parse_context<char>    |
  +-----------------------+-------------------------------------+
  | wformat_parse_context | basic_format_parse_context<wchar_t> |
  +-----------------------+-------------------------------------+
  \endrst
 */
template <typename Char, typename ErrorHandler = detail::error_handler>
class basic_format_parse_context : private ErrorHandler {
 private:
  basic_string_view<Char> format_str_;
  int next_arg_id_;

 public:
  using char_type = Char;
  using iterator = typename basic_string_view<Char>::iterator;

  explicit constexpr basic_format_parse_context(
	  basic_string_view<Char> format_str, ErrorHandler eh = {},
	  int next_arg_id = 0)
	  : ErrorHandler(eh), format_str_(format_str), next_arg_id_(next_arg_id) {}

  /**
	Returns an iterator to the beginning of the format string range being
	parsed.
   */
  constexpr iterator begin() const FMT_NOEXCEPT { return format_str_.begin(); }

  /**
	Returns an iterator past the end of the format string range being parsed.
   */
  constexpr iterator end() const FMT_NOEXCEPT { return format_str_.end(); }

  /** Advances the begin iterator to ``it``. */
  FMT_CONSTEXPR void advance_to(iterator it) {
	format_str_.remove_prefix(detail::to_unsigned(it - begin()));
  }

  /**
	Reports an error if using the manual argument indexing; otherwise returns
	the next argument index and switches to the automatic indexing.
   */
  FMT_CONSTEXPR int next_arg_id() {
	// Don't check if the argument id is valid to avoid overhead and because it
	// will be checked during formatting anyway.
	if (next_arg_id_ >= 0) return next_arg_id_++;
	on_error("cannot switch from manual to automatic argument indexing");
	return 0;
  }

  /**
	Reports an error if using the automatic argument indexing; otherwise
	switches to the manual indexing.
   */
  FMT_CONSTEXPR void check_arg_id(int) {
	if (next_arg_id_ > 0)
	  on_error("cannot switch from automatic to manual argument indexing");
	else
	  next_arg_id_ = -1;
  }

  FMT_CONSTEXPR void check_arg_id(basic_string_view<Char>) {}

  FMT_CONSTEXPR void on_error(const char* message) {
	ErrorHandler::on_error(message);
  }

  constexpr ErrorHandler error_handler() const { return *this; }
};

using format_parse_context = basic_format_parse_context<char>;
using wformat_parse_context = basic_format_parse_context<wchar_t>;

template <typename Context> class basic_format_arg;
template <typename Context> class basic_format_args;
template <typename Context> class dynamic_format_arg_store;

// A formatter for objects of type T.
template <typename T, typename Char = char, typename Enable = void>
struct formatter {
  // A deleted default constructor indicates a disabled formatter.
  formatter() = delete;
};

// Specifies if T has an enabled formatter specialization. A type can be
// formattable even if it doesn't have a formatter e.g. via a conversion.
template <typename T, typename Context>
using has_formatter =
	std::is_constructible<typename Context::template formatter_type<T>>;

// Checks whether T is a container with contiguous storage.
template <typename T> struct is_contiguous : std::false_type {};
template <typename Char>
struct is_contiguous<std::basic_string<Char>> : std::true_type {};

namespace detail {

// Extracts a reference to the container from back_insert_iterator.
template <typename Container>
inline Container& get_container(std::back_insert_iterator<Container> it) {
  using bi_iterator = std::back_insert_iterator<Container>;
  struct accessor : bi_iterator {
	accessor(bi_iterator iter) : bi_iterator(iter) {}
	using bi_iterator::container;
  };
  return *accessor(it).container;
}

/**
  \rst
  A contiguous memory buffer with an optional growing ability. It is an internal
  class and shouldn't be used directly, only via `~fmt::basic_memory_buffer`.
  \endrst
 */
template <typename T> class buffer {
 private:
  T* ptr_;
  size_t size_;
  size_t capacity_;

 protected:
  // Don't initialize ptr_ since it is not accessed to save a few cycles.
  FMT_SUPPRESS_MSC_WARNING(26495)
  buffer(size_t sz) FMT_NOEXCEPT : size_(sz), capacity_(sz) {}

  buffer(T* p = nullptr, size_t sz = 0, size_t cap = 0) FMT_NOEXCEPT
	  : ptr_(p),
		size_(sz),
		capacity_(cap) {}

  ~buffer() = default;

  /** Sets the buffer data and capacity. */
  void set(T* buf_data, size_t buf_capacity) FMT_NOEXCEPT {
	ptr_ = buf_data;
	capacity_ = buf_capacity;
  }

  /** Increases the buffer capacity to hold at least *capacity* elements. */
  virtual void grow(size_t capacity) = 0;

 public:
  using value_type = T;
  using const_reference = const T&;

  buffer(const buffer&) = delete;
  void operator=(const buffer&) = delete;

  T* begin() FMT_NOEXCEPT { return ptr_; }
  T* end() FMT_NOEXCEPT { return ptr_ + size_; }

  const T* begin() const FMT_NOEXCEPT { return ptr_; }
  const T* end() const FMT_NOEXCEPT { return ptr_ + size_; }

  /** Returns the size of this buffer. */
  size_t size() const FMT_NOEXCEPT { return size_; }

  /** Returns the capacity of this buffer. */
  size_t capacity() const FMT_NOEXCEPT { return capacity_; }

  /** Returns a pointer to the buffer data. */
  T* data() FMT_NOEXCEPT { return ptr_; }

  /** Returns a pointer to the buffer data. */
  const T* data() const FMT_NOEXCEPT { return ptr_; }

  /** Clears this buffer. */
  void clear() { size_ = 0; }

  // Tries resizing the buffer to contain *count* elements. If T is a POD type
  // the new elements may not be initialized.
  void try_resize(size_t count) {
	try_reserve(count);
	size_ = count <= capacity_ ? count : capacity_;
  }

  // Tries increasing the buffer capacity to *new_capacity*. It can increase the
  // capacity by a smaller amount than requested but guarantees there is space
  // for at least one additional element either by increasing the capacity or by
  // flushing the buffer if it is full.
  void try_reserve(size_t new_capacity) {
	if (new_capacity > capacity_) grow(new_capacity);
  }

  void push_back(const T& value) {
	try_reserve(size_ + 1);
	ptr_[size_++] = value;
  }

  /** Appends data to the end of the buffer. */
  template <typename U> void append(const U* begin, const U* end);

  template <typename I> T& operator[](I index) { return ptr_[index]; }
  template <typename I> const T& operator[](I index) const {
	return ptr_[index];
  }
};

struct buffer_traits {
  explicit buffer_traits(size_t) {}
  size_t count() const { return 0; }
  size_t limit(size_t size) { return size; }
};

class fixed_buffer_traits {
 private:
  size_t count_ = 0;
  size_t limit_;

 public:
  explicit fixed_buffer_traits(size_t limit) : limit_(limit) {}
  size_t count() const { return count_; }
  size_t limit(size_t size) {
	size_t n = limit_ > count_ ? limit_ - count_ : 0;
	count_ += size;
	return size < n ? size : n;
  }
};

// A buffer that writes to an output iterator when flushed.
template <typename OutputIt, typename T, typename Traits = buffer_traits>
class iterator_buffer final : public Traits, public buffer<T> {
 private:
  OutputIt out_;
  enum { buffer_size = 256 };
  T data_[buffer_size];

 protected:
  void grow(size_t) final FMT_OVERRIDE {
	if (this->size() == buffer_size) flush();
  }
  void flush();

 public:
  explicit iterator_buffer(OutputIt out, size_t n = buffer_size)
	  : Traits(n),
		buffer<T>(data_, 0, buffer_size),
		out_(out) {}
  ~iterator_buffer() { flush(); }

  OutputIt out() {
	flush();
	return out_;
  }
  size_t count() const { return Traits::count() + this->size(); }
};

template <typename T> class iterator_buffer<T*, T> final : public buffer<T> {
 protected:
  void grow(size_t) final FMT_OVERRIDE {}

 public:
  explicit iterator_buffer(T* out, size_t = 0) : buffer<T>(out, 0, ~size_t()) {}

  T* out() { return &*this->end(); }
};

// A buffer that writes to a container with the contiguous storage.
template <typename Container>
class iterator_buffer<std::back_insert_iterator<Container>,
					  enable_if_t<is_contiguous<Container>::value,
								  typename Container::value_type>>
	final : public buffer<typename Container::value_type> {
 private:
  Container& container_;

 protected:
  void grow(size_t capacity) final FMT_OVERRIDE {
	container_.resize(capacity);
	this->set(&container_[0], capacity);
  }

 public:
  explicit iterator_buffer(Container& c)
	  : buffer<typename Container::value_type>(c.size()), container_(c) {}
  explicit iterator_buffer(std::back_insert_iterator<Container> out, size_t = 0)
	  : iterator_buffer(get_container(out)) {}
  std::back_insert_iterator<Container> out() {
	return std::back_inserter(container_);
  }
};

// A buffer that counts the number of code units written discarding the output.
template <typename T = char> class counting_buffer final : public buffer<T> {
 private:
  enum { buffer_size = 256 };
  T data_[buffer_size];
  size_t count_ = 0;

 protected:
  void grow(size_t) final FMT_OVERRIDE {
	if (this->size() != buffer_size) return;
	count_ += this->size();
	this->clear();
  }

 public:
  counting_buffer() : buffer<T>(data_, 0, buffer_size) {}

  size_t count() { return count_ + this->size(); }
};

// An output iterator that appends to the buffer.
// It is used to reduce symbol sizes for the common case.
template <typename T>
class buffer_appender : public std::back_insert_iterator<buffer<T>> {
  using base = std::back_insert_iterator<buffer<T>>;

 public:
  explicit buffer_appender(buffer<T>& buf) : base(buf) {}
  buffer_appender(base it) : base(it) {}

  buffer_appender& operator++() {
	base::operator++();
	return *this;
  }

  buffer_appender operator++(int) {
	buffer_appender tmp = *this;
	++*this;
	return tmp;
  }
};

// Maps an output iterator into a buffer.
template <typename T, typename OutputIt>
iterator_buffer<OutputIt, T> get_buffer(OutputIt);
template <typename T> buffer<T>& get_buffer(buffer_appender<T>);

template <typename OutputIt> OutputIt get_buffer_init(OutputIt out) {
  return out;
}
template <typename T> buffer<T>& get_buffer_init(buffer_appender<T> out) {
  return get_container(out);
}

template <typename Buffer>
auto get_iterator(Buffer& buf) -> decltype(buf.out()) {
  return buf.out();
}
template <typename T> buffer_appender<T> get_iterator(buffer<T>& buf) {
  return buffer_appender<T>(buf);
}

template <typename T, typename Char = char, typename Enable = void>
struct fallback_formatter {
  fallback_formatter() = delete;
};

// Specifies if T has an enabled fallback_formatter specialization.
template <typename T, typename Context>
using has_fallback_formatter =
	std::is_constructible<fallback_formatter<T, typename Context::char_type>>;

struct view {};

template <typename Char, typename T> struct named_arg : view {
  const Char* name;
  const T& value;
  named_arg(const Char* n, const T& v) : name(n), value(v) {}
};

template <typename Char> struct named_arg_info {
  const Char* name;
  int id;
};

template <typename T, typename Char, size_t NUM_ARGS, size_t NUM_NAMED_ARGS>
struct arg_data {
  // args_[0].named_args points to named_args_ to avoid bloating format_args.
  // +1 to workaround a bug in gcc 7.5 that causes duplicated-branches warning.
  T args_[1 + (NUM_ARGS != 0 ? NUM_ARGS : +1)];
  named_arg_info<Char> named_args_[NUM_NAMED_ARGS];

  template <typename... U>
  arg_data(const U&... init) : args_{T(named_args_, NUM_NAMED_ARGS), init...} {}
  arg_data(const arg_data& other) = delete;
  const T* args() const { return args_ + 1; }
  named_arg_info<Char>* named_args() { return named_args_; }
};

template <typename T, typename Char, size_t NUM_ARGS>
struct arg_data<T, Char, NUM_ARGS, 0> {
  // +1 to workaround a bug in gcc 7.5 that causes duplicated-branches warning.
  T args_[NUM_ARGS != 0 ? NUM_ARGS : +1];

  template <typename... U>
  FMT_INLINE arg_data(const U&... init) : args_{init...} {}
  FMT_INLINE const T* args() const { return args_; }
  FMT_INLINE std::nullptr_t named_args() { return nullptr; }
};

template <typename Char>
inline void init_named_args(named_arg_info<Char>*, int, int) {}

template <typename Char, typename T, typename... Tail>
void init_named_args(named_arg_info<Char>* named_args, int arg_count,
					 int named_arg_count, const T&, const Tail&... args) {
  init_named_args(named_args, arg_count + 1, named_arg_count, args...);
}

template <typename Char, typename T, typename... Tail>
void init_named_args(named_arg_info<Char>* named_args, int arg_count,
					 int named_arg_count, const named_arg<Char, T>& arg,
					 const Tail&... args) {
  named_args[named_arg_count++] = {arg.name, arg_count};
  init_named_args(named_args, arg_count + 1, named_arg_count, args...);
}

template <typename... Args>
FMT_INLINE void init_named_args(std::nullptr_t, int, int, const Args&...) {}

template <typename T> struct is_named_arg : std::false_type {};

template <typename T, typename Char>
struct is_named_arg<named_arg<Char, T>> : std::true_type {};

template <bool B = false> constexpr size_t count() { return B ? 1 : 0; }
template <bool B1, bool B2, bool... Tail> constexpr size_t count() {
  return (B1 ? 1 : 0) + count<B2, Tail...>();
}

template <typename... Args> constexpr size_t count_named_args() {
  return count<is_named_arg<Args>::value...>();
}

enum class type {
  none_type,
  // Integer types should go first,
  int_type,
  uint_type,
  long_long_type,
  ulong_long_type,
  int128_type,
  uint128_type,
  bool_type,
  char_type,
  last_integer_type = char_type,
  // followed by floating-point types.
  float_type,
  double_type,
  long_double_type,
  last_numeric_type = long_double_type,
  cstring_type,
  string_type,
  pointer_type,
  custom_type
};

// Maps core type T to the corresponding type enum constant.
template <typename T, typename Char>
struct type_constant : std::integral_constant<type, type::custom_type> {};

#define FMT_TYPE_CONSTANT(Type, constant) \
  template <typename Char>                \
  struct type_constant<Type, Char>        \
	  : std::integral_constant<type, type::constant> {}

FMT_TYPE_CONSTANT(int, int_type);
FMT_TYPE_CONSTANT(unsigned, uint_type);
FMT_TYPE_CONSTANT(long long, long_long_type);
FMT_TYPE_CONSTANT(unsigned long long, ulong_long_type);
FMT_TYPE_CONSTANT(int128_t, int128_type);
FMT_TYPE_CONSTANT(uint128_t, uint128_type);
FMT_TYPE_CONSTANT(bool, bool_type);
FMT_TYPE_CONSTANT(Char, char_type);
FMT_TYPE_CONSTANT(float, float_type);
FMT_TYPE_CONSTANT(double, double_type);
FMT_TYPE_CONSTANT(long double, long_double_type);
FMT_TYPE_CONSTANT(const Char*, cstring_type);
FMT_TYPE_CONSTANT(basic_string_view<Char>, string_type);
FMT_TYPE_CONSTANT(const void*, pointer_type);

constexpr bool is_integral_type(type t) {
  return t > type::none_type && t <= type::last_integer_type;
}

constexpr bool is_arithmetic_type(type t) {
  return t > type::none_type && t <= type::last_numeric_type;
}

template <typename Char> struct string_value {
  const Char* data;
  size_t size;
};

template <typename Char> struct named_arg_value {
  const named_arg_info<Char>* data;
  size_t size;
};

template <typename Context> struct custom_value {
  using parse_context = typename Context::parse_context_type;
  const void* value;
  void (*format)(const void* arg, parse_context& parse_ctx, Context& ctx);
};

// A formatting argument value.
template <typename Context> class value {
 public:
  using char_type = typename Context::char_type;

  union {
	int int_value;
	unsigned uint_value;
	long long long_long_value;
	unsigned long long ulong_long_value;
	int128_t int128_value;
	uint128_t uint128_value;
	bool bool_value;
	char_type char_value;
	float float_value;
	double double_value;
	long double long_double_value;
	const void* pointer;
	string_value<char_type> string;
	custom_value<Context> custom;
	named_arg_value<char_type> named_args;
  };

  constexpr FMT_INLINE value(int val = 0) : int_value(val) {}
  constexpr FMT_INLINE value(unsigned val) : uint_value(val) {}
  FMT_INLINE value(long long val) : long_long_value(val) {}
  FMT_INLINE value(unsigned long long val) : ulong_long_value(val) {}
  FMT_INLINE value(int128_t val) : int128_value(val) {}
  FMT_INLINE value(uint128_t val) : uint128_value(val) {}
  FMT_INLINE value(float val) : float_value(val) {}
  FMT_INLINE value(double val) : double_value(val) {}
  FMT_INLINE value(long double val) : long_double_value(val) {}
  FMT_INLINE value(bool val) : bool_value(val) {}
  FMT_INLINE value(char_type val) : char_value(val) {}
  FMT_INLINE value(const char_type* val) { string.data = val; }
  FMT_INLINE value(basic_string_view<char_type> val) {
	string.data = val.data();
	string.size = val.size();
  }
  FMT_INLINE value(const void* val) : pointer(val) {}
  FMT_INLINE value(const named_arg_info<char_type>* args, size_t size)
	  : named_args{args, size} {}

  template <typename T> FMT_INLINE value(const T& val) {
	custom.value = &val;
	// Get the formatter type through the context to allow different contexts
	// have different extension points, e.g. `formatter<T>` for `format` and
	// `printf_formatter<T>` for `printf`.
	custom.format = format_custom_arg<
		T, conditional_t<has_formatter<T, Context>::value,
						 typename Context::template formatter_type<T>,
						 fallback_formatter<T, char_type>>>;
  }

 private:
  // Formats an argument of a custom type, such as a user-defined class.
  template <typename T, typename Formatter>
  static void format_custom_arg(const void* arg,
								typename Context::parse_context_type& parse_ctx,
								Context& ctx) {
	Formatter f;
	parse_ctx.advance_to(f.parse(parse_ctx));
	ctx.advance_to(f.format(*static_cast<const T*>(arg), ctx));
  }
};

template <typename Context, typename T>
FMT_CONSTEXPR basic_format_arg<Context> make_arg(const T& value);

// To minimize the number of types we need to deal with, long is translated
// either to int or to long long depending on its size.
enum { long_short = sizeof(long) == sizeof(int) };
using long_type = conditional_t<long_short, int, long long>;
using ulong_type = conditional_t<long_short, unsigned, unsigned long long>;

struct unformattable {};

// Maps formatting arguments to core types.
template <typename Context> struct arg_mapper {
  using char_type = typename Context::char_type;

  FMT_CONSTEXPR int map(signed char val) { return val; }
  FMT_CONSTEXPR unsigned map(unsigned char val) { return val; }
  FMT_CONSTEXPR int map(short val) { return val; }
  FMT_CONSTEXPR unsigned map(unsigned short val) { return val; }
  FMT_CONSTEXPR int map(int val) { return val; }
  FMT_CONSTEXPR unsigned map(unsigned val) { return val; }
  FMT_CONSTEXPR long_type map(long val) { return val; }
  FMT_CONSTEXPR ulong_type map(unsigned long val) { return val; }
  FMT_CONSTEXPR long long map(long long val) { return val; }
  FMT_CONSTEXPR unsigned long long map(unsigned long long val) { return val; }
  FMT_CONSTEXPR int128_t map(int128_t val) { return val; }
  FMT_CONSTEXPR uint128_t map(uint128_t val) { return val; }
  FMT_CONSTEXPR bool map(bool val) { return val; }

  template <typename T, FMT_ENABLE_IF(is_char<T>::value)>
  FMT_CONSTEXPR char_type map(T val) {
	static_assert(
		std::is_same<T, char>::value || std::is_same<T, char_type>::value,
		"mixing character types is disallowed");
	return val;
  }

  FMT_CONSTEXPR float map(float val) { return val; }
  FMT_CONSTEXPR double map(double val) { return val; }
  FMT_CONSTEXPR long double map(long double val) { return val; }

  FMT_CONSTEXPR const char_type* map(char_type* val) { return val; }
  FMT_CONSTEXPR const char_type* map(const char_type* val) { return val; }
  template <typename T, FMT_ENABLE_IF(is_string<T>::value)>
  FMT_CONSTEXPR basic_string_view<char_type> map(const T& val) {
	static_assert(std::is_same<char_type, char_t<T>>::value,
				  "mixing character types is disallowed");
	return to_string_view(val);
  }
  template <typename T,
			FMT_ENABLE_IF(
				std::is_constructible<basic_string_view<char_type>, T>::value &&
				!is_string<T>::value && !has_formatter<T, Context>::value &&
				!has_fallback_formatter<T, Context>::value)>
  FMT_CONSTEXPR basic_string_view<char_type> map(const T& val) {
	return basic_string_view<char_type>(val);
  }
  template <
	  typename T,
	  FMT_ENABLE_IF(
		  std::is_constructible<std_string_view<char_type>, T>::value &&
		  !std::is_constructible<basic_string_view<char_type>, T>::value &&
		  !is_string<T>::value && !has_formatter<T, Context>::value &&
		  !has_fallback_formatter<T, Context>::value)>
  FMT_CONSTEXPR basic_string_view<char_type> map(const T& val) {
	return std_string_view<char_type>(val);
  }
  FMT_CONSTEXPR const char* map(const signed char* val) {
	static_assert(std::is_same<char_type, char>::value, "invalid string type");
	return reinterpret_cast<const char*>(val);
  }
  FMT_CONSTEXPR const char* map(const unsigned char* val) {
	static_assert(std::is_same<char_type, char>::value, "invalid string type");
	return reinterpret_cast<const char*>(val);
  }
  FMT_CONSTEXPR const char* map(signed char* val) {
	const auto* const_val = val;
	return map(const_val);
  }
  FMT_CONSTEXPR const char* map(unsigned char* val) {
	const auto* const_val = val;
	return map(const_val);
  }

  FMT_CONSTEXPR const void* map(void* val) { return val; }
  FMT_CONSTEXPR const void* map(const void* val) { return val; }
  FMT_CONSTEXPR const void* map(std::nullptr_t val) { return val; }
  template <typename T> FMT_CONSTEXPR int map(const T*) {
	// Formatting of arbitrary pointers is disallowed. If you want to output
	// a pointer cast it to "void *" or "const void *". In particular, this
	// forbids formatting of "[const] volatile char *" which is printed as bool
	// by iostreams.
	static_assert(!sizeof(T), "formatting of non-void pointers is disallowed");
	return 0;
  }

  template <typename T,
			FMT_ENABLE_IF(std::is_enum<T>::value &&
						  !has_formatter<T, Context>::value &&
						  !has_fallback_formatter<T, Context>::value)>
  FMT_CONSTEXPR auto map(const T& val)
	  -> decltype(std::declval<arg_mapper>().map(
		  static_cast<typename std::underlying_type<T>::type>(val))) {
	return map(static_cast<typename std::underlying_type<T>::type>(val));
  }
  template <typename T,
			FMT_ENABLE_IF(!is_string<T>::value && !is_char<T>::value &&
						  (has_formatter<T, Context>::value ||
						   has_fallback_formatter<T, Context>::value))>
  FMT_CONSTEXPR const T& map(const T& val) {
	return val;
  }

  template <typename T>
  FMT_CONSTEXPR auto map(const named_arg<char_type, T>& val)
	  -> decltype(std::declval<arg_mapper>().map(val.value)) {
	return map(val.value);
  }

  unformattable map(...) { return {}; }
};

// A type constant after applying arg_mapper<Context>.
template <typename T, typename Context>
using mapped_type_constant =
	type_constant<decltype(arg_mapper<Context>().map(std::declval<const T&>())),
				  typename Context::char_type>;

enum { packed_arg_bits = 4 };
// Maximum number of arguments with packed types.
enum { max_packed_args = 62 / packed_arg_bits };
enum : unsigned long long { is_unpacked_bit = 1ULL << 63 };
enum : unsigned long long { has_named_args_bit = 1ULL << 62 };
}  // namespace detail

// A formatting argument. It is a trivially copyable/constructible type to
// allow storage in basic_memory_buffer.
template <typename Context> class basic_format_arg {
 private:
  detail::value<Context> value_;
  detail::type type_;

  template <typename ContextType, typename T>
  friend FMT_CONSTEXPR basic_format_arg<ContextType> detail::make_arg(
	  const T& value);

  template <typename Visitor, typename Ctx>
  friend FMT_CONSTEXPR auto visit_format_arg(Visitor&& vis,
											 const basic_format_arg<Ctx>& arg)
	  -> decltype(vis(0));

  friend class basic_format_args<Context>;
  friend class dynamic_format_arg_store<Context>;

  using char_type = typename Context::char_type;

  template <typename T, typename Char, size_t NUM_ARGS, size_t NUM_NAMED_ARGS>
  friend struct detail::arg_data;

  basic_format_arg(const detail::named_arg_info<char_type>* args, size_t size)
	  : value_(args, size) {}

 public:
  class handle {
   public:
	explicit handle(detail::custom_value<Context> custom) : custom_(custom) {}

	void format(typename Context::parse_context_type& parse_ctx,
				Context& ctx) const {
	  custom_.format(custom_.value, parse_ctx, ctx);
	}

   private:
	detail::custom_value<Context> custom_;
  };

  constexpr basic_format_arg() : type_(detail::type::none_type) {}

  constexpr explicit operator bool() const FMT_NOEXCEPT {
	return type_ != detail::type::none_type;
  }

  detail::type type() const { return type_; }

  bool is_integral() const { return detail::is_integral_type(type_); }
  bool is_arithmetic() const { return detail::is_arithmetic_type(type_); }
};

/**
  \rst
  Visits an argument dispatching to the appropriate visit method based on
  the argument type. For example, if the argument type is ``double`` then
  ``vis(value)`` will be called with the value of type ``double``.
  \endrst
 */
template <typename Visitor, typename Context>
FMT_CONSTEXPR_DECL FMT_INLINE auto visit_format_arg(
	Visitor&& vis, const basic_format_arg<Context>& arg) -> decltype(vis(0)) {
  using char_type = typename Context::char_type;
  switch (arg.type_) {
  case detail::type::none_type:
	break;
  case detail::type::int_type:
	return vis(arg.value_.int_value);
  case detail::type::uint_type:
	return vis(arg.value_.uint_value);
  case detail::type::long_long_type:
	return vis(arg.value_.long_long_value);
  case detail::type::ulong_long_type:
	return vis(arg.value_.ulong_long_value);
#if FMT_USE_INT128
  case detail::type::int128_type:
	return vis(arg.value_.int128_value);
  case detail::type::uint128_type:
	return vis(arg.value_.uint128_value);
#else
  case detail::type::int128_type:
  case detail::type::uint128_type:
	break;
#endif
  case detail::type::bool_type:
	return vis(arg.value_.bool_value);
  case detail::type::char_type:
	return vis(arg.value_.char_value);
  case detail::type::float_type:
	return vis(arg.value_.float_value);
  case detail::type::double_type:
	return vis(arg.value_.double_value);
  case detail::type::long_double_type:
	return vis(arg.value_.long_double_value);
  case detail::type::cstring_type:
	return vis(arg.value_.string.data);
  case detail::type::string_type:
	return vis(basic_string_view<char_type>(arg.value_.string.data,
											arg.value_.string.size));
  case detail::type::pointer_type:
	return vis(arg.value_.pointer);
  case detail::type::custom_type:
	return vis(typename basic_format_arg<Context>::handle(arg.value_.custom));
  }
  return vis(monostate());
}

template <typename T> struct formattable : std::false_type {};

namespace detail {

// A workaround for gcc 4.8 to make void_t work in a SFINAE context.
template <typename... Ts> struct void_t_impl { using type = void; };
template <typename... Ts>
using void_t = typename detail::void_t_impl<Ts...>::type;

template <typename It, typename T, typename Enable = void>
struct is_output_iterator : std::false_type {};

template <typename It, typename T>
struct is_output_iterator<
	It, T,
	void_t<typename std::iterator_traits<It>::iterator_category,
		   decltype(*std::declval<It>() = std::declval<T>())>>
	: std::true_type {};

template <typename OutputIt>
struct is_back_insert_iterator : std::false_type {};
template <typename Container>
struct is_back_insert_iterator<std::back_insert_iterator<Container>>
	: std::true_type {};

template <typename OutputIt>
struct is_contiguous_back_insert_iterator : std::false_type {};
template <typename Container>
struct is_contiguous_back_insert_iterator<std::back_insert_iterator<Container>>
	: is_contiguous<Container> {};
template <typename Char>
struct is_contiguous_back_insert_iterator<buffer_appender<Char>>
	: std::true_type {};

// A type-erased reference to an std::locale to avoid heavy <locale> include.
class locale_ref {
 private:
  const void* locale_;  // A type-erased pointer to std::locale.

 public:
  locale_ref() : locale_(nullptr) {}
  template <typename Locale> explicit locale_ref(const Locale& loc);

  explicit operator bool() const FMT_NOEXCEPT { return locale_ != nullptr; }

  template <typename Locale> Locale get() const;
};

template <typename> constexpr unsigned long long encode_types() { return 0; }

template <typename Context, typename Arg, typename... Args>
constexpr unsigned long long encode_types() {
  return static_cast<unsigned>(mapped_type_constant<Arg, Context>::value) |
		 (encode_types<Context, Args...>() << packed_arg_bits);
}

template <typename Context, typename T>
FMT_CONSTEXPR basic_format_arg<Context> make_arg(const T& value) {
  basic_format_arg<Context> arg;
  arg.type_ = mapped_type_constant<T, Context>::value;
  arg.value_ = arg_mapper<Context>().map(value);
  return arg;
}

template <typename T> int check(unformattable) {
  static_assert(
	  formattable<T>(),
	  "Cannot format an argument. To make type T formattable provide a "
	  "formatter<T> specialization: https://fmt.dev/latest/api.html#udt");
  return 0;
}
template <typename T, typename U> inline const U& check(const U& val) {
  return val;
}

// The type template parameter is there to avoid an ODR violation when using
// a fallback formatter in one translation unit and an implicit conversion in
// another (not recommended).
template <bool IS_PACKED, typename Context, type, typename T,
		  FMT_ENABLE_IF(IS_PACKED)>
inline value<Context> make_arg(const T& val) {
  return check<T>(arg_mapper<Context>().map(val));
}

template <bool IS_PACKED, typename Context, type, typename T,
		  FMT_ENABLE_IF(!IS_PACKED)>
inline basic_format_arg<Context> make_arg(const T& value) {
  return make_arg<Context>(value);
}

template <typename T> struct is_reference_wrapper : std::false_type {};
template <typename T>
struct is_reference_wrapper<std::reference_wrapper<T>> : std::true_type {};

template <typename T> const T& unwrap(const T& v) { return v; }
template <typename T> const T& unwrap(const std::reference_wrapper<T>& v) {
  return static_cast<const T&>(v);
}

class dynamic_arg_list {
  // Workaround for clang's -Wweak-vtables. Unlike for regular classes, for
  // templates it doesn't complain about inability to deduce single translation
  // unit for placing vtable. So storage_node_base is made a fake template.
  template <typename = void> struct node {
	virtual ~node() = default;
	std::unique_ptr<node<>> next;
  };

  template <typename T> struct typed_node : node<> {
	T value;

	template <typename Arg>
	FMT_CONSTEXPR typed_node(const Arg& arg) : value(arg) {}

	template <typename Char>
	FMT_CONSTEXPR typed_node(const basic_string_view<Char>& arg)
		: value(arg.data(), arg.size()) {}
  };

  std::unique_ptr<node<>> head_;

 public:
  template <typename T, typename Arg> const T& push(const Arg& arg) {
	auto new_node = std::unique_ptr<typed_node<T>>(new typed_node<T>(arg));
	auto& value = new_node->value;
	new_node->next = std::move(head_);
	head_ = std::move(new_node);
	return value;
  }
};
}  // namespace detail

// Formatting context.
template <typename OutputIt, typename Char> class basic_format_context {
 public:
  /** The character type for the output. */
  using char_type = Char;

 private:
  OutputIt out_;
  basic_format_args<basic_format_context> args_;
  detail::locale_ref loc_;

 public:
  using iterator = OutputIt;
  using format_arg = basic_format_arg<basic_format_context>;
  using parse_context_type = basic_format_parse_context<Char>;
  template <typename T> using formatter_type = formatter<T, char_type>;

  basic_format_context(const basic_format_context&) = delete;
  void operator=(const basic_format_context&) = delete;
  /**
   Constructs a ``basic_format_context`` object. References to the arguments are
   stored in the object so make sure they have appropriate lifetimes.
   */
  basic_format_context(OutputIt out,
					   basic_format_args<basic_format_context> ctx_args,
					   detail::locale_ref loc = detail::locale_ref())
	  : out_(out), args_(ctx_args), loc_(loc) {}

  format_arg arg(int id) const { return args_.get(id); }
  format_arg arg(basic_string_view<char_type> name) { return args_.get(name); }
  int arg_id(basic_string_view<char_type> name) { return args_.get_id(name); }
  const basic_format_args<basic_format_context>& args() const { return args_; }

  detail::error_handler error_handler() { return {}; }
  void on_error(const char* message) { error_handler().on_error(message); }

  // Returns an iterator to the beginning of the output range.
  iterator out() { return out_; }

  // Advances the begin iterator to ``it``.
  void advance_to(iterator it) {
	if (!detail::is_back_insert_iterator<iterator>()) out_ = it;
  }

  detail::locale_ref locale() { return loc_; }
};

template <typename Char>
using buffer_context =
	basic_format_context<detail::buffer_appender<Char>, Char>;
using format_context = buffer_context<char>;
using wformat_context = buffer_context<wchar_t>;

// Workaround an alias issue: https://stackoverflow.com/q/62767544/471164.
#define FMT_BUFFER_CONTEXT(Char) \
  basic_format_context<detail::buffer_appender<Char>, Char>

/**
  \rst
  An array of references to arguments. It can be implicitly converted into
  `~fmt::basic_format_args` for passing into type-erased formatting functions
  such as `~fmt::vformat`.
  \endrst
 */
template <typename Context, typename... Args>
class format_arg_store
#if FMT_GCC_VERSION && FMT_GCC_VERSION < 409
	// Workaround a GCC template argument substitution bug.
	: public basic_format_args<Context>
#endif
{
 private:
  static const size_t num_args = sizeof...(Args);
  static const size_t num_named_args = detail::count_named_args<Args...>();
  static const bool is_packed = num_args <= detail::max_packed_args;

  using value_type = conditional_t<is_packed, detail::value<Context>,
								   basic_format_arg<Context>>;

  detail::arg_data<value_type, typename Context::char_type, num_args,
				   num_named_args>
	  data_;

  friend class basic_format_args<Context>;

  static constexpr unsigned long long desc =
	  (is_packed ? detail::encode_types<Context, Args...>()
				 : detail::is_unpacked_bit | num_args) |
	  (num_named_args != 0
		   ? static_cast<unsigned long long>(detail::has_named_args_bit)
		   : 0);

 public:
  format_arg_store(const Args&... args)
	  :
#if FMT_GCC_VERSION && FMT_GCC_VERSION < 409
		basic_format_args<Context>(*this),
#endif
		data_{detail::make_arg<
			is_packed, Context,
			detail::mapped_type_constant<Args, Context>::value>(args)...} {
	detail::init_named_args(data_.named_args(), 0, 0, args...);
  }
};

/**
  \rst
  Constructs a `~fmt::format_arg_store` object that contains references to
  arguments and can be implicitly converted to `~fmt::format_args`. `Context`
  can be omitted in which case it defaults to `~fmt::context`.
  See `~fmt::arg` for lifetime considerations.
  \endrst
 */
template <typename Context = format_context, typename... Args>
inline format_arg_store<Context, Args...> make_format_args(
	const Args&... args) {
  return {args...};
}

/**
  \rst
  Constructs a `~fmt::format_arg_store` object that contains references
  to arguments and can be implicitly converted to `~fmt::format_args`.
  If ``format_str`` is a compile-time string then `make_args_checked` checks
  its validity at compile time.
  \endrst
 */
template <typename... Args, typename S, typename Char = char_t<S>>
inline auto make_args_checked(const S& format_str,
							  const remove_reference_t<Args>&... args)
	-> format_arg_store<buffer_context<Char>, remove_reference_t<Args>...> {
  static_assert(
	  detail::count<(
			  std::is_base_of<detail::view, remove_reference_t<Args>>::value &&
			  std::is_reference<Args>::value)...>() == 0,
	  "passing views as lvalues is disallowed");
  detail::check_format_string<Args...>(format_str);
  return {args...};
}

/**
  \rst
  Returns a named argument to be used in a formatting function. It should only
  be used in a call to a formatting function.

  **Example**::

	fmt::print("Elapsed time: {s:.2f} seconds", fmt::arg("s", 1.23));
  \endrst
 */
template <typename Char, typename T>
inline detail::named_arg<Char, T> arg(const Char* name, const T& arg) {
  static_assert(!detail::is_named_arg<T>(), "nested named arguments");
  return {name, arg};
}

/**
  \rst
  A dynamic version of `fmt::format_arg_store`.
  It's equipped with a storage to potentially temporary objects which lifetimes
  could be shorter than the format arguments object.

  It can be implicitly converted into `~fmt::basic_format_args` for passing
  into type-erased formatting functions such as `~fmt::vformat`.
  \endrst
 */
template <typename Context>
class dynamic_format_arg_store
#if FMT_GCC_VERSION && FMT_GCC_VERSION < 409
	// Workaround a GCC template argument substitution bug.
	: public basic_format_args<Context>
#endif
{
 private:
  using char_type = typename Context::char_type;

  template <typename T> struct need_copy {
	static constexpr detail::type mapped_type =
		detail::mapped_type_constant<T, Context>::value;

	enum {
	  value = !(detail::is_reference_wrapper<T>::value ||
				std::is_same<T, basic_string_view<char_type>>::value ||
				std::is_same<T, detail::std_string_view<char_type>>::value ||
				(mapped_type != detail::type::cstring_type &&
				 mapped_type != detail::type::string_type &&
				 mapped_type != detail::type::custom_type))
	};
  };

  template <typename T>
  using stored_type = conditional_t<detail::is_string<T>::value,
									std::basic_string<char_type>, T>;

  // Storage of basic_format_arg must be contiguous.
  std::vector<basic_format_arg<Context>> data_;
  std::vector<detail::named_arg_info<char_type>> named_info_;

  // Storage of arguments not fitting into basic_format_arg must grow
  // without relocation because items in data_ refer to it.
  detail::dynamic_arg_list dynamic_args_;

  friend class basic_format_args<Context>;

  unsigned long long get_types() const {
	return detail::is_unpacked_bit | data_.size() |
		   (named_info_.empty()
				? 0ULL
				: static_cast<unsigned long long>(detail::has_named_args_bit));
  }

  const basic_format_arg<Context>* data() const {
	return named_info_.empty() ? data_.data() : data_.data() + 1;
  }

  template <typename T> void emplace_arg(const T& arg) {
	data_.emplace_back(detail::make_arg<Context>(arg));
  }

  template <typename T>
  void emplace_arg(const detail::named_arg<char_type, T>& arg) {
	if (named_info_.empty()) {
	  constexpr const detail::named_arg_info<char_type>* zero_ptr{nullptr};
	  data_.insert(data_.begin(), {zero_ptr, 0});
	}
	data_.emplace_back(detail::make_arg<Context>(detail::unwrap(arg.value)));
	auto pop_one = [](std::vector<basic_format_arg<Context>>* data) {
	  data->pop_back();
	};
	std::unique_ptr<std::vector<basic_format_arg<Context>>, decltype(pop_one)>
		guard{&data_, pop_one};
	named_info_.push_back({arg.name, static_cast<int>(data_.size() - 2u)});
	data_[0].value_.named_args = {named_info_.data(), named_info_.size()};
	guard.release();
  }

 public:
  /**
	\rst
	Adds an argument into the dynamic store for later passing to a formatting
	function.

	Note that custom types and string types (but not string views) are copied
	into the store dynamically allocating memory if necessary.

	**Example**::

	  fmt::dynamic_format_arg_store<fmt::format_context> store;
	  store.push_back(42);
	  store.push_back("abc");
	  store.push_back(1.5f);
	  std::string result = fmt::vformat("{} and {} and {}", store);
	\endrst
  */
  template <typename T> void push_back(const T& arg) {
	if (detail::const_check(need_copy<T>::value))
	  emplace_arg(dynamic_args_.push<stored_type<T>>(arg));
	else
	  emplace_arg(detail::unwrap(arg));
  }

  /**
	\rst
	Adds a reference to the argument into the dynamic store for later passing to
	a formatting function. Supports named arguments wrapped in
	``std::reference_wrapper`` via ``std::ref()``/``std::cref()``.

	**Example**::

	  fmt::dynamic_format_arg_store<fmt::format_context> store;
	  char str[] = "1234567890";
	  store.push_back(std::cref(str));
	  int a1_val{42};
	  auto a1 = fmt::arg("a1_", a1_val);
	  store.push_back(std::cref(a1));

	  // Changing str affects the output but only for string and custom types.
	  str[0] = 'X';

	  std::string result = fmt::vformat("{} and {a1_}");
	  assert(result == "X234567890 and 42");
	\endrst
  */
  template <typename T> void push_back(std::reference_wrapper<T> arg) {
	static_assert(
		detail::is_named_arg<typename std::remove_cv<T>::type>::value ||
			need_copy<T>::value,
		"objects of built-in types and string views are always copied");
	emplace_arg(arg.get());
  }

  /**
	Adds named argument into the dynamic store for later passing to a formatting
	function. ``std::reference_wrapper`` is supported to avoid copying of the
	argument.
  */
  template <typename T>
  void push_back(const detail::named_arg<char_type, T>& arg) {
	const char_type* arg_name =
		dynamic_args_.push<std::basic_string<char_type>>(arg.name).c_str();
	if (detail::const_check(need_copy<T>::value)) {
	  emplace_arg(
		  fmt::arg(arg_name, dynamic_args_.push<stored_type<T>>(arg.value)));
	} else {
	  emplace_arg(fmt::arg(arg_name, arg.value));
	}
  }

  /** Erase all elements from the store */
  void clear() {
	data_.clear();
	named_info_.clear();
	dynamic_args_ = detail::dynamic_arg_list();
  }

  /**
	\rst
	Reserves space to store at least *new_cap* arguments including
	*new_cap_named* named arguments.
	\endrst
  */
  void reserve(size_t new_cap, size_t new_cap_named) {
	FMT_ASSERT(new_cap >= new_cap_named,
			   "Set of arguments includes set of named arguments");
	data_.reserve(new_cap);
	named_info_.reserve(new_cap_named);
  }
};

/**
  \rst
  A view of a collection of formatting arguments. To avoid lifetime issues it
  should only be used as a parameter type in type-erased functions such as
  ``vformat``::

	void vlog(string_view format_str, format_args args);  // OK
	format_args args = make_format_args(42);  // Error: dangling reference
  \endrst
 */
template <typename Context> class basic_format_args {
 public:
  using size_type = int;
  using format_arg = basic_format_arg<Context>;

 private:
  // A descriptor that contains information about formatting arguments.
  // If the number of arguments is less or equal to max_packed_args then
  // argument types are passed in the descriptor. This reduces binary code size
  // per formatting function call.
  unsigned long long desc_;
  union {
	// If is_packed() returns true then argument values are stored in values_;
	// otherwise they are stored in args_. This is done to improve cache
	// locality and reduce compiled code size since storing larger objects
	// may require more code (at least on x86-64) even if the same amount of
	// data is actually copied to stack. It saves ~10% on the bloat test.
	const detail::value<Context>* values_;
	const format_arg* args_;
  };

  bool is_packed() const { return (desc_ & detail::is_unpacked_bit) == 0; }
  bool has_named_args() const {
	return (desc_ & detail::has_named_args_bit) != 0;
  }

  detail::type type(int index) const {
	int shift = index * detail::packed_arg_bits;
	unsigned int mask = (1 << detail::packed_arg_bits) - 1;
	return static_cast<detail::type>((desc_ >> shift) & mask);
  }

  basic_format_args(unsigned long long desc,
					const detail::value<Context>* values)
	  : desc_(desc), values_(values) {}
  basic_format_args(unsigned long long desc, const format_arg* args)
	  : desc_(desc), args_(args) {}

 public:
  basic_format_args() : desc_(0) {}

  /**
   \rst
   Constructs a `basic_format_args` object from `~fmt::format_arg_store`.
   \endrst
   */
  template <typename... Args>
  FMT_INLINE basic_format_args(const format_arg_store<Context, Args...>& store)
	  : basic_format_args(store.desc, store.data_.args()) {}

  /**
   \rst
   Constructs a `basic_format_args` object from
   `~fmt::dynamic_format_arg_store`.
   \endrst
   */
  FMT_INLINE basic_format_args(const dynamic_format_arg_store<Context>& store)
	  : basic_format_args(store.get_types(), store.data()) {}

  /**
   \rst
   Constructs a `basic_format_args` object from a dynamic set of arguments.
   \endrst
   */
  basic_format_args(const format_arg* args, int count)
	  : basic_format_args(detail::is_unpacked_bit | detail::to_unsigned(count),
						  args) {}

  /** Returns the argument with the specified id. */
  format_arg get(int id) const {
	format_arg arg;
	if (!is_packed()) {
	  if (id < max_size()) arg = args_[id];
	  return arg;
	}
	if (id >= detail::max_packed_args) return arg;
	arg.type_ = type(id);
	if (arg.type_ == detail::type::none_type) return arg;
	arg.value_ = values_[id];
	return arg;
  }

  template <typename Char> format_arg get(basic_string_view<Char> name) const {
	int id = get_id(name);
	return id >= 0 ? get(id) : format_arg();
  }

  template <typename Char> int get_id(basic_string_view<Char> name) const {
	if (!has_named_args()) return -1;
	const auto& named_args =
		(is_packed() ? values_[-1] : args_[-1].value_).named_args;
	for (size_t i = 0; i < named_args.size; ++i) {
	  if (named_args.data[i].name == name) return named_args.data[i].id;
	}
	return -1;
  }

  int max_size() const {
	unsigned long long max_packed = detail::max_packed_args;
	return static_cast<int>(is_packed() ? max_packed
										: desc_ & ~detail::is_unpacked_bit);
  }
};

#ifdef FMT_ARM_ABI_COMPATIBILITY
/** An alias to ``basic_format_args<format_context>``. */
// Separate types would result in shorter symbols but break ABI compatibility
// between clang and gcc on ARM (#1919).
using format_args = basic_format_args<format_context>;
using wformat_args = basic_format_args<wformat_context>;
#else
// DEPRECATED! These are kept for ABI compatibility.
// It is a separate type rather than an alias to make symbols readable.
struct format_args : basic_format_args<format_context> {
  template <typename... Args>
  FMT_INLINE format_args(const Args&... args) : basic_format_args(args...) {}
};
struct wformat_args : basic_format_args<wformat_context> {
  using basic_format_args::basic_format_args;
};
#endif

namespace detail {

template <typename Char, FMT_ENABLE_IF(!std::is_same<Char, char>::value)>
std::basic_string<Char> vformat(
	basic_string_view<Char> format_str,
	basic_format_args<buffer_context<type_identity_t<Char>>> args);

FMT_API std::string vformat(string_view format_str, format_args args);

template <typename Char>
void vformat_to(
	buffer<Char>& buf, basic_string_view<Char> format_str,
	basic_format_args<FMT_BUFFER_CONTEXT(type_identity_t<Char>)> args,
	detail::locale_ref loc = {});

template <typename Char, typename Args,
		  FMT_ENABLE_IF(!std::is_same<Char, char>::value)>
inline void vprint_mojibake(std::FILE*, basic_string_view<Char>, const Args&) {}

FMT_API void vprint_mojibake(std::FILE*, string_view, format_args);
#ifndef _WIN32
inline void vprint_mojibake(std::FILE*, string_view, format_args) {}
#endif
}  // namespace detail

/** Formats a string and writes the output to ``out``. */
// GCC 8 and earlier cannot handle std::back_insert_iterator<Container> with
// vformat_to<ArgFormatter>(...) overload, so SFINAE on iterator type instead.
template <typename OutputIt, typename S, typename Char = char_t<S>,
		  bool enable = detail::is_output_iterator<OutputIt, Char>::value>
auto vformat_to(OutputIt out, const S& format_str,
				basic_format_args<buffer_context<type_identity_t<Char>>> args)
	-> typename std::enable_if<enable, OutputIt>::type {
  decltype(detail::get_buffer<Char>(out)) buf(detail::get_buffer_init(out));
  detail::vformat_to(buf, to_string_view(format_str), args);
  return detail::get_iterator(buf);
}

/**
 \rst
 Formats arguments, writes the result to the output iterator ``out`` and returns
 the iterator past the end of the output range.

 **Example**::

   std::vector<char> out;
   fmt::format_to(std::back_inserter(out), "{}", 42);
 \endrst
 */
// We cannot use FMT_ENABLE_IF because of a bug in gcc 8.3.
template <typename OutputIt, typename S, typename... Args,
		  bool enable = detail::is_output_iterator<OutputIt, char_t<S>>::value>
inline auto format_to(OutputIt out, const S& format_str, Args&&... args) ->
	typename std::enable_if<enable, OutputIt>::type {
  const auto& vargs = fmt::make_args_checked<Args...>(format_str, args...);
  return vformat_to(out, to_string_view(format_str), vargs);
}

template <typename OutputIt> struct format_to_n_result {
  /** Iterator past the end of the output range. */
  OutputIt out;
  /** Total (not truncated) output size. */
  size_t size;
};

template <typename OutputIt, typename Char, typename... Args,
		  FMT_ENABLE_IF(detail::is_output_iterator<OutputIt, Char>::value)>
inline format_to_n_result<OutputIt> vformat_to_n(
	OutputIt out, size_t n, basic_string_view<Char> format_str,
	basic_format_args<buffer_context<type_identity_t<Char>>> args) {
  detail::iterator_buffer<OutputIt, Char, detail::fixed_buffer_traits> buf(out,
																		   n);
  detail::vformat_to(buf, format_str, args);
  return {buf.out(), buf.count()};
}

/**
 \rst
 Formats arguments, writes up to ``n`` characters of the result to the output
 iterator ``out`` and returns the total output size and the iterator past the
 end of the output range.
 \endrst
 */
template <typename OutputIt, typename S, typename... Args,
		  bool enable = detail::is_output_iterator<OutputIt, char_t<S>>::value>
inline auto format_to_n(OutputIt out, size_t n, const S& format_str,
						const Args&... args) ->
	typename std::enable_if<enable, format_to_n_result<OutputIt>>::type {
  const auto& vargs = fmt::make_args_checked<Args...>(format_str, args...);
  return vformat_to_n(out, n, to_string_view(format_str), vargs);
}

/**
  Returns the number of characters in the output of
  ``format(format_str, args...)``.
 */
template <typename... Args>
inline size_t formatted_size(string_view format_str, Args&&... args) {
  const auto& vargs = fmt::make_args_checked<Args...>(format_str, args...);
  detail::counting_buffer<> buf;
  detail::vformat_to(buf, format_str, vargs);
  return buf.count();
}

template <typename S, typename Char = char_t<S>>
FMT_INLINE std::basic_string<Char> vformat(
	const S& format_str,
	basic_format_args<buffer_context<type_identity_t<Char>>> args) {
  return detail::vformat(to_string_view(format_str), args);
}

/**
  \rst
  Formats arguments and returns the result as a string.

  **Example**::

	#include <dmcore.h>
	std::string message = fmt::format("The answer is {}", 42);
  \endrst
*/
// Pass char_t as a default template parameter instead of using
// std::basic_string<char_t<S>> to reduce the symbol size.
template <typename S, typename... Args, typename Char = char_t<S>>
FMT_INLINE std::basic_string<Char> format(const S& format_str, Args&&... args) {
  const auto& vargs = fmt::make_args_checked<Args...>(format_str, args...);
  return detail::vformat(to_string_view(format_str), vargs);
}

FMT_API void vprint(string_view, format_args);
FMT_API void vprint(std::FILE*, string_view, format_args);

/**
  \rst
  Formats ``args`` according to specifications in ``format_str`` and writes the
  output to the file ``f``. Strings are assumed to be Unicode-encoded unless the
  ``FMT_UNICODE`` macro is set to 0.

  **Example**::

	fmt::print(stderr, "Don't {}!", "panic");
  \endrst
 */
template <typename S, typename... Args, typename Char = char_t<S>>
inline void print(std::FILE* f, const S& format_str, Args&&... args) {
  const auto& vargs = fmt::make_args_checked<Args...>(format_str, args...);
  return detail::is_unicode<Char>()
			 ? vprint(f, to_string_view(format_str), vargs)
			 : detail::vprint_mojibake(f, to_string_view(format_str), vargs);
}

/**
  \rst
  Formats ``args`` according to specifications in ``format_str`` and writes
  the output to ``stdout``. Strings are assumed to be Unicode-encoded unless
  the ``FMT_UNICODE`` macro is set to 0.

  **Example**::

	fmt::print("Elapsed time: {0:.2f} seconds", 1.23);
  \endrst
 */
template <typename S, typename... Args, typename Char = char_t<S>>
inline void print(const S& format_str, Args&&... args) {
  const auto& vargs = fmt::make_args_checked<Args...>(format_str, args...);
  return detail::is_unicode<Char>()
			 ? vprint(to_string_view(format_str), vargs)
			 : detail::vprint_mojibake(stdout, to_string_view(format_str),
									   vargs);
}
FMT_END_NAMESPACE

#endif  // FMT_CORE_H_

/*** End of inlined file: dmcore.h ***/

#ifdef __INTEL_COMPILER
#  define FMT_ICC_VERSION __INTEL_COMPILER
#elif defined(__ICL)
#  define FMT_ICC_VERSION __ICL
#else
#  define FMT_ICC_VERSION 0
#endif

#ifdef __NVCC__
#  define FMT_CUDA_VERSION (__CUDACC_VER_MAJOR__ * 100 + __CUDACC_VER_MINOR__)
#else
#  define FMT_CUDA_VERSION 0
#endif

#ifdef __has_builtin
#  define FMT_HAS_BUILTIN(x) __has_builtin(x)
#else
#  define FMT_HAS_BUILTIN(x) 0
#endif

#if FMT_GCC_VERSION || FMT_CLANG_VERSION
#  define FMT_NOINLINE __attribute__((noinline))
#else
#  define FMT_NOINLINE
#endif

#if __cplusplus == 201103L || __cplusplus == 201402L
#  if defined(__INTEL_COMPILER) || defined(__PGI)
#    define FMT_FALLTHROUGH
#  elif defined(__clang__)
#    define FMT_FALLTHROUGH [[clang::fallthrough]]
#  elif FMT_GCC_VERSION >= 700 && \
	  (!defined(__EDG_VERSION__) || __EDG_VERSION__ >= 520)
#    define FMT_FALLTHROUGH [[gnu::fallthrough]]
#  else
#    define FMT_FALLTHROUGH
#  endif
#elif FMT_HAS_CPP17_ATTRIBUTE(fallthrough) || \
	(defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)
#  define FMT_FALLTHROUGH [[fallthrough]]
#else
#  define FMT_FALLTHROUGH
#endif

#ifndef FMT_MAYBE_UNUSED
#  if FMT_HAS_CPP17_ATTRIBUTE(maybe_unused)
#    define FMT_MAYBE_UNUSED [[maybe_unused]]
#  else
#    define FMT_MAYBE_UNUSED
#  endif
#endif

#ifndef FMT_THROW
#  if FMT_EXCEPTIONS
#    if FMT_MSC_VER || FMT_NVCC
FMT_BEGIN_NAMESPACE
namespace detail {
template <typename Exception> inline void do_throw(const Exception& x) {
  // Silence unreachable code warnings in MSVC and NVCC because these
  // are nearly impossible to fix in a generic code.
  volatile bool b = true;
  if (b) throw x;
}
}  // namespace detail
FMT_END_NAMESPACE
#      define FMT_THROW(x) detail::do_throw(x)
#    else
#      define FMT_THROW(x) throw x
#    endif
#  else
#    define FMT_THROW(x)              \
	  do {                            \
		static_cast<void>(sizeof(x)); \
		FMT_ASSERT(false, "");        \
	  } while (false)
#  endif
#endif

#if FMT_EXCEPTIONS
#  define FMT_TRY try
#  define FMT_CATCH(x) catch (x)
#else
#  define FMT_TRY if (true)
#  define FMT_CATCH(x) if (false)
#endif

#ifndef FMT_USE_USER_DEFINED_LITERALS
// EDG based compilers (Intel, NVIDIA, Elbrus, etc), GCC and MSVC support UDLs.
#  if (FMT_HAS_FEATURE(cxx_user_literals) || FMT_GCC_VERSION >= 407 || \
	   FMT_MSC_VER >= 1900) &&                                         \
	  (!defined(__EDG_VERSION__) || __EDG_VERSION__ >= /* UDL feature */ 480)
#    define FMT_USE_USER_DEFINED_LITERALS 1
#  else
#    define FMT_USE_USER_DEFINED_LITERALS 0
#  endif
#endif

#ifndef FMT_USE_UDL_TEMPLATE
// EDG frontend based compilers (icc, nvcc, PGI, etc) and GCC < 6.4 do not
// properly support UDL templates and GCC >= 9 warns about them.
#  if FMT_USE_USER_DEFINED_LITERALS &&                         \
	  (!defined(__EDG_VERSION__) || __EDG_VERSION__ >= 501) && \
	  ((FMT_GCC_VERSION >= 604 && __cplusplus >= 201402L) ||   \
	   FMT_CLANG_VERSION >= 304) &&                            \
	  !defined(__PGI) && !defined(__NVCC__)
#    define FMT_USE_UDL_TEMPLATE 1
#  else
#    define FMT_USE_UDL_TEMPLATE 0
#  endif
#endif

#ifndef FMT_USE_FLOAT
#  define FMT_USE_FLOAT 1
#endif

#ifndef FMT_USE_DOUBLE
#  define FMT_USE_DOUBLE 1
#endif

#ifndef FMT_USE_LONG_DOUBLE
#  define FMT_USE_LONG_DOUBLE 1
#endif

// Defining FMT_REDUCE_INT_INSTANTIATIONS to 1, will reduce the number of
// int_writer template instances to just one by only using the largest integer
// type. This results in a reduction in binary size but will cause a decrease in
// integer formatting performance.
#if !defined(FMT_REDUCE_INT_INSTANTIATIONS)
#  define FMT_REDUCE_INT_INSTANTIATIONS 0
#endif

// __builtin_clz is broken in clang with Microsoft CodeGen:
// https://github.com/fmtlib/dmissues/519
#if (FMT_GCC_VERSION || FMT_HAS_BUILTIN(__builtin_clz)) && !FMT_MSC_VER
#  define FMT_BUILTIN_CLZ(n) __builtin_clz(n)
#endif
#if (FMT_GCC_VERSION || FMT_HAS_BUILTIN(__builtin_clzll)) && !FMT_MSC_VER
#  define FMT_BUILTIN_CLZLL(n) __builtin_clzll(n)
#endif
#if (FMT_GCC_VERSION || FMT_HAS_BUILTIN(__builtin_ctz))
#  define FMT_BUILTIN_CTZ(n) __builtin_ctz(n)
#endif
#if (FMT_GCC_VERSION || FMT_HAS_BUILTIN(__builtin_ctzll))
#  define FMT_BUILTIN_CTZLL(n) __builtin_ctzll(n)
#endif

#if FMT_MSC_VER
#  include <intrin.h>  // _BitScanReverse[64], _BitScanForward[64], _umul128
#endif

// Some compilers masquerade as both MSVC and GCC-likes or otherwise support
// __builtin_clz and __builtin_clzll, so only define FMT_BUILTIN_CLZ using the
// MSVC intrinsics if the clz and clzll builtins are not available.
#if FMT_MSC_VER && !defined(FMT_BUILTIN_CLZLL) && \
	!defined(FMT_BUILTIN_CTZLL) && !defined(_MANAGED)
FMT_BEGIN_NAMESPACE
namespace detail {
// Avoid Clang with Microsoft CodeGen's -Wunknown-pragmas warning.
#  ifndef __clang__
#    pragma intrinsic(_BitScanForward)
#    pragma intrinsic(_BitScanReverse)
#  endif
#  if defined(_WIN64) && !defined(__clang__)
#    pragma intrinsic(_BitScanForward64)
#    pragma intrinsic(_BitScanReverse64)
#  endif

inline int clz(uint32_t x) {
  unsigned long r = 0;
  _BitScanReverse(&r, x);
  FMT_ASSERT(x != 0, "");
  // Static analysis complains about using uninitialized data
  // "r", but the only way that can happen is if "x" is 0,
  // which the callers guarantee to not happen.
  FMT_SUPPRESS_MSC_WARNING(6102)
  return 31 ^ static_cast<int>(r);
}
#  define FMT_BUILTIN_CLZ(n) detail::clz(n)

inline int clzll(uint64_t x) {
  unsigned long r = 0;
#  ifdef _WIN64
  _BitScanReverse64(&r, x);
#  else
  // Scan the high 32 bits.
  if (_BitScanReverse(&r, static_cast<uint32_t>(x >> 32))) return 63 ^ (r + 32);
  // Scan the low 32 bits.
  _BitScanReverse(&r, static_cast<uint32_t>(x));
#  endif
  FMT_ASSERT(x != 0, "");
  FMT_SUPPRESS_MSC_WARNING(6102)  // Suppress a bogus static analysis warning.
  return 63 ^ static_cast<int>(r);
}
#  define FMT_BUILTIN_CLZLL(n) detail::clzll(n)

inline int ctz(uint32_t x) {
  unsigned long r = 0;
  _BitScanForward(&r, x);
  FMT_ASSERT(x != 0, "");
  FMT_SUPPRESS_MSC_WARNING(6102)  // Suppress a bogus static analysis warning.
  return static_cast<int>(r);
}
#  define FMT_BUILTIN_CTZ(n) detail::ctz(n)

inline int ctzll(uint64_t x) {
  unsigned long r = 0;
  FMT_ASSERT(x != 0, "");
  FMT_SUPPRESS_MSC_WARNING(6102)  // Suppress a bogus static analysis warning.
#  ifdef _WIN64
  _BitScanForward64(&r, x);
#  else
  // Scan the low 32 bits.
  if (_BitScanForward(&r, static_cast<uint32_t>(x))) return static_cast<int>(r);
  // Scan the high 32 bits.
  _BitScanForward(&r, static_cast<uint32_t>(x >> 32));
  r += 32;
#  endif
  return static_cast<int>(r);
}
#  define FMT_BUILTIN_CTZLL(n) detail::ctzll(n)
}  // namespace detail
FMT_END_NAMESPACE
#endif

// Enable the deprecated numeric alignment.
#ifndef FMT_DEPRECATED_NUMERIC_ALIGN
#  define FMT_DEPRECATED_NUMERIC_ALIGN 0
#endif

FMT_BEGIN_NAMESPACE
namespace detail {

// An equivalent of `*reinterpret_cast<Dest*>(&source)` that doesn't have
// undefined behavior (e.g. due to type aliasing).
// Example: uint64_t d = bit_cast<uint64_t>(2.718);
template <typename Dest, typename Source>
inline Dest bit_cast(const Source& source) {
  static_assert(sizeof(Dest) == sizeof(Source), "size mismatch");
  Dest dest;
  std::memcpy(&dest, &source, sizeof(dest));
  return dest;
}

inline bool is_big_endian() {
  const auto u = 1u;
  struct bytes {
	char data[sizeof(u)];
  };
  return bit_cast<bytes>(u).data[0] == 0;
}

// A fallback implementation of uintptr_t for systems that lack it.
struct fallback_uintptr {
  unsigned char value[sizeof(void*)];

  fallback_uintptr() = default;
  explicit fallback_uintptr(const void* p) {
	*this = bit_cast<fallback_uintptr>(p);
	if (is_big_endian()) {
	  for (size_t i = 0, j = sizeof(void*) - 1; i < j; ++i, --j)
		std::swap(value[i], value[j]);
	}
  }
};
#ifdef UINTPTR_MAX
using uintptr_t = ::uintptr_t;
inline uintptr_t to_uintptr(const void* p) { return bit_cast<uintptr_t>(p); }
#else
using uintptr_t = fallback_uintptr;
inline fallback_uintptr to_uintptr(const void* p) {
  return fallback_uintptr(p);
}
#endif

// Returns the largest possible value for type T. Same as
// std::numeric_limits<T>::max() but shorter and not affected by the max macro.
template <typename T> constexpr T max_value() {
  return (std::numeric_limits<T>::max)();
}
template <typename T> constexpr int num_bits() {
  return std::numeric_limits<T>::digits;
}
// std::numeric_limits<T>::digits may return 0 for 128-bit ints.
template <> constexpr int num_bits<int128_t>() { return 128; }
template <> constexpr int num_bits<uint128_t>() { return 128; }
template <> constexpr int num_bits<fallback_uintptr>() {
  return static_cast<int>(sizeof(void*) *
						  std::numeric_limits<unsigned char>::digits);
}

FMT_INLINE void assume(bool condition) {
  (void)condition;
#if FMT_HAS_BUILTIN(__builtin_assume)
  __builtin_assume(condition);
#endif
}

// An approximation of iterator_t for pre-C++20 systems.
template <typename T>
using iterator_t = decltype(std::begin(std::declval<T&>()));
template <typename T> using sentinel_t = decltype(std::end(std::declval<T&>()));

// A workaround for std::string not having mutable data() until C++17.
template <typename Char> inline Char* get_data(std::basic_string<Char>& s) {
  return &s[0];
}
template <typename Container>
inline typename Container::value_type* get_data(Container& c) {
  return c.data();
}

#if defined(_SECURE_SCL) && _SECURE_SCL
// Make a checked iterator to avoid MSVC warnings.
template <typename T> using checked_ptr = stdext::checked_array_iterator<T*>;
template <typename T> checked_ptr<T> make_checked(T* p, size_t size) {
  return {p, size};
}
#else
template <typename T> using checked_ptr = T*;
template <typename T> inline T* make_checked(T* p, size_t) { return p; }
#endif

template <typename Container, FMT_ENABLE_IF(is_contiguous<Container>::value)>
#if FMT_CLANG_VERSION
__attribute__((no_sanitize("undefined")))
#endif
inline checked_ptr<typename Container::value_type>
reserve(std::back_insert_iterator<Container> it, size_t n) {
  Container& c = get_container(it);
  size_t size = c.size();
  c.resize(size + n);
  return make_checked(get_data(c) + size, n);
}

template <typename T>
inline buffer_appender<T> reserve(buffer_appender<T> it, size_t n) {
  buffer<T>& buf = get_container(it);
  buf.try_reserve(buf.size() + n);
  return it;
}

template <typename Iterator> inline Iterator& reserve(Iterator& it, size_t) {
  return it;
}

template <typename T, typename OutputIt>
constexpr T* to_pointer(OutputIt, size_t) {
  return nullptr;
}
template <typename T> T* to_pointer(buffer_appender<T> it, size_t n) {
  buffer<T>& buf = get_container(it);
  auto size = buf.size();
  if (buf.capacity() < size + n) return nullptr;
  buf.try_resize(size + n);
  return buf.data() + size;
}

template <typename Container, FMT_ENABLE_IF(is_contiguous<Container>::value)>
inline std::back_insert_iterator<Container> base_iterator(
	std::back_insert_iterator<Container>& it,
	checked_ptr<typename Container::value_type>) {
  return it;
}

template <typename Iterator>
inline Iterator base_iterator(Iterator, Iterator it) {
  return it;
}

// An output iterator that counts the number of objects written to it and
// discards them.
class counting_iterator {
 private:
  size_t count_;

 public:
  using iterator_category = std::output_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using pointer = void;
  using reference = void;
  using _Unchecked_type = counting_iterator;  // Mark iterator as checked.

  struct value_type {
	template <typename T> void operator=(const T&) {}
  };

  counting_iterator() : count_(0) {}

  size_t count() const { return count_; }

  counting_iterator& operator++() {
	++count_;
	return *this;
  }
  counting_iterator operator++(int) {
	auto it = *this;
	++*this;
	return it;
  }

  friend counting_iterator operator+(counting_iterator it, difference_type n) {
	it.count_ += static_cast<size_t>(n);
	return it;
  }

  value_type operator*() const { return {}; }
};

template <typename OutputIt> class truncating_iterator_base {
 protected:
  OutputIt out_;
  size_t limit_;
  size_t count_;

  truncating_iterator_base(OutputIt out, size_t limit)
	  : out_(out), limit_(limit), count_(0) {}

 public:
  using iterator_category = std::output_iterator_tag;
  using value_type = typename std::iterator_traits<OutputIt>::value_type;
  using difference_type = void;
  using pointer = void;
  using reference = void;
  using _Unchecked_type =
	  truncating_iterator_base;  // Mark iterator as checked.

  OutputIt base() const { return out_; }
  size_t count() const { return count_; }
};

// An output iterator that truncates the output and counts the number of objects
// written to it.
template <typename OutputIt,
		  typename Enable = typename std::is_void<
			  typename std::iterator_traits<OutputIt>::value_type>::type>
class truncating_iterator;

template <typename OutputIt>
class truncating_iterator<OutputIt, std::false_type>
	: public truncating_iterator_base<OutputIt> {
  mutable typename truncating_iterator_base<OutputIt>::value_type blackhole_;

 public:
  using value_type = typename truncating_iterator_base<OutputIt>::value_type;

  truncating_iterator(OutputIt out, size_t limit)
	  : truncating_iterator_base<OutputIt>(out, limit) {}

  truncating_iterator& operator++() {
	if (this->count_++ < this->limit_) ++this->out_;
	return *this;
  }

  truncating_iterator operator++(int) {
	auto it = *this;
	++*this;
	return it;
  }

  value_type& operator*() const {
	return this->count_ < this->limit_ ? *this->out_ : blackhole_;
  }
};

template <typename OutputIt>
class truncating_iterator<OutputIt, std::true_type>
	: public truncating_iterator_base<OutputIt> {
 public:
  truncating_iterator(OutputIt out, size_t limit)
	  : truncating_iterator_base<OutputIt>(out, limit) {}

  template <typename T> truncating_iterator& operator=(T val) {
	if (this->count_++ < this->limit_) *this->out_++ = val;
	return *this;
  }

  truncating_iterator& operator++() { return *this; }
  truncating_iterator& operator++(int) { return *this; }
  truncating_iterator& operator*() { return *this; }
};

template <typename Char>
inline size_t count_code_points(basic_string_view<Char> s) {
  return s.size();
}

// Counts the number of code points in a UTF-8 string.
inline size_t count_code_points(basic_string_view<char> s) {
  const char* data = s.data();
  size_t num_code_points = 0;
  for (size_t i = 0, size = s.size(); i != size; ++i) {
	if ((data[i] & 0xc0) != 0x80) ++num_code_points;
  }
  return num_code_points;
}

inline size_t count_code_points(basic_string_view<char8_type> s) {
  return count_code_points(basic_string_view<char>(
	  reinterpret_cast<const char*>(s.data()), s.size()));
}

template <typename Char>
inline size_t code_point_index(basic_string_view<Char> s, size_t n) {
  size_t size = s.size();
  return n < size ? n : size;
}

// Calculates the index of the nth code point in a UTF-8 string.
inline size_t code_point_index(basic_string_view<char8_type> s, size_t n) {
  const char8_type* data = s.data();
  size_t num_code_points = 0;
  for (size_t i = 0, size = s.size(); i != size; ++i) {
	if ((data[i] & 0xc0) != 0x80 && ++num_code_points > n) {
	  return i;
	}
  }
  return s.size();
}

template <typename InputIt, typename OutChar>
using needs_conversion = bool_constant<
	std::is_same<typename std::iterator_traits<InputIt>::value_type,
				 char>::value &&
	std::is_same<OutChar, char8_type>::value>;

template <typename OutChar, typename InputIt, typename OutputIt,
		  FMT_ENABLE_IF(!needs_conversion<InputIt, OutChar>::value)>
OutputIt copy_str(InputIt begin, InputIt end, OutputIt it) {
  return std::copy(begin, end, it);
}

template <typename OutChar, typename InputIt, typename OutputIt,
		  FMT_ENABLE_IF(needs_conversion<InputIt, OutChar>::value)>
OutputIt copy_str(InputIt begin, InputIt end, OutputIt it) {
  return std::transform(begin, end, it,
						[](char c) { return static_cast<char8_type>(c); });
}

template <typename Char, typename InputIt>
inline counting_iterator copy_str(InputIt begin, InputIt end,
								  counting_iterator it) {
  return it + (end - begin);
}

template <typename T>
using is_fast_float = bool_constant<std::numeric_limits<T>::is_iec559 &&
									sizeof(T) <= sizeof(double)>;

#ifndef FMT_USE_FULL_CACHE_DRAGONBOX
#  define FMT_USE_FULL_CACHE_DRAGONBOX 0
#endif

template <typename T>
template <typename U>
void buffer<T>::append(const U* begin, const U* end) {
  do {
	auto count = to_unsigned(end - begin);
	try_reserve(size_ + count);
	auto free_cap = capacity_ - size_;
	if (free_cap < count) count = free_cap;
	std::uninitialized_copy_n(begin, count, make_checked(ptr_ + size_, count));
	size_ += count;
	begin += count;
  } while (begin != end);
}

template <typename OutputIt, typename T, typename Traits>
void iterator_buffer<OutputIt, T, Traits>::flush() {
  out_ = std::copy_n(data_, this->limit(this->size()), out_);
  this->clear();
}
}  // namespace detail

// The number of characters to store in the basic_memory_buffer object itself
// to avoid dynamic memory allocation.
enum { inline_buffer_size = 500 };

/**
  \rst
  A dynamically growing memory buffer for trivially copyable/constructible types
  with the first ``SIZE`` elements stored in the object itself.

  You can use one of the following type aliases for common character types:

  +----------------+------------------------------+
  | Type           | Definition                   |
  +================+==============================+
  | memory_buffer  | basic_memory_buffer<char>    |
  +----------------+------------------------------+
  | wmemory_buffer | basic_memory_buffer<wchar_t> |
  +----------------+------------------------------+

  **Example**::

	 fmt::memory_buffer out;
	 format_to(out, "The answer is {}.", 42);

  This will append the following output to the ``out`` object:

  .. code-block:: none

	 The answer is 42.

  The output can be converted to an ``std::string`` with ``to_string(out)``.
  \endrst
 */
template <typename T, size_t SIZE = inline_buffer_size,
		  typename Allocator = std::allocator<T>>
class basic_memory_buffer final : public detail::buffer<T> {
 private:
  T store_[SIZE];

  // Don't inherit from Allocator avoid generating type_info for it.
  Allocator alloc_;

  // Deallocate memory allocated by the buffer.
  void deallocate() {
	T* data = this->data();
	if (data != store_) alloc_.deallocate(data, this->capacity());
  }

 protected:
  void grow(size_t size) final FMT_OVERRIDE;

 public:
  using value_type = T;
  using const_reference = const T&;

  explicit basic_memory_buffer(const Allocator& alloc = Allocator())
	  : alloc_(alloc) {
	this->set(store_, SIZE);
  }
  ~basic_memory_buffer() { deallocate(); }

 private:
  // Move data from other to this buffer.
  void move(basic_memory_buffer& other) {
	alloc_ = std::move(other.alloc_);
	T* data = other.data();
	size_t size = other.size(), capacity = other.capacity();
	if (data == other.store_) {
	  this->set(store_, capacity);
	  std::uninitialized_copy(other.store_, other.store_ + size,
							  detail::make_checked(store_, capacity));
	} else {
	  this->set(data, capacity);
	  // Set pointer to the inline array so that delete is not called
	  // when deallocating.
	  other.set(other.store_, 0);
	}
	this->resize(size);
  }

 public:
  /**
	\rst
	Constructs a :class:`fmt::basic_memory_buffer` object moving the content
	of the other object to it.
	\endrst
   */
  basic_memory_buffer(basic_memory_buffer&& other) FMT_NOEXCEPT { move(other); }

  /**
	\rst
	Moves the content of the other ``basic_memory_buffer`` object to this one.
	\endrst
   */
  basic_memory_buffer& operator=(basic_memory_buffer&& other) FMT_NOEXCEPT {
	FMT_ASSERT(this != &other, "");
	deallocate();
	move(other);
	return *this;
  }

  // Returns a copy of the allocator associated with this buffer.
  Allocator get_allocator() const { return alloc_; }

  /**
	Resizes the buffer to contain *count* elements. If T is a POD type new
	elements may not be initialized.
   */
  void resize(size_t count) { this->try_resize(count); }

  /** Increases the buffer capacity to *new_capacity*. */
  void reserve(size_t new_capacity) { this->try_reserve(new_capacity); }

  // Directly append data into the buffer
  using detail::buffer<T>::append;
  template <typename ContiguousRange>
  void append(const ContiguousRange& range) {
	append(range.data(), range.data() + range.size());
  }
};

template <typename T, size_t SIZE, typename Allocator>
void basic_memory_buffer<T, SIZE, Allocator>::grow(size_t size) {
#ifdef FMT_FUZZ
  if (size > 5000) throw std::runtime_error("fuzz mode - won't grow that much");
#endif
  size_t old_capacity = this->capacity();
  size_t new_capacity = old_capacity + old_capacity / 2;
  if (size > new_capacity) new_capacity = size;
  T* old_data = this->data();
  T* new_data =
	  std::allocator_traits<Allocator>::allocate(alloc_, new_capacity);
  // The following code doesn't throw, so the raw pointer above doesn't leak.
  std::uninitialized_copy(old_data, old_data + this->size(),
						  detail::make_checked(new_data, new_capacity));
  this->set(new_data, new_capacity);
  // deallocate must not throw according to the standard, but even if it does,
  // the buffer already uses the new storage and will deallocate it in
  // destructor.
  if (old_data != store_) alloc_.deallocate(old_data, old_capacity);
}

using memory_buffer = basic_memory_buffer<char>;
using wmemory_buffer = basic_memory_buffer<wchar_t>;

template <typename T, size_t SIZE, typename Allocator>
struct is_contiguous<basic_memory_buffer<T, SIZE, Allocator>> : std::true_type {
};

/** A formatting error such as invalid format string. */
FMT_CLASS_API
class FMT_API format_error : public std::runtime_error {
 public:
  explicit format_error(const char* message) : std::runtime_error(message) {}
  explicit format_error(const std::string& message)
	  : std::runtime_error(message) {}
  format_error(const format_error&) = default;
  format_error& operator=(const format_error&) = default;
  format_error(format_error&&) = default;
  format_error& operator=(format_error&&) = default;
  ~format_error() FMT_NOEXCEPT FMT_OVERRIDE;
};

namespace detail {

template <typename T>
using is_signed =
	std::integral_constant<bool, std::numeric_limits<T>::is_signed ||
									 std::is_same<T, int128_t>::value>;

// Returns true if value is negative, false otherwise.
// Same as `value < 0` but doesn't produce warnings if T is an unsigned type.
template <typename T, FMT_ENABLE_IF(is_signed<T>::value)>
FMT_CONSTEXPR bool is_negative(T value) {
  return value < 0;
}
template <typename T, FMT_ENABLE_IF(!is_signed<T>::value)>
FMT_CONSTEXPR bool is_negative(T) {
  return false;
}

template <typename T, FMT_ENABLE_IF(std::is_floating_point<T>::value)>
FMT_CONSTEXPR bool is_supported_floating_point(T) {
  return (std::is_same<T, float>::value && FMT_USE_FLOAT) ||
		 (std::is_same<T, double>::value && FMT_USE_DOUBLE) ||
		 (std::is_same<T, long double>::value && FMT_USE_LONG_DOUBLE);
}

// Smallest of uint32_t, uint64_t, uint128_t that is large enough to
// represent all values of an integral type T.
template <typename T>
using uint32_or_64_or_128_t =
	conditional_t<num_bits<T>() <= 32 && !FMT_REDUCE_INT_INSTANTIATIONS,
				  uint32_t,
				  conditional_t<num_bits<T>() <= 64, uint64_t, uint128_t>>;

// 128-bit integer type used internally
struct FMT_EXTERN_TEMPLATE_API uint128_wrapper {
  uint128_wrapper() = default;

#if FMT_USE_INT128
  uint128_t internal_;

  uint128_wrapper(uint64_t high, uint64_t low) FMT_NOEXCEPT
	  : internal_{static_cast<uint128_t>(low) |
				  (static_cast<uint128_t>(high) << 64)} {}

  uint128_wrapper(uint128_t u) : internal_{u} {}

  uint64_t high() const FMT_NOEXCEPT { return uint64_t(internal_ >> 64); }
  uint64_t low() const FMT_NOEXCEPT { return uint64_t(internal_); }

  uint128_wrapper& operator+=(uint64_t n) FMT_NOEXCEPT {
	internal_ += n;
	return *this;
  }
#else
  uint64_t high_;
  uint64_t low_;

  uint128_wrapper(uint64_t high, uint64_t low) FMT_NOEXCEPT : high_{high},
															  low_{low} {}

  uint64_t high() const FMT_NOEXCEPT { return high_; }
  uint64_t low() const FMT_NOEXCEPT { return low_; }

  uint128_wrapper& operator+=(uint64_t n) FMT_NOEXCEPT {
#  if defined(_MSC_VER) && defined(_M_X64)
	unsigned char carry = _addcarry_u64(0, low_, n, &low_);
	_addcarry_u64(carry, high_, 0, &high_);
	return *this;
#  else
	uint64_t sum = low_ + n;
	high_ += (sum < low_ ? 1 : 0);
	low_ = sum;
	return *this;
#  endif
  }
#endif
};

// Table entry type for divisibility test used internally
template <typename T> struct FMT_EXTERN_TEMPLATE_API divtest_table_entry {
  T mod_inv;
  T max_quotient;
};

// Static data is placed in this class template for the header-only config.
template <typename T = void> struct FMT_EXTERN_TEMPLATE_API basic_data {
  static const uint64_t powers_of_10_64[];
  static const uint32_t zero_or_powers_of_10_32_new[];
  static const uint64_t zero_or_powers_of_10_64_new[];
  static const uint64_t grisu_pow10_significands[];
  static const int16_t grisu_pow10_exponents[];
  static const divtest_table_entry<uint32_t> divtest_table_for_pow5_32[];
  static const divtest_table_entry<uint64_t> divtest_table_for_pow5_64[];
  static const uint64_t dragonbox_pow10_significands_64[];
  static const uint128_wrapper dragonbox_pow10_significands_128[];
  // log10(2) = 0x0.4d104d427de7fbcc...
  static const uint64_t log10_2_significand = 0x4d104d427de7fbcc;
#if !FMT_USE_FULL_CACHE_DRAGONBOX
  static const uint64_t powers_of_5_64[];
  static const uint32_t dragonbox_pow10_recovery_errors[];
#endif
  // GCC generates slightly better code for pairs than chars.
  using digit_pair = char[2];
  static const digit_pair digits[];
  static const char hex_digits[];
  static const char foreground_color[];
  static const char background_color[];
  static const char reset_color[5];
  static const wchar_t wreset_color[5];
  static const char signs[];
  static const char left_padding_shifts[5];
  static const char right_padding_shifts[5];

  // DEPRECATED! These are for ABI compatibility.
  static const uint32_t zero_or_powers_of_10_32[];
  static const uint64_t zero_or_powers_of_10_64[];
};

// Maps bsr(n) to ceil(log10(pow(2, bsr(n) + 1) - 1)).
// This is a function instead of an array to workaround a bug in GCC10 (#1810).
FMT_INLINE uint16_t bsr2log10(int bsr) {
  static constexpr uint16_t data[] = {
	  1,  1,  1,  2,  2,  2,  3,  3,  3,  4,  4,  4,  4,  5,  5,  5,
	  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9,  10, 10, 10,
	  10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 15, 15,
	  15, 16, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19, 19, 20};
  return data[bsr];
}

#ifndef FMT_EXPORTED
FMT_EXTERN template struct basic_data<void>;
#endif

// This is a struct rather than an alias to avoid shadowing warnings in gcc.
struct data : basic_data<> {};

#ifdef FMT_BUILTIN_CLZLL
// Returns the number of decimal digits in n. Leading zeros are not counted
// except for n == 0 in which case count_digits returns 1.
inline int count_digits(uint64_t n) {
  // https://github.com/fmtlib/format-benchmark/blob/master/digits10
  auto t = bsr2log10(FMT_BUILTIN_CLZLL(n | 1) ^ 63);
  return t - (n < data::zero_or_powers_of_10_64_new[t]);
}
#else
// Fallback version of count_digits used when __builtin_clz is not available.
inline int count_digits(uint64_t n) {
  int count = 1;
  for (;;) {
	// Integer division is slow so do it for a group of four digits instead
	// of for every digit. The idea comes from the talk by Alexandrescu
	// "Three Optimization Tips for C++". See speed-test for a comparison.
	if (n < 10) return count;
	if (n < 100) return count + 1;
	if (n < 1000) return count + 2;
	if (n < 10000) return count + 3;
	n /= 10000u;
	count += 4;
  }
}
#endif

#if FMT_USE_INT128
inline int count_digits(uint128_t n) {
  int count = 1;
  for (;;) {
	// Integer division is slow so do it for a group of four digits instead
	// of for every digit. The idea comes from the talk by Alexandrescu
	// "Three Optimization Tips for C++". See speed-test for a comparison.
	if (n < 10) return count;
	if (n < 100) return count + 1;
	if (n < 1000) return count + 2;
	if (n < 10000) return count + 3;
	n /= 10000U;
	count += 4;
  }
}
#endif

// Counts the number of digits in n. BITS = log2(radix).
template <unsigned BITS, typename UInt> inline int count_digits(UInt n) {
  int num_digits = 0;
  do {
	++num_digits;
  } while ((n >>= BITS) != 0);
  return num_digits;
}

template <> int count_digits<4>(detail::fallback_uintptr n);

#if FMT_GCC_VERSION || FMT_CLANG_VERSION
#  define FMT_ALWAYS_INLINE inline __attribute__((always_inline))
#elif FMT_MSC_VER
#  define FMT_ALWAYS_INLINE __forceinline
#else
#  define FMT_ALWAYS_INLINE inline
#endif

// To suppress unnecessary security cookie checks
#if FMT_MSC_VER && !FMT_CLANG_VERSION
#  define FMT_SAFEBUFFERS __declspec(safebuffers)
#else
#  define FMT_SAFEBUFFERS
#endif

#ifdef FMT_BUILTIN_CLZ
// Optional version of count_digits for better performance on 32-bit platforms.
inline int count_digits(uint32_t n) {
  auto t = bsr2log10(FMT_BUILTIN_CLZ(n | 1) ^ 31);
  return t - (n < data::zero_or_powers_of_10_32_new[t]);
}
#endif

template <typename Int> constexpr int digits10() FMT_NOEXCEPT {
  return std::numeric_limits<Int>::digits10;
}
template <> constexpr int digits10<int128_t>() FMT_NOEXCEPT { return 38; }
template <> constexpr int digits10<uint128_t>() FMT_NOEXCEPT { return 38; }

template <typename Char> FMT_API std::string grouping_impl(locale_ref loc);
template <typename Char> inline std::string grouping(locale_ref loc) {
  return grouping_impl<char>(loc);
}
template <> inline std::string grouping<wchar_t>(locale_ref loc) {
  return grouping_impl<wchar_t>(loc);
}

template <typename Char> FMT_API Char thousands_sep_impl(locale_ref loc);
template <typename Char> inline Char thousands_sep(locale_ref loc) {
  return Char(thousands_sep_impl<char>(loc));
}
template <> inline wchar_t thousands_sep(locale_ref loc) {
  return thousands_sep_impl<wchar_t>(loc);
}

template <typename Char> FMT_API Char decimal_point_impl(locale_ref loc);
template <typename Char> inline Char decimal_point(locale_ref loc) {
  return Char(decimal_point_impl<char>(loc));
}
template <> inline wchar_t decimal_point(locale_ref loc) {
  return decimal_point_impl<wchar_t>(loc);
}

// Compares two characters for equality.
template <typename Char> bool equal2(const Char* lhs, const char* rhs) {
  return lhs[0] == rhs[0] && lhs[1] == rhs[1];
}
inline bool equal2(const char* lhs, const char* rhs) {
  return memcmp(lhs, rhs, 2) == 0;
}

// Copies two characters from src to dst.
template <typename Char> void copy2(Char* dst, const char* src) {
  *dst++ = static_cast<Char>(*src++);
  *dst = static_cast<Char>(*src);
}
FMT_INLINE void copy2(char* dst, const char* src) { memcpy(dst, src, 2); }

template <typename Iterator> struct format_decimal_result {
  Iterator begin;
  Iterator end;
};

// Formats a decimal unsigned integer value writing into out pointing to a
// buffer of specified size. The caller must ensure that the buffer is large
// enough.
template <typename Char, typename UInt>
inline format_decimal_result<Char*> format_decimal(Char* out, UInt value,
												   int size) {
  FMT_ASSERT(size >= count_digits(value), "invalid digit count");
  out += size;
  Char* end = out;
  while (value >= 100) {
	// Integer division is slow so do it for a group of two digits instead
	// of for every digit. The idea comes from the talk by Alexandrescu
	// "Three Optimization Tips for C++". See speed-test for a comparison.
	out -= 2;
	copy2(out, data::digits[value % 100]);
	value /= 100;
  }
  if (value < 10) {
	*--out = static_cast<Char>('0' + value);
	return {out, end};
  }
  out -= 2;
  copy2(out, data::digits[value]);
  return {out, end};
}

template <typename Char, typename UInt, typename Iterator,
		  FMT_ENABLE_IF(!std::is_pointer<remove_cvref_t<Iterator>>::value)>
inline format_decimal_result<Iterator> format_decimal(Iterator out, UInt value,
													  int size) {
  // Buffer is large enough to hold all digits (digits10 + 1).
  Char buffer[digits10<UInt>() + 1];
  auto end = format_decimal(buffer, value, size).end;
  return {out, detail::copy_str<Char>(buffer, end, out)};
}

template <unsigned BASE_BITS, typename Char, typename UInt>
inline Char* format_uint(Char* buffer, UInt value, int num_digits,
						 bool upper = false) {
  buffer += num_digits;
  Char* end = buffer;
  do {
	const char* digits = upper ? "0123456789ABCDEF" : data::hex_digits;
	unsigned digit = (value & ((1 << BASE_BITS) - 1));
	*--buffer = static_cast<Char>(BASE_BITS < 4 ? static_cast<char>('0' + digit)
												: digits[digit]);
  } while ((value >>= BASE_BITS) != 0);
  return end;
}

template <unsigned BASE_BITS, typename Char>
Char* format_uint(Char* buffer, detail::fallback_uintptr n, int num_digits,
				  bool = false) {
  auto char_digits = std::numeric_limits<unsigned char>::digits / 4;
  int start = (num_digits + char_digits - 1) / char_digits - 1;
  if (int start_digits = num_digits % char_digits) {
	unsigned value = n.value[start--];
	buffer = format_uint<BASE_BITS>(buffer, value, start_digits);
  }
  for (; start >= 0; --start) {
	unsigned value = n.value[start];
	buffer += char_digits;
	auto p = buffer;
	for (int i = 0; i < char_digits; ++i) {
	  unsigned digit = (value & ((1 << BASE_BITS) - 1));
	  *--p = static_cast<Char>(data::hex_digits[digit]);
	  value >>= BASE_BITS;
	}
  }
  return buffer;
}

template <unsigned BASE_BITS, typename Char, typename It, typename UInt>
inline It format_uint(It out, UInt value, int num_digits, bool upper = false) {
  if (auto ptr = to_pointer<Char>(out, to_unsigned(num_digits))) {
	format_uint<BASE_BITS>(ptr, value, num_digits, upper);
	return out;
  }
  // Buffer should be large enough to hold all digits (digits / BASE_BITS + 1).
  char buffer[num_bits<UInt>() / BASE_BITS + 1];
  format_uint<BASE_BITS>(buffer, value, num_digits, upper);
  return detail::copy_str<Char>(buffer, buffer + num_digits, out);
}

// A converter from UTF-8 to UTF-16.
class utf8_to_utf16 {
 private:
  wmemory_buffer buffer_;

 public:
  FMT_API explicit utf8_to_utf16(string_view s);
  operator wstring_view() const { return {&buffer_[0], size()}; }
  size_t size() const { return buffer_.size() - 1; }
  const wchar_t* c_str() const { return &buffer_[0]; }
  std::wstring str() const { return {&buffer_[0], size()}; }
};

template <typename T = void> struct null {};

// Workaround an array initialization issue in gcc 4.8.
template <typename Char> struct fill_t {
 private:
  enum { max_size = 4 };
  Char data_[max_size] = {Char(' '), Char(0), Char(0), Char(0)};
  unsigned char size_ = 1;

 public:
  FMT_CONSTEXPR void operator=(basic_string_view<Char> s) {
	auto size = s.size();
	if (size > max_size) {
	  FMT_THROW(format_error("invalid fill"));
	  return;
	}
	for (size_t i = 0; i < size; ++i) data_[i] = s[i];
	size_ = static_cast<unsigned char>(size);
  }

  size_t size() const { return size_; }
  const Char* data() const { return data_; }

  FMT_CONSTEXPR Char& operator[](size_t index) { return data_[index]; }
  FMT_CONSTEXPR const Char& operator[](size_t index) const {
	return data_[index];
  }
};
}  // namespace detail

// We cannot use enum classes as bit fields because of a gcc bug
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=61414.
namespace align {
enum type { none, left, right, center, numeric };
}
using align_t = align::type;

namespace sign {
enum type { none, minus, plus, space };
}
using sign_t = sign::type;

// Format specifiers for built-in and string types.
template <typename Char> struct basic_format_specs {
  int width;
  int precision;
  char type;
  align_t align : 4;
  sign_t sign : 3;
  bool alt : 1;  // Alternate form ('#').
  detail::fill_t<Char> fill;

  constexpr basic_format_specs()
	  : width(0),
		precision(-1),
		type(0),
		align(align::none),
		sign(sign::none),
		alt(false) {}
};

using format_specs = basic_format_specs<char>;

namespace detail {
namespace dragonbox {

// Type-specific information that Dragonbox uses.
template <class T> struct float_info;

template <> struct float_info<float> {
  using carrier_uint = uint32_t;
  static const int significand_bits = 23;
  static const int exponent_bits = 8;
  static const int min_exponent = -126;
  static const int max_exponent = 127;
  static const int exponent_bias = -127;
  static const int decimal_digits = 9;
  static const int kappa = 1;
  static const int big_divisor = 100;
  static const int small_divisor = 10;
  static const int min_k = -31;
  static const int max_k = 46;
  static const int cache_bits = 64;
  static const int divisibility_check_by_5_threshold = 39;
  static const int case_fc_pm_half_lower_threshold = -1;
  static const int case_fc_pm_half_upper_threshold = 6;
  static const int case_fc_lower_threshold = -2;
  static const int case_fc_upper_threshold = 6;
  static const int case_shorter_interval_left_endpoint_lower_threshold = 2;
  static const int case_shorter_interval_left_endpoint_upper_threshold = 3;
  static const int shorter_interval_tie_lower_threshold = -35;
  static const int shorter_interval_tie_upper_threshold = -35;
  static const int max_trailing_zeros = 7;
};

template <> struct float_info<double> {
  using carrier_uint = uint64_t;
  static const int significand_bits = 52;
  static const int exponent_bits = 11;
  static const int min_exponent = -1022;
  static const int max_exponent = 1023;
  static const int exponent_bias = -1023;
  static const int decimal_digits = 17;
  static const int kappa = 2;
  static const int big_divisor = 1000;
  static const int small_divisor = 100;
  static const int min_k = -292;
  static const int max_k = 326;
  static const int cache_bits = 128;
  static const int divisibility_check_by_5_threshold = 86;
  static const int case_fc_pm_half_lower_threshold = -2;
  static const int case_fc_pm_half_upper_threshold = 9;
  static const int case_fc_lower_threshold = -4;
  static const int case_fc_upper_threshold = 9;
  static const int case_shorter_interval_left_endpoint_lower_threshold = 2;
  static const int case_shorter_interval_left_endpoint_upper_threshold = 3;
  static const int shorter_interval_tie_lower_threshold = -77;
  static const int shorter_interval_tie_upper_threshold = -77;
  static const int max_trailing_zeros = 16;
};

template <typename T> struct decimal_fp {
  using significand_type = typename float_info<T>::carrier_uint;
  significand_type significand;
  int exponent;
};

template <typename T> FMT_API decimal_fp<T> to_decimal(T x) FMT_NOEXCEPT;
}  // namespace dragonbox

template <typename T>
constexpr typename dragonbox::float_info<T>::carrier_uint exponent_mask() {
  using uint = typename dragonbox::float_info<T>::carrier_uint;
  return ((uint(1) << dragonbox::float_info<T>::exponent_bits) - 1)
		 << dragonbox::float_info<T>::significand_bits;
}

// A floating-point presentation format.
enum class float_format : unsigned char {
  general,  // General: exponent notation or fixed point based on magnitude.
  exp,      // Exponent notation with the default precision of 6, e.g. 1.2e-3.
  fixed,    // Fixed point with the default precision of 6, e.g. 0.0012.
  hex
};

struct float_specs {
  int precision;
  float_format format : 8;
  sign_t sign : 8;
  bool upper : 1;
  bool locale : 1;
  bool binary32 : 1;
  bool use_grisu : 1;
  bool showpoint : 1;
};

// Writes the exponent exp in the form "[+-]d{2,3}" to buffer.
template <typename Char, typename It> It write_exponent(int exp, It it) {
  FMT_ASSERT(-10000 < exp && exp < 10000, "exponent out of range");
  if (exp < 0) {
	*it++ = static_cast<Char>('-');
	exp = -exp;
  } else {
	*it++ = static_cast<Char>('+');
  }
  if (exp >= 100) {
	const char* top = data::digits[exp / 100];
	if (exp >= 1000) *it++ = static_cast<Char>(top[0]);
	*it++ = static_cast<Char>(top[1]);
	exp %= 100;
  }
  const char* d = data::digits[exp];
  *it++ = static_cast<Char>(d[0]);
  *it++ = static_cast<Char>(d[1]);
  return it;
}

template <typename T>
int format_float(T value, int precision, float_specs specs, buffer<char>& buf);

// Formats a floating-point number with snprintf.
template <typename T>
int snprintf_float(T value, int precision, float_specs specs,
				   buffer<char>& buf);

template <typename T> T promote_float(T value) { return value; }
inline double promote_float(float value) { return static_cast<double>(value); }

template <typename Handler>
FMT_CONSTEXPR void handle_int_type_spec(char spec, Handler&& handler) {
  switch (spec) {
  case 0:
  case 'd':
	handler.on_dec();
	break;
  case 'x':
  case 'X':
	handler.on_hex();
	break;
  case 'b':
  case 'B':
	handler.on_bin();
	break;
  case 'o':
	handler.on_oct();
	break;
#ifdef FMT_DEPRECATED_N_SPECIFIER
  case 'n':
#endif
  case 'L':
	handler.on_num();
	break;
  case 'c':
	handler.on_chr();
	break;
  default:
	handler.on_error();
  }
}

template <typename ErrorHandler = error_handler, typename Char>
FMT_CONSTEXPR float_specs parse_float_type_spec(
	const basic_format_specs<Char>& specs, ErrorHandler&& eh = {}) {
  auto result = float_specs();
  result.showpoint = specs.alt;
  switch (specs.type) {
  case 0:
	result.format = float_format::general;
	result.showpoint |= specs.precision > 0;
	break;
  case 'G':
	result.upper = true;
	FMT_FALLTHROUGH;
  case 'g':
	result.format = float_format::general;
	break;
  case 'E':
	result.upper = true;
	FMT_FALLTHROUGH;
  case 'e':
	result.format = float_format::exp;
	result.showpoint |= specs.precision != 0;
	break;
  case 'F':
	result.upper = true;
	FMT_FALLTHROUGH;
  case 'f':
	result.format = float_format::fixed;
	result.showpoint |= specs.precision != 0;
	break;
  case 'A':
	result.upper = true;
	FMT_FALLTHROUGH;
  case 'a':
	result.format = float_format::hex;
	break;
#ifdef FMT_DEPRECATED_N_SPECIFIER
  case 'n':
#endif
  case 'L':
	result.locale = true;
	break;
  default:
	eh.on_error("invalid type specifier");
	break;
  }
  return result;
}

template <typename Char, typename Handler>
FMT_CONSTEXPR void handle_char_specs(const basic_format_specs<Char>* specs,
									 Handler&& handler) {
  if (!specs) return handler.on_char();
  if (specs->type && specs->type != 'c') return handler.on_int();
  if (specs->align == align::numeric || specs->sign != sign::none || specs->alt)
	handler.on_error("invalid format specifier for char");
  handler.on_char();
}

template <typename Char, typename Handler>
FMT_CONSTEXPR void handle_cstring_type_spec(Char spec, Handler&& handler) {
  if (spec == 0 || spec == 's')
	handler.on_string();
  else if (spec == 'p')
	handler.on_pointer();
  else
	handler.on_error("invalid type specifier");
}

template <typename Char, typename ErrorHandler>
FMT_CONSTEXPR void check_string_type_spec(Char spec, ErrorHandler&& eh) {
  if (spec != 0 && spec != 's') eh.on_error("invalid type specifier");
}

template <typename Char, typename ErrorHandler>
FMT_CONSTEXPR void check_pointer_type_spec(Char spec, ErrorHandler&& eh) {
  if (spec != 0 && spec != 'p') eh.on_error("invalid type specifier");
}

template <typename ErrorHandler> class int_type_checker : private ErrorHandler {
 public:
  FMT_CONSTEXPR explicit int_type_checker(ErrorHandler eh) : ErrorHandler(eh) {}

  FMT_CONSTEXPR void on_dec() {}
  FMT_CONSTEXPR void on_hex() {}
  FMT_CONSTEXPR void on_bin() {}
  FMT_CONSTEXPR void on_oct() {}
  FMT_CONSTEXPR void on_num() {}
  FMT_CONSTEXPR void on_chr() {}

  FMT_CONSTEXPR void on_error() {
	ErrorHandler::on_error("invalid type specifier");
  }
};

template <typename ErrorHandler>
class char_specs_checker : public ErrorHandler {
 private:
  char type_;

 public:
  FMT_CONSTEXPR char_specs_checker(char type, ErrorHandler eh)
	  : ErrorHandler(eh), type_(type) {}

  FMT_CONSTEXPR void on_int() {
	handle_int_type_spec(type_, int_type_checker<ErrorHandler>(*this));
  }
  FMT_CONSTEXPR void on_char() {}
};

template <typename ErrorHandler>
class cstring_type_checker : public ErrorHandler {
 public:
  FMT_CONSTEXPR explicit cstring_type_checker(ErrorHandler eh)
	  : ErrorHandler(eh) {}

  FMT_CONSTEXPR void on_string() {}
  FMT_CONSTEXPR void on_pointer() {}
};

template <typename OutputIt, typename Char>
FMT_NOINLINE OutputIt fill(OutputIt it, size_t n, const fill_t<Char>& fill) {
  auto fill_size = fill.size();
  if (fill_size == 1) return std::fill_n(it, n, fill[0]);
  for (size_t i = 0; i < n; ++i) it = std::copy_n(fill.data(), fill_size, it);
  return it;
}

// Writes the output of f, padded according to format specifications in specs.
// size: output size in code units.
// width: output display width in (terminal) column positions.
template <align::type align = align::left, typename OutputIt, typename Char,
		  typename F>
inline OutputIt write_padded(OutputIt out,
							 const basic_format_specs<Char>& specs, size_t size,
							 size_t width, F&& f) {
  static_assert(align == align::left || align == align::right, "");
  unsigned spec_width = to_unsigned(specs.width);
  size_t padding = spec_width > width ? spec_width - width : 0;
  auto* shifts = align == align::left ? data::left_padding_shifts
									  : data::right_padding_shifts;
  size_t left_padding = padding >> shifts[specs.align];
  auto it = reserve(out, size + padding * specs.fill.size());
  it = fill(it, left_padding, specs.fill);
  it = f(it);
  it = fill(it, padding - left_padding, specs.fill);
  return base_iterator(out, it);
}

template <align::type align = align::left, typename OutputIt, typename Char,
		  typename F>
inline OutputIt write_padded(OutputIt out,
							 const basic_format_specs<Char>& specs, size_t size,
							 F&& f) {
  return write_padded<align>(out, specs, size, size, f);
}

template <typename Char, typename OutputIt>
OutputIt write_bytes(OutputIt out, string_view bytes,
					 const basic_format_specs<Char>& specs) {
  using iterator = remove_reference_t<decltype(reserve(out, 0))>;
  return write_padded(out, specs, bytes.size(), [bytes](iterator it) {
	const char* data = bytes.data();
	return copy_str<Char>(data, data + bytes.size(), it);
  });
}

// Data for write_int that doesn't depend on output iterator type. It is used to
// avoid template code bloat.
template <typename Char> struct write_int_data {
  size_t size;
  size_t padding;

  write_int_data(int num_digits, string_view prefix,
				 const basic_format_specs<Char>& specs)
	  : size(prefix.size() + to_unsigned(num_digits)), padding(0) {
	if (specs.align == align::numeric) {
	  auto width = to_unsigned(specs.width);
	  if (width > size) {
		padding = width - size;
		size = width;
	  }
	} else if (specs.precision > num_digits) {
	  size = prefix.size() + to_unsigned(specs.precision);
	  padding = to_unsigned(specs.precision - num_digits);
	}
  }
};

// Writes an integer in the format
//   <left-padding><prefix><numeric-padding><digits><right-padding>
// where <digits> are written by f(it).
template <typename OutputIt, typename Char, typename F>
OutputIt write_int(OutputIt out, int num_digits, string_view prefix,
				   const basic_format_specs<Char>& specs, F f) {
  auto data = write_int_data<Char>(num_digits, prefix, specs);
  using iterator = remove_reference_t<decltype(reserve(out, 0))>;
  return write_padded<align::right>(out, specs, data.size, [=](iterator it) {
	if (prefix.size() != 0)
	  it = copy_str<Char>(prefix.begin(), prefix.end(), it);
	it = std::fill_n(it, data.padding, static_cast<Char>('0'));
	return f(it);
  });
}

template <typename StrChar, typename Char, typename OutputIt>
OutputIt write(OutputIt out, basic_string_view<StrChar> s,
			   const basic_format_specs<Char>& specs) {
  auto data = s.data();
  auto size = s.size();
  if (specs.precision >= 0 && to_unsigned(specs.precision) < size)
	size = code_point_index(s, to_unsigned(specs.precision));
  auto width = specs.width != 0
				   ? count_code_points(basic_string_view<StrChar>(data, size))
				   : 0;
  using iterator = remove_reference_t<decltype(reserve(out, 0))>;
  return write_padded(out, specs, size, width, [=](iterator it) {
	return copy_str<Char>(data, data + size, it);
  });
}

// The handle_int_type_spec handler that writes an integer.
template <typename OutputIt, typename Char, typename UInt> struct int_writer {
  OutputIt out;
  locale_ref locale;
  const basic_format_specs<Char>& specs;
  UInt abs_value;
  char prefix[4];
  unsigned prefix_size;

  using iterator =
	  remove_reference_t<decltype(reserve(std::declval<OutputIt&>(), 0))>;

  string_view get_prefix() const { return string_view(prefix, prefix_size); }

  template <typename Int>
  int_writer(OutputIt output, locale_ref loc, Int value,
			 const basic_format_specs<Char>& s)
	  : out(output),
		locale(loc),
		specs(s),
		abs_value(static_cast<UInt>(value)),
		prefix_size(0) {
	static_assert(std::is_same<uint32_or_64_or_128_t<Int>, UInt>::value, "");
	if (is_negative(value)) {
	  prefix[0] = '-';
	  ++prefix_size;
	  abs_value = 0 - abs_value;
	} else if (specs.sign != sign::none && specs.sign != sign::minus) {
	  prefix[0] = specs.sign == sign::plus ? '+' : ' ';
	  ++prefix_size;
	}
  }

  void on_dec() {
	auto num_digits = count_digits(abs_value);
	out = write_int(
		out, num_digits, get_prefix(), specs, [this, num_digits](iterator it) {
		  return format_decimal<Char>(it, abs_value, num_digits).end;
		});
  }

  void on_hex() {
	if (specs.alt) {
	  prefix[prefix_size++] = '0';
	  prefix[prefix_size++] = specs.type;
	}
	int num_digits = count_digits<4>(abs_value);
	out = write_int(out, num_digits, get_prefix(), specs,
					[this, num_digits](iterator it) {
					  return format_uint<4, Char>(it, abs_value, num_digits,
												  specs.type != 'x');
					});
  }

  void on_bin() {
	if (specs.alt) {
	  prefix[prefix_size++] = '0';
	  prefix[prefix_size++] = static_cast<char>(specs.type);
	}
	int num_digits = count_digits<1>(abs_value);
	out = write_int(out, num_digits, get_prefix(), specs,
					[this, num_digits](iterator it) {
					  return format_uint<1, Char>(it, abs_value, num_digits);
					});
  }

  void on_oct() {
	int num_digits = count_digits<3>(abs_value);
	if (specs.alt && specs.precision <= num_digits && abs_value != 0) {
	  // Octal prefix '0' is counted as a digit, so only add it if precision
	  // is not greater than the number of digits.
	  prefix[prefix_size++] = '0';
	}
	out = write_int(out, num_digits, get_prefix(), specs,
					[this, num_digits](iterator it) {
					  return format_uint<3, Char>(it, abs_value, num_digits);
					});
  }

  enum { sep_size = 1 };

  void on_num() {
	std::string groups = grouping<Char>(locale);
	if (groups.empty()) return on_dec();
	auto sep = thousands_sep<Char>(locale);
	if (!sep) return on_dec();
	int num_digits = count_digits(abs_value);
	int size = num_digits, n = num_digits;
	std::string::const_iterator group = groups.cbegin();
	while (group != groups.cend() && n > *group && *group > 0 &&
		   *group != max_value<char>()) {
	  size += sep_size;
	  n -= *group;
	  ++group;
	}
	if (group == groups.cend()) size += sep_size * ((n - 1) / groups.back());
	char digits[40];
	format_decimal(digits, abs_value, num_digits);
	basic_memory_buffer<Char> buffer;
	size += static_cast<int>(prefix_size);
	const auto usize = to_unsigned(size);
	buffer.resize(usize);
	basic_string_view<Char> s(&sep, sep_size);
	// Index of a decimal digit with the least significant digit having index 0.
	int digit_index = 0;
	group = groups.cbegin();
	auto p = buffer.data() + size - 1;
	for (int i = num_digits - 1; i > 0; --i) {
	  *p-- = static_cast<Char>(digits[i]);
	  if (*group <= 0 || ++digit_index % *group != 0 ||
		  *group == max_value<char>())
		continue;
	  if (group + 1 != groups.cend()) {
		digit_index = 0;
		++group;
	  }
	  std::uninitialized_copy(s.data(), s.data() + s.size(),
							  make_checked(p, s.size()));
	  p -= s.size();
	}
	*p-- = static_cast<Char>(*digits);
	if (prefix_size != 0) *p = static_cast<Char>('-');
	auto data = buffer.data();
	out = write_padded<align::right>(
		out, specs, usize, usize,
		[=](iterator it) { return copy_str<Char>(data, data + size, it); });
  }

  void on_chr() { *out++ = static_cast<Char>(abs_value); }

  FMT_NORETURN void on_error() {
	FMT_THROW(format_error("invalid type specifier"));
  }
};

template <typename Char, typename OutputIt>
OutputIt write_nonfinite(OutputIt out, bool isinf,
						 const basic_format_specs<Char>& specs,
						 const float_specs& fspecs) {
  auto str =
	  isinf ? (fspecs.upper ? "INF" : "inf") : (fspecs.upper ? "NAN" : "nan");
  constexpr size_t str_size = 3;
  auto sign = fspecs.sign;
  auto size = str_size + (sign ? 1 : 0);
  using iterator = remove_reference_t<decltype(reserve(out, 0))>;
  return write_padded(out, specs, size, [=](iterator it) {
	if (sign) *it++ = static_cast<Char>(data::signs[sign]);
	return copy_str<Char>(str, str + str_size, it);
  });
}

// A decimal floating-point number significand * pow(10, exp).
struct big_decimal_fp {
  const char* significand;
  int significand_size;
  int exponent;
};

inline int get_significand_size(const big_decimal_fp& fp) {
  return fp.significand_size;
}
template <typename T>
inline int get_significand_size(const dragonbox::decimal_fp<T>& fp) {
  return count_digits(fp.significand);
}

template <typename Char, typename OutputIt>
inline OutputIt write_significand(OutputIt out, const char* significand,
								  int& significand_size) {
  return copy_str<Char>(significand, significand + significand_size, out);
}
template <typename Char, typename OutputIt, typename UInt>
inline OutputIt write_significand(OutputIt out, UInt significand,
								  int significand_size) {
  return format_decimal<Char>(out, significand, significand_size).end;
}

template <typename Char, typename UInt,
		  FMT_ENABLE_IF(std::is_integral<UInt>::value)>
inline Char* write_significand(Char* out, UInt significand,
							   int significand_size, int integral_size,
							   Char decimal_point) {
  if (!decimal_point)
	return format_decimal(out, significand, significand_size).end;
  auto end = format_decimal(out + 1, significand, significand_size).end;
  if (integral_size == 1)
	out[0] = out[1];
  else
	std::copy_n(out + 1, integral_size, out);
  out[integral_size] = decimal_point;
  return end;
}

template <typename OutputIt, typename UInt, typename Char,
		  FMT_ENABLE_IF(!std::is_pointer<remove_cvref_t<OutputIt>>::value)>
inline OutputIt write_significand(OutputIt out, UInt significand,
								  int significand_size, int integral_size,
								  Char decimal_point) {
  // Buffer is large enough to hold digits (digits10 + 1) and a decimal point.
  Char buffer[digits10<UInt>() + 2];
  auto end = write_significand(buffer, significand, significand_size,
							   integral_size, decimal_point);
  return detail::copy_str<Char>(buffer, end, out);
}

template <typename OutputIt, typename Char>
inline OutputIt write_significand(OutputIt out, const char* significand,
								  int significand_size, int integral_size,
								  Char decimal_point) {
  out = detail::copy_str<Char>(significand, significand + integral_size, out);
  if (!decimal_point) return out;
  *out++ = decimal_point;
  return detail::copy_str<Char>(significand + integral_size,
								significand + significand_size, out);
}

template <typename OutputIt, typename DecimalFP, typename Char>
OutputIt write_float(OutputIt out, const DecimalFP& fp,
					 const basic_format_specs<Char>& specs, float_specs fspecs,
					 Char decimal_point) {
  auto significand = fp.significand;
  int significand_size = get_significand_size(fp);
  static const Char zero = static_cast<Char>('0');
  auto sign = fspecs.sign;
  size_t size = to_unsigned(significand_size) + (sign ? 1 : 0);
  using iterator = remove_reference_t<decltype(reserve(out, 0))>;

  int output_exp = fp.exponent + significand_size - 1;
  auto use_exp_format = [=]() {
	if (fspecs.format == float_format::exp) return true;
	if (fspecs.format != float_format::general) return false;
	// Use the fixed notation if the exponent is in [exp_lower, exp_upper),
	// e.g. 0.0001 instead of 1e-04. Otherwise use the exponent notation.
	const int exp_lower = -4, exp_upper = 16;
	return output_exp < exp_lower ||
		   output_exp >= (fspecs.precision > 0 ? fspecs.precision : exp_upper);
  };
  if (use_exp_format()) {
	int num_zeros = 0;
	if (fspecs.showpoint) {
	  num_zeros = (std::max)(fspecs.precision - significand_size, 0);
	  size += to_unsigned(num_zeros);
	} else if (significand_size == 1) {
	  decimal_point = Char();
	}
	auto abs_output_exp = output_exp >= 0 ? output_exp : -output_exp;
	int exp_digits = 2;
	if (abs_output_exp >= 100) exp_digits = abs_output_exp >= 1000 ? 4 : 3;

	size += to_unsigned((decimal_point ? 1 : 0) + 2 + exp_digits);
	char exp_char = fspecs.upper ? 'E' : 'e';
	auto write = [=](iterator it) {
	  if (sign) *it++ = static_cast<Char>(data::signs[sign]);
	  // Insert a decimal point after the first digit and add an exponent.
	  it = write_significand(it, significand, significand_size, 1,
							 decimal_point);
	  if (num_zeros > 0) it = std::fill_n(it, num_zeros, zero);
	  *it++ = static_cast<Char>(exp_char);
	  return write_exponent<Char>(output_exp, it);
	};
	return specs.width > 0 ? write_padded<align::right>(out, specs, size, write)
						   : base_iterator(out, write(reserve(out, size)));
  }

  int exp = fp.exponent + significand_size;
  if (fp.exponent >= 0) {
	// 1234e5 -> 123400000[.0+]
	size += to_unsigned(fp.exponent);
	int num_zeros = fspecs.precision - exp;
#ifdef FMT_FUZZ
	if (num_zeros > 5000)
	  throw std::runtime_error("fuzz mode - avoiding excessive cpu use");
#endif
	if (fspecs.showpoint) {
	  if (num_zeros <= 0 && fspecs.format != float_format::fixed) num_zeros = 1;
	  if (num_zeros > 0) size += to_unsigned(num_zeros);
	}
	return write_padded<align::right>(out, specs, size, [&](iterator it) {
	  if (sign) *it++ = static_cast<Char>(data::signs[sign]);
	  it = write_significand<Char>(it, significand, significand_size);
	  it = std::fill_n(it, fp.exponent, zero);
	  if (!fspecs.showpoint) return it;
	  *it++ = decimal_point;
	  return num_zeros > 0 ? std::fill_n(it, num_zeros, zero) : it;
	});
  } else if (exp > 0) {
	// 1234e-2 -> 12.34[0+]
	int num_zeros = fspecs.showpoint ? fspecs.precision - significand_size : 0;
	size += 1 + to_unsigned(num_zeros > 0 ? num_zeros : 0);
	return write_padded<align::right>(out, specs, size, [&](iterator it) {
	  if (sign) *it++ = static_cast<Char>(data::signs[sign]);
	  it = write_significand(it, significand, significand_size, exp,
							 decimal_point);
	  return num_zeros > 0 ? std::fill_n(it, num_zeros, zero) : it;
	});
  }
  // 1234e-6 -> 0.001234
  int num_zeros = -exp;
  if (significand_size == 0 && fspecs.precision >= 0 &&
	  fspecs.precision < num_zeros) {
	num_zeros = fspecs.precision;
  }
  size += 2 + to_unsigned(num_zeros);
  return write_padded<align::right>(out, specs, size, [&](iterator it) {
	if (sign) *it++ = static_cast<Char>(data::signs[sign]);
	*it++ = zero;
	if (num_zeros == 0 && significand_size == 0 && !fspecs.showpoint) return it;
	*it++ = decimal_point;
	it = std::fill_n(it, num_zeros, zero);
	return write_significand<Char>(it, significand, significand_size);
  });
}

template <typename Char, typename OutputIt, typename T,
		  FMT_ENABLE_IF(std::is_floating_point<T>::value)>
OutputIt write(OutputIt out, T value, basic_format_specs<Char> specs,
			   locale_ref loc = {}) {
  if (const_check(!is_supported_floating_point(value))) return out;
  float_specs fspecs = parse_float_type_spec(specs);
  fspecs.sign = specs.sign;
  if (std::signbit(value)) {  // value < 0 is false for NaN so use signbit.
	fspecs.sign = sign::minus;
	value = -value;
  } else if (fspecs.sign == sign::minus) {
	fspecs.sign = sign::none;
  }

  if (!std::isfinite(value))
	return write_nonfinite(out, std::isinf(value), specs, fspecs);

  if (specs.align == align::numeric && fspecs.sign) {
	auto it = reserve(out, 1);
	*it++ = static_cast<Char>(data::signs[fspecs.sign]);
	out = base_iterator(out, it);
	fspecs.sign = sign::none;
	if (specs.width != 0) --specs.width;
  }

  memory_buffer buffer;
  if (fspecs.format == float_format::hex) {
	if (fspecs.sign) buffer.push_back(data::signs[fspecs.sign]);
	snprintf_float(promote_float(value), specs.precision, fspecs, buffer);
	return write_bytes(out, {buffer.data(), buffer.size()}, specs);
  }
  int precision = specs.precision >= 0 || !specs.type ? specs.precision : 6;
  if (fspecs.format == float_format::exp) {
	if (precision == max_value<int>())
	  FMT_THROW(format_error("number is too big"));
	else
	  ++precision;
  }
  if (const_check(std::is_same<T, float>())) fspecs.binary32 = true;
  fspecs.use_grisu = is_fast_float<T>();
  int exp = format_float(promote_float(value), precision, fspecs, buffer);
  fspecs.precision = precision;
  Char point =
	  fspecs.locale ? decimal_point<Char>(loc) : static_cast<Char>('.');
  auto fp = big_decimal_fp{buffer.data(), static_cast<int>(buffer.size()), exp};
  return write_float(out, fp, specs, fspecs, point);
}

template <typename Char, typename OutputIt, typename T,
		  FMT_ENABLE_IF(is_fast_float<T>::value)>
OutputIt write(OutputIt out, T value) {
  if (const_check(!is_supported_floating_point(value))) return out;

  using floaty = conditional_t<std::is_same<T, long double>::value, double, T>;
  using uint = typename dragonbox::float_info<floaty>::carrier_uint;
  auto bits = bit_cast<uint>(value);

  auto fspecs = float_specs();
  auto sign_bit = bits & (uint(1) << (num_bits<uint>() - 1));
  if (sign_bit != 0) {
	fspecs.sign = sign::minus;
	value = -value;
  }

  static const auto specs = basic_format_specs<Char>();
  uint mask = exponent_mask<floaty>();
  if ((bits & mask) == mask)
	return write_nonfinite(out, std::isinf(value), specs, fspecs);

  auto dec = dragonbox::to_decimal(static_cast<floaty>(value));
  return write_float(out, dec, specs, fspecs, static_cast<Char>('.'));
}

template <typename Char, typename OutputIt, typename T,
		  FMT_ENABLE_IF(std::is_floating_point<T>::value &&
						!is_fast_float<T>::value)>
inline OutputIt write(OutputIt out, T value) {
  return write(out, value, basic_format_specs<Char>());
}

template <typename Char, typename OutputIt>
OutputIt write_char(OutputIt out, Char value,
					const basic_format_specs<Char>& specs) {
  using iterator = remove_reference_t<decltype(reserve(out, 0))>;
  return write_padded(out, specs, 1, [=](iterator it) {
	*it++ = value;
	return it;
  });
}

template <typename Char, typename OutputIt, typename UIntPtr>
OutputIt write_ptr(OutputIt out, UIntPtr value,
				   const basic_format_specs<Char>* specs) {
  int num_digits = count_digits<4>(value);
  auto size = to_unsigned(num_digits) + size_t(2);
  using iterator = remove_reference_t<decltype(reserve(out, 0))>;
  auto write = [=](iterator it) {
	*it++ = static_cast<Char>('0');
	*it++ = static_cast<Char>('x');
	return format_uint<4, Char>(it, value, num_digits);
  };
  return specs ? write_padded<align::right>(out, *specs, size, write)
			   : base_iterator(out, write(reserve(out, size)));
}

template <typename T> struct is_integral : std::is_integral<T> {};
template <> struct is_integral<int128_t> : std::true_type {};
template <> struct is_integral<uint128_t> : std::true_type {};

template <typename Char, typename OutputIt>
OutputIt write(OutputIt out, monostate) {
  FMT_ASSERT(false, "");
  return out;
}

template <typename Char, typename OutputIt,
		  FMT_ENABLE_IF(!std::is_same<Char, char>::value)>
OutputIt write(OutputIt out, string_view value) {
  auto it = reserve(out, value.size());
  it = copy_str<Char>(value.begin(), value.end(), it);
  return base_iterator(out, it);
}

template <typename Char, typename OutputIt>
OutputIt write(OutputIt out, basic_string_view<Char> value) {
  auto it = reserve(out, value.size());
  it = std::copy(value.begin(), value.end(), it);
  return base_iterator(out, it);
}

template <typename Char>
buffer_appender<Char> write(buffer_appender<Char> out,
							basic_string_view<Char> value) {
  get_container(out).append(value.begin(), value.end());
  return out;
}

template <typename Char, typename OutputIt, typename T,
		  FMT_ENABLE_IF(is_integral<T>::value &&
						!std::is_same<T, bool>::value &&
						!std::is_same<T, Char>::value)>
OutputIt write(OutputIt out, T value) {
  auto abs_value = static_cast<uint32_or_64_or_128_t<T>>(value);
  bool negative = is_negative(value);
  // Don't do -abs_value since it trips unsigned-integer-overflow sanitizer.
  if (negative) abs_value = ~abs_value + 1;
  int num_digits = count_digits(abs_value);
  auto size = (negative ? 1 : 0) + static_cast<size_t>(num_digits);
  auto it = reserve(out, size);
  if (auto ptr = to_pointer<Char>(it, size)) {
	if (negative) *ptr++ = static_cast<Char>('-');
	format_decimal<Char>(ptr, abs_value, num_digits);
	return out;
  }
  if (negative) *it++ = static_cast<Char>('-');
  it = format_decimal<Char>(it, abs_value, num_digits).end;
  return base_iterator(out, it);
}

template <typename Char, typename OutputIt>
OutputIt write(OutputIt out, bool value) {
  return write<Char>(out, string_view(value ? "true" : "false"));
}

template <typename Char, typename OutputIt>
OutputIt write(OutputIt out, Char value) {
  auto it = reserve(out, 1);
  *it++ = value;
  return base_iterator(out, it);
}

template <typename Char, typename OutputIt>
OutputIt write(OutputIt out, const Char* value) {
  if (!value) {
	FMT_THROW(format_error("string pointer is null"));
  } else {
	auto length = std::char_traits<Char>::length(value);
	out = write(out, basic_string_view<Char>(value, length));
  }
  return out;
}

template <typename Char, typename OutputIt>
OutputIt write(OutputIt out, const void* value) {
  return write_ptr<Char>(out, to_uintptr(value), nullptr);
}

template <typename Char, typename OutputIt, typename T>
auto write(OutputIt out, const T& value) -> typename std::enable_if<
	mapped_type_constant<T, basic_format_context<OutputIt, Char>>::value ==
		type::custom_type,
	OutputIt>::type {
  using context_type = basic_format_context<OutputIt, Char>;
  using formatter_type =
	  conditional_t<has_formatter<T, context_type>::value,
					typename context_type::template formatter_type<T>,
					fallback_formatter<T, Char>>;
  context_type ctx(out, {}, {});
  return formatter_type().format(value, ctx);
}

// An argument visitor that formats the argument and writes it via the output
// iterator. It's a class and not a generic lambda for compatibility with C++11.
template <typename OutputIt, typename Char> struct default_arg_formatter {
  using context = basic_format_context<OutputIt, Char>;

  OutputIt out;
  basic_format_args<context> args;
  locale_ref loc;

  template <typename T> OutputIt operator()(T value) {
	return write<Char>(out, value);
  }

  OutputIt operator()(typename basic_format_arg<context>::handle handle) {
	basic_format_parse_context<Char> parse_ctx({});
	basic_format_context<OutputIt, Char> format_ctx(out, args, loc);
	handle.format(parse_ctx, format_ctx);
	return format_ctx.out();
  }
};

template <typename OutputIt, typename Char,
		  typename ErrorHandler = error_handler>
class arg_formatter_base {
 public:
  using iterator = OutputIt;
  using char_type = Char;
  using format_specs = basic_format_specs<Char>;

 private:
  iterator out_;
  locale_ref locale_;
  format_specs* specs_;

  // Attempts to reserve space for n extra characters in the output range.
  // Returns a pointer to the reserved range or a reference to out_.
  auto reserve(size_t n) -> decltype(detail::reserve(out_, n)) {
	return detail::reserve(out_, n);
  }

  using reserve_iterator = remove_reference_t<decltype(
	  detail::reserve(std::declval<iterator&>(), 0))>;

  template <typename T> void write_int(T value, const format_specs& spec) {
	using uint_type = uint32_or_64_or_128_t<T>;
	int_writer<iterator, Char, uint_type> w(out_, locale_, value, spec);
	handle_int_type_spec(spec.type, w);
	out_ = w.out;
  }

  void write(char value) {
	auto&& it = reserve(1);
	*it++ = value;
  }

  template <typename Ch, FMT_ENABLE_IF(std::is_same<Ch, Char>::value)>
  void write(Ch value) {
	out_ = detail::write<Char>(out_, value);
  }

  void write(string_view value) {
	auto&& it = reserve(value.size());
	it = copy_str<Char>(value.begin(), value.end(), it);
  }
  void write(wstring_view value) {
	static_assert(std::is_same<Char, wchar_t>::value, "");
	auto&& it = reserve(value.size());
	it = std::copy(value.begin(), value.end(), it);
  }

  template <typename Ch>
  void write(const Ch* s, size_t size, const format_specs& specs) {
	auto width = specs.width != 0
					 ? count_code_points(basic_string_view<Ch>(s, size))
					 : 0;
	out_ = write_padded(out_, specs, size, width, [=](reserve_iterator it) {
	  return copy_str<Char>(s, s + size, it);
	});
  }

  template <typename Ch>
  void write(basic_string_view<Ch> s, const format_specs& specs = {}) {
	out_ = detail::write(out_, s, specs);
  }

  void write_pointer(const void* p) {
	out_ = write_ptr<char_type>(out_, to_uintptr(p), specs_);
  }

  struct char_spec_handler : ErrorHandler {
	arg_formatter_base& formatter;
	Char value;

	char_spec_handler(arg_formatter_base& f, Char val)
		: formatter(f), value(val) {}

	void on_int() {
	  // char is only formatted as int if there are specs.
	  formatter.write_int(static_cast<int>(value), *formatter.specs_);
	}
	void on_char() {
	  if (formatter.specs_)
		formatter.out_ = write_char(formatter.out_, value, *formatter.specs_);
	  else
		formatter.write(value);
	}
  };

  struct cstring_spec_handler : error_handler {
	arg_formatter_base& formatter;
	const Char* value;

	cstring_spec_handler(arg_formatter_base& f, const Char* val)
		: formatter(f), value(val) {}

	void on_string() { formatter.write(value); }
	void on_pointer() { formatter.write_pointer(value); }
  };

 protected:
  iterator out() { return out_; }
  format_specs* specs() { return specs_; }

  void write(bool value) {
	if (specs_)
	  write(string_view(value ? "true" : "false"), *specs_);
	else
	  out_ = detail::write<Char>(out_, value);
  }

  void write(const Char* value) {
	if (!value) {
	  FMT_THROW(format_error("string pointer is null"));
	} else {
	  auto length = std::char_traits<char_type>::length(value);
	  basic_string_view<char_type> sv(value, length);
	  specs_ ? write(sv, *specs_) : write(sv);
	}
  }

 public:
  arg_formatter_base(OutputIt out, format_specs* s, locale_ref loc)
	  : out_(out), locale_(loc), specs_(s) {}

  iterator operator()(monostate) {
	FMT_ASSERT(false, "invalid argument type");
	return out_;
  }

  template <typename T, FMT_ENABLE_IF(is_integral<T>::value)>
  FMT_INLINE iterator operator()(T value) {
	if (specs_)
	  write_int(value, *specs_);
	else
	  out_ = detail::write<Char>(out_, value);
	return out_;
  }

  iterator operator()(Char value) {
	handle_char_specs(specs_,
					  char_spec_handler(*this, static_cast<Char>(value)));
	return out_;
  }

  iterator operator()(bool value) {
	if (specs_ && specs_->type) return (*this)(value ? 1 : 0);
	write(value != 0);
	return out_;
  }

  template <typename T, FMT_ENABLE_IF(std::is_floating_point<T>::value)>
  iterator operator()(T value) {
	auto specs = specs_ ? *specs_ : format_specs();
	if (const_check(is_supported_floating_point(value)))
	  out_ = detail::write(out_, value, specs, locale_);
	else
	  FMT_ASSERT(false, "unsupported float argument type");
	return out_;
  }

  iterator operator()(const Char* value) {
	if (!specs_) return write(value), out_;
	handle_cstring_type_spec(specs_->type, cstring_spec_handler(*this, value));
	return out_;
  }

  iterator operator()(basic_string_view<Char> value) {
	if (specs_) {
	  check_string_type_spec(specs_->type, error_handler());
	  write(value, *specs_);
	} else {
	  write(value);
	}
	return out_;
  }

  iterator operator()(const void* value) {
	if (specs_) check_pointer_type_spec(specs_->type, error_handler());
	write_pointer(value);
	return out_;
  }
};

/** The default argument formatter. */
template <typename OutputIt, typename Char>
class arg_formatter : public arg_formatter_base<OutputIt, Char> {
 private:
  using char_type = Char;
  using base = arg_formatter_base<OutputIt, Char>;
  using context_type = basic_format_context<OutputIt, Char>;

  context_type& ctx_;
  basic_format_parse_context<char_type>* parse_ctx_;
  const Char* ptr_;

 public:
  using iterator = typename base::iterator;
  using format_specs = typename base::format_specs;

  /**
	\rst
	Constructs an argument formatter object.
	*ctx* is a reference to the formatting context,
	*specs* contains format specifier information for standard argument types.
	\endrst
   */
  explicit arg_formatter(
	  context_type& ctx,
	  basic_format_parse_context<char_type>* parse_ctx = nullptr,
	  format_specs* specs = nullptr, const Char* ptr = nullptr)
	  : base(ctx.out(), specs, ctx.locale()),
		ctx_(ctx),
		parse_ctx_(parse_ctx),
		ptr_(ptr) {}

  using base::operator();

  /** Formats an argument of a user-defined type. */
  iterator operator()(typename basic_format_arg<context_type>::handle handle) {
	if (ptr_) advance_to(*parse_ctx_, ptr_);
	handle.format(*parse_ctx_, ctx_);
	return ctx_.out();
  }
};

template <typename Char> FMT_CONSTEXPR bool is_name_start(Char c) {
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || '_' == c;
}

// Parses the range [begin, end) as an unsigned integer. This function assumes
// that the range is non-empty and the first character is a digit.
template <typename Char, typename ErrorHandler>
FMT_CONSTEXPR int parse_nonnegative_int(const Char*& begin, const Char* end,
										ErrorHandler&& eh) {
  FMT_ASSERT(begin != end && '0' <= *begin && *begin <= '9', "");
  unsigned value = 0;
  // Convert to unsigned to prevent a warning.
  constexpr unsigned max_int = max_value<int>();
  unsigned big = max_int / 10;
  do {
	// Check for overflow.
	if (value > big) {
	  value = max_int + 1;
	  break;
	}
	value = value * 10 + unsigned(*begin - '0');
	++begin;
  } while (begin != end && '0' <= *begin && *begin <= '9');
  if (value > max_int) eh.on_error("number is too big");
  return static_cast<int>(value);
}

template <typename Context> class custom_formatter {
 private:
  using char_type = typename Context::char_type;

  basic_format_parse_context<char_type>& parse_ctx_;
  Context& ctx_;

 public:
  explicit custom_formatter(basic_format_parse_context<char_type>& parse_ctx,
							Context& ctx)
	  : parse_ctx_(parse_ctx), ctx_(ctx) {}

  void operator()(typename basic_format_arg<Context>::handle h) const {
	h.format(parse_ctx_, ctx_);
  }

  template <typename T> void operator()(T) const {}
};

template <typename T>
using is_integer =
	bool_constant<is_integral<T>::value && !std::is_same<T, bool>::value &&
				  !std::is_same<T, char>::value &&
				  !std::is_same<T, wchar_t>::value>;

template <typename ErrorHandler> class width_checker {
 public:
  explicit FMT_CONSTEXPR width_checker(ErrorHandler& eh) : handler_(eh) {}

  template <typename T, FMT_ENABLE_IF(is_integer<T>::value)>
  FMT_CONSTEXPR unsigned long long operator()(T value) {
	if (is_negative(value)) handler_.on_error("negative width");
	return static_cast<unsigned long long>(value);
  }

  template <typename T, FMT_ENABLE_IF(!is_integer<T>::value)>
  FMT_CONSTEXPR unsigned long long operator()(T) {
	handler_.on_error("width is not integer");
	return 0;
  }

 private:
  ErrorHandler& handler_;
};

template <typename ErrorHandler> class precision_checker {
 public:
  explicit FMT_CONSTEXPR precision_checker(ErrorHandler& eh) : handler_(eh) {}

  template <typename T, FMT_ENABLE_IF(is_integer<T>::value)>
  FMT_CONSTEXPR unsigned long long operator()(T value) {
	if (is_negative(value)) handler_.on_error("negative precision");
	return static_cast<unsigned long long>(value);
  }

  template <typename T, FMT_ENABLE_IF(!is_integer<T>::value)>
  FMT_CONSTEXPR unsigned long long operator()(T) {
	handler_.on_error("precision is not integer");
	return 0;
  }

 private:
  ErrorHandler& handler_;
};

// A format specifier handler that sets fields in basic_format_specs.
template <typename Char> class specs_setter {
 public:
  explicit FMT_CONSTEXPR specs_setter(basic_format_specs<Char>& specs)
	  : specs_(specs) {}

  FMT_CONSTEXPR specs_setter(const specs_setter& other)
	  : specs_(other.specs_) {}

  FMT_CONSTEXPR void on_align(align_t align) { specs_.align = align; }
  FMT_CONSTEXPR void on_fill(basic_string_view<Char> fill) {
	specs_.fill = fill;
  }
  FMT_CONSTEXPR void on_plus() { specs_.sign = sign::plus; }
  FMT_CONSTEXPR void on_minus() { specs_.sign = sign::minus; }
  FMT_CONSTEXPR void on_space() { specs_.sign = sign::space; }
  FMT_CONSTEXPR void on_hash() { specs_.alt = true; }

  FMT_CONSTEXPR void on_zero() {
	specs_.align = align::numeric;
	specs_.fill[0] = Char('0');
  }

  FMT_CONSTEXPR void on_width(int width) { specs_.width = width; }
  FMT_CONSTEXPR void on_precision(int precision) {
	specs_.precision = precision;
  }
  FMT_CONSTEXPR void end_precision() {}

  FMT_CONSTEXPR void on_type(Char type) {
	specs_.type = static_cast<char>(type);
  }

 protected:
  basic_format_specs<Char>& specs_;
};

template <typename ErrorHandler> class numeric_specs_checker {
 public:
  FMT_CONSTEXPR numeric_specs_checker(ErrorHandler& eh, detail::type arg_type)
	  : error_handler_(eh), arg_type_(arg_type) {}

  FMT_CONSTEXPR void require_numeric_argument() {
	if (!is_arithmetic_type(arg_type_))
	  error_handler_.on_error("format specifier requires numeric argument");
  }

  FMT_CONSTEXPR void check_sign() {
	require_numeric_argument();
	if (is_integral_type(arg_type_) && arg_type_ != type::int_type &&
		arg_type_ != type::long_long_type && arg_type_ != type::char_type) {
	  error_handler_.on_error("format specifier requires signed argument");
	}
  }

  FMT_CONSTEXPR void check_precision() {
	if (is_integral_type(arg_type_) || arg_type_ == type::pointer_type)
	  error_handler_.on_error("precision not allowed for this argument type");
  }

 private:
  ErrorHandler& error_handler_;
  detail::type arg_type_;
};

// A format specifier handler that checks if specifiers are consistent with the
// argument type.
template <typename Handler> class specs_checker : public Handler {
 private:
  numeric_specs_checker<Handler> checker_;

  // Suppress an MSVC warning about using this in initializer list.
  FMT_CONSTEXPR Handler& error_handler() { return *this; }

 public:
  FMT_CONSTEXPR specs_checker(const Handler& handler, detail::type arg_type)
	  : Handler(handler), checker_(error_handler(), arg_type) {}

  FMT_CONSTEXPR specs_checker(const specs_checker& other)
	  : Handler(other), checker_(error_handler(), other.arg_type_) {}

  FMT_CONSTEXPR void on_align(align_t align) {
	if (align == align::numeric) checker_.require_numeric_argument();
	Handler::on_align(align);
  }

  FMT_CONSTEXPR void on_plus() {
	checker_.check_sign();
	Handler::on_plus();
  }

  FMT_CONSTEXPR void on_minus() {
	checker_.check_sign();
	Handler::on_minus();
  }

  FMT_CONSTEXPR void on_space() {
	checker_.check_sign();
	Handler::on_space();
  }

  FMT_CONSTEXPR void on_hash() {
	checker_.require_numeric_argument();
	Handler::on_hash();
  }

  FMT_CONSTEXPR void on_zero() {
	checker_.require_numeric_argument();
	Handler::on_zero();
  }

  FMT_CONSTEXPR void end_precision() { checker_.check_precision(); }
};

template <template <typename> class Handler, typename FormatArg,
		  typename ErrorHandler>
FMT_CONSTEXPR int get_dynamic_spec(FormatArg arg, ErrorHandler eh) {
  unsigned long long value = visit_format_arg(Handler<ErrorHandler>(eh), arg);
  if (value > to_unsigned(max_value<int>())) eh.on_error("number is too big");
  return static_cast<int>(value);
}

struct auto_id {};

template <typename Context, typename ID>
FMT_CONSTEXPR typename Context::format_arg get_arg(Context& ctx, ID id) {
  auto arg = ctx.arg(id);
  if (!arg) ctx.on_error("argument not found");
  return arg;
}

// The standard format specifier handler with checking.
template <typename ParseContext, typename Context>
class specs_handler : public specs_setter<typename Context::char_type> {
 public:
  using char_type = typename Context::char_type;

  FMT_CONSTEXPR specs_handler(basic_format_specs<char_type>& specs,
							  ParseContext& parse_ctx, Context& ctx)
	  : specs_setter<char_type>(specs),
		parse_context_(parse_ctx),
		context_(ctx) {}

  template <typename Id> FMT_CONSTEXPR void on_dynamic_width(Id arg_id) {
	this->specs_.width = get_dynamic_spec<width_checker>(
		get_arg(arg_id), context_.error_handler());
  }

  template <typename Id> FMT_CONSTEXPR void on_dynamic_precision(Id arg_id) {
	this->specs_.precision = get_dynamic_spec<precision_checker>(
		get_arg(arg_id), context_.error_handler());
  }

  void on_error(const char* message) { context_.on_error(message); }

 private:
  // This is only needed for compatibility with gcc 4.4.
  using format_arg = typename Context::format_arg;

  FMT_CONSTEXPR format_arg get_arg(auto_id) {
	return detail::get_arg(context_, parse_context_.next_arg_id());
  }

  FMT_CONSTEXPR format_arg get_arg(int arg_id) {
	parse_context_.check_arg_id(arg_id);
	return detail::get_arg(context_, arg_id);
  }

  FMT_CONSTEXPR format_arg get_arg(basic_string_view<char_type> arg_id) {
	parse_context_.check_arg_id(arg_id);
	return detail::get_arg(context_, arg_id);
  }

  ParseContext& parse_context_;
  Context& context_;
};

enum class arg_id_kind { none, index, name };

// An argument reference.
template <typename Char> struct arg_ref {
  FMT_CONSTEXPR arg_ref() : kind(arg_id_kind::none), val() {}

  FMT_CONSTEXPR explicit arg_ref(int index)
	  : kind(arg_id_kind::index), val(index) {}
  FMT_CONSTEXPR explicit arg_ref(basic_string_view<Char> name)
	  : kind(arg_id_kind::name), val(name) {}

  FMT_CONSTEXPR arg_ref& operator=(int idx) {
	kind = arg_id_kind::index;
	val.index = idx;
	return *this;
  }

  arg_id_kind kind;
  union value {
	FMT_CONSTEXPR value(int id = 0) : index{id} {}
	FMT_CONSTEXPR value(basic_string_view<Char> n) : name(n) {}

	int index;
	basic_string_view<Char> name;
  } val;
};

// Format specifiers with width and precision resolved at formatting rather
// than parsing time to allow re-using the same parsed specifiers with
// different sets of arguments (precompilation of format strings).
template <typename Char>
struct dynamic_format_specs : basic_format_specs<Char> {
  arg_ref<Char> width_ref;
  arg_ref<Char> precision_ref;
};

// Format spec handler that saves references to arguments representing dynamic
// width and precision to be resolved at formatting time.
template <typename ParseContext>
class dynamic_specs_handler
	: public specs_setter<typename ParseContext::char_type> {
 public:
  using char_type = typename ParseContext::char_type;

  FMT_CONSTEXPR dynamic_specs_handler(dynamic_format_specs<char_type>& specs,
									  ParseContext& ctx)
	  : specs_setter<char_type>(specs), specs_(specs), context_(ctx) {}

  FMT_CONSTEXPR dynamic_specs_handler(const dynamic_specs_handler& other)
	  : specs_setter<char_type>(other),
		specs_(other.specs_),
		context_(other.context_) {}

  template <typename Id> FMT_CONSTEXPR void on_dynamic_width(Id arg_id) {
	specs_.width_ref = make_arg_ref(arg_id);
  }

  template <typename Id> FMT_CONSTEXPR void on_dynamic_precision(Id arg_id) {
	specs_.precision_ref = make_arg_ref(arg_id);
  }

  FMT_CONSTEXPR void on_error(const char* message) {
	context_.on_error(message);
  }

 private:
  using arg_ref_type = arg_ref<char_type>;

  FMT_CONSTEXPR arg_ref_type make_arg_ref(int arg_id) {
	context_.check_arg_id(arg_id);
	return arg_ref_type(arg_id);
  }

  FMT_CONSTEXPR arg_ref_type make_arg_ref(auto_id) {
	return arg_ref_type(context_.next_arg_id());
  }

  FMT_CONSTEXPR arg_ref_type make_arg_ref(basic_string_view<char_type> arg_id) {
	context_.check_arg_id(arg_id);
	basic_string_view<char_type> format_str(
		context_.begin(), to_unsigned(context_.end() - context_.begin()));
	return arg_ref_type(arg_id);
  }

  dynamic_format_specs<char_type>& specs_;
  ParseContext& context_;
};

template <typename Char, typename IDHandler>
FMT_CONSTEXPR const Char* parse_arg_id(const Char* begin, const Char* end,
									   IDHandler&& handler) {
  FMT_ASSERT(begin != end, "");
  Char c = *begin;
  if (c == '}' || c == ':') {
	handler();
	return begin;
  }
  if (c >= '0' && c <= '9') {
	int index = 0;
	if (c != '0')
	  index = parse_nonnegative_int(begin, end, handler);
	else
	  ++begin;
	if (begin == end || (*begin != '}' && *begin != ':'))
	  handler.on_error("invalid format string");
	else
	  handler(index);
	return begin;
  }
  if (!is_name_start(c)) {
	handler.on_error("invalid format string");
	return begin;
  }
  auto it = begin;
  do {
	++it;
  } while (it != end && (is_name_start(c = *it) || ('0' <= c && c <= '9')));
  handler(basic_string_view<Char>(begin, to_unsigned(it - begin)));
  return it;
}

// Adapts SpecHandler to IDHandler API for dynamic width.
template <typename SpecHandler, typename Char> struct width_adapter {
  explicit FMT_CONSTEXPR width_adapter(SpecHandler& h) : handler(h) {}

  FMT_CONSTEXPR void operator()() { handler.on_dynamic_width(auto_id()); }
  FMT_CONSTEXPR void operator()(int id) { handler.on_dynamic_width(id); }
  FMT_CONSTEXPR void operator()(basic_string_view<Char> id) {
	handler.on_dynamic_width(id);
  }

  FMT_CONSTEXPR void on_error(const char* message) {
	handler.on_error(message);
  }

  SpecHandler& handler;
};

// Adapts SpecHandler to IDHandler API for dynamic precision.
template <typename SpecHandler, typename Char> struct precision_adapter {
  explicit FMT_CONSTEXPR precision_adapter(SpecHandler& h) : handler(h) {}

  FMT_CONSTEXPR void operator()() { handler.on_dynamic_precision(auto_id()); }
  FMT_CONSTEXPR void operator()(int id) { handler.on_dynamic_precision(id); }
  FMT_CONSTEXPR void operator()(basic_string_view<Char> id) {
	handler.on_dynamic_precision(id);
  }

  FMT_CONSTEXPR void on_error(const char* message) {
	handler.on_error(message);
  }

  SpecHandler& handler;
};

template <typename Char>
FMT_CONSTEXPR int code_point_length(const Char* begin) {
  if (const_check(sizeof(Char) != 1)) return 1;
  constexpr char lengths[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
							  0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 4, 0};
  int len = lengths[static_cast<unsigned char>(*begin) >> 3];

  // Compute the pointer to the next character early so that the next
  // iteration can start working on the next character. Neither Clang
  // nor GCC figure out this reordering on their own.
  return len + !len;
}

template <typename Char> constexpr bool is_ascii_letter(Char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// Converts a character to ASCII. Returns a number > 127 on conversion failure.
template <typename Char, FMT_ENABLE_IF(std::is_integral<Char>::value)>
constexpr Char to_ascii(Char value) {
  return value;
}
template <typename Char, FMT_ENABLE_IF(std::is_enum<Char>::value)>
constexpr typename std::underlying_type<Char>::type to_ascii(Char value) {
  return value;
}

// Parses fill and alignment.
template <typename Char, typename Handler>
FMT_CONSTEXPR const Char* parse_align(const Char* begin, const Char* end,
									  Handler&& handler) {
  FMT_ASSERT(begin != end, "");
  auto align = align::none;
  auto p = begin + code_point_length(begin);
  if (p >= end) p = begin;
  for (;;) {
	switch (to_ascii(*p)) {
	case '<':
	  align = align::left;
	  break;
	case '>':
	  align = align::right;
	  break;
#if FMT_DEPRECATED_NUMERIC_ALIGN
	case '=':
	  align = align::numeric;
	  break;
#endif
	case '^':
	  align = align::center;
	  break;
	}
	if (align != align::none) {
	  if (p != begin) {
		auto c = *begin;
		if (c == '{')
		  return handler.on_error("invalid fill character '{'"), begin;
		handler.on_fill(basic_string_view<Char>(begin, to_unsigned(p - begin)));
		begin = p + 1;
	  } else
		++begin;
	  handler.on_align(align);
	  break;
	} else if (p == begin) {
	  break;
	}
	p = begin;
  }
  return begin;
}

template <typename Char, typename Handler>
FMT_CONSTEXPR const Char* parse_width(const Char* begin, const Char* end,
									  Handler&& handler) {
  FMT_ASSERT(begin != end, "");
  if ('0' <= *begin && *begin <= '9') {
	handler.on_width(parse_nonnegative_int(begin, end, handler));
  } else if (*begin == '{') {
	++begin;
	if (begin != end)
	  begin = parse_arg_id(begin, end, width_adapter<Handler, Char>(handler));
	if (begin == end || *begin != '}')
	  return handler.on_error("invalid format string"), begin;
	++begin;
  }
  return begin;
}

template <typename Char, typename Handler>
FMT_CONSTEXPR const Char* parse_precision(const Char* begin, const Char* end,
										  Handler&& handler) {
  ++begin;
  auto c = begin != end ? *begin : Char();
  if ('0' <= c && c <= '9') {
	handler.on_precision(parse_nonnegative_int(begin, end, handler));
  } else if (c == '{') {
	++begin;
	if (begin != end) {
	  begin =
		  parse_arg_id(begin, end, precision_adapter<Handler, Char>(handler));
	}
	if (begin == end || *begin++ != '}')
	  return handler.on_error("invalid format string"), begin;
  } else {
	return handler.on_error("missing precision specifier"), begin;
  }
  handler.end_precision();
  return begin;
}

// Parses standard format specifiers and sends notifications about parsed
// components to handler.
template <typename Char, typename SpecHandler>
FMT_CONSTEXPR const Char* parse_format_specs(const Char* begin, const Char* end,
											 SpecHandler&& handler) {
  if (begin == end) return begin;

  begin = parse_align(begin, end, handler);
  if (begin == end) return begin;

  // Parse sign.
  switch (to_ascii(*begin)) {
  case '+':
	handler.on_plus();
	++begin;
	break;
  case '-':
	handler.on_minus();
	++begin;
	break;
  case ' ':
	handler.on_space();
	++begin;
	break;
  }
  if (begin == end) return begin;

  if (*begin == '#') {
	handler.on_hash();
	if (++begin == end) return begin;
  }

  // Parse zero flag.
  if (*begin == '0') {
	handler.on_zero();
	if (++begin == end) return begin;
  }

  begin = parse_width(begin, end, handler);
  if (begin == end) return begin;

  // Parse precision.
  if (*begin == '.') {
	begin = parse_precision(begin, end, handler);
  }

  // Parse type.
  if (begin != end && *begin != '}') handler.on_type(*begin++);
  return begin;
}

// Return the result via the out param to workaround gcc bug 77539.
template <bool IS_CONSTEXPR, typename T, typename Ptr = const T*>
FMT_CONSTEXPR bool find(Ptr first, Ptr last, T value, Ptr& out) {
  for (out = first; out != last; ++out) {
	if (*out == value) return true;
  }
  return false;
}

template <>
inline bool find<false, char>(const char* first, const char* last, char value,
							  const char*& out) {
  out = static_cast<const char*>(
	  std::memchr(first, value, detail::to_unsigned(last - first)));
  return out != nullptr;
}

template <typename Handler, typename Char> struct id_adapter {
  Handler& handler;
  int arg_id;

  FMT_CONSTEXPR void operator()() { arg_id = handler.on_arg_id(); }
  FMT_CONSTEXPR void operator()(int id) { arg_id = handler.on_arg_id(id); }
  FMT_CONSTEXPR void operator()(basic_string_view<Char> id) {
	arg_id = handler.on_arg_id(id);
  }
  FMT_CONSTEXPR void on_error(const char* message) {
	handler.on_error(message);
  }
};

template <typename Char, typename Handler>
FMT_CONSTEXPR const Char* parse_replacement_field(const Char* begin,
												  const Char* end,
												  Handler&& handler) {
  ++begin;
  if (begin == end) return handler.on_error("invalid format string"), end;
  if (*begin == '}') {
	handler.on_replacement_field(handler.on_arg_id(), begin);
  } else if (*begin == '{') {
	handler.on_text(begin, begin + 1);
  } else {
	auto adapter = id_adapter<Handler, Char>{handler, 0};
	begin = parse_arg_id(begin, end, adapter);
	Char c = begin != end ? *begin : Char();
	if (c == '}') {
	  handler.on_replacement_field(adapter.arg_id, begin);
	} else if (c == ':') {
	  begin = handler.on_format_specs(adapter.arg_id, begin + 1, end);
	  if (begin == end || *begin != '}')
		return handler.on_error("unknown format specifier"), end;
	} else {
	  return handler.on_error("missing '}' in format string"), end;
	}
  }
  return begin + 1;
}

template <bool IS_CONSTEXPR, typename Char, typename Handler>
FMT_CONSTEXPR_DECL FMT_INLINE void parse_format_string(
	basic_string_view<Char> format_str, Handler&& handler) {
  auto begin = format_str.data();
  auto end = begin + format_str.size();
  if (end - begin < 32) {
	// Use a simple loop instead of memchr for small strings.
	const Char* p = begin;
	while (p != end) {
	  auto c = *p++;
	  if (c == '{') {
		handler.on_text(begin, p - 1);
		begin = p = parse_replacement_field(p - 1, end, handler);
	  } else if (c == '}') {
		if (p == end || *p != '}')
		  return handler.on_error("unmatched '}' in format string");
		handler.on_text(begin, p);
		begin = ++p;
	  }
	}
	handler.on_text(begin, end);
	return;
  }
  struct writer {
	FMT_CONSTEXPR void operator()(const Char* pbegin, const Char* pend) {
	  if (pbegin == pend) return;
	  for (;;) {
		const Char* p = nullptr;
		if (!find<IS_CONSTEXPR>(pbegin, pend, '}', p))
		  return handler_.on_text(pbegin, pend);
		++p;
		if (p == pend || *p != '}')
		  return handler_.on_error("unmatched '}' in format string");
		handler_.on_text(pbegin, p);
		pbegin = p + 1;
	  }
	}
	Handler& handler_;
  } write{handler};
  while (begin != end) {
	// Doing two passes with memchr (one for '{' and another for '}') is up to
	// 2.5x faster than the naive one-pass implementation on big format strings.
	const Char* p = begin;
	if (*begin != '{' && !find<IS_CONSTEXPR>(begin + 1, end, '{', p))
	  return write(begin, end);
	write(begin, p);
	begin = parse_replacement_field(p, end, handler);
  }
}

template <typename T, typename ParseContext>
FMT_CONSTEXPR const typename ParseContext::char_type* parse_format_specs(
	ParseContext& ctx) {
  using char_type = typename ParseContext::char_type;
  using context = buffer_context<char_type>;
  using mapped_type =
	  conditional_t<detail::mapped_type_constant<T, context>::value !=
						type::custom_type,
					decltype(arg_mapper<context>().map(std::declval<T>())), T>;
  auto f = conditional_t<has_formatter<mapped_type, context>::value,
						 formatter<mapped_type, char_type>,
						 detail::fallback_formatter<T, char_type>>();
  return f.parse(ctx);
}

template <typename OutputIt, typename Char, typename Context>
struct format_handler : detail::error_handler {
  basic_format_parse_context<Char> parse_context;
  Context context;

  format_handler(OutputIt out, basic_string_view<Char> str,
				 basic_format_args<Context> format_args, detail::locale_ref loc)
	  : parse_context(str), context(out, format_args, loc) {}

  void on_text(const Char* begin, const Char* end) {
	auto size = to_unsigned(end - begin);
	auto out = context.out();
	auto&& it = reserve(out, size);
	it = std::copy_n(begin, size, it);
	context.advance_to(out);
  }

  int on_arg_id() { return parse_context.next_arg_id(); }
  int on_arg_id(int id) { return parse_context.check_arg_id(id), id; }
  int on_arg_id(basic_string_view<Char> id) {
	int arg_id = context.arg_id(id);
	if (arg_id < 0) on_error("argument not found");
	return arg_id;
  }

  FMT_INLINE void on_replacement_field(int id, const Char*) {
	auto arg = get_arg(context, id);
	context.advance_to(visit_format_arg(
		default_arg_formatter<OutputIt, Char>{context.out(), context.args(),
											  context.locale()},
		arg));
  }

  const Char* on_format_specs(int id, const Char* begin, const Char* end) {
	auto arg = get_arg(context, id);
	if (arg.type() == type::custom_type) {
	  advance_to(parse_context, begin);
	  visit_format_arg(custom_formatter<Context>(parse_context, context), arg);
	  return parse_context.begin();
	}
	auto specs = basic_format_specs<Char>();
	if (begin + 1 < end && begin[1] == '}' && is_ascii_letter(*begin)) {
	  specs.type = static_cast<char>(*begin++);
	} else {
	  using parse_context_t = basic_format_parse_context<Char>;
	  specs_checker<specs_handler<parse_context_t, Context>> handler(
		  specs_handler<parse_context_t, Context>(specs, parse_context,
												  context),
		  arg.type());
	  begin = parse_format_specs(begin, end, handler);
	  if (begin == end || *begin != '}')
		on_error("missing '}' in format string");
	}
	context.advance_to(visit_format_arg(
		arg_formatter<OutputIt, Char>(context, &parse_context, &specs), arg));
	return begin;
  }
};

// A parse context with extra argument id checks. It is only used at compile
// time because adding checks at runtime would introduce substantial overhead
// and would be redundant since argument ids are checked when arguments are
// retrieved anyway.
template <typename Char, typename ErrorHandler = error_handler>
class compile_parse_context
	: public basic_format_parse_context<Char, ErrorHandler> {
 private:
  int num_args_;
  using base = basic_format_parse_context<Char, ErrorHandler>;

 public:
  explicit FMT_CONSTEXPR compile_parse_context(
	  basic_string_view<Char> format_str, int num_args = max_value<int>(),
	  ErrorHandler eh = {})
	  : base(format_str, eh), num_args_(num_args) {}

  FMT_CONSTEXPR int next_arg_id() {
	int id = base::next_arg_id();
	if (id >= num_args_) this->on_error("argument not found");
	return id;
  }

  FMT_CONSTEXPR void check_arg_id(int id) {
	base::check_arg_id(id);
	if (id >= num_args_) this->on_error("argument not found");
  }
  using base::check_arg_id;
};

template <typename Char, typename ErrorHandler, typename... Args>
class format_string_checker {
 public:
  explicit FMT_CONSTEXPR format_string_checker(
	  basic_string_view<Char> format_str, ErrorHandler eh)
	  : context_(format_str, num_args, eh),
		parse_funcs_{&parse_format_specs<Args, parse_context_type>...} {}

  FMT_CONSTEXPR void on_text(const Char*, const Char*) {}

  FMT_CONSTEXPR int on_arg_id() { return context_.next_arg_id(); }
  FMT_CONSTEXPR int on_arg_id(int id) { return context_.check_arg_id(id), id; }
  FMT_CONSTEXPR int on_arg_id(basic_string_view<Char>) {
	on_error("compile-time checks don't support named arguments");
	return 0;
  }

  FMT_CONSTEXPR void on_replacement_field(int, const Char*) {}

  FMT_CONSTEXPR const Char* on_format_specs(int id, const Char* begin,
											const Char*) {
	advance_to(context_, begin);
	return id < num_args ? parse_funcs_[id](context_) : begin;
  }

  FMT_CONSTEXPR void on_error(const char* message) {
	context_.on_error(message);
  }

 private:
  using parse_context_type = compile_parse_context<Char, ErrorHandler>;
  enum { num_args = sizeof...(Args) };

  // Format specifier parsing function.
  using parse_func = const Char* (*)(parse_context_type&);

  parse_context_type context_;
  parse_func parse_funcs_[num_args > 0 ? num_args : 1];
};

// Converts string literals to basic_string_view.
template <typename Char, size_t N>
FMT_CONSTEXPR basic_string_view<Char> compile_string_to_view(
	const Char (&s)[N]) {
  // Remove trailing null character if needed. Won't be present if this is used
  // with raw character array (i.e. not defined as a string).
  return {s,
		  N - ((std::char_traits<Char>::to_int_type(s[N - 1]) == 0) ? 1 : 0)};
}

// Converts string_view to basic_string_view.
template <typename Char>
FMT_CONSTEXPR basic_string_view<Char> compile_string_to_view(
	const std_string_view<Char>& s) {
  return {s.data(), s.size()};
}

#define FMT_STRING_IMPL(s, base)                                  \
  [] {                                                            \
	/* Use a macro-like name to avoid shadowing warnings. */      \
	struct FMT_COMPILE_STRING : base {                            \
	  using char_type = fmt::remove_cvref_t<decltype(s[0])>;      \
	  FMT_MAYBE_UNUSED FMT_CONSTEXPR                              \
	  operator fmt::basic_string_view<char_type>() const {        \
		return fmt::detail::compile_string_to_view<char_type>(s); \
	  }                                                           \
	};                                                            \
	return FMT_COMPILE_STRING();                                  \
  }()

/**
  \rst
  Constructs a compile-time format string from a string literal *s*.

  **Example**::

	// A compile-time error because 'd' is an invalid specifier for strings.
	std::string s = fmt::format(FMT_STRING("{:d}"), "foo");
  \endrst
 */
#define FMT_STRING(s) FMT_STRING_IMPL(s, fmt::compile_string)

template <typename... Args, typename S,
		  enable_if_t<(is_compile_string<S>::value), int>>
void check_format_string(S format_str) {
  FMT_CONSTEXPR_DECL auto s = to_string_view(format_str);
  using checker = format_string_checker<typename S::char_type, error_handler,
										remove_cvref_t<Args>...>;
  FMT_CONSTEXPR_DECL bool invalid_format =
	  (parse_format_string<true>(s, checker(s, {})), true);
  (void)invalid_format;
}

template <template <typename> class Handler, typename Context>
void handle_dynamic_spec(int& value, arg_ref<typename Context::char_type> ref,
						 Context& ctx) {
  switch (ref.kind) {
  case arg_id_kind::none:
	break;
  case arg_id_kind::index:
	value = detail::get_dynamic_spec<Handler>(ctx.arg(ref.val.index),
											  ctx.error_handler());
	break;
  case arg_id_kind::name:
	value = detail::get_dynamic_spec<Handler>(ctx.arg(ref.val.name),
											  ctx.error_handler());
	break;
  }
}

using format_func = void (*)(detail::buffer<char>&, int, string_view);

FMT_API void format_error_code(buffer<char>& out, int error_code,
							   string_view message) FMT_NOEXCEPT;

FMT_API void report_error(format_func func, int error_code,
						  string_view message) FMT_NOEXCEPT;
}  // namespace detail

template <typename OutputIt, typename Char>
using arg_formatter FMT_DEPRECATED_ALIAS =
	detail::arg_formatter<OutputIt, Char>;

/**
 An error returned by an operating system or a language runtime,
 for example a file opening error.
*/
FMT_CLASS_API
class FMT_API system_error : public std::runtime_error {
 private:
  void init(int err_code, string_view format_str, format_args args);

 protected:
  int error_code_;

  system_error() : std::runtime_error(""), error_code_(0) {}

 public:
  /**
   \rst
   Constructs a :class:`fmt::system_error` object with a description
   formatted with `fmt::format_system_error`. *message* and additional
   arguments passed into the constructor are formatted similarly to
   `fmt::format`.

   **Example**::

	 // This throws a system_error with the description
	 //   cannot open file 'madeup': No such file or directory
	 // or similar (system message may vary).
	 const char *filename = "madeup";
	 std::FILE *file = std::fopen(filename, "r");
	 if (!file)
	   throw fmt::system_error(errno, "cannot open file '{}'", filename);
   \endrst
  */
  template <typename... Args>
  system_error(int error_code, string_view message, const Args&... args)
	  : std::runtime_error("") {
	init(error_code, message, make_format_args(args...));
  }
  system_error(const system_error&) = default;
  system_error& operator=(const system_error&) = default;
  system_error(system_error&&) = default;
  system_error& operator=(system_error&&) = default;
  ~system_error() FMT_NOEXCEPT FMT_OVERRIDE;

  int error_code() const { return error_code_; }
};

/**
  \rst
  Formats an error returned by an operating system or a language runtime,
  for example a file opening error, and writes it to *out* in the following
  form:

  .. parsed-literal::
	 *<message>*: *<system-message>*

  where *<message>* is the passed message and *<system-message>* is
  the system message corresponding to the error code.
  *error_code* is a system error code as given by ``errno``.
  If *error_code* is not a valid error code such as -1, the system message
  may look like "Unknown error -1" and is platform-dependent.
  \endrst
 */
FMT_API void format_system_error(detail::buffer<char>& out, int error_code,
								 string_view message) FMT_NOEXCEPT;

// Reports a system error without throwing an exception.
// Can be used to report errors from destructors.
FMT_API void report_system_error(int error_code,
								 string_view message) FMT_NOEXCEPT;

/** Fast integer formatter. */
class format_int {
 private:
  // Buffer should be large enough to hold all digits (digits10 + 1),
  // a sign and a null character.
  enum { buffer_size = std::numeric_limits<unsigned long long>::digits10 + 3 };
  mutable char buffer_[buffer_size];
  char* str_;

  template <typename UInt> char* format_unsigned(UInt value) {
	auto n = static_cast<detail::uint32_or_64_or_128_t<UInt>>(value);
	return detail::format_decimal(buffer_, n, buffer_size - 1).begin;
  }

  template <typename Int> char* format_signed(Int value) {
	auto abs_value = static_cast<detail::uint32_or_64_or_128_t<Int>>(value);
	bool negative = value < 0;
	if (negative) abs_value = 0 - abs_value;
	auto begin = format_unsigned(abs_value);
	if (negative) *--begin = '-';
	return begin;
  }

 public:
  explicit format_int(int value) : str_(format_signed(value)) {}
  explicit format_int(long value) : str_(format_signed(value)) {}
  explicit format_int(long long value) : str_(format_signed(value)) {}
  explicit format_int(unsigned value) : str_(format_unsigned(value)) {}
  explicit format_int(unsigned long value) : str_(format_unsigned(value)) {}
  explicit format_int(unsigned long long value)
	  : str_(format_unsigned(value)) {}

  /** Returns the number of characters written to the output buffer. */
  size_t size() const {
	return detail::to_unsigned(buffer_ - str_ + buffer_size - 1);
  }

  /**
	Returns a pointer to the output buffer content. No terminating null
	character is appended.
   */
  const char* data() const { return str_; }

  /**
	Returns a pointer to the output buffer content with terminating null
	character appended.
   */
  const char* c_str() const {
	buffer_[buffer_size - 1] = '\0';
	return str_;
  }

  /**
	\rst
	Returns the content of the output buffer as an ``std::string``.
	\endrst
   */
  std::string str() const { return std::string(str_, size()); }
};

// A formatter specialization for the core types corresponding to detail::type
// constants.
template <typename T, typename Char>
struct formatter<T, Char,
				 enable_if_t<detail::type_constant<T, Char>::value !=
							 detail::type::custom_type>> {
  FMT_CONSTEXPR formatter() = default;

  // Parses format specifiers stopping either at the end of the range or at the
  // terminating '}'.
  template <typename ParseContext>
  FMT_CONSTEXPR auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
	using handler_type = detail::dynamic_specs_handler<ParseContext>;
	auto type = detail::type_constant<T, Char>::value;
	detail::specs_checker<handler_type> handler(handler_type(specs_, ctx),
												type);
	auto it = parse_format_specs(ctx.begin(), ctx.end(), handler);
	auto eh = ctx.error_handler();
	switch (type) {
	case detail::type::none_type:
	  FMT_ASSERT(false, "invalid argument type");
	  break;
	case detail::type::int_type:
	case detail::type::uint_type:
	case detail::type::long_long_type:
	case detail::type::ulong_long_type:
	case detail::type::int128_type:
	case detail::type::uint128_type:
	case detail::type::bool_type:
	  handle_int_type_spec(specs_.type,
						   detail::int_type_checker<decltype(eh)>(eh));
	  break;
	case detail::type::char_type:
	  handle_char_specs(
		  &specs_, detail::char_specs_checker<decltype(eh)>(specs_.type, eh));
	  break;
	case detail::type::float_type:
	  if (detail::const_check(FMT_USE_FLOAT))
		detail::parse_float_type_spec(specs_, eh);
	  else
		FMT_ASSERT(false, "float support disabled");
	  break;
	case detail::type::double_type:
	  if (detail::const_check(FMT_USE_DOUBLE))
		detail::parse_float_type_spec(specs_, eh);
	  else
		FMT_ASSERT(false, "double support disabled");
	  break;
	case detail::type::long_double_type:
	  if (detail::const_check(FMT_USE_LONG_DOUBLE))
		detail::parse_float_type_spec(specs_, eh);
	  else
		FMT_ASSERT(false, "long double support disabled");
	  break;
	case detail::type::cstring_type:
	  detail::handle_cstring_type_spec(
		  specs_.type, detail::cstring_type_checker<decltype(eh)>(eh));
	  break;
	case detail::type::string_type:
	  detail::check_string_type_spec(specs_.type, eh);
	  break;
	case detail::type::pointer_type:
	  detail::check_pointer_type_spec(specs_.type, eh);
	  break;
	case detail::type::custom_type:
	  // Custom format specifiers should be checked in parse functions of
	  // formatter specializations.
	  break;
	}
	return it;
  }

  template <typename FormatContext>
  auto format(const T& val, FormatContext& ctx) -> decltype(ctx.out()) {
	detail::handle_dynamic_spec<detail::width_checker>(specs_.width,
													   specs_.width_ref, ctx);
	detail::handle_dynamic_spec<detail::precision_checker>(
		specs_.precision, specs_.precision_ref, ctx);
	using af = detail::arg_formatter<typename FormatContext::iterator,
									 typename FormatContext::char_type>;
	return visit_format_arg(af(ctx, nullptr, &specs_),
							detail::make_arg<FormatContext>(val));
  }

 private:
  detail::dynamic_format_specs<Char> specs_;
};

#define FMT_FORMAT_AS(Type, Base)                                             \
  template <typename Char>                                                    \
  struct formatter<Type, Char> : formatter<Base, Char> {                      \
	template <typename FormatContext>                                         \
	auto format(Type const& val, FormatContext& ctx) -> decltype(ctx.out()) { \
	  return formatter<Base, Char>::format(val, ctx);                         \
	}                                                                         \
  }

FMT_FORMAT_AS(signed char, int);
FMT_FORMAT_AS(unsigned char, unsigned);
FMT_FORMAT_AS(short, int);
FMT_FORMAT_AS(unsigned short, unsigned);
FMT_FORMAT_AS(long, long long);
FMT_FORMAT_AS(unsigned long, unsigned long long);
FMT_FORMAT_AS(Char*, const Char*);
FMT_FORMAT_AS(std::basic_string<Char>, basic_string_view<Char>);
FMT_FORMAT_AS(std::nullptr_t, const void*);
FMT_FORMAT_AS(detail::std_string_view<Char>, basic_string_view<Char>);

template <typename Char>
struct formatter<void*, Char> : formatter<const void*, Char> {
  template <typename FormatContext>
  auto format(void* val, FormatContext& ctx) -> decltype(ctx.out()) {
	return formatter<const void*, Char>::format(val, ctx);
  }
};

template <typename Char, size_t N>
struct formatter<Char[N], Char> : formatter<basic_string_view<Char>, Char> {
  template <typename FormatContext>
  auto format(const Char* val, FormatContext& ctx) -> decltype(ctx.out()) {
	return formatter<basic_string_view<Char>, Char>::format(val, ctx);
  }
};

// A formatter for types known only at run time such as variant alternatives.
//
// Usage:
//   using variant = std::variant<int, std::string>;
//   template <>
//   struct formatter<variant>: dynamic_formatter<> {
//     auto format(const variant& v, format_context& ctx) {
//       return visit([&](const auto& val) {
//           return dynamic_formatter<>::format(val, ctx);
//       }, v);
//     }
//   };
template <typename Char = char> class dynamic_formatter {
 private:
  struct null_handler : detail::error_handler {
	void on_align(align_t) {}
	void on_plus() {}
	void on_minus() {}
	void on_space() {}
	void on_hash() {}
  };

 public:
  template <typename ParseContext>
  auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
	format_str_ = ctx.begin();
	// Checks are deferred to formatting time when the argument type is known.
	detail::dynamic_specs_handler<ParseContext> handler(specs_, ctx);
	return parse_format_specs(ctx.begin(), ctx.end(), handler);
  }

  template <typename T, typename FormatContext>
  auto format(const T& val, FormatContext& ctx) -> decltype(ctx.out()) {
	handle_specs(ctx);
	detail::specs_checker<null_handler> checker(
		null_handler(), detail::mapped_type_constant<T, FormatContext>::value);
	checker.on_align(specs_.align);
	switch (specs_.sign) {
	case sign::none:
	  break;
	case sign::plus:
	  checker.on_plus();
	  break;
	case sign::minus:
	  checker.on_minus();
	  break;
	case sign::space:
	  checker.on_space();
	  break;
	}
	if (specs_.alt) checker.on_hash();
	if (specs_.precision >= 0) checker.end_precision();
	using af = detail::arg_formatter<typename FormatContext::iterator,
									 typename FormatContext::char_type>;
	visit_format_arg(af(ctx, nullptr, &specs_),
					 detail::make_arg<FormatContext>(val));
	return ctx.out();
  }

 private:
  template <typename Context> void handle_specs(Context& ctx) {
	detail::handle_dynamic_spec<detail::width_checker>(specs_.width,
													   specs_.width_ref, ctx);
	detail::handle_dynamic_spec<detail::precision_checker>(
		specs_.precision, specs_.precision_ref, ctx);
  }

  detail::dynamic_format_specs<Char> specs_;
  const Char* format_str_;
};

template <typename Char, typename ErrorHandler>
FMT_CONSTEXPR void advance_to(
	basic_format_parse_context<Char, ErrorHandler>& ctx, const Char* p) {
  ctx.advance_to(ctx.begin() + (p - &*ctx.begin()));
}

/**
  \rst
  Converts ``p`` to ``const void*`` for pointer formatting.

  **Example**::

	auto s = fmt::format("{}", fmt::ptr(p));
  \endrst
 */
template <typename T> inline const void* ptr(const T* p) { return p; }
template <typename T> inline const void* ptr(const std::unique_ptr<T>& p) {
  return p.get();
}
template <typename T> inline const void* ptr(const std::shared_ptr<T>& p) {
  return p.get();
}

class bytes {
 private:
  string_view data_;
  friend struct formatter<bytes>;

 public:
  explicit bytes(string_view data) : data_(data) {}
};

template <> struct formatter<bytes> {
 private:
  detail::dynamic_format_specs<char> specs_;

 public:
  template <typename ParseContext>
  FMT_CONSTEXPR auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
	using handler_type = detail::dynamic_specs_handler<ParseContext>;
	detail::specs_checker<handler_type> handler(handler_type(specs_, ctx),
												detail::type::string_type);
	auto it = parse_format_specs(ctx.begin(), ctx.end(), handler);
	detail::check_string_type_spec(specs_.type, ctx.error_handler());
	return it;
  }

  template <typename FormatContext>
  auto format(bytes b, FormatContext& ctx) -> decltype(ctx.out()) {
	detail::handle_dynamic_spec<detail::width_checker>(specs_.width,
													   specs_.width_ref, ctx);
	detail::handle_dynamic_spec<detail::precision_checker>(
		specs_.precision, specs_.precision_ref, ctx);
	return detail::write_bytes(ctx.out(), b.data_, specs_);
  }
};

template <typename It, typename Sentinel, typename Char>
struct arg_join : detail::view {
  It begin;
  Sentinel end;
  basic_string_view<Char> sep;

  arg_join(It b, Sentinel e, basic_string_view<Char> s)
	  : begin(b), end(e), sep(s) {}
};

template <typename It, typename Sentinel, typename Char>
struct formatter<arg_join<It, Sentinel, Char>, Char>
	: formatter<typename std::iterator_traits<It>::value_type, Char> {
  template <typename FormatContext>
  auto format(const arg_join<It, Sentinel, Char>& value, FormatContext& ctx)
	  -> decltype(ctx.out()) {
	using base = formatter<typename std::iterator_traits<It>::value_type, Char>;
	auto it = value.begin;
	auto out = ctx.out();
	if (it != value.end) {
	  out = base::format(*it++, ctx);
	  while (it != value.end) {
		out = std::copy(value.sep.begin(), value.sep.end(), out);
		ctx.advance_to(out);
		out = base::format(*it++, ctx);
	  }
	}
	return out;
  }
};

/**
  Returns an object that formats the iterator range `[begin, end)` with elements
  separated by `sep`.
 */
template <typename It, typename Sentinel>
arg_join<It, Sentinel, char> join(It begin, Sentinel end, string_view sep) {
  return {begin, end, sep};
}

template <typename It, typename Sentinel>
arg_join<It, Sentinel, wchar_t> join(It begin, Sentinel end, wstring_view sep) {
  return {begin, end, sep};
}

/**
  \rst
  Returns an object that formats `range` with elements separated by `sep`.

  **Example**::

	std::vector<int> v = {1, 2, 3};
	fmt::print("{}", fmt::join(v, ", "));
	// Output: "1, 2, 3"

  ``fmt::join`` applies passed format specifiers to the range elements::

	fmt::print("{:02}", fmt::join(v, ", "));
	// Output: "01, 02, 03"
  \endrst
 */
template <typename Range>
arg_join<detail::iterator_t<Range>, detail::sentinel_t<Range>, char> join(
	Range&& range, string_view sep) {
  return join(std::begin(range), std::end(range), sep);
}

template <typename Range>
arg_join<detail::iterator_t<Range>, detail::sentinel_t<Range>, wchar_t> join(
	Range&& range, wstring_view sep) {
  return join(std::begin(range), std::end(range), sep);
}

/**
  \rst
  Converts *value* to ``std::string`` using the default format for type *T*.

  **Example**::

	#include <dmformat.h>

	std::string answer = fmt::to_string(42);
  \endrst
 */
template <typename T, FMT_ENABLE_IF(!std::is_integral<T>::value)>
inline std::string to_string(const T& value) {
  std::string result;
  detail::write<char>(std::back_inserter(result), value);
  return result;
}

template <typename T, FMT_ENABLE_IF(std::is_integral<T>::value)>
inline std::string to_string(T value) {
  // The buffer should be large enough to store the number including the sign or
  // "false" for bool.
  constexpr int max_size = detail::digits10<T>() + 2;
  char buffer[max_size > 5 ? static_cast<unsigned>(max_size) : 5];
  char* begin = buffer;
  return std::string(begin, detail::write<char>(begin, value));
}

/**
  Converts *value* to ``std::wstring`` using the default format for type *T*.
 */
template <typename T> inline std::wstring to_wstring(const T& value) {
  return format(L"{}", value);
}

template <typename Char, size_t SIZE>
std::basic_string<Char> to_string(const basic_memory_buffer<Char, SIZE>& buf) {
  auto size = buf.size();
  detail::assume(size < std::basic_string<Char>().max_size());
  return std::basic_string<Char>(buf.data(), size);
}

template <typename Char>
void detail::vformat_to(
	detail::buffer<Char>& buf, basic_string_view<Char> format_str,
	basic_format_args<buffer_context<type_identity_t<Char>>> args,
	detail::locale_ref loc) {
  using iterator = typename buffer_context<Char>::iterator;
  auto out = buffer_appender<Char>(buf);
  if (format_str.size() == 2 && equal2(format_str.data(), "{}")) {
	auto arg = args.get(0);
	if (!arg) error_handler().on_error("argument not found");
	visit_format_arg(default_arg_formatter<iterator, Char>{out, args, loc},
					 arg);
	return;
  }
  format_handler<iterator, Char, buffer_context<Char>> h(out, format_str, args,
														 loc);
  parse_format_string<false>(format_str, h);
}

#ifndef FMT_HEADER_ONLY
extern template void detail::vformat_to(detail::buffer<char>&, string_view,
										basic_format_args<format_context>,
										detail::locale_ref);
namespace detail {

extern template FMT_API std::string grouping_impl<char>(locale_ref loc);
extern template FMT_API std::string grouping_impl<wchar_t>(locale_ref loc);
extern template FMT_API char thousands_sep_impl<char>(locale_ref loc);
extern template FMT_API wchar_t thousands_sep_impl<wchar_t>(locale_ref loc);
extern template FMT_API char decimal_point_impl(locale_ref loc);
extern template FMT_API wchar_t decimal_point_impl(locale_ref loc);
extern template int format_float<double>(double value, int precision,
										 float_specs specs, buffer<char>& buf);
extern template int format_float<long double>(long double value, int precision,
											  float_specs specs,
											  buffer<char>& buf);
int snprintf_float(float value, int precision, float_specs specs,
				   buffer<char>& buf) = delete;
extern template int snprintf_float<double>(double value, int precision,
										   float_specs specs,
										   buffer<char>& buf);
extern template int snprintf_float<long double>(long double value,
												int precision,
												float_specs specs,
												buffer<char>& buf);
}  // namespace detail
#endif

template <typename S, typename Char = char_t<S>,
		  FMT_ENABLE_IF(detail::is_string<S>::value)>
inline void vformat_to(
	detail::buffer<Char>& buf, const S& format_str,
	basic_format_args<FMT_BUFFER_CONTEXT(type_identity_t<Char>)> args) {
  return detail::vformat_to(buf, to_string_view(format_str), args);
}

template <typename S, typename... Args, size_t SIZE = inline_buffer_size,
		  typename Char = enable_if_t<detail::is_string<S>::value, char_t<S>>>
inline typename buffer_context<Char>::iterator format_to(
	basic_memory_buffer<Char, SIZE>& buf, const S& format_str, Args&&... args) {
  const auto& vargs = fmt::make_args_checked<Args...>(format_str, args...);
  detail::vformat_to(buf, to_string_view(format_str), vargs);
  return detail::buffer_appender<Char>(buf);
}

template <typename OutputIt, typename Char = char>
using format_context_t = basic_format_context<OutputIt, Char>;

template <typename OutputIt, typename Char = char>
using format_args_t = basic_format_args<format_context_t<OutputIt, Char>>;

template <typename OutputIt, typename Char = typename OutputIt::value_type>
using format_to_n_context FMT_DEPRECATED_ALIAS = buffer_context<Char>;

template <typename OutputIt, typename Char = typename OutputIt::value_type>
using format_to_n_args FMT_DEPRECATED_ALIAS =
	basic_format_args<buffer_context<Char>>;

template <typename OutputIt, typename Char, typename... Args>
FMT_DEPRECATED format_arg_store<buffer_context<Char>, Args...>
make_format_to_n_args(const Args&... args) {
  return format_arg_store<buffer_context<Char>, Args...>(args...);
}

template <typename Char, enable_if_t<(!std::is_same<Char, char>::value), int>>
std::basic_string<Char> detail::vformat(
	basic_string_view<Char> format_str,
	basic_format_args<buffer_context<type_identity_t<Char>>> args) {
  basic_memory_buffer<Char> buffer;
  detail::vformat_to(buffer, format_str, args);
  return to_string(buffer);
}

template <typename Char, FMT_ENABLE_IF(std::is_same<Char, wchar_t>::value)>
void vprint(std::FILE* f, basic_string_view<Char> format_str,
			wformat_args args) {
  wmemory_buffer buffer;
  detail::vformat_to(buffer, format_str, args);
  buffer.push_back(L'\0');
  if (std::fputws(buffer.data(), f) == -1)
	FMT_THROW(system_error(errno, "cannot write to file"));
}

template <typename Char, FMT_ENABLE_IF(std::is_same<Char, wchar_t>::value)>
void vprint(basic_string_view<Char> format_str, wformat_args args) {
  vprint(stdout, format_str, args);
}

#if FMT_USE_USER_DEFINED_LITERALS
namespace detail {

#  if FMT_USE_UDL_TEMPLATE
template <typename Char, Char... CHARS> class udl_formatter {
 public:
  template <typename... Args>
  std::basic_string<Char> operator()(Args&&... args) const {
	static FMT_CONSTEXPR_DECL Char s[] = {CHARS..., '\0'};
	return format(FMT_STRING(s), std::forward<Args>(args)...);
  }
};
#  else
template <typename Char> struct udl_formatter {
  basic_string_view<Char> str;

  template <typename... Args>
  std::basic_string<Char> operator()(Args&&... args) const {
	return format(str, std::forward<Args>(args)...);
  }
};
#  endif  // FMT_USE_UDL_TEMPLATE

template <typename Char> struct udl_arg {
  const Char* str;

  template <typename T> named_arg<Char, T> operator=(T&& value) const {
	return {str, std::forward<T>(value)};
  }
};
}  // namespace detail

inline namespace literals {
#  if FMT_USE_UDL_TEMPLATE
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wpedantic"
#    if FMT_CLANG_VERSION
#      pragma GCC diagnostic ignored "-Wgnu-string-literal-operator-template"
#    endif
template <typename Char, Char... CHARS>
FMT_CONSTEXPR detail::udl_formatter<Char, CHARS...> operator""_format() {
  return {};
}
#    pragma GCC diagnostic pop
#  else
/**
  \rst
  User-defined literal equivalent of :func:`fmt::format`.

  **Example**::

	using namespace fmt::literals;
	std::string message = "The answer is {}"_format(42);
  \endrst
 */
FMT_CONSTEXPR detail::udl_formatter<char> operator"" _format(const char* s,
															 size_t n) {
  return { {s, n} };
}
FMT_CONSTEXPR detail::udl_formatter<wchar_t> operator"" _format(
	const wchar_t* s, size_t n) {
  return { {s, n} };
}
#  endif  // FMT_USE_UDL_TEMPLATE

/**
  \rst
  User-defined literal equivalent of :func:`fmt::arg`.

  **Example**::

	using namespace fmt::literals;
	fmt::print("Elapsed time: {s:.2f} seconds", "s"_a=1.23);
  \endrst
 */
FMT_CONSTEXPR detail::udl_arg<char> operator"" _a(const char* s, size_t) {
  return {s};
}
FMT_CONSTEXPR detail::udl_arg<wchar_t> operator"" _a(const wchar_t* s, size_t) {
  return {s};
}
}  // namespace literals
#endif  // FMT_USE_USER_DEFINED_LITERALS
FMT_END_NAMESPACE

#ifdef FMT_HEADER_ONLY
#  define FMT_FUNC inline

/*** Start of inlined file: dmformat-inl.h ***/
// Formatting library for C++ - implementation
//
// Copyright (c) 2012 - 2016, Victor Zverovich
// All rights reserved.
//
// For the license information refer to format.h.

#ifndef FMT_FORMAT_INL_H_
#define FMT_FORMAT_INL_H_

#include <cassert>
#include <cctype>
#include <climits>
#include <cmath>
#include <cstdarg>
#include <cstring>  // std::memmove
#include <cwchar>
#include <exception>

#ifndef FMT_STATIC_THOUSANDS_SEPARATOR
#  include <locale>
#endif

#ifdef _WIN32
#  include <io.h>  // _isatty
#endif

// Dummy implementations of strerror_r and strerror_s called if corresponding
// system functions are not available.
inline fmt::detail::null<> strerror_r(int, char*, ...) { return {}; }
inline fmt::detail::null<> strerror_s(char*, size_t, ...) { return {}; }

FMT_BEGIN_NAMESPACE
namespace detail {

FMT_FUNC void assert_fail(const char* file, int line, const char* message) {
  // Use unchecked std::fprintf to avoid triggering another assertion when
  // writing to stderr fails
  std::fprintf(stderr, "%s:%d: assertion failed: %s", file, line, message);
  // Chosen instead of std::abort to satisfy Clang in CUDA mode during device
  // code pass.
  std::terminate();
}

#ifndef _MSC_VER
#  define FMT_SNPRINTF snprintf
#else  // _MSC_VER
inline int fmt_snprintf(char* buffer, size_t size, const char* format, ...) {
  va_list args;
  va_start(args, format);
  int result = vsnprintf_s(buffer, size, _TRUNCATE, format, args);
  va_end(args);
  return result;
}
#  define FMT_SNPRINTF fmt_snprintf
#endif  // _MSC_VER

// A portable thread-safe version of strerror.
// Sets buffer to point to a string describing the error code.
// This can be either a pointer to a string stored in buffer,
// or a pointer to some static immutable string.
// Returns one of the following values:
//   0      - success
//   ERANGE - buffer is not large enough to store the error message
//   other  - failure
// Buffer should be at least of size 1.
inline int safe_strerror(int error_code, char*& buffer,
						 size_t buffer_size) FMT_NOEXCEPT {
  FMT_ASSERT(buffer != nullptr && buffer_size != 0, "invalid buffer");

  class dispatcher {
   private:
	int error_code_;
	char*& buffer_;
	size_t buffer_size_;

	// A noop assignment operator to avoid bogus warnings.
	void operator=(const dispatcher&) {}

	// Handle the result of XSI-compliant version of strerror_r.
	int handle(int result) {
	  // glibc versions before 2.13 return result in errno.
	  return result == -1 ? errno : result;
	}

	// Handle the result of GNU-specific version of strerror_r.
	FMT_MAYBE_UNUSED
	int handle(char* message) {
	  // If the buffer is full then the message is probably truncated.
	  if (message == buffer_ && strlen(buffer_) == buffer_size_ - 1)
		return ERANGE;
	  buffer_ = message;
	  return 0;
	}

	// Handle the case when strerror_r is not available.
	FMT_MAYBE_UNUSED
	int handle(detail::null<>) {
	  return fallback(strerror_s(buffer_, buffer_size_, error_code_));
	}

	// Fallback to strerror_s when strerror_r is not available.
	FMT_MAYBE_UNUSED
	int fallback(int result) {
	  // If the buffer is full then the message is probably truncated.
	  return result == 0 && strlen(buffer_) == buffer_size_ - 1 ? ERANGE
																: result;
	}

#if !FMT_MSC_VER
	// Fallback to strerror if strerror_r and strerror_s are not available.
	int fallback(detail::null<>) {
	  errno = 0;
	  buffer_ = strerror(error_code_);
	  return errno;
	}
#endif

   public:
	dispatcher(int err_code, char*& buf, size_t buf_size)
		: error_code_(err_code), buffer_(buf), buffer_size_(buf_size) {}

	int run() { return handle(strerror_r(error_code_, buffer_, buffer_size_)); }
  };
  return dispatcher(error_code, buffer, buffer_size).run();
}

FMT_FUNC void format_error_code(detail::buffer<char>& out, int error_code,
								string_view message) FMT_NOEXCEPT {
  // Report error code making sure that the output fits into
  // inline_buffer_size to avoid dynamic memory allocation and potential
  // bad_alloc.
  out.try_resize(0);
  static const char SEP[] = ": ";
  static const char ERROR_STR[] = "error ";
  // Subtract 2 to account for terminating null characters in SEP and ERROR_STR.
  size_t error_code_size = sizeof(SEP) + sizeof(ERROR_STR) - 2;
  auto abs_value = static_cast<uint32_or_64_or_128_t<int>>(error_code);
  if (detail::is_negative(error_code)) {
	abs_value = 0 - abs_value;
	++error_code_size;
  }
  error_code_size += detail::to_unsigned(detail::count_digits(abs_value));
  auto it = buffer_appender<char>(out);
  if (message.size() <= inline_buffer_size - error_code_size)
	format_to(it, "{}{}", message, SEP);
  format_to(it, "{}{}", ERROR_STR, error_code);
  assert(out.size() <= inline_buffer_size);
}

FMT_FUNC void report_error(format_func func, int error_code,
						   string_view message) FMT_NOEXCEPT {
  memory_buffer full_message;
  func(full_message, error_code, message);
  // Don't use fwrite_fully because the latter may throw.
  (void)std::fwrite(full_message.data(), full_message.size(), 1, stderr);
  std::fputc('\n', stderr);
}

// A wrapper around fwrite that throws on error.
inline void fwrite_fully(const void* ptr, size_t size, size_t count,
						 FILE* stream) {
  size_t written = std::fwrite(ptr, size, count, stream);
  if (written < count) FMT_THROW(system_error(errno, "cannot write to file"));
}
}  // namespace detail

#if !defined(FMT_STATIC_THOUSANDS_SEPARATOR)
namespace detail {

template <typename Locale>
locale_ref::locale_ref(const Locale& loc) : locale_(&loc) {
  static_assert(std::is_same<Locale, std::locale>::value, "");
}

template <typename Locale> Locale locale_ref::get() const {
  static_assert(std::is_same<Locale, std::locale>::value, "");
  return locale_ ? *static_cast<const std::locale*>(locale_) : std::locale();
}

template <typename Char> FMT_FUNC std::string grouping_impl(locale_ref loc) {
  return std::use_facet<std::numpunct<Char>>(loc.get<std::locale>()).grouping();
}
template <typename Char> FMT_FUNC Char thousands_sep_impl(locale_ref loc) {
  return std::use_facet<std::numpunct<Char>>(loc.get<std::locale>())
	  .thousands_sep();
}
template <typename Char> FMT_FUNC Char decimal_point_impl(locale_ref loc) {
  return std::use_facet<std::numpunct<Char>>(loc.get<std::locale>())
	  .decimal_point();
}
}  // namespace detail
#else
template <typename Char>
FMT_FUNC std::string detail::grouping_impl(locale_ref) {
  return "\03";
}
template <typename Char> FMT_FUNC Char detail::thousands_sep_impl(locale_ref) {
  return FMT_STATIC_THOUSANDS_SEPARATOR;
}
template <typename Char> FMT_FUNC Char detail::decimal_point_impl(locale_ref) {
  return '.';
}
#endif

FMT_API FMT_FUNC format_error::~format_error() FMT_NOEXCEPT = default;
FMT_API FMT_FUNC system_error::~system_error() FMT_NOEXCEPT = default;

FMT_FUNC void system_error::init(int err_code, string_view format_str,
								 format_args args) {
  error_code_ = err_code;
  memory_buffer buffer;
  format_system_error(buffer, err_code, vformat(format_str, args));
  std::runtime_error& base = *this;
  base = std::runtime_error(to_string(buffer));
}

namespace detail {

template <> FMT_FUNC int count_digits<4>(detail::fallback_uintptr n) {
  // fallback_uintptr is always stored in little endian.
  int i = static_cast<int>(sizeof(void*)) - 1;
  while (i > 0 && n.value[i] == 0) --i;
  auto char_digits = std::numeric_limits<unsigned char>::digits / 4;
  return i >= 0 ? i * char_digits + count_digits<4, unsigned>(n.value[i]) : 1;
}

template <typename T>
const typename basic_data<T>::digit_pair basic_data<T>::digits[] = {
	{'0', '0'}, {'0', '1'}, {'0', '2'}, {'0', '3'}, {'0', '4'}, {'0', '5'},
	{'0', '6'}, {'0', '7'}, {'0', '8'}, {'0', '9'}, {'1', '0'}, {'1', '1'},
	{'1', '2'}, {'1', '3'}, {'1', '4'}, {'1', '5'}, {'1', '6'}, {'1', '7'},
	{'1', '8'}, {'1', '9'}, {'2', '0'}, {'2', '1'}, {'2', '2'}, {'2', '3'},
	{'2', '4'}, {'2', '5'}, {'2', '6'}, {'2', '7'}, {'2', '8'}, {'2', '9'},
	{'3', '0'}, {'3', '1'}, {'3', '2'}, {'3', '3'}, {'3', '4'}, {'3', '5'},
	{'3', '6'}, {'3', '7'}, {'3', '8'}, {'3', '9'}, {'4', '0'}, {'4', '1'},
	{'4', '2'}, {'4', '3'}, {'4', '4'}, {'4', '5'}, {'4', '6'}, {'4', '7'},
	{'4', '8'}, {'4', '9'}, {'5', '0'}, {'5', '1'}, {'5', '2'}, {'5', '3'},
	{'5', '4'}, {'5', '5'}, {'5', '6'}, {'5', '7'}, {'5', '8'}, {'5', '9'},
	{'6', '0'}, {'6', '1'}, {'6', '2'}, {'6', '3'}, {'6', '4'}, {'6', '5'},
	{'6', '6'}, {'6', '7'}, {'6', '8'}, {'6', '9'}, {'7', '0'}, {'7', '1'},
	{'7', '2'}, {'7', '3'}, {'7', '4'}, {'7', '5'}, {'7', '6'}, {'7', '7'},
	{'7', '8'}, {'7', '9'}, {'8', '0'}, {'8', '1'}, {'8', '2'}, {'8', '3'},
	{'8', '4'}, {'8', '5'}, {'8', '6'}, {'8', '7'}, {'8', '8'}, {'8', '9'},
	{'9', '0'}, {'9', '1'}, {'9', '2'}, {'9', '3'}, {'9', '4'}, {'9', '5'},
	{'9', '6'}, {'9', '7'}, {'9', '8'}, {'9', '9'} };

template <typename T>
const char basic_data<T>::hex_digits[] = "0123456789abcdef";

#define FMT_POWERS_OF_10(factor)                                             \
  factor * 10, (factor)*100, (factor)*1000, (factor)*10000, (factor)*100000, \
	  (factor)*1000000, (factor)*10000000, (factor)*100000000,               \
	  (factor)*1000000000

template <typename T>
const uint64_t basic_data<T>::powers_of_10_64[] = {
	1, FMT_POWERS_OF_10(1), FMT_POWERS_OF_10(1000000000ULL),
	10000000000000000000ULL};

template <typename T>
const uint32_t basic_data<T>::zero_or_powers_of_10_32[] = {0,
														   FMT_POWERS_OF_10(1)};
template <typename T>
const uint64_t basic_data<T>::zero_or_powers_of_10_64[] = {
	0, FMT_POWERS_OF_10(1), FMT_POWERS_OF_10(1000000000ULL),
	10000000000000000000ULL};

template <typename T>
const uint32_t basic_data<T>::zero_or_powers_of_10_32_new[] = {
	0, 0, FMT_POWERS_OF_10(1)};

template <typename T>
const uint64_t basic_data<T>::zero_or_powers_of_10_64_new[] = {
	0, 0, FMT_POWERS_OF_10(1), FMT_POWERS_OF_10(1000000000ULL),
	10000000000000000000ULL};

// Normalized 64-bit significands of pow(10, k), for k = -348, -340, ..., 340.
// These are generated by support/compute-powers.py.
template <typename T>
const uint64_t basic_data<T>::grisu_pow10_significands[] = {
	0xfa8fd5a0081c0288, 0xbaaee17fa23ebf76, 0x8b16fb203055ac76,
	0xcf42894a5dce35ea, 0x9a6bb0aa55653b2d, 0xe61acf033d1a45df,
	0xab70fe17c79ac6ca, 0xff77b1fcbebcdc4f, 0xbe5691ef416bd60c,
	0x8dd01fad907ffc3c, 0xd3515c2831559a83, 0x9d71ac8fada6c9b5,
	0xea9c227723ee8bcb, 0xaecc49914078536d, 0x823c12795db6ce57,
	0xc21094364dfb5637, 0x9096ea6f3848984f, 0xd77485cb25823ac7,
	0xa086cfcd97bf97f4, 0xef340a98172aace5, 0xb23867fb2a35b28e,
	0x84c8d4dfd2c63f3b, 0xc5dd44271ad3cdba, 0x936b9fcebb25c996,
	0xdbac6c247d62a584, 0xa3ab66580d5fdaf6, 0xf3e2f893dec3f126,
	0xb5b5ada8aaff80b8, 0x87625f056c7c4a8b, 0xc9bcff6034c13053,
	0x964e858c91ba2655, 0xdff9772470297ebd, 0xa6dfbd9fb8e5b88f,
	0xf8a95fcf88747d94, 0xb94470938fa89bcf, 0x8a08f0f8bf0f156b,
	0xcdb02555653131b6, 0x993fe2c6d07b7fac, 0xe45c10c42a2b3b06,
	0xaa242499697392d3, 0xfd87b5f28300ca0e, 0xbce5086492111aeb,
	0x8cbccc096f5088cc, 0xd1b71758e219652c, 0x9c40000000000000,
	0xe8d4a51000000000, 0xad78ebc5ac620000, 0x813f3978f8940984,
	0xc097ce7bc90715b3, 0x8f7e32ce7bea5c70, 0xd5d238a4abe98068,
	0x9f4f2726179a2245, 0xed63a231d4c4fb27, 0xb0de65388cc8ada8,
	0x83c7088e1aab65db, 0xc45d1df942711d9a, 0x924d692ca61be758,
	0xda01ee641a708dea, 0xa26da3999aef774a, 0xf209787bb47d6b85,
	0xb454e4a179dd1877, 0x865b86925b9bc5c2, 0xc83553c5c8965d3d,
	0x952ab45cfa97a0b3, 0xde469fbd99a05fe3, 0xa59bc234db398c25,
	0xf6c69a72a3989f5c, 0xb7dcbf5354e9bece, 0x88fcf317f22241e2,
	0xcc20ce9bd35c78a5, 0x98165af37b2153df, 0xe2a0b5dc971f303a,
	0xa8d9d1535ce3b396, 0xfb9b7cd9a4a7443c, 0xbb764c4ca7a44410,
	0x8bab8eefb6409c1a, 0xd01fef10a657842c, 0x9b10a4e5e9913129,
	0xe7109bfba19c0c9d, 0xac2820d9623bf429, 0x80444b5e7aa7cf85,
	0xbf21e44003acdd2d, 0x8e679c2f5e44ff8f, 0xd433179d9c8cb841,
	0x9e19db92b4e31ba9, 0xeb96bf6ebadf77d9, 0xaf87023b9bf0ee6b,
};

// Binary exponents of pow(10, k), for k = -348, -340, ..., 340, corresponding
// to significands above.
template <typename T>
const int16_t basic_data<T>::grisu_pow10_exponents[] = {
	-1220, -1193, -1166, -1140, -1113, -1087, -1060, -1034, -1007, -980, -954,
	-927,  -901,  -874,  -847,  -821,  -794,  -768,  -741,  -715,  -688, -661,
	-635,  -608,  -582,  -555,  -529,  -502,  -475,  -449,  -422,  -396, -369,
	-343,  -316,  -289,  -263,  -236,  -210,  -183,  -157,  -130,  -103, -77,
	-50,   -24,   3,     30,    56,    83,    109,   136,   162,   189,  216,
	242,   269,   295,   322,   348,   375,   402,   428,   455,   481,  508,
	534,   561,   588,   614,   641,   667,   694,   720,   747,   774,  800,
	827,   853,   880,   907,   933,   960,   986,   1013,  1039,  1066};

template <typename T>
const divtest_table_entry<uint32_t> basic_data<T>::divtest_table_for_pow5_32[] =
	{ {0x00000001, 0xffffffff}, {0xcccccccd, 0x33333333},
	 {0xc28f5c29, 0x0a3d70a3}, {0x26e978d5, 0x020c49ba},
	 {0x3afb7e91, 0x0068db8b}, {0x0bcbe61d, 0x0014f8b5},
	 {0x68c26139, 0x000431bd}, {0xae8d46a5, 0x0000d6bf},
	 {0x22e90e21, 0x00002af3}, {0x3a2e9c6d, 0x00000897},
	 {0x3ed61f49, 0x000001b7} };

template <typename T>
const divtest_table_entry<uint64_t> basic_data<T>::divtest_table_for_pow5_64[] =
	{ {0x0000000000000001, 0xffffffffffffffff},
	 {0xcccccccccccccccd, 0x3333333333333333},
	 {0x8f5c28f5c28f5c29, 0x0a3d70a3d70a3d70},
	 {0x1cac083126e978d5, 0x020c49ba5e353f7c},
	 {0xd288ce703afb7e91, 0x0068db8bac710cb2},
	 {0x5d4e8fb00bcbe61d, 0x0014f8b588e368f0},
	 {0x790fb65668c26139, 0x000431bde82d7b63},
	 {0xe5032477ae8d46a5, 0x0000d6bf94d5e57a},
	 {0xc767074b22e90e21, 0x00002af31dc46118},
	 {0x8e47ce423a2e9c6d, 0x0000089705f4136b},
	 {0x4fa7f60d3ed61f49, 0x000001b7cdfd9d7b},
	 {0x0fee64690c913975, 0x00000057f5ff85e5},
	 {0x3662e0e1cf503eb1, 0x000000119799812d},
	 {0xa47a2cf9f6433fbd, 0x0000000384b84d09},
	 {0x54186f653140a659, 0x00000000b424dc35},
	 {0x7738164770402145, 0x0000000024075f3d},
	 {0xe4a4d1417cd9a041, 0x000000000734aca5},
	 {0xc75429d9e5c5200d, 0x000000000170ef54},
	 {0xc1773b91fac10669, 0x000000000049c977},
	 {0x26b172506559ce15, 0x00000000000ec1e4},
	 {0xd489e3a9addec2d1, 0x000000000002f394},
	 {0x90e860bb892c8d5d, 0x000000000000971d},
	 {0x502e79bf1b6f4f79, 0x0000000000001e39},
	 {0xdcd618596be30fe5, 0x000000000000060b} };

template <typename T>
const uint64_t basic_data<T>::dragonbox_pow10_significands_64[] = {
	0x81ceb32c4b43fcf5, 0xa2425ff75e14fc32, 0xcad2f7f5359a3b3f,
	0xfd87b5f28300ca0e, 0x9e74d1b791e07e49, 0xc612062576589ddb,
	0xf79687aed3eec552, 0x9abe14cd44753b53, 0xc16d9a0095928a28,
	0xf1c90080baf72cb2, 0x971da05074da7bef, 0xbce5086492111aeb,
	0xec1e4a7db69561a6, 0x9392ee8e921d5d08, 0xb877aa3236a4b44a,
	0xe69594bec44de15c, 0x901d7cf73ab0acda, 0xb424dc35095cd810,
	0xe12e13424bb40e14, 0x8cbccc096f5088cc, 0xafebff0bcb24aaff,
	0xdbe6fecebdedd5bf, 0x89705f4136b4a598, 0xabcc77118461cefd,
	0xd6bf94d5e57a42bd, 0x8637bd05af6c69b6, 0xa7c5ac471b478424,
	0xd1b71758e219652c, 0x83126e978d4fdf3c, 0xa3d70a3d70a3d70b,
	0xcccccccccccccccd, 0x8000000000000000, 0xa000000000000000,
	0xc800000000000000, 0xfa00000000000000, 0x9c40000000000000,
	0xc350000000000000, 0xf424000000000000, 0x9896800000000000,
	0xbebc200000000000, 0xee6b280000000000, 0x9502f90000000000,
	0xba43b74000000000, 0xe8d4a51000000000, 0x9184e72a00000000,
	0xb5e620f480000000, 0xe35fa931a0000000, 0x8e1bc9bf04000000,
	0xb1a2bc2ec5000000, 0xde0b6b3a76400000, 0x8ac7230489e80000,
	0xad78ebc5ac620000, 0xd8d726b7177a8000, 0x878678326eac9000,
	0xa968163f0a57b400, 0xd3c21bcecceda100, 0x84595161401484a0,
	0xa56fa5b99019a5c8, 0xcecb8f27f4200f3a, 0x813f3978f8940984,
	0xa18f07d736b90be5, 0xc9f2c9cd04674ede, 0xfc6f7c4045812296,
	0x9dc5ada82b70b59d, 0xc5371912364ce305, 0xf684df56c3e01bc6,
	0x9a130b963a6c115c, 0xc097ce7bc90715b3, 0xf0bdc21abb48db20,
	0x96769950b50d88f4, 0xbc143fa4e250eb31, 0xeb194f8e1ae525fd,
	0x92efd1b8d0cf37be, 0xb7abc627050305ad, 0xe596b7b0c643c719,
	0x8f7e32ce7bea5c6f, 0xb35dbf821ae4f38b, 0xe0352f62a19e306e};

template <typename T>
const uint128_wrapper basic_data<T>::dragonbox_pow10_significands_128[] = {
#if FMT_USE_FULL_CACHE_DRAGONBOX
	{0xff77b1fcbebcdc4f, 0x25e8e89c13bb0f7b},
	{0x9faacf3df73609b1, 0x77b191618c54e9ad},
	{0xc795830d75038c1d, 0xd59df5b9ef6a2418},
	{0xf97ae3d0d2446f25, 0x4b0573286b44ad1e},
	{0x9becce62836ac577, 0x4ee367f9430aec33},
	{0xc2e801fb244576d5, 0x229c41f793cda740},
	{0xf3a20279ed56d48a, 0x6b43527578c11110},
	{0x9845418c345644d6, 0x830a13896b78aaaa},
	{0xbe5691ef416bd60c, 0x23cc986bc656d554},
	{0xedec366b11c6cb8f, 0x2cbfbe86b7ec8aa9},
	{0x94b3a202eb1c3f39, 0x7bf7d71432f3d6aa},
	{0xb9e08a83a5e34f07, 0xdaf5ccd93fb0cc54},
	{0xe858ad248f5c22c9, 0xd1b3400f8f9cff69},
	{0x91376c36d99995be, 0x23100809b9c21fa2},
	{0xb58547448ffffb2d, 0xabd40a0c2832a78b},
	{0xe2e69915b3fff9f9, 0x16c90c8f323f516d},
	{0x8dd01fad907ffc3b, 0xae3da7d97f6792e4},
	{0xb1442798f49ffb4a, 0x99cd11cfdf41779d},
	{0xdd95317f31c7fa1d, 0x40405643d711d584},
	{0x8a7d3eef7f1cfc52, 0x482835ea666b2573},
	{0xad1c8eab5ee43b66, 0xda3243650005eed0},
	{0xd863b256369d4a40, 0x90bed43e40076a83},
	{0x873e4f75e2224e68, 0x5a7744a6e804a292},
	{0xa90de3535aaae202, 0x711515d0a205cb37},
	{0xd3515c2831559a83, 0x0d5a5b44ca873e04},
	{0x8412d9991ed58091, 0xe858790afe9486c3},
	{0xa5178fff668ae0b6, 0x626e974dbe39a873},
	{0xce5d73ff402d98e3, 0xfb0a3d212dc81290},
	{0x80fa687f881c7f8e, 0x7ce66634bc9d0b9a},
	{0xa139029f6a239f72, 0x1c1fffc1ebc44e81},
	{0xc987434744ac874e, 0xa327ffb266b56221},
	{0xfbe9141915d7a922, 0x4bf1ff9f0062baa9},
	{0x9d71ac8fada6c9b5, 0x6f773fc3603db4aa},
	{0xc4ce17b399107c22, 0xcb550fb4384d21d4},
	{0xf6019da07f549b2b, 0x7e2a53a146606a49},
	{0x99c102844f94e0fb, 0x2eda7444cbfc426e},
	{0xc0314325637a1939, 0xfa911155fefb5309},
	{0xf03d93eebc589f88, 0x793555ab7eba27cb},
	{0x96267c7535b763b5, 0x4bc1558b2f3458df},
	{0xbbb01b9283253ca2, 0x9eb1aaedfb016f17},
	{0xea9c227723ee8bcb, 0x465e15a979c1cadd},
	{0x92a1958a7675175f, 0x0bfacd89ec191eca},
	{0xb749faed14125d36, 0xcef980ec671f667c},
	{0xe51c79a85916f484, 0x82b7e12780e7401b},
	{0x8f31cc0937ae58d2, 0xd1b2ecb8b0908811},
	{0xb2fe3f0b8599ef07, 0x861fa7e6dcb4aa16},
	{0xdfbdcece67006ac9, 0x67a791e093e1d49b},
	{0x8bd6a141006042bd, 0xe0c8bb2c5c6d24e1},
	{0xaecc49914078536d, 0x58fae9f773886e19},
	{0xda7f5bf590966848, 0xaf39a475506a899f},
	{0x888f99797a5e012d, 0x6d8406c952429604},
	{0xaab37fd7d8f58178, 0xc8e5087ba6d33b84},
	{0xd5605fcdcf32e1d6, 0xfb1e4a9a90880a65},
	{0x855c3be0a17fcd26, 0x5cf2eea09a550680},
	{0xa6b34ad8c9dfc06f, 0xf42faa48c0ea481f},
	{0xd0601d8efc57b08b, 0xf13b94daf124da27},
	{0x823c12795db6ce57, 0x76c53d08d6b70859},
	{0xa2cb1717b52481ed, 0x54768c4b0c64ca6f},
	{0xcb7ddcdda26da268, 0xa9942f5dcf7dfd0a},
	{0xfe5d54150b090b02, 0xd3f93b35435d7c4d},
	{0x9efa548d26e5a6e1, 0xc47bc5014a1a6db0},
	{0xc6b8e9b0709f109a, 0x359ab6419ca1091c},
	{0xf867241c8cc6d4c0, 0xc30163d203c94b63},
	{0x9b407691d7fc44f8, 0x79e0de63425dcf1e},
	{0xc21094364dfb5636, 0x985915fc12f542e5},
	{0xf294b943e17a2bc4, 0x3e6f5b7b17b2939e},
	{0x979cf3ca6cec5b5a, 0xa705992ceecf9c43},
	{0xbd8430bd08277231, 0x50c6ff782a838354},
	{0xece53cec4a314ebd, 0xa4f8bf5635246429},
	{0x940f4613ae5ed136, 0x871b7795e136be9a},
	{0xb913179899f68584, 0x28e2557b59846e40},
	{0xe757dd7ec07426e5, 0x331aeada2fe589d0},
	{0x9096ea6f3848984f, 0x3ff0d2c85def7622},
	{0xb4bca50b065abe63, 0x0fed077a756b53aa},
	{0xe1ebce4dc7f16dfb, 0xd3e8495912c62895},
	{0x8d3360f09cf6e4bd, 0x64712dd7abbbd95d},
	{0xb080392cc4349dec, 0xbd8d794d96aacfb4},
	{0xdca04777f541c567, 0xecf0d7a0fc5583a1},
	{0x89e42caaf9491b60, 0xf41686c49db57245},
	{0xac5d37d5b79b6239, 0x311c2875c522ced6},
	{0xd77485cb25823ac7, 0x7d633293366b828c},
	{0x86a8d39ef77164bc, 0xae5dff9c02033198},
	{0xa8530886b54dbdeb, 0xd9f57f830283fdfd},
	{0xd267caa862a12d66, 0xd072df63c324fd7c},
	{0x8380dea93da4bc60, 0x4247cb9e59f71e6e},
	{0xa46116538d0deb78, 0x52d9be85f074e609},
	{0xcd795be870516656, 0x67902e276c921f8c},
	{0x806bd9714632dff6, 0x00ba1cd8a3db53b7},
	{0xa086cfcd97bf97f3, 0x80e8a40eccd228a5},
	{0xc8a883c0fdaf7df0, 0x6122cd128006b2ce},
	{0xfad2a4b13d1b5d6c, 0x796b805720085f82},
	{0x9cc3a6eec6311a63, 0xcbe3303674053bb1},
	{0xc3f490aa77bd60fc, 0xbedbfc4411068a9d},
	{0xf4f1b4d515acb93b, 0xee92fb5515482d45},
	{0x991711052d8bf3c5, 0x751bdd152d4d1c4b},
	{0xbf5cd54678eef0b6, 0xd262d45a78a0635e},
	{0xef340a98172aace4, 0x86fb897116c87c35},
	{0x9580869f0e7aac0e, 0xd45d35e6ae3d4da1},
	{0xbae0a846d2195712, 0x8974836059cca10a},
	{0xe998d258869facd7, 0x2bd1a438703fc94c},
	{0x91ff83775423cc06, 0x7b6306a34627ddd0},
	{0xb67f6455292cbf08, 0x1a3bc84c17b1d543},
	{0xe41f3d6a7377eeca, 0x20caba5f1d9e4a94},
	{0x8e938662882af53e, 0x547eb47b7282ee9d},
	{0xb23867fb2a35b28d, 0xe99e619a4f23aa44},
	{0xdec681f9f4c31f31, 0x6405fa00e2ec94d5},
	{0x8b3c113c38f9f37e, 0xde83bc408dd3dd05},
	{0xae0b158b4738705e, 0x9624ab50b148d446},
	{0xd98ddaee19068c76, 0x3badd624dd9b0958},
	{0x87f8a8d4cfa417c9, 0xe54ca5d70a80e5d7},
	{0xa9f6d30a038d1dbc, 0x5e9fcf4ccd211f4d},
	{0xd47487cc8470652b, 0x7647c32000696720},
	{0x84c8d4dfd2c63f3b, 0x29ecd9f40041e074},
	{0xa5fb0a17c777cf09, 0xf468107100525891},
	{0xcf79cc9db955c2cc, 0x7182148d4066eeb5},
	{0x81ac1fe293d599bf, 0xc6f14cd848405531},
	{0xa21727db38cb002f, 0xb8ada00e5a506a7d},
	{0xca9cf1d206fdc03b, 0xa6d90811f0e4851d},
	{0xfd442e4688bd304a, 0x908f4a166d1da664},
	{0x9e4a9cec15763e2e, 0x9a598e4e043287ff},
	{0xc5dd44271ad3cdba, 0x40eff1e1853f29fe},
	{0xf7549530e188c128, 0xd12bee59e68ef47d},
	{0x9a94dd3e8cf578b9, 0x82bb74f8301958cf},
	{0xc13a148e3032d6e7, 0xe36a52363c1faf02},
	{0xf18899b1bc3f8ca1, 0xdc44e6c3cb279ac2},
	{0x96f5600f15a7b7e5, 0x29ab103a5ef8c0ba},
	{0xbcb2b812db11a5de, 0x7415d448f6b6f0e8},
	{0xebdf661791d60f56, 0x111b495b3464ad22},
	{0x936b9fcebb25c995, 0xcab10dd900beec35},
	{0xb84687c269ef3bfb, 0x3d5d514f40eea743},
	{0xe65829b3046b0afa, 0x0cb4a5a3112a5113},
	{0x8ff71a0fe2c2e6dc, 0x47f0e785eaba72ac},
	{0xb3f4e093db73a093, 0x59ed216765690f57},
	{0xe0f218b8d25088b8, 0x306869c13ec3532d},
	{0x8c974f7383725573, 0x1e414218c73a13fc},
	{0xafbd2350644eeacf, 0xe5d1929ef90898fb},
	{0xdbac6c247d62a583, 0xdf45f746b74abf3a},
	{0x894bc396ce5da772, 0x6b8bba8c328eb784},
	{0xab9eb47c81f5114f, 0x066ea92f3f326565},
	{0xd686619ba27255a2, 0xc80a537b0efefebe},
	{0x8613fd0145877585, 0xbd06742ce95f5f37},
	{0xa798fc4196e952e7, 0x2c48113823b73705},
	{0xd17f3b51fca3a7a0, 0xf75a15862ca504c6},
	{0x82ef85133de648c4, 0x9a984d73dbe722fc},
	{0xa3ab66580d5fdaf5, 0xc13e60d0d2e0ebbb},
	{0xcc963fee10b7d1b3, 0x318df905079926a9},
	{0xffbbcfe994e5c61f, 0xfdf17746497f7053},
	{0x9fd561f1fd0f9bd3, 0xfeb6ea8bedefa634},
	{0xc7caba6e7c5382c8, 0xfe64a52ee96b8fc1},
	{0xf9bd690a1b68637b, 0x3dfdce7aa3c673b1},
	{0x9c1661a651213e2d, 0x06bea10ca65c084f},
	{0xc31bfa0fe5698db8, 0x486e494fcff30a63},
	{0xf3e2f893dec3f126, 0x5a89dba3c3efccfb},
	{0x986ddb5c6b3a76b7, 0xf89629465a75e01d},
	{0xbe89523386091465, 0xf6bbb397f1135824},
	{0xee2ba6c0678b597f, 0x746aa07ded582e2d},
	{0x94db483840b717ef, 0xa8c2a44eb4571cdd},
	{0xba121a4650e4ddeb, 0x92f34d62616ce414},
	{0xe896a0d7e51e1566, 0x77b020baf9c81d18},
	{0x915e2486ef32cd60, 0x0ace1474dc1d122f},
	{0xb5b5ada8aaff80b8, 0x0d819992132456bb},
	{0xe3231912d5bf60e6, 0x10e1fff697ed6c6a},
	{0x8df5efabc5979c8f, 0xca8d3ffa1ef463c2},
	{0xb1736b96b6fd83b3, 0xbd308ff8a6b17cb3},
	{0xddd0467c64bce4a0, 0xac7cb3f6d05ddbdf},
	{0x8aa22c0dbef60ee4, 0x6bcdf07a423aa96c},
	{0xad4ab7112eb3929d, 0x86c16c98d2c953c7},
	{0xd89d64d57a607744, 0xe871c7bf077ba8b8},
	{0x87625f056c7c4a8b, 0x11471cd764ad4973},
	{0xa93af6c6c79b5d2d, 0xd598e40d3dd89bd0},
	{0xd389b47879823479, 0x4aff1d108d4ec2c4},
	{0x843610cb4bf160cb, 0xcedf722a585139bb},
	{0xa54394fe1eedb8fe, 0xc2974eb4ee658829},
	{0xce947a3da6a9273e, 0x733d226229feea33},
	{0x811ccc668829b887, 0x0806357d5a3f5260},
	{0xa163ff802a3426a8, 0xca07c2dcb0cf26f8},
	{0xc9bcff6034c13052, 0xfc89b393dd02f0b6},
	{0xfc2c3f3841f17c67, 0xbbac2078d443ace3},
	{0x9d9ba7832936edc0, 0xd54b944b84aa4c0e},
	{0xc5029163f384a931, 0x0a9e795e65d4df12},
	{0xf64335bcf065d37d, 0x4d4617b5ff4a16d6},
	{0x99ea0196163fa42e, 0x504bced1bf8e4e46},
	{0xc06481fb9bcf8d39, 0xe45ec2862f71e1d7},
	{0xf07da27a82c37088, 0x5d767327bb4e5a4d},
	{0x964e858c91ba2655, 0x3a6a07f8d510f870},
	{0xbbe226efb628afea, 0x890489f70a55368c},
	{0xeadab0aba3b2dbe5, 0x2b45ac74ccea842f},
	{0x92c8ae6b464fc96f, 0x3b0b8bc90012929e},
	{0xb77ada0617e3bbcb, 0x09ce6ebb40173745},
	{0xe55990879ddcaabd, 0xcc420a6a101d0516},
	{0x8f57fa54c2a9eab6, 0x9fa946824a12232e},
	{0xb32df8e9f3546564, 0x47939822dc96abfa},
	{0xdff9772470297ebd, 0x59787e2b93bc56f8},
	{0x8bfbea76c619ef36, 0x57eb4edb3c55b65b},
	{0xaefae51477a06b03, 0xede622920b6b23f2},
	{0xdab99e59958885c4, 0xe95fab368e45ecee},
	{0x88b402f7fd75539b, 0x11dbcb0218ebb415},
	{0xaae103b5fcd2a881, 0xd652bdc29f26a11a},
	{0xd59944a37c0752a2, 0x4be76d3346f04960},
	{0x857fcae62d8493a5, 0x6f70a4400c562ddc},
	{0xa6dfbd9fb8e5b88e, 0xcb4ccd500f6bb953},
	{0xd097ad07a71f26b2, 0x7e2000a41346a7a8},
	{0x825ecc24c873782f, 0x8ed400668c0c28c9},
	{0xa2f67f2dfa90563b, 0x728900802f0f32fb},
	{0xcbb41ef979346bca, 0x4f2b40a03ad2ffba},
	{0xfea126b7d78186bc, 0xe2f610c84987bfa9},
	{0x9f24b832e6b0f436, 0x0dd9ca7d2df4d7ca},
	{0xc6ede63fa05d3143, 0x91503d1c79720dbc},
	{0xf8a95fcf88747d94, 0x75a44c6397ce912b},
	{0x9b69dbe1b548ce7c, 0xc986afbe3ee11abb},
	{0xc24452da229b021b, 0xfbe85badce996169},
	{0xf2d56790ab41c2a2, 0xfae27299423fb9c4},
	{0x97c560ba6b0919a5, 0xdccd879fc967d41b},
	{0xbdb6b8e905cb600f, 0x5400e987bbc1c921},
	{0xed246723473e3813, 0x290123e9aab23b69},
	{0x9436c0760c86e30b, 0xf9a0b6720aaf6522},
	{0xb94470938fa89bce, 0xf808e40e8d5b3e6a},
	{0xe7958cb87392c2c2, 0xb60b1d1230b20e05},
	{0x90bd77f3483bb9b9, 0xb1c6f22b5e6f48c3},
	{0xb4ecd5f01a4aa828, 0x1e38aeb6360b1af4},
	{0xe2280b6c20dd5232, 0x25c6da63c38de1b1},
	{0x8d590723948a535f, 0x579c487e5a38ad0f},
	{0xb0af48ec79ace837, 0x2d835a9df0c6d852},
	{0xdcdb1b2798182244, 0xf8e431456cf88e66},
	{0x8a08f0f8bf0f156b, 0x1b8e9ecb641b5900},
	{0xac8b2d36eed2dac5, 0xe272467e3d222f40},
	{0xd7adf884aa879177, 0x5b0ed81dcc6abb10},
	{0x86ccbb52ea94baea, 0x98e947129fc2b4ea},
	{0xa87fea27a539e9a5, 0x3f2398d747b36225},
	{0xd29fe4b18e88640e, 0x8eec7f0d19a03aae},
	{0x83a3eeeef9153e89, 0x1953cf68300424ad},
	{0xa48ceaaab75a8e2b, 0x5fa8c3423c052dd8},
	{0xcdb02555653131b6, 0x3792f412cb06794e},
	{0x808e17555f3ebf11, 0xe2bbd88bbee40bd1},
	{0xa0b19d2ab70e6ed6, 0x5b6aceaeae9d0ec5},
	{0xc8de047564d20a8b, 0xf245825a5a445276},
	{0xfb158592be068d2e, 0xeed6e2f0f0d56713},
	{0x9ced737bb6c4183d, 0x55464dd69685606c},
	{0xc428d05aa4751e4c, 0xaa97e14c3c26b887},
	{0xf53304714d9265df, 0xd53dd99f4b3066a9},
	{0x993fe2c6d07b7fab, 0xe546a8038efe402a},
	{0xbf8fdb78849a5f96, 0xde98520472bdd034},
	{0xef73d256a5c0f77c, 0x963e66858f6d4441},
	{0x95a8637627989aad, 0xdde7001379a44aa9},
	{0xbb127c53b17ec159, 0x5560c018580d5d53},
	{0xe9d71b689dde71af, 0xaab8f01e6e10b4a7},
	{0x9226712162ab070d, 0xcab3961304ca70e9},
	{0xb6b00d69bb55c8d1, 0x3d607b97c5fd0d23},
	{0xe45c10c42a2b3b05, 0x8cb89a7db77c506b},
	{0x8eb98a7a9a5b04e3, 0x77f3608e92adb243},
	{0xb267ed1940f1c61c, 0x55f038b237591ed4},
	{0xdf01e85f912e37a3, 0x6b6c46dec52f6689},
	{0x8b61313bbabce2c6, 0x2323ac4b3b3da016},
	{0xae397d8aa96c1b77, 0xabec975e0a0d081b},
	{0xd9c7dced53c72255, 0x96e7bd358c904a22},
	{0x881cea14545c7575, 0x7e50d64177da2e55},
	{0xaa242499697392d2, 0xdde50bd1d5d0b9ea},
	{0xd4ad2dbfc3d07787, 0x955e4ec64b44e865},
	{0x84ec3c97da624ab4, 0xbd5af13bef0b113f},
	{0xa6274bbdd0fadd61, 0xecb1ad8aeacdd58f},
	{0xcfb11ead453994ba, 0x67de18eda5814af3},
	{0x81ceb32c4b43fcf4, 0x80eacf948770ced8},
	{0xa2425ff75e14fc31, 0xa1258379a94d028e},
	{0xcad2f7f5359a3b3e, 0x096ee45813a04331},
	{0xfd87b5f28300ca0d, 0x8bca9d6e188853fd},
	{0x9e74d1b791e07e48, 0x775ea264cf55347e},
	{0xc612062576589dda, 0x95364afe032a819e},
	{0xf79687aed3eec551, 0x3a83ddbd83f52205},
	{0x9abe14cd44753b52, 0xc4926a9672793543},
	{0xc16d9a0095928a27, 0x75b7053c0f178294},
	{0xf1c90080baf72cb1, 0x5324c68b12dd6339},
	{0x971da05074da7bee, 0xd3f6fc16ebca5e04},
	{0xbce5086492111aea, 0x88f4bb1ca6bcf585},
	{0xec1e4a7db69561a5, 0x2b31e9e3d06c32e6},
	{0x9392ee8e921d5d07, 0x3aff322e62439fd0},
	{0xb877aa3236a4b449, 0x09befeb9fad487c3},
	{0xe69594bec44de15b, 0x4c2ebe687989a9b4},
	{0x901d7cf73ab0acd9, 0x0f9d37014bf60a11},
	{0xb424dc35095cd80f, 0x538484c19ef38c95},
	{0xe12e13424bb40e13, 0x2865a5f206b06fba},
	{0x8cbccc096f5088cb, 0xf93f87b7442e45d4},
	{0xafebff0bcb24aafe, 0xf78f69a51539d749},
	{0xdbe6fecebdedd5be, 0xb573440e5a884d1c},
	{0x89705f4136b4a597, 0x31680a88f8953031},
	{0xabcc77118461cefc, 0xfdc20d2b36ba7c3e},
	{0xd6bf94d5e57a42bc, 0x3d32907604691b4d},
	{0x8637bd05af6c69b5, 0xa63f9a49c2c1b110},
	{0xa7c5ac471b478423, 0x0fcf80dc33721d54},
	{0xd1b71758e219652b, 0xd3c36113404ea4a9},
	{0x83126e978d4fdf3b, 0x645a1cac083126ea},
	{0xa3d70a3d70a3d70a, 0x3d70a3d70a3d70a4},
	{0xcccccccccccccccc, 0xcccccccccccccccd},
	{0x8000000000000000, 0x0000000000000000},
	{0xa000000000000000, 0x0000000000000000},
	{0xc800000000000000, 0x0000000000000000},
	{0xfa00000000000000, 0x0000000000000000},
	{0x9c40000000000000, 0x0000000000000000},
	{0xc350000000000000, 0x0000000000000000},
	{0xf424000000000000, 0x0000000000000000},
	{0x9896800000000000, 0x0000000000000000},
	{0xbebc200000000000, 0x0000000000000000},
	{0xee6b280000000000, 0x0000000000000000},
	{0x9502f90000000000, 0x0000000000000000},
	{0xba43b74000000000, 0x0000000000000000},
	{0xe8d4a51000000000, 0x0000000000000000},
	{0x9184e72a00000000, 0x0000000000000000},
	{0xb5e620f480000000, 0x0000000000000000},
	{0xe35fa931a0000000, 0x0000000000000000},
	{0x8e1bc9bf04000000, 0x0000000000000000},
	{0xb1a2bc2ec5000000, 0x0000000000000000},
	{0xde0b6b3a76400000, 0x0000000000000000},
	{0x8ac7230489e80000, 0x0000000000000000},
	{0xad78ebc5ac620000, 0x0000000000000000},
	{0xd8d726b7177a8000, 0x0000000000000000},
	{0x878678326eac9000, 0x0000000000000000},
	{0xa968163f0a57b400, 0x0000000000000000},
	{0xd3c21bcecceda100, 0x0000000000000000},
	{0x84595161401484a0, 0x0000000000000000},
	{0xa56fa5b99019a5c8, 0x0000000000000000},
	{0xcecb8f27f4200f3a, 0x0000000000000000},
	{0x813f3978f8940984, 0x4000000000000000},
	{0xa18f07d736b90be5, 0x5000000000000000},
	{0xc9f2c9cd04674ede, 0xa400000000000000},
	{0xfc6f7c4045812296, 0x4d00000000000000},
	{0x9dc5ada82b70b59d, 0xf020000000000000},
	{0xc5371912364ce305, 0x6c28000000000000},
	{0xf684df56c3e01bc6, 0xc732000000000000},
	{0x9a130b963a6c115c, 0x3c7f400000000000},
	{0xc097ce7bc90715b3, 0x4b9f100000000000},
	{0xf0bdc21abb48db20, 0x1e86d40000000000},
	{0x96769950b50d88f4, 0x1314448000000000},
	{0xbc143fa4e250eb31, 0x17d955a000000000},
	{0xeb194f8e1ae525fd, 0x5dcfab0800000000},
	{0x92efd1b8d0cf37be, 0x5aa1cae500000000},
	{0xb7abc627050305ad, 0xf14a3d9e40000000},
	{0xe596b7b0c643c719, 0x6d9ccd05d0000000},
	{0x8f7e32ce7bea5c6f, 0xe4820023a2000000},
	{0xb35dbf821ae4f38b, 0xdda2802c8a800000},
	{0xe0352f62a19e306e, 0xd50b2037ad200000},
	{0x8c213d9da502de45, 0x4526f422cc340000},
	{0xaf298d050e4395d6, 0x9670b12b7f410000},
	{0xdaf3f04651d47b4c, 0x3c0cdd765f114000},
	{0x88d8762bf324cd0f, 0xa5880a69fb6ac800},
	{0xab0e93b6efee0053, 0x8eea0d047a457a00},
	{0xd5d238a4abe98068, 0x72a4904598d6d880},
	{0x85a36366eb71f041, 0x47a6da2b7f864750},
	{0xa70c3c40a64e6c51, 0x999090b65f67d924},
	{0xd0cf4b50cfe20765, 0xfff4b4e3f741cf6d},
	{0x82818f1281ed449f, 0xbff8f10e7a8921a4},
	{0xa321f2d7226895c7, 0xaff72d52192b6a0d},
	{0xcbea6f8ceb02bb39, 0x9bf4f8a69f764490},
	{0xfee50b7025c36a08, 0x02f236d04753d5b4},
	{0x9f4f2726179a2245, 0x01d762422c946590},
	{0xc722f0ef9d80aad6, 0x424d3ad2b7b97ef5},
	{0xf8ebad2b84e0d58b, 0xd2e0898765a7deb2},
	{0x9b934c3b330c8577, 0x63cc55f49f88eb2f},
	{0xc2781f49ffcfa6d5, 0x3cbf6b71c76b25fb},
	{0xf316271c7fc3908a, 0x8bef464e3945ef7a},
	{0x97edd871cfda3a56, 0x97758bf0e3cbb5ac},
	{0xbde94e8e43d0c8ec, 0x3d52eeed1cbea317},
	{0xed63a231d4c4fb27, 0x4ca7aaa863ee4bdd},
	{0x945e455f24fb1cf8, 0x8fe8caa93e74ef6a},
	{0xb975d6b6ee39e436, 0xb3e2fd538e122b44},
	{0xe7d34c64a9c85d44, 0x60dbbca87196b616},
	{0x90e40fbeea1d3a4a, 0xbc8955e946fe31cd},
	{0xb51d13aea4a488dd, 0x6babab6398bdbe41},
	{0xe264589a4dcdab14, 0xc696963c7eed2dd1},
	{0x8d7eb76070a08aec, 0xfc1e1de5cf543ca2},
	{0xb0de65388cc8ada8, 0x3b25a55f43294bcb},
	{0xdd15fe86affad912, 0x49ef0eb713f39ebe},
	{0x8a2dbf142dfcc7ab, 0x6e3569326c784337},
	{0xacb92ed9397bf996, 0x49c2c37f07965404},
	{0xd7e77a8f87daf7fb, 0xdc33745ec97be906},
	{0x86f0ac99b4e8dafd, 0x69a028bb3ded71a3},
	{0xa8acd7c0222311bc, 0xc40832ea0d68ce0c},
	{0xd2d80db02aabd62b, 0xf50a3fa490c30190},
	{0x83c7088e1aab65db, 0x792667c6da79e0fa},
	{0xa4b8cab1a1563f52, 0x577001b891185938},
	{0xcde6fd5e09abcf26, 0xed4c0226b55e6f86},
	{0x80b05e5ac60b6178, 0x544f8158315b05b4},
	{0xa0dc75f1778e39d6, 0x696361ae3db1c721},
	{0xc913936dd571c84c, 0x03bc3a19cd1e38e9},
	{0xfb5878494ace3a5f, 0x04ab48a04065c723},
	{0x9d174b2dcec0e47b, 0x62eb0d64283f9c76},
	{0xc45d1df942711d9a, 0x3ba5d0bd324f8394},
	{0xf5746577930d6500, 0xca8f44ec7ee36479},
	{0x9968bf6abbe85f20, 0x7e998b13cf4e1ecb},
	{0xbfc2ef456ae276e8, 0x9e3fedd8c321a67e},
	{0xefb3ab16c59b14a2, 0xc5cfe94ef3ea101e},
	{0x95d04aee3b80ece5, 0xbba1f1d158724a12},
	{0xbb445da9ca61281f, 0x2a8a6e45ae8edc97},
	{0xea1575143cf97226, 0xf52d09d71a3293bd},
	{0x924d692ca61be758, 0x593c2626705f9c56},
	{0xb6e0c377cfa2e12e, 0x6f8b2fb00c77836c},
	{0xe498f455c38b997a, 0x0b6dfb9c0f956447},
	{0x8edf98b59a373fec, 0x4724bd4189bd5eac},
	{0xb2977ee300c50fe7, 0x58edec91ec2cb657},
	{0xdf3d5e9bc0f653e1, 0x2f2967b66737e3ed},
	{0x8b865b215899f46c, 0xbd79e0d20082ee74},
	{0xae67f1e9aec07187, 0xecd8590680a3aa11},
	{0xda01ee641a708de9, 0xe80e6f4820cc9495},
	{0x884134fe908658b2, 0x3109058d147fdcdd},
	{0xaa51823e34a7eede, 0xbd4b46f0599fd415},
	{0xd4e5e2cdc1d1ea96, 0x6c9e18ac7007c91a},
	{0x850fadc09923329e, 0x03e2cf6bc604ddb0},
	{0xa6539930bf6bff45, 0x84db8346b786151c},
	{0xcfe87f7cef46ff16, 0xe612641865679a63},
	{0x81f14fae158c5f6e, 0x4fcb7e8f3f60c07e},
	{0xa26da3999aef7749, 0xe3be5e330f38f09d},
	{0xcb090c8001ab551c, 0x5cadf5bfd3072cc5},
	{0xfdcb4fa002162a63, 0x73d9732fc7c8f7f6},
	{0x9e9f11c4014dda7e, 0x2867e7fddcdd9afa},
	{0xc646d63501a1511d, 0xb281e1fd541501b8},
	{0xf7d88bc24209a565, 0x1f225a7ca91a4226},
	{0x9ae757596946075f, 0x3375788de9b06958},
	{0xc1a12d2fc3978937, 0x0052d6b1641c83ae},
	{0xf209787bb47d6b84, 0xc0678c5dbd23a49a},
	{0x9745eb4d50ce6332, 0xf840b7ba963646e0},
	{0xbd176620a501fbff, 0xb650e5a93bc3d898},
	{0xec5d3fa8ce427aff, 0xa3e51f138ab4cebe},
	{0x93ba47c980e98cdf, 0xc66f336c36b10137},
	{0xb8a8d9bbe123f017, 0xb80b0047445d4184},
	{0xe6d3102ad96cec1d, 0xa60dc059157491e5},
	{0x9043ea1ac7e41392, 0x87c89837ad68db2f},
	{0xb454e4a179dd1877, 0x29babe4598c311fb},
	{0xe16a1dc9d8545e94, 0xf4296dd6fef3d67a},
	{0x8ce2529e2734bb1d, 0x1899e4a65f58660c},
	{0xb01ae745b101e9e4, 0x5ec05dcff72e7f8f},
	{0xdc21a1171d42645d, 0x76707543f4fa1f73},
	{0x899504ae72497eba, 0x6a06494a791c53a8},
	{0xabfa45da0edbde69, 0x0487db9d17636892},
	{0xd6f8d7509292d603, 0x45a9d2845d3c42b6},
	{0x865b86925b9bc5c2, 0x0b8a2392ba45a9b2},
	{0xa7f26836f282b732, 0x8e6cac7768d7141e},
	{0xd1ef0244af2364ff, 0x3207d795430cd926},
	{0x8335616aed761f1f, 0x7f44e6bd49e807b8},
	{0xa402b9c5a8d3a6e7, 0x5f16206c9c6209a6},
	{0xcd036837130890a1, 0x36dba887c37a8c0f},
	{0x802221226be55a64, 0xc2494954da2c9789},
	{0xa02aa96b06deb0fd, 0xf2db9baa10b7bd6c},
	{0xc83553c5c8965d3d, 0x6f92829494e5acc7},
	{0xfa42a8b73abbf48c, 0xcb772339ba1f17f9},
	{0x9c69a97284b578d7, 0xff2a760414536efb},
	{0xc38413cf25e2d70d, 0xfef5138519684aba},
	{0xf46518c2ef5b8cd1, 0x7eb258665fc25d69},
	{0x98bf2f79d5993802, 0xef2f773ffbd97a61},
	{0xbeeefb584aff8603, 0xaafb550ffacfd8fa},
	{0xeeaaba2e5dbf6784, 0x95ba2a53f983cf38},
	{0x952ab45cfa97a0b2, 0xdd945a747bf26183},
	{0xba756174393d88df, 0x94f971119aeef9e4},
	{0xe912b9d1478ceb17, 0x7a37cd5601aab85d},
	{0x91abb422ccb812ee, 0xac62e055c10ab33a},
	{0xb616a12b7fe617aa, 0x577b986b314d6009},
	{0xe39c49765fdf9d94, 0xed5a7e85fda0b80b},
	{0x8e41ade9fbebc27d, 0x14588f13be847307},
	{0xb1d219647ae6b31c, 0x596eb2d8ae258fc8},
	{0xde469fbd99a05fe3, 0x6fca5f8ed9aef3bb},
	{0x8aec23d680043bee, 0x25de7bb9480d5854},
	{0xada72ccc20054ae9, 0xaf561aa79a10ae6a},
	{0xd910f7ff28069da4, 0x1b2ba1518094da04},
	{0x87aa9aff79042286, 0x90fb44d2f05d0842},
	{0xa99541bf57452b28, 0x353a1607ac744a53},
	{0xd3fa922f2d1675f2, 0x42889b8997915ce8},
	{0x847c9b5d7c2e09b7, 0x69956135febada11},
	{0xa59bc234db398c25, 0x43fab9837e699095},
	{0xcf02b2c21207ef2e, 0x94f967e45e03f4bb},
	{0x8161afb94b44f57d, 0x1d1be0eebac278f5},
	{0xa1ba1ba79e1632dc, 0x6462d92a69731732},
	{0xca28a291859bbf93, 0x7d7b8f7503cfdcfe},
	{0xfcb2cb35e702af78, 0x5cda735244c3d43e},
	{0x9defbf01b061adab, 0x3a0888136afa64a7},
	{0xc56baec21c7a1916, 0x088aaa1845b8fdd0},
	{0xf6c69a72a3989f5b, 0x8aad549e57273d45},
	{0x9a3c2087a63f6399, 0x36ac54e2f678864b},
	{0xc0cb28a98fcf3c7f, 0x84576a1bb416a7dd},
	{0xf0fdf2d3f3c30b9f, 0x656d44a2a11c51d5},
	{0x969eb7c47859e743, 0x9f644ae5a4b1b325},
	{0xbc4665b596706114, 0x873d5d9f0dde1fee},
	{0xeb57ff22fc0c7959, 0xa90cb506d155a7ea},
	{0x9316ff75dd87cbd8, 0x09a7f12442d588f2},
	{0xb7dcbf5354e9bece, 0x0c11ed6d538aeb2f},
	{0xe5d3ef282a242e81, 0x8f1668c8a86da5fa},
	{0x8fa475791a569d10, 0xf96e017d694487bc},
	{0xb38d92d760ec4455, 0x37c981dcc395a9ac},
	{0xe070f78d3927556a, 0x85bbe253f47b1417},
	{0x8c469ab843b89562, 0x93956d7478ccec8e},
	{0xaf58416654a6babb, 0x387ac8d1970027b2},
	{0xdb2e51bfe9d0696a, 0x06997b05fcc0319e},
	{0x88fcf317f22241e2, 0x441fece3bdf81f03},
	{0xab3c2fddeeaad25a, 0xd527e81cad7626c3},
	{0xd60b3bd56a5586f1, 0x8a71e223d8d3b074},
	{0x85c7056562757456, 0xf6872d5667844e49},
	{0xa738c6bebb12d16c, 0xb428f8ac016561db},
	{0xd106f86e69d785c7, 0xe13336d701beba52},
	{0x82a45b450226b39c, 0xecc0024661173473},
	{0xa34d721642b06084, 0x27f002d7f95d0190},
	{0xcc20ce9bd35c78a5, 0x31ec038df7b441f4},
	{0xff290242c83396ce, 0x7e67047175a15271},
	{0x9f79a169bd203e41, 0x0f0062c6e984d386},
	{0xc75809c42c684dd1, 0x52c07b78a3e60868},
	{0xf92e0c3537826145, 0xa7709a56ccdf8a82},
	{0x9bbcc7a142b17ccb, 0x88a66076400bb691},
	{0xc2abf989935ddbfe, 0x6acff893d00ea435},
	{0xf356f7ebf83552fe, 0x0583f6b8c4124d43},
	{0x98165af37b2153de, 0xc3727a337a8b704a},
	{0xbe1bf1b059e9a8d6, 0x744f18c0592e4c5c},
	{0xeda2ee1c7064130c, 0x1162def06f79df73},
	{0x9485d4d1c63e8be7, 0x8addcb5645ac2ba8},
	{0xb9a74a0637ce2ee1, 0x6d953e2bd7173692},
	{0xe8111c87c5c1ba99, 0xc8fa8db6ccdd0437},
	{0x910ab1d4db9914a0, 0x1d9c9892400a22a2},
	{0xb54d5e4a127f59c8, 0x2503beb6d00cab4b},
	{0xe2a0b5dc971f303a, 0x2e44ae64840fd61d},
	{0x8da471a9de737e24, 0x5ceaecfed289e5d2},
	{0xb10d8e1456105dad, 0x7425a83e872c5f47},
	{0xdd50f1996b947518, 0xd12f124e28f77719},
	{0x8a5296ffe33cc92f, 0x82bd6b70d99aaa6f},
	{0xace73cbfdc0bfb7b, 0x636cc64d1001550b},
	{0xd8210befd30efa5a, 0x3c47f7e05401aa4e},
	{0x8714a775e3e95c78, 0x65acfaec34810a71},
	{0xa8d9d1535ce3b396, 0x7f1839a741a14d0d},
	{0xd31045a8341ca07c, 0x1ede48111209a050},
	{0x83ea2b892091e44d, 0x934aed0aab460432},
	{0xa4e4b66b68b65d60, 0xf81da84d5617853f},
	{0xce1de40642e3f4b9, 0x36251260ab9d668e},
	{0x80d2ae83e9ce78f3, 0xc1d72b7c6b426019},
	{0xa1075a24e4421730, 0xb24cf65b8612f81f},
	{0xc94930ae1d529cfc, 0xdee033f26797b627},
	{0xfb9b7cd9a4a7443c, 0x169840ef017da3b1},
	{0x9d412e0806e88aa5, 0x8e1f289560ee864e},
	{0xc491798a08a2ad4e, 0xf1a6f2bab92a27e2},
	{0xf5b5d7ec8acb58a2, 0xae10af696774b1db},
	{0x9991a6f3d6bf1765, 0xacca6da1e0a8ef29},
	{0xbff610b0cc6edd3f, 0x17fd090a58d32af3},
	{0xeff394dcff8a948e, 0xddfc4b4cef07f5b0},
	{0x95f83d0a1fb69cd9, 0x4abdaf101564f98e},
	{0xbb764c4ca7a4440f, 0x9d6d1ad41abe37f1},
	{0xea53df5fd18d5513, 0x84c86189216dc5ed},
	{0x92746b9be2f8552c, 0x32fd3cf5b4e49bb4},
	{0xb7118682dbb66a77, 0x3fbc8c33221dc2a1},
	{0xe4d5e82392a40515, 0x0fabaf3feaa5334a},
	{0x8f05b1163ba6832d, 0x29cb4d87f2a7400e},
	{0xb2c71d5bca9023f8, 0x743e20e9ef511012},
	{0xdf78e4b2bd342cf6, 0x914da9246b255416},
	{0x8bab8eefb6409c1a, 0x1ad089b6c2f7548e},
	{0xae9672aba3d0c320, 0xa184ac2473b529b1},
	{0xda3c0f568cc4f3e8, 0xc9e5d72d90a2741e},
	{0x8865899617fb1871, 0x7e2fa67c7a658892},
	{0xaa7eebfb9df9de8d, 0xddbb901b98feeab7},
	{0xd51ea6fa85785631, 0x552a74227f3ea565},
	{0x8533285c936b35de, 0xd53a88958f87275f},
	{0xa67ff273b8460356, 0x8a892abaf368f137},
	{0xd01fef10a657842c, 0x2d2b7569b0432d85},
	{0x8213f56a67f6b29b, 0x9c3b29620e29fc73},
	{0xa298f2c501f45f42, 0x8349f3ba91b47b8f},
	{0xcb3f2f7642717713, 0x241c70a936219a73},
	{0xfe0efb53d30dd4d7, 0xed238cd383aa0110},
	{0x9ec95d1463e8a506, 0xf4363804324a40aa},
	{0xc67bb4597ce2ce48, 0xb143c6053edcd0d5},
	{0xf81aa16fdc1b81da, 0xdd94b7868e94050a},
	{0x9b10a4e5e9913128, 0xca7cf2b4191c8326},
	{0xc1d4ce1f63f57d72, 0xfd1c2f611f63a3f0},
	{0xf24a01a73cf2dccf, 0xbc633b39673c8cec},
	{0x976e41088617ca01, 0xd5be0503e085d813},
	{0xbd49d14aa79dbc82, 0x4b2d8644d8a74e18},
	{0xec9c459d51852ba2, 0xddf8e7d60ed1219e},
	{0x93e1ab8252f33b45, 0xcabb90e5c942b503},
	{0xb8da1662e7b00a17, 0x3d6a751f3b936243},
	{0xe7109bfba19c0c9d, 0x0cc512670a783ad4},
	{0x906a617d450187e2, 0x27fb2b80668b24c5},
	{0xb484f9dc9641e9da, 0xb1f9f660802dedf6},
	{0xe1a63853bbd26451, 0x5e7873f8a0396973},
	{0x8d07e33455637eb2, 0xdb0b487b6423e1e8},
	{0xb049dc016abc5e5f, 0x91ce1a9a3d2cda62},
	{0xdc5c5301c56b75f7, 0x7641a140cc7810fb},
	{0x89b9b3e11b6329ba, 0xa9e904c87fcb0a9d},
	{0xac2820d9623bf429, 0x546345fa9fbdcd44},
	{0xd732290fbacaf133, 0xa97c177947ad4095},
	{0x867f59a9d4bed6c0, 0x49ed8eabcccc485d},
	{0xa81f301449ee8c70, 0x5c68f256bfff5a74},
	{0xd226fc195c6a2f8c, 0x73832eec6fff3111},
	{0x83585d8fd9c25db7, 0xc831fd53c5ff7eab},
	{0xa42e74f3d032f525, 0xba3e7ca8b77f5e55},
	{0xcd3a1230c43fb26f, 0x28ce1bd2e55f35eb},
	{0x80444b5e7aa7cf85, 0x7980d163cf5b81b3},
	{0xa0555e361951c366, 0xd7e105bcc332621f},
	{0xc86ab5c39fa63440, 0x8dd9472bf3fefaa7},
	{0xfa856334878fc150, 0xb14f98f6f0feb951},
	{0x9c935e00d4b9d8d2, 0x6ed1bf9a569f33d3},
	{0xc3b8358109e84f07, 0x0a862f80ec4700c8},
	{0xf4a642e14c6262c8, 0xcd27bb612758c0fa},
	{0x98e7e9cccfbd7dbd, 0x8038d51cb897789c},
	{0xbf21e44003acdd2c, 0xe0470a63e6bd56c3},
	{0xeeea5d5004981478, 0x1858ccfce06cac74},
	{0x95527a5202df0ccb, 0x0f37801e0c43ebc8},
	{0xbaa718e68396cffd, 0xd30560258f54e6ba},
	{0xe950df20247c83fd, 0x47c6b82ef32a2069},
	{0x91d28b7416cdd27e, 0x4cdc331d57fa5441},
	{0xb6472e511c81471d, 0xe0133fe4adf8e952},
	{0xe3d8f9e563a198e5, 0x58180fddd97723a6},
	{0x8e679c2f5e44ff8f, 0x570f09eaa7ea7648},
	{0xb201833b35d63f73, 0x2cd2cc6551e513da},
	{0xde81e40a034bcf4f, 0xf8077f7ea65e58d1},
	{0x8b112e86420f6191, 0xfb04afaf27faf782},
	{0xadd57a27d29339f6, 0x79c5db9af1f9b563},
	{0xd94ad8b1c7380874, 0x18375281ae7822bc},
	{0x87cec76f1c830548, 0x8f2293910d0b15b5},
	{0xa9c2794ae3a3c69a, 0xb2eb3875504ddb22},
	{0xd433179d9c8cb841, 0x5fa60692a46151eb},
	{0x849feec281d7f328, 0xdbc7c41ba6bcd333},
	{0xa5c7ea73224deff3, 0x12b9b522906c0800},
	{0xcf39e50feae16bef, 0xd768226b34870a00},
	{0x81842f29f2cce375, 0xe6a1158300d46640},
	{0xa1e53af46f801c53, 0x60495ae3c1097fd0},
	{0xca5e89b18b602368, 0x385bb19cb14bdfc4},
	{0xfcf62c1dee382c42, 0x46729e03dd9ed7b5},
	{0x9e19db92b4e31ba9, 0x6c07a2c26a8346d1},
	{0xc5a05277621be293, 0xc7098b7305241885},
	{0xf70867153aa2db38, 0xb8cbee4fc66d1ea7}
#else
	{0xff77b1fcbebcdc4f, 0x25e8e89c13bb0f7b},
	{0xce5d73ff402d98e3, 0xfb0a3d212dc81290},
	{0xa6b34ad8c9dfc06f, 0xf42faa48c0ea481f},
	{0x86a8d39ef77164bc, 0xae5dff9c02033198},
	{0xd98ddaee19068c76, 0x3badd624dd9b0958},
	{0xafbd2350644eeacf, 0xe5d1929ef90898fb},
	{0x8df5efabc5979c8f, 0xca8d3ffa1ef463c2},
	{0xe55990879ddcaabd, 0xcc420a6a101d0516},
	{0xb94470938fa89bce, 0xf808e40e8d5b3e6a},
	{0x95a8637627989aad, 0xdde7001379a44aa9},
	{0xf1c90080baf72cb1, 0x5324c68b12dd6339},
	{0xc350000000000000, 0x0000000000000000},
	{0x9dc5ada82b70b59d, 0xf020000000000000},
	{0xfee50b7025c36a08, 0x02f236d04753d5b4},
	{0xcde6fd5e09abcf26, 0xed4c0226b55e6f86},
	{0xa6539930bf6bff45, 0x84db8346b786151c},
	{0x865b86925b9bc5c2, 0x0b8a2392ba45a9b2},
	{0xd910f7ff28069da4, 0x1b2ba1518094da04},
	{0xaf58416654a6babb, 0x387ac8d1970027b2},
	{0x8da471a9de737e24, 0x5ceaecfed289e5d2},
	{0xe4d5e82392a40515, 0x0fabaf3feaa5334a},
	{0xb8da1662e7b00a17, 0x3d6a751f3b936243},
	{0x95527a5202df0ccb, 0x0f37801e0c43ebc8}
#endif
};

#if !FMT_USE_FULL_CACHE_DRAGONBOX
template <typename T>
const uint64_t basic_data<T>::powers_of_5_64[] = {
	0x0000000000000001, 0x0000000000000005, 0x0000000000000019,
	0x000000000000007d, 0x0000000000000271, 0x0000000000000c35,
	0x0000000000003d09, 0x000000000001312d, 0x000000000005f5e1,
	0x00000000001dcd65, 0x00000000009502f9, 0x0000000002e90edd,
	0x000000000e8d4a51, 0x0000000048c27395, 0x000000016bcc41e9,
	0x000000071afd498d, 0x0000002386f26fc1, 0x000000b1a2bc2ec5,
	0x000003782dace9d9, 0x00001158e460913d, 0x000056bc75e2d631,
	0x0001b1ae4d6e2ef5, 0x000878678326eac9, 0x002a5a058fc295ed,
	0x00d3c21bcecceda1, 0x0422ca8b0a00a425, 0x14adf4b7320334b9};

template <typename T>
const uint32_t basic_data<T>::dragonbox_pow10_recovery_errors[] = {
	0x50001400, 0x54044100, 0x54014555, 0x55954415, 0x54115555, 0x00000001,
	0x50000000, 0x00104000, 0x54010004, 0x05004001, 0x55555544, 0x41545555,
	0x54040551, 0x15445545, 0x51555514, 0x10000015, 0x00101100, 0x01100015,
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x04450514, 0x45414110,
	0x55555145, 0x50544050, 0x15040155, 0x11054140, 0x50111514, 0x11451454,
	0x00400541, 0x00000000, 0x55555450, 0x10056551, 0x10054011, 0x55551014,
	0x69514555, 0x05151109, 0x00155555};
#endif

template <typename T>
const char basic_data<T>::foreground_color[] = "\x1b[38;2;";
template <typename T>
const char basic_data<T>::background_color[] = "\x1b[48;2;";
template <typename T> const char basic_data<T>::reset_color[] = "\x1b[0m";
template <typename T> const wchar_t basic_data<T>::wreset_color[] = L"\x1b[0m";
template <typename T> const char basic_data<T>::signs[] = {0, '-', '+', ' '};
template <typename T>
const char basic_data<T>::left_padding_shifts[] = {31, 31, 0, 1, 0};
template <typename T>
const char basic_data<T>::right_padding_shifts[] = {0, 31, 0, 1, 0};

template <typename T> struct bits {
  static FMT_CONSTEXPR_DECL const int value =
	  static_cast<int>(sizeof(T) * std::numeric_limits<unsigned char>::digits);
};

class fp;
template <int SHIFT = 0> fp normalize(fp value);

// Lower (upper) boundary is a value half way between a floating-point value
// and its predecessor (successor). Boundaries have the same exponent as the
// value so only significands are stored.
struct boundaries {
  uint64_t lower;
  uint64_t upper;
};

// A handmade floating-point number f * pow(2, e).
class fp {
 private:
  using significand_type = uint64_t;

  template <typename Float>
  using is_supported_float = bool_constant<sizeof(Float) == sizeof(uint64_t) ||
										   sizeof(Float) == sizeof(uint32_t)>;

 public:
  significand_type f;
  int e;

  // All sizes are in bits.
  // Subtract 1 to account for an implicit most significant bit in the
  // normalized form.
  static FMT_CONSTEXPR_DECL const int double_significand_size =
	  std::numeric_limits<double>::digits - 1;
  static FMT_CONSTEXPR_DECL const uint64_t implicit_bit =
	  1ULL << double_significand_size;
  static FMT_CONSTEXPR_DECL const int significand_size =
	  bits<significand_type>::value;

  fp() : f(0), e(0) {}
  fp(uint64_t f_val, int e_val) : f(f_val), e(e_val) {}

  // Constructs fp from an IEEE754 double. It is a template to prevent compile
  // errors on platforms where double is not IEEE754.
  template <typename Double> explicit fp(Double d) { assign(d); }

  // Assigns d to this and return true iff predecessor is closer than successor.
  template <typename Float, FMT_ENABLE_IF(is_supported_float<Float>::value)>
  bool assign(Float d) {
	// Assume float is in the format [sign][exponent][significand].
	using limits = std::numeric_limits<Float>;
	const int float_significand_size = limits::digits - 1;
	const int exponent_size =
		bits<Float>::value - float_significand_size - 1;  // -1 for sign
	const uint64_t float_implicit_bit = 1ULL << float_significand_size;
	const uint64_t significand_mask = float_implicit_bit - 1;
	const uint64_t exponent_mask = (~0ULL >> 1) & ~significand_mask;
	const int exponent_bias = (1 << exponent_size) - limits::max_exponent - 1;
	constexpr bool is_double = sizeof(Float) == sizeof(uint64_t);
	auto u = bit_cast<conditional_t<is_double, uint64_t, uint32_t>>(d);
	f = u & significand_mask;
	int biased_e =
		static_cast<int>((u & exponent_mask) >> float_significand_size);
	// Predecessor is closer if d is a normalized power of 2 (f == 0) other than
	// the smallest normalized number (biased_e > 1).
	bool is_predecessor_closer = f == 0 && biased_e > 1;
	if (biased_e != 0)
	  f += float_implicit_bit;
	else
	  biased_e = 1;  // Subnormals use biased exponent 1 (min exponent).
	e = biased_e - exponent_bias - float_significand_size;
	return is_predecessor_closer;
  }

  template <typename Float, FMT_ENABLE_IF(!is_supported_float<Float>::value)>
  bool assign(Float) {
	*this = fp();
	return false;
  }
};

// Normalizes the value converted from double and multiplied by (1 << SHIFT).
template <int SHIFT> fp normalize(fp value) {
  // Handle subnormals.
  const auto shifted_implicit_bit = fp::implicit_bit << SHIFT;
  while ((value.f & shifted_implicit_bit) == 0) {
	value.f <<= 1;
	--value.e;
  }
  // Subtract 1 to account for hidden bit.
  const auto offset =
	  fp::significand_size - fp::double_significand_size - SHIFT - 1;
  value.f <<= offset;
  value.e -= offset;
  return value;
}

inline bool operator==(fp x, fp y) { return x.f == y.f && x.e == y.e; }

// Computes lhs * rhs / pow(2, 64) rounded to nearest with half-up tie breaking.
inline uint64_t multiply(uint64_t lhs, uint64_t rhs) {
#if FMT_USE_INT128
  auto product = static_cast<__uint128_t>(lhs) * rhs;
  auto f = static_cast<uint64_t>(product >> 64);
  return (static_cast<uint64_t>(product) & (1ULL << 63)) != 0 ? f + 1 : f;
#else
  // Multiply 32-bit parts of significands.
  uint64_t mask = (1ULL << 32) - 1;
  uint64_t a = lhs >> 32, b = lhs & mask;
  uint64_t c = rhs >> 32, d = rhs & mask;
  uint64_t ac = a * c, bc = b * c, ad = a * d, bd = b * d;
  // Compute mid 64-bit of result and round.
  uint64_t mid = (bd >> 32) + (ad & mask) + (bc & mask) + (1U << 31);
  return ac + (ad >> 32) + (bc >> 32) + (mid >> 32);
#endif
}

inline fp operator*(fp x, fp y) { return {multiply(x.f, y.f), x.e + y.e + 64}; }

// Returns a cached power of 10 `c_k = c_k.f * pow(2, c_k.e)` such that its
// (binary) exponent satisfies `min_exponent <= c_k.e <= min_exponent + 28`.
inline fp get_cached_power(int min_exponent, int& pow10_exponent) {
  const int shift = 32;
  const auto significand = static_cast<int64_t>(data::log10_2_significand);
  int index = static_cast<int>(
	  ((min_exponent + fp::significand_size - 1) * (significand >> shift) +
	   ((int64_t(1) << shift) - 1))  // ceil
	  >> 32                          // arithmetic shift
  );
  // Decimal exponent of the first (smallest) cached power of 10.
  const int first_dec_exp = -348;
  // Difference between 2 consecutive decimal exponents in cached powers of 10.
  const int dec_exp_step = 8;
  index = (index - first_dec_exp - 1) / dec_exp_step + 1;
  pow10_exponent = first_dec_exp + index * dec_exp_step;
  return {data::grisu_pow10_significands[index],
		  data::grisu_pow10_exponents[index]};
}

// A simple accumulator to hold the sums of terms in bigint::square if uint128_t
// is not available.
struct accumulator {
  uint64_t lower;
  uint64_t upper;

  accumulator() : lower(0), upper(0) {}
  explicit operator uint32_t() const { return static_cast<uint32_t>(lower); }

  void operator+=(uint64_t n) {
	lower += n;
	if (lower < n) ++upper;
  }
  void operator>>=(int shift) {
	assert(shift == 32);
	(void)shift;
	lower = (upper << 32) | (lower >> 32);
	upper >>= 32;
  }
};

class bigint {
 private:
  // A bigint is stored as an array of bigits (big digits), with bigit at index
  // 0 being the least significant one.
  using bigit = uint32_t;
  using double_bigit = uint64_t;
  enum { bigits_capacity = 32 };
  basic_memory_buffer<bigit, bigits_capacity> bigits_;
  int exp_;

  bigit operator[](int index) const { return bigits_[to_unsigned(index)]; }
  bigit& operator[](int index) { return bigits_[to_unsigned(index)]; }

  static FMT_CONSTEXPR_DECL const int bigit_bits = bits<bigit>::value;

  friend struct formatter<bigint>;

  void subtract_bigits(int index, bigit other, bigit& borrow) {
	auto result = static_cast<double_bigit>((*this)[index]) - other - borrow;
	(*this)[index] = static_cast<bigit>(result);
	borrow = static_cast<bigit>(result >> (bigit_bits * 2 - 1));
  }

  void remove_leading_zeros() {
	int num_bigits = static_cast<int>(bigits_.size()) - 1;
	while (num_bigits > 0 && (*this)[num_bigits] == 0) --num_bigits;
	bigits_.resize(to_unsigned(num_bigits + 1));
  }

  // Computes *this -= other assuming aligned bigints and *this >= other.
  void subtract_aligned(const bigint& other) {
	FMT_ASSERT(other.exp_ >= exp_, "unaligned bigints");
	FMT_ASSERT(compare(*this, other) >= 0, "");
	bigit borrow = 0;
	int i = other.exp_ - exp_;
	for (size_t j = 0, n = other.bigits_.size(); j != n; ++i, ++j)
	  subtract_bigits(i, other.bigits_[j], borrow);
	while (borrow > 0) subtract_bigits(i, 0, borrow);
	remove_leading_zeros();
  }

  void multiply(uint32_t value) {
	const double_bigit wide_value = value;
	bigit carry = 0;
	for (size_t i = 0, n = bigits_.size(); i < n; ++i) {
	  double_bigit result = bigits_[i] * wide_value + carry;
	  bigits_[i] = static_cast<bigit>(result);
	  carry = static_cast<bigit>(result >> bigit_bits);
	}
	if (carry != 0) bigits_.push_back(carry);
  }

  void multiply(uint64_t value) {
	const bigit mask = ~bigit(0);
	const double_bigit lower = value & mask;
	const double_bigit upper = value >> bigit_bits;
	double_bigit carry = 0;
	for (size_t i = 0, n = bigits_.size(); i < n; ++i) {
	  double_bigit result = bigits_[i] * lower + (carry & mask);
	  carry =
		  bigits_[i] * upper + (result >> bigit_bits) + (carry >> bigit_bits);
	  bigits_[i] = static_cast<bigit>(result);
	}
	while (carry != 0) {
	  bigits_.push_back(carry & mask);
	  carry >>= bigit_bits;
	}
  }

 public:
  bigint() : exp_(0) {}
  explicit bigint(uint64_t n) { assign(n); }
  ~bigint() { assert(bigits_.capacity() <= bigits_capacity); }

  bigint(const bigint&) = delete;
  void operator=(const bigint&) = delete;

  void assign(const bigint& other) {
	auto size = other.bigits_.size();
	bigits_.resize(size);
	auto data = other.bigits_.data();
	std::copy(data, data + size, make_checked(bigits_.data(), size));
	exp_ = other.exp_;
  }

  void assign(uint64_t n) {
	size_t num_bigits = 0;
	do {
	  bigits_[num_bigits++] = n & ~bigit(0);
	  n >>= bigit_bits;
	} while (n != 0);
	bigits_.resize(num_bigits);
	exp_ = 0;
  }

  int num_bigits() const { return static_cast<int>(bigits_.size()) + exp_; }

  FMT_NOINLINE bigint& operator<<=(int shift) {
	assert(shift >= 0);
	exp_ += shift / bigit_bits;
	shift %= bigit_bits;
	if (shift == 0) return *this;
	bigit carry = 0;
	for (size_t i = 0, n = bigits_.size(); i < n; ++i) {
	  bigit c = bigits_[i] >> (bigit_bits - shift);
	  bigits_[i] = (bigits_[i] << shift) + carry;
	  carry = c;
	}
	if (carry != 0) bigits_.push_back(carry);
	return *this;
  }

  template <typename Int> bigint& operator*=(Int value) {
	FMT_ASSERT(value > 0, "");
	multiply(uint32_or_64_or_128_t<Int>(value));
	return *this;
  }

  friend int compare(const bigint& lhs, const bigint& rhs) {
	int num_lhs_bigits = lhs.num_bigits(), num_rhs_bigits = rhs.num_bigits();
	if (num_lhs_bigits != num_rhs_bigits)
	  return num_lhs_bigits > num_rhs_bigits ? 1 : -1;
	int i = static_cast<int>(lhs.bigits_.size()) - 1;
	int j = static_cast<int>(rhs.bigits_.size()) - 1;
	int end = i - j;
	if (end < 0) end = 0;
	for (; i >= end; --i, --j) {
	  bigit lhs_bigit = lhs[i], rhs_bigit = rhs[j];
	  if (lhs_bigit != rhs_bigit) return lhs_bigit > rhs_bigit ? 1 : -1;
	}
	if (i != j) return i > j ? 1 : -1;
	return 0;
  }

  // Returns compare(lhs1 + lhs2, rhs).
  friend int add_compare(const bigint& lhs1, const bigint& lhs2,
						 const bigint& rhs) {
	int max_lhs_bigits = (std::max)(lhs1.num_bigits(), lhs2.num_bigits());
	int num_rhs_bigits = rhs.num_bigits();
	if (max_lhs_bigits + 1 < num_rhs_bigits) return -1;
	if (max_lhs_bigits > num_rhs_bigits) return 1;
	auto get_bigit = [](const bigint& n, int i) -> bigit {
	  return i >= n.exp_ && i < n.num_bigits() ? n[i - n.exp_] : 0;
	};
	double_bigit borrow = 0;
	int min_exp = (std::min)((std::min)(lhs1.exp_, lhs2.exp_), rhs.exp_);
	for (int i = num_rhs_bigits - 1; i >= min_exp; --i) {
	  double_bigit sum =
		  static_cast<double_bigit>(get_bigit(lhs1, i)) + get_bigit(lhs2, i);
	  bigit rhs_bigit = get_bigit(rhs, i);
	  if (sum > rhs_bigit + borrow) return 1;
	  borrow = rhs_bigit + borrow - sum;
	  if (borrow > 1) return -1;
	  borrow <<= bigit_bits;
	}
	return borrow != 0 ? -1 : 0;
  }

  // Assigns pow(10, exp) to this bigint.
  void assign_pow10(int exp) {
	assert(exp >= 0);
	if (exp == 0) return assign(1);
	// Find the top bit.
	int bitmask = 1;
	while (exp >= bitmask) bitmask <<= 1;
	bitmask >>= 1;
	// pow(10, exp) = pow(5, exp) * pow(2, exp). First compute pow(5, exp) by
	// repeated squaring and multiplication.
	assign(5);
	bitmask >>= 1;
	while (bitmask != 0) {
	  square();
	  if ((exp & bitmask) != 0) *this *= 5;
	  bitmask >>= 1;
	}
	*this <<= exp;  // Multiply by pow(2, exp) by shifting.
  }

  void square() {
	basic_memory_buffer<bigit, bigits_capacity> n(std::move(bigits_));
	int num_bigits = static_cast<int>(bigits_.size());
	int num_result_bigits = 2 * num_bigits;
	bigits_.resize(to_unsigned(num_result_bigits));
	using accumulator_t = conditional_t<FMT_USE_INT128, uint128_t, accumulator>;
	auto sum = accumulator_t();
	for (int bigit_index = 0; bigit_index < num_bigits; ++bigit_index) {
	  // Compute bigit at position bigit_index of the result by adding
	  // cross-product terms n[i] * n[j] such that i + j == bigit_index.
	  for (int i = 0, j = bigit_index; j >= 0; ++i, --j) {
		// Most terms are multiplied twice which can be optimized in the future.
		sum += static_cast<double_bigit>(n[i]) * n[j];
	  }
	  (*this)[bigit_index] = static_cast<bigit>(sum);
	  sum >>= bits<bigit>::value;  // Compute the carry.
	}
	// Do the same for the top half.
	for (int bigit_index = num_bigits; bigit_index < num_result_bigits;
		 ++bigit_index) {
	  for (int j = num_bigits - 1, i = bigit_index - j; i < num_bigits;)
		sum += static_cast<double_bigit>(n[i++]) * n[j--];
	  (*this)[bigit_index] = static_cast<bigit>(sum);
	  sum >>= bits<bigit>::value;
	}
	--num_result_bigits;
	remove_leading_zeros();
	exp_ *= 2;
  }

  // If this bigint has a bigger exponent than other, adds trailing zero to make
  // exponents equal. This simplifies some operations such as subtraction.
  void align(const bigint& other) {
	int exp_difference = exp_ - other.exp_;
	if (exp_difference <= 0) return;
	int num_bigits = static_cast<int>(bigits_.size());
	bigits_.resize(to_unsigned(num_bigits + exp_difference));
	for (int i = num_bigits - 1, j = i + exp_difference; i >= 0; --i, --j)
	  bigits_[j] = bigits_[i];
	std::uninitialized_fill_n(bigits_.data(), exp_difference, 0);
	exp_ -= exp_difference;
  }

  // Divides this bignum by divisor, assigning the remainder to this and
  // returning the quotient.
  int divmod_assign(const bigint& divisor) {
	FMT_ASSERT(this != &divisor, "");
	if (compare(*this, divisor) < 0) return 0;
	FMT_ASSERT(divisor.bigits_[divisor.bigits_.size() - 1u] != 0, "");
	align(divisor);
	int quotient = 0;
	do {
	  subtract_aligned(divisor);
	  ++quotient;
	} while (compare(*this, divisor) >= 0);
	return quotient;
  }
};

enum class round_direction { unknown, up, down };

// Given the divisor (normally a power of 10), the remainder = v % divisor for
// some number v and the error, returns whether v should be rounded up, down, or
// whether the rounding direction can't be determined due to error.
// error should be less than divisor / 2.
inline round_direction get_round_direction(uint64_t divisor, uint64_t remainder,
										   uint64_t error) {
  FMT_ASSERT(remainder < divisor, "");  // divisor - remainder won't overflow.
  FMT_ASSERT(error < divisor, "");      // divisor - error won't overflow.
  FMT_ASSERT(error < divisor - error, "");  // error * 2 won't overflow.
  // Round down if (remainder + error) * 2 <= divisor.
  if (remainder <= divisor - remainder && error * 2 <= divisor - remainder * 2)
	return round_direction::down;
  // Round up if (remainder - error) * 2 >= divisor.
  if (remainder >= error &&
	  remainder - error >= divisor - (remainder - error)) {
	return round_direction::up;
  }
  return round_direction::unknown;
}

namespace digits {
enum result {
  more,  // Generate more digits.
  done,  // Done generating digits.
  error  // Digit generation cancelled due to an error.
};
}

// Generates output using the Grisu digit-gen algorithm.
// error: the size of the region (lower, upper) outside of which numbers
// definitely do not round to value (Delta in Grisu3).
template <typename Handler>
FMT_ALWAYS_INLINE digits::result grisu_gen_digits(fp value, uint64_t error,
												  int& exp, Handler& handler) {
  const fp one(1ULL << -value.e, value.e);
  // The integral part of scaled value (p1 in Grisu) = value / one. It cannot be
  // zero because it contains a product of two 64-bit numbers with MSB set (due
  // to normalization) - 1, shifted right by at most 60 bits.
  auto integral = static_cast<uint32_t>(value.f >> -one.e);
  FMT_ASSERT(integral != 0, "");
  FMT_ASSERT(integral == value.f >> -one.e, "");
  // The fractional part of scaled value (p2 in Grisu) c = value % one.
  uint64_t fractional = value.f & (one.f - 1);
  exp = count_digits(integral);  // kappa in Grisu.
  // Divide by 10 to prevent overflow.
  auto result = handler.on_start(data::powers_of_10_64[exp - 1] << -one.e,
								 value.f / 10, error * 10, exp);
  if (result != digits::more) return result;
  // Generate digits for the integral part. This can produce up to 10 digits.
  do {
	uint32_t digit = 0;
	auto divmod_integral = [&](uint32_t divisor) {
	  digit = integral / divisor;
	  integral %= divisor;
	};
	// This optimization by Milo Yip reduces the number of integer divisions by
	// one per iteration.
	switch (exp) {
	case 10:
	  divmod_integral(1000000000);
	  break;
	case 9:
	  divmod_integral(100000000);
	  break;
	case 8:
	  divmod_integral(10000000);
	  break;
	case 7:
	  divmod_integral(1000000);
	  break;
	case 6:
	  divmod_integral(100000);
	  break;
	case 5:
	  divmod_integral(10000);
	  break;
	case 4:
	  divmod_integral(1000);
	  break;
	case 3:
	  divmod_integral(100);
	  break;
	case 2:
	  divmod_integral(10);
	  break;
	case 1:
	  digit = integral;
	  integral = 0;
	  break;
	default:
	  FMT_ASSERT(false, "invalid number of digits");
	}
	--exp;
	auto remainder = (static_cast<uint64_t>(integral) << -one.e) + fractional;
	result = handler.on_digit(static_cast<char>('0' + digit),
							  data::powers_of_10_64[exp] << -one.e, remainder,
							  error, exp, true);
	if (result != digits::more) return result;
  } while (exp > 0);
  // Generate digits for the fractional part.
  for (;;) {
	fractional *= 10;
	error *= 10;
	char digit = static_cast<char>('0' + (fractional >> -one.e));
	fractional &= one.f - 1;
	--exp;
	result = handler.on_digit(digit, one.f, fractional, error, exp, false);
	if (result != digits::more) return result;
  }
}

// The fixed precision digit handler.
struct fixed_handler {
  char* buf;
  int size;
  int precision;
  int exp10;
  bool fixed;

  digits::result on_start(uint64_t divisor, uint64_t remainder, uint64_t error,
						  int& exp) {
	// Non-fixed formats require at least one digit and no precision adjustment.
	if (!fixed) return digits::more;
	// Adjust fixed precision by exponent because it is relative to decimal
	// point.
	precision += exp + exp10;
	// Check if precision is satisfied just by leading zeros, e.g.
	// format("{:.2f}", 0.001) gives "0.00" without generating any digits.
	if (precision > 0) return digits::more;
	if (precision < 0) return digits::done;
	auto dir = get_round_direction(divisor, remainder, error);
	if (dir == round_direction::unknown) return digits::error;
	buf[size++] = dir == round_direction::up ? '1' : '0';
	return digits::done;
  }

  digits::result on_digit(char digit, uint64_t divisor, uint64_t remainder,
						  uint64_t error, int, bool integral) {
	FMT_ASSERT(remainder < divisor, "");
	buf[size++] = digit;
	if (!integral && error >= remainder) return digits::error;
	if (size < precision) return digits::more;
	if (!integral) {
	  // Check if error * 2 < divisor with overflow prevention.
	  // The check is not needed for the integral part because error = 1
	  // and divisor > (1 << 32) there.
	  if (error >= divisor || error >= divisor - error) return digits::error;
	} else {
	  FMT_ASSERT(error == 1 && divisor > 2, "");
	}
	auto dir = get_round_direction(divisor, remainder, error);
	if (dir != round_direction::up)
	  return dir == round_direction::down ? digits::done : digits::error;
	++buf[size - 1];
	for (int i = size - 1; i > 0 && buf[i] > '9'; --i) {
	  buf[i] = '0';
	  ++buf[i - 1];
	}
	if (buf[0] > '9') {
	  buf[0] = '1';
	  if (fixed)
		buf[size++] = '0';
	  else
		++exp10;
	}
	return digits::done;
  }
};

// Implementation of Dragonbox algorithm: https://github.com/jk-jeon/dragonbox.
namespace dragonbox {
// Computes 128-bit result of multiplication of two 64-bit unsigned integers.
FMT_SAFEBUFFERS inline uint128_wrapper umul128(uint64_t x,
											   uint64_t y) FMT_NOEXCEPT {
#if FMT_USE_INT128
  return static_cast<uint128_t>(x) * static_cast<uint128_t>(y);
#elif defined(_MSC_VER) && defined(_M_X64)
  uint128_wrapper result;
  result.low_ = _umul128(x, y, &result.high_);
  return result;
#else
  const uint64_t mask = (uint64_t(1) << 32) - uint64_t(1);

  uint64_t a = x >> 32;
  uint64_t b = x & mask;
  uint64_t c = y >> 32;
  uint64_t d = y & mask;

  uint64_t ac = a * c;
  uint64_t bc = b * c;
  uint64_t ad = a * d;
  uint64_t bd = b * d;

  uint64_t intermediate = (bd >> 32) + (ad & mask) + (bc & mask);

  return {ac + (intermediate >> 32) + (ad >> 32) + (bc >> 32),
		  (intermediate << 32) + (bd & mask)};
#endif
}

// Computes upper 64 bits of multiplication of two 64-bit unsigned integers.
FMT_SAFEBUFFERS inline uint64_t umul128_upper64(uint64_t x,
												uint64_t y) FMT_NOEXCEPT {
#if FMT_USE_INT128
  auto p = static_cast<uint128_t>(x) * static_cast<uint128_t>(y);
  return static_cast<uint64_t>(p >> 64);
#elif defined(_MSC_VER) && defined(_M_X64)
  return __umulh(x, y);
#else
  return umul128(x, y).high();
#endif
}

// Computes upper 64 bits of multiplication of a 64-bit unsigned integer and a
// 128-bit unsigned integer.
FMT_SAFEBUFFERS inline uint64_t umul192_upper64(uint64_t x, uint128_wrapper y)
	FMT_NOEXCEPT {
  uint128_wrapper g0 = umul128(x, y.high());
  g0 += umul128_upper64(x, y.low());
  return g0.high();
}

// Computes upper 32 bits of multiplication of a 32-bit unsigned integer and a
// 64-bit unsigned integer.
inline uint32_t umul96_upper32(uint32_t x, uint64_t y) FMT_NOEXCEPT {
  return static_cast<uint32_t>(umul128_upper64(x, y));
}

// Computes middle 64 bits of multiplication of a 64-bit unsigned integer and a
// 128-bit unsigned integer.
FMT_SAFEBUFFERS inline uint64_t umul192_middle64(uint64_t x, uint128_wrapper y)
	FMT_NOEXCEPT {
  uint64_t g01 = x * y.high();
  uint64_t g10 = umul128_upper64(x, y.low());
  return g01 + g10;
}

// Computes lower 64 bits of multiplication of a 32-bit unsigned integer and a
// 64-bit unsigned integer.
inline uint64_t umul96_lower64(uint32_t x, uint64_t y) FMT_NOEXCEPT {
  return x * y;
}

// Computes floor(log10(pow(2, e))) for e in [-1700, 1700] using the method from
// https://fmt.dev/papers/Grisu-Exact.pdf#page=5, section 3.4.
inline int floor_log10_pow2(int e) FMT_NOEXCEPT {
  FMT_ASSERT(e <= 1700 && e >= -1700, "too large exponent");
  const int shift = 22;
  return (e * static_cast<int>(data::log10_2_significand >> (64 - shift))) >>
		 shift;
}

// Various fast log computations.
inline int floor_log2_pow10(int e) FMT_NOEXCEPT {
  FMT_ASSERT(e <= 1233 && e >= -1233, "too large exponent");
  const uint64_t log2_10_integer_part = 3;
  const uint64_t log2_10_fractional_digits = 0x5269e12f346e2bf9;
  const int shift_amount = 19;
  return (e * static_cast<int>(
				  (log2_10_integer_part << shift_amount) |
				  (log2_10_fractional_digits >> (64 - shift_amount)))) >>
		 shift_amount;
}
inline int floor_log10_pow2_minus_log10_4_over_3(int e) FMT_NOEXCEPT {
  FMT_ASSERT(e <= 1700 && e >= -1700, "too large exponent");
  const uint64_t log10_4_over_3_fractional_digits = 0x1ffbfc2bbc780375;
  const int shift_amount = 22;
  return (e * static_cast<int>(data::log10_2_significand >>
							   (64 - shift_amount)) -
		  static_cast<int>(log10_4_over_3_fractional_digits >>
						   (64 - shift_amount))) >>
		 shift_amount;
}

// Returns true iff x is divisible by pow(2, exp).
inline bool divisible_by_power_of_2(uint32_t x, int exp) FMT_NOEXCEPT {
  FMT_ASSERT(exp >= 1, "");
  FMT_ASSERT(x != 0, "");
#ifdef FMT_BUILTIN_CTZ
  return FMT_BUILTIN_CTZ(x) >= exp;
#else
  return exp < num_bits<uint32_t>() && x == ((x >> exp) << exp);
#endif
}
inline bool divisible_by_power_of_2(uint64_t x, int exp) FMT_NOEXCEPT {
  FMT_ASSERT(exp >= 1, "");
  FMT_ASSERT(x != 0, "");
#ifdef FMT_BUILTIN_CTZLL
  return FMT_BUILTIN_CTZLL(x) >= exp;
#else
  return exp < num_bits<uint64_t>() && x == ((x >> exp) << exp);
#endif
}

// Returns true iff x is divisible by pow(5, exp).
inline bool divisible_by_power_of_5(uint32_t x, int exp) FMT_NOEXCEPT {
  FMT_ASSERT(exp <= 10, "too large exponent");
  return x * data::divtest_table_for_pow5_32[exp].mod_inv <=
		 data::divtest_table_for_pow5_32[exp].max_quotient;
}
inline bool divisible_by_power_of_5(uint64_t x, int exp) FMT_NOEXCEPT {
  FMT_ASSERT(exp <= 23, "too large exponent");
  return x * data::divtest_table_for_pow5_64[exp].mod_inv <=
		 data::divtest_table_for_pow5_64[exp].max_quotient;
}

// Replaces n by floor(n / pow(5, N)) returning true if and only if n is
// divisible by pow(5, N).
// Precondition: n <= 2 * pow(5, N + 1).
template <int N>
bool check_divisibility_and_divide_by_pow5(uint32_t& n) FMT_NOEXCEPT {
  static constexpr struct {
	uint32_t magic_number;
	int bits_for_comparison;
	uint32_t threshold;
	int shift_amount;
  } infos[] = { {0xcccd, 16, 0x3333, 18}, {0xa429, 8, 0x0a, 20} };
  constexpr auto info = infos[N - 1];
  n *= info.magic_number;
  const uint32_t comparison_mask = (1u << info.bits_for_comparison) - 1;
  bool result = (n & comparison_mask) <= info.threshold;
  n >>= info.shift_amount;
  return result;
}

// Computes floor(n / pow(10, N)) for small n and N.
// Precondition: n <= pow(10, N + 1).
template <int N> uint32_t small_division_by_pow10(uint32_t n) FMT_NOEXCEPT {
  static constexpr struct {
	uint32_t magic_number;
	int shift_amount;
	uint32_t divisor_times_10;
  } infos[] = { {0xcccd, 19, 100}, {0xa3d8, 22, 1000} };
  constexpr auto info = infos[N - 1];
  FMT_ASSERT(n <= info.divisor_times_10, "n is too large");
  return n * info.magic_number >> info.shift_amount;
}

// Computes floor(n / 10^(kappa + 1)) (float)
inline uint32_t divide_by_10_to_kappa_plus_1(uint32_t n) FMT_NOEXCEPT {
  return n / float_info<float>::big_divisor;
}
// Computes floor(n / 10^(kappa + 1)) (double)
inline uint64_t divide_by_10_to_kappa_plus_1(uint64_t n) FMT_NOEXCEPT {
  return umul128_upper64(n, 0x83126e978d4fdf3c) >> 9;
}

// Various subroutines using pow10 cache
template <class T> struct cache_accessor;

template <> struct cache_accessor<float> {
  using carrier_uint = float_info<float>::carrier_uint;
  using cache_entry_type = uint64_t;

  static uint64_t get_cached_power(int k) FMT_NOEXCEPT {
	FMT_ASSERT(k >= float_info<float>::min_k && k <= float_info<float>::max_k,
			   "k is out of range");
	return data::dragonbox_pow10_significands_64[k - float_info<float>::min_k];
  }

  static carrier_uint compute_mul(carrier_uint u,
								  const cache_entry_type& cache) FMT_NOEXCEPT {
	return umul96_upper32(u, cache);
  }

  static uint32_t compute_delta(const cache_entry_type& cache,
								int beta_minus_1) FMT_NOEXCEPT {
	return static_cast<uint32_t>(cache >> (64 - 1 - beta_minus_1));
  }

  static bool compute_mul_parity(carrier_uint two_f,
								 const cache_entry_type& cache,
								 int beta_minus_1) FMT_NOEXCEPT {
	FMT_ASSERT(beta_minus_1 >= 1, "");
	FMT_ASSERT(beta_minus_1 < 64, "");

	return ((umul96_lower64(two_f, cache) >> (64 - beta_minus_1)) & 1) != 0;
  }

  static carrier_uint compute_left_endpoint_for_shorter_interval_case(
	  const cache_entry_type& cache, int beta_minus_1) FMT_NOEXCEPT {
	return static_cast<carrier_uint>(
		(cache - (cache >> (float_info<float>::significand_bits + 2))) >>
		(64 - float_info<float>::significand_bits - 1 - beta_minus_1));
  }

  static carrier_uint compute_right_endpoint_for_shorter_interval_case(
	  const cache_entry_type& cache, int beta_minus_1) FMT_NOEXCEPT {
	return static_cast<carrier_uint>(
		(cache + (cache >> (float_info<float>::significand_bits + 1))) >>
		(64 - float_info<float>::significand_bits - 1 - beta_minus_1));
  }

  static carrier_uint compute_round_up_for_shorter_interval_case(
	  const cache_entry_type& cache, int beta_minus_1) FMT_NOEXCEPT {
	return (static_cast<carrier_uint>(
				cache >>
				(64 - float_info<float>::significand_bits - 2 - beta_minus_1)) +
			1) /
		   2;
  }
};

template <> struct cache_accessor<double> {
  using carrier_uint = float_info<double>::carrier_uint;
  using cache_entry_type = uint128_wrapper;

  static uint128_wrapper get_cached_power(int k) FMT_NOEXCEPT {
	FMT_ASSERT(k >= float_info<double>::min_k && k <= float_info<double>::max_k,
			   "k is out of range");

#if FMT_USE_FULL_CACHE_DRAGONBOX
	return data::dragonbox_pow10_significands_128[k -
												  float_info<double>::min_k];
#else
	static const int compression_ratio = 27;

	// Compute base index.
	int cache_index = (k - float_info<double>::min_k) / compression_ratio;
	int kb = cache_index * compression_ratio + float_info<double>::min_k;
	int offset = k - kb;

	// Get base cache.
	uint128_wrapper base_cache =
		data::dragonbox_pow10_significands_128[cache_index];
	if (offset == 0) return base_cache;

	// Compute the required amount of bit-shift.
	int alpha = floor_log2_pow10(kb + offset) - floor_log2_pow10(kb) - offset;
	FMT_ASSERT(alpha > 0 && alpha < 64, "shifting error detected");

	// Try to recover the real cache.
	uint64_t pow5 = data::powers_of_5_64[offset];
	uint128_wrapper recovered_cache = umul128(base_cache.high(), pow5);
	uint128_wrapper middle_low =
		umul128(base_cache.low() - (kb < 0 ? 1u : 0u), pow5);

	recovered_cache += middle_low.high();

	uint64_t high_to_middle = recovered_cache.high() << (64 - alpha);
	uint64_t middle_to_low = recovered_cache.low() << (64 - alpha);

	recovered_cache =
		uint128_wrapper{(recovered_cache.low() >> alpha) | high_to_middle,
						((middle_low.low() >> alpha) | middle_to_low)};

	if (kb < 0) recovered_cache += 1;

	// Get error.
	int error_idx = (k - float_info<double>::min_k) / 16;
	uint32_t error = (data::dragonbox_pow10_recovery_errors[error_idx] >>
					  ((k - float_info<double>::min_k) % 16) * 2) &
					 0x3;

	// Add the error back.
	FMT_ASSERT(recovered_cache.low() + error >= recovered_cache.low(), "");
	return {recovered_cache.high(), recovered_cache.low() + error};
#endif
  }

  static carrier_uint compute_mul(carrier_uint u,
								  const cache_entry_type& cache) FMT_NOEXCEPT {
	return umul192_upper64(u, cache);
  }

  static uint32_t compute_delta(cache_entry_type const& cache,
								int beta_minus_1) FMT_NOEXCEPT {
	return static_cast<uint32_t>(cache.high() >> (64 - 1 - beta_minus_1));
  }

  static bool compute_mul_parity(carrier_uint two_f,
								 const cache_entry_type& cache,
								 int beta_minus_1) FMT_NOEXCEPT {
	FMT_ASSERT(beta_minus_1 >= 1, "");
	FMT_ASSERT(beta_minus_1 < 64, "");

	return ((umul192_middle64(two_f, cache) >> (64 - beta_minus_1)) & 1) != 0;
  }

  static carrier_uint compute_left_endpoint_for_shorter_interval_case(
	  const cache_entry_type& cache, int beta_minus_1) FMT_NOEXCEPT {
	return (cache.high() -
			(cache.high() >> (float_info<double>::significand_bits + 2))) >>
		   (64 - float_info<double>::significand_bits - 1 - beta_minus_1);
  }

  static carrier_uint compute_right_endpoint_for_shorter_interval_case(
	  const cache_entry_type& cache, int beta_minus_1) FMT_NOEXCEPT {
	return (cache.high() +
			(cache.high() >> (float_info<double>::significand_bits + 1))) >>
		   (64 - float_info<double>::significand_bits - 1 - beta_minus_1);
  }

  static carrier_uint compute_round_up_for_shorter_interval_case(
	  const cache_entry_type& cache, int beta_minus_1) FMT_NOEXCEPT {
	return ((cache.high() >>
			 (64 - float_info<double>::significand_bits - 2 - beta_minus_1)) +
			1) /
		   2;
  }
};

// Various integer checks
template <class T>
bool is_left_endpoint_integer_shorter_interval(int exponent) FMT_NOEXCEPT {
  return exponent >=
			 float_info<
				 T>::case_shorter_interval_left_endpoint_lower_threshold &&
		 exponent <=
			 float_info<T>::case_shorter_interval_left_endpoint_upper_threshold;
}
template <class T>
bool is_endpoint_integer(typename float_info<T>::carrier_uint two_f,
						 int exponent, int minus_k) FMT_NOEXCEPT {
  if (exponent < float_info<T>::case_fc_pm_half_lower_threshold) return false;
  // For k >= 0.
  if (exponent <= float_info<T>::case_fc_pm_half_upper_threshold) return true;
  // For k < 0.
  if (exponent > float_info<T>::divisibility_check_by_5_threshold) return false;
  return divisible_by_power_of_5(two_f, minus_k);
}

template <class T>
bool is_center_integer(typename float_info<T>::carrier_uint two_f, int exponent,
					   int minus_k) FMT_NOEXCEPT {
  // Exponent for 5 is negative.
  if (exponent > float_info<T>::divisibility_check_by_5_threshold) return false;
  if (exponent > float_info<T>::case_fc_upper_threshold)
	return divisible_by_power_of_5(two_f, minus_k);
  // Both exponents are nonnegative.
  if (exponent >= float_info<T>::case_fc_lower_threshold) return true;
  // Exponent for 2 is negative.
  return divisible_by_power_of_2(two_f, minus_k - exponent + 1);
}

// Remove trailing zeros from n and return the number of zeros removed (float)
FMT_ALWAYS_INLINE int remove_trailing_zeros(uint32_t& n) FMT_NOEXCEPT {
#ifdef FMT_BUILTIN_CTZ
  int t = FMT_BUILTIN_CTZ(n);
#else
  int t = ctz(n);
#endif
  if (t > float_info<float>::max_trailing_zeros)
	t = float_info<float>::max_trailing_zeros;

  const uint32_t mod_inv1 = 0xcccccccd;
  const uint32_t max_quotient1 = 0x33333333;
  const uint32_t mod_inv2 = 0xc28f5c29;
  const uint32_t max_quotient2 = 0x0a3d70a3;

  int s = 0;
  for (; s < t - 1; s += 2) {
	if (n * mod_inv2 > max_quotient2) break;
	n *= mod_inv2;
  }
  if (s < t && n * mod_inv1 <= max_quotient1) {
	n *= mod_inv1;
	++s;
  }
  n >>= s;
  return s;
}

// Removes trailing zeros and returns the number of zeros removed (double)
FMT_ALWAYS_INLINE int remove_trailing_zeros(uint64_t& n) FMT_NOEXCEPT {
#ifdef FMT_BUILTIN_CTZLL
  int t = FMT_BUILTIN_CTZLL(n);
#else
  int t = ctzll(n);
#endif
  if (t > float_info<double>::max_trailing_zeros)
	t = float_info<double>::max_trailing_zeros;
  // Divide by 10^8 and reduce to 32-bits
  // Since ret_value.significand <= (2^64 - 1) / 1000 < 10^17,
  // both of the quotient and the r should fit in 32-bits

  const uint32_t mod_inv1 = 0xcccccccd;
  const uint32_t max_quotient1 = 0x33333333;
  const uint64_t mod_inv8 = 0xc767074b22e90e21;
  const uint64_t max_quotient8 = 0x00002af31dc46118;

  // If the number is divisible by 1'0000'0000, work with the quotient
  if (t >= 8) {
	auto quotient_candidate = n * mod_inv8;

	if (quotient_candidate <= max_quotient8) {
	  auto quotient = static_cast<uint32_t>(quotient_candidate >> 8);

	  int s = 8;
	  for (; s < t; ++s) {
		if (quotient * mod_inv1 > max_quotient1) break;
		quotient *= mod_inv1;
	  }
	  quotient >>= (s - 8);
	  n = quotient;
	  return s;
	}
  }

  // Otherwise, work with the remainder
  auto quotient = static_cast<uint32_t>(n / 100000000);
  auto remainder = static_cast<uint32_t>(n - 100000000 * quotient);

  if (t == 0 || remainder * mod_inv1 > max_quotient1) {
	return 0;
  }
  remainder *= mod_inv1;

  if (t == 1 || remainder * mod_inv1 > max_quotient1) {
	n = (remainder >> 1) + quotient * 10000000ull;
	return 1;
  }
  remainder *= mod_inv1;

  if (t == 2 || remainder * mod_inv1 > max_quotient1) {
	n = (remainder >> 2) + quotient * 1000000ull;
	return 2;
  }
  remainder *= mod_inv1;

  if (t == 3 || remainder * mod_inv1 > max_quotient1) {
	n = (remainder >> 3) + quotient * 100000ull;
	return 3;
  }
  remainder *= mod_inv1;

  if (t == 4 || remainder * mod_inv1 > max_quotient1) {
	n = (remainder >> 4) + quotient * 10000ull;
	return 4;
  }
  remainder *= mod_inv1;

  if (t == 5 || remainder * mod_inv1 > max_quotient1) {
	n = (remainder >> 5) + quotient * 1000ull;
	return 5;
  }
  remainder *= mod_inv1;

  if (t == 6 || remainder * mod_inv1 > max_quotient1) {
	n = (remainder >> 6) + quotient * 100ull;
	return 6;
  }
  remainder *= mod_inv1;

  n = (remainder >> 7) + quotient * 10ull;
  return 7;
}

// The main algorithm for shorter interval case
template <class T>
FMT_ALWAYS_INLINE FMT_SAFEBUFFERS decimal_fp<T> shorter_interval_case(
	int exponent) FMT_NOEXCEPT {
  decimal_fp<T> ret_value;
  // Compute k and beta
  const int minus_k = floor_log10_pow2_minus_log10_4_over_3(exponent);
  const int beta_minus_1 = exponent + floor_log2_pow10(-minus_k);

  // Compute xi and zi
  using cache_entry_type = typename cache_accessor<T>::cache_entry_type;
  const cache_entry_type cache = cache_accessor<T>::get_cached_power(-minus_k);

  auto xi = cache_accessor<T>::compute_left_endpoint_for_shorter_interval_case(
	  cache, beta_minus_1);
  auto zi = cache_accessor<T>::compute_right_endpoint_for_shorter_interval_case(
	  cache, beta_minus_1);

  // If the left endpoint is not an integer, increase it
  if (!is_left_endpoint_integer_shorter_interval<T>(exponent)) ++xi;

  // Try bigger divisor
  ret_value.significand = zi / 10;

  // If succeed, remove trailing zeros if necessary and return
  if (ret_value.significand * 10 >= xi) {
	ret_value.exponent = minus_k + 1;
	ret_value.exponent += remove_trailing_zeros(ret_value.significand);
	return ret_value;
  }

  // Otherwise, compute the round-up of y
  ret_value.significand =
	  cache_accessor<T>::compute_round_up_for_shorter_interval_case(
		  cache, beta_minus_1);
  ret_value.exponent = minus_k;

  // When tie occurs, choose one of them according to the rule
  if (exponent >= float_info<T>::shorter_interval_tie_lower_threshold &&
	  exponent <= float_info<T>::shorter_interval_tie_upper_threshold) {
	ret_value.significand = ret_value.significand % 2 == 0
								? ret_value.significand
								: ret_value.significand - 1;
  } else if (ret_value.significand < xi) {
	++ret_value.significand;
  }
  return ret_value;
}

template <typename T>
FMT_SAFEBUFFERS decimal_fp<T> to_decimal(T x) FMT_NOEXCEPT {
  // Step 1: integer promotion & Schubfach multiplier calculation.

  using carrier_uint = typename float_info<T>::carrier_uint;
  using cache_entry_type = typename cache_accessor<T>::cache_entry_type;
  auto br = bit_cast<carrier_uint>(x);

  // Extract significand bits and exponent bits.
  const carrier_uint significand_mask =
	  (static_cast<carrier_uint>(1) << float_info<T>::significand_bits) - 1;
  carrier_uint significand = (br & significand_mask);
  int exponent = static_cast<int>((br & exponent_mask<T>()) >>
								  float_info<T>::significand_bits);

  if (exponent != 0) {  // Check if normal.
	exponent += float_info<T>::exponent_bias - float_info<T>::significand_bits;

	// Shorter interval case; proceed like Schubfach.
	if (significand == 0) return shorter_interval_case<T>(exponent);

	significand |=
		(static_cast<carrier_uint>(1) << float_info<T>::significand_bits);
  } else {
	// Subnormal case; the interval is always regular.
	if (significand == 0) return {0, 0};
	exponent = float_info<T>::min_exponent - float_info<T>::significand_bits;
  }

  const bool include_left_endpoint = (significand % 2 == 0);
  const bool include_right_endpoint = include_left_endpoint;

  // Compute k and beta.
  const int minus_k = floor_log10_pow2(exponent) - float_info<T>::kappa;
  const cache_entry_type cache = cache_accessor<T>::get_cached_power(-minus_k);
  const int beta_minus_1 = exponent + floor_log2_pow10(-minus_k);

  // Compute zi and deltai
  // 10^kappa <= deltai < 10^(kappa + 1)
  const uint32_t deltai = cache_accessor<T>::compute_delta(cache, beta_minus_1);
  const carrier_uint two_fc = significand << 1;
  const carrier_uint two_fr = two_fc | 1;
  const carrier_uint zi =
	  cache_accessor<T>::compute_mul(two_fr << beta_minus_1, cache);

  // Step 2: Try larger divisor; remove trailing zeros if necessary

  // Using an upper bound on zi, we might be able to optimize the division
  // better than the compiler; we are computing zi / big_divisor here
  decimal_fp<T> ret_value;
  ret_value.significand = divide_by_10_to_kappa_plus_1(zi);
  uint32_t r = static_cast<uint32_t>(zi - float_info<T>::big_divisor *
											  ret_value.significand);

  if (r > deltai) {
	goto small_divisor_case_label;
  } else if (r < deltai) {
	// Exclude the right endpoint if necessary
	if (r == 0 && !include_right_endpoint &&
		is_endpoint_integer<T>(two_fr, exponent, minus_k)) {
	  --ret_value.significand;
	  r = float_info<T>::big_divisor;
	  goto small_divisor_case_label;
	}
  } else {
	// r == deltai; compare fractional parts
	// Check conditions in the order different from the paper
	// to take advantage of short-circuiting
	const carrier_uint two_fl = two_fc - 1;
	if ((!include_left_endpoint ||
		 !is_endpoint_integer<T>(two_fl, exponent, minus_k)) &&
		!cache_accessor<T>::compute_mul_parity(two_fl, cache, beta_minus_1)) {
	  goto small_divisor_case_label;
	}
  }
  ret_value.exponent = minus_k + float_info<T>::kappa + 1;

  // We may need to remove trailing zeros
  ret_value.exponent += remove_trailing_zeros(ret_value.significand);
  return ret_value;

  // Step 3: Find the significand with the smaller divisor

small_divisor_case_label:
  ret_value.significand *= 10;
  ret_value.exponent = minus_k + float_info<T>::kappa;

  const uint32_t mask = (1u << float_info<T>::kappa) - 1;
  auto dist = r - (deltai / 2) + (float_info<T>::small_divisor / 2);

  // Is dist divisible by 2^kappa?
  if ((dist & mask) == 0) {
	const bool approx_y_parity =
		((dist ^ (float_info<T>::small_divisor / 2)) & 1) != 0;
	dist >>= float_info<T>::kappa;

	// Is dist divisible by 5^kappa?
	if (check_divisibility_and_divide_by_pow5<float_info<T>::kappa>(dist)) {
	  ret_value.significand += dist;

	  // Check z^(f) >= epsilon^(f)
	  // We have either yi == zi - epsiloni or yi == (zi - epsiloni) - 1,
	  // where yi == zi - epsiloni if and only if z^(f) >= epsilon^(f)
	  // Since there are only 2 possibilities, we only need to care about the
	  // parity. Also, zi and r should have the same parity since the divisor
	  // is an even number
	  if (cache_accessor<T>::compute_mul_parity(two_fc, cache, beta_minus_1) !=
		  approx_y_parity) {
		--ret_value.significand;
	  } else {
		// If z^(f) >= epsilon^(f), we might have a tie
		// when z^(f) == epsilon^(f), or equivalently, when y is an integer
		if (is_center_integer<T>(two_fc, exponent, minus_k)) {
		  ret_value.significand = ret_value.significand % 2 == 0
									  ? ret_value.significand
									  : ret_value.significand - 1;
		}
	  }
	}
	// Is dist not divisible by 5^kappa?
	else {
	  ret_value.significand += dist;
	}
  }
  // Is dist not divisible by 2^kappa?
  else {
	// Since we know dist is small, we might be able to optimize the division
	// better than the compiler; we are computing dist / small_divisor here
	ret_value.significand +=
		small_division_by_pow10<float_info<T>::kappa>(dist);
  }
  return ret_value;
}
}  // namespace dragonbox

// Formats value using a variation of the Fixed-Precision Positive
// Floating-Point Printout ((FPP)^2) algorithm by Steele & White:
// https://fmt.dev/p372-steele.pdf.
template <typename Double>
void fallback_format(Double d, int num_digits, bool binary32, buffer<char>& buf,
					 int& exp10) {
  bigint numerator;    // 2 * R in (FPP)^2.
  bigint denominator;  // 2 * S in (FPP)^2.
  // lower and upper are differences between value and corresponding boundaries.
  bigint lower;             // (M^- in (FPP)^2).
  bigint upper_store;       // upper's value if different from lower.
  bigint* upper = nullptr;  // (M^+ in (FPP)^2).
  fp value;
  // Shift numerator and denominator by an extra bit or two (if lower boundary
  // is closer) to make lower and upper integers. This eliminates multiplication
  // by 2 during later computations.
  const bool is_predecessor_closer =
	  binary32 ? value.assign(static_cast<float>(d)) : value.assign(d);
  int shift = is_predecessor_closer ? 2 : 1;
  uint64_t significand = value.f << shift;
  if (value.e >= 0) {
	numerator.assign(significand);
	numerator <<= value.e;
	lower.assign(1);
	lower <<= value.e;
	if (shift != 1) {
	  upper_store.assign(1);
	  upper_store <<= value.e + 1;
	  upper = &upper_store;
	}
	denominator.assign_pow10(exp10);
	denominator <<= shift;
  } else if (exp10 < 0) {
	numerator.assign_pow10(-exp10);
	lower.assign(numerator);
	if (shift != 1) {
	  upper_store.assign(numerator);
	  upper_store <<= 1;
	  upper = &upper_store;
	}
	numerator *= significand;
	denominator.assign(1);
	denominator <<= shift - value.e;
  } else {
	numerator.assign(significand);
	denominator.assign_pow10(exp10);
	denominator <<= shift - value.e;
	lower.assign(1);
	if (shift != 1) {
	  upper_store.assign(1ULL << 1);
	  upper = &upper_store;
	}
  }
  // Invariant: value == (numerator / denominator) * pow(10, exp10).
  if (num_digits < 0) {
	// Generate the shortest representation.
	if (!upper) upper = &lower;
	bool even = (value.f & 1) == 0;
	num_digits = 0;
	char* data = buf.data();
	for (;;) {
	  int digit = numerator.divmod_assign(denominator);
	  bool low = compare(numerator, lower) - even < 0;  // numerator <[=] lower.
	  // numerator + upper >[=] pow10:
	  bool high = add_compare(numerator, *upper, denominator) + even > 0;
	  data[num_digits++] = static_cast<char>('0' + digit);
	  if (low || high) {
		if (!low) {
		  ++data[num_digits - 1];
		} else if (high) {
		  int result = add_compare(numerator, numerator, denominator);
		  // Round half to even.
		  if (result > 0 || (result == 0 && (digit % 2) != 0))
			++data[num_digits - 1];
		}
		buf.try_resize(to_unsigned(num_digits));
		exp10 -= num_digits - 1;
		return;
	  }
	  numerator *= 10;
	  lower *= 10;
	  if (upper != &lower) *upper *= 10;
	}
  }
  // Generate the given number of digits.
  exp10 -= num_digits - 1;
  if (num_digits == 0) {
	buf.try_resize(1);
	denominator *= 10;
	buf[0] = add_compare(numerator, numerator, denominator) > 0 ? '1' : '0';
	return;
  }
  buf.try_resize(to_unsigned(num_digits));
  for (int i = 0; i < num_digits - 1; ++i) {
	int digit = numerator.divmod_assign(denominator);
	buf[i] = static_cast<char>('0' + digit);
	numerator *= 10;
  }
  int digit = numerator.divmod_assign(denominator);
  auto result = add_compare(numerator, numerator, denominator);
  if (result > 0 || (result == 0 && (digit % 2) != 0)) {
	if (digit == 9) {
	  const auto overflow = '0' + 10;
	  buf[num_digits - 1] = overflow;
	  // Propagate the carry.
	  for (int i = num_digits - 1; i > 0 && buf[i] == overflow; --i) {
		buf[i] = '0';
		++buf[i - 1];
	  }
	  if (buf[0] == overflow) {
		buf[0] = '1';
		++exp10;
	  }
	  return;
	}
	++digit;
  }
  buf[num_digits - 1] = static_cast<char>('0' + digit);
}

template <typename T>
int format_float(T value, int precision, float_specs specs, buffer<char>& buf) {
  static_assert(!std::is_same<T, float>::value, "");
  FMT_ASSERT(value >= 0, "value is negative");

  const bool fixed = specs.format == float_format::fixed;
  if (value <= 0) {  // <= instead of == to silence a warning.
	if (precision <= 0 || !fixed) {
	  buf.push_back('0');
	  return 0;
	}
	buf.try_resize(to_unsigned(precision));
	std::uninitialized_fill_n(buf.data(), precision, '0');
	return -precision;
  }

  if (!specs.use_grisu) return snprintf_float(value, precision, specs, buf);

  if (precision < 0) {
	// Use Dragonbox for the shortest format.
	if (specs.binary32) {
	  auto dec = dragonbox::to_decimal(static_cast<float>(value));
	  write<char>(buffer_appender<char>(buf), dec.significand);
	  return dec.exponent;
	}
	auto dec = dragonbox::to_decimal(static_cast<double>(value));
	write<char>(buffer_appender<char>(buf), dec.significand);
	return dec.exponent;
  }

  // Use Grisu + Dragon4 for the given precision:
  // https://www.cs.tufts.edu/~nr/cs257/archive/florian-loitsch/printf.pdf.
  int exp = 0;
  const int min_exp = -60;  // alpha in Grisu.
  int cached_exp10 = 0;     // K in Grisu.
  fp normalized = normalize(fp(value));
  const auto cached_pow = get_cached_power(
	  min_exp - (normalized.e + fp::significand_size), cached_exp10);
  normalized = normalized * cached_pow;
  // Limit precision to the maximum possible number of significant digits in an
  // IEEE754 double because we don't need to generate zeros.
  const int max_double_digits = 767;
  if (precision > max_double_digits) precision = max_double_digits;
  fixed_handler handler{buf.data(), 0, precision, -cached_exp10, fixed};
  if (grisu_gen_digits(normalized, 1, exp, handler) == digits::error) {
	exp += handler.size - cached_exp10 - 1;
	fallback_format(value, handler.precision, specs.binary32, buf, exp);
  } else {
	exp += handler.exp10;
	buf.try_resize(to_unsigned(handler.size));
  }
  if (!fixed && !specs.showpoint) {
	// Remove trailing zeros.
	auto num_digits = buf.size();
	while (num_digits > 0 && buf[num_digits - 1] == '0') {
	  --num_digits;
	  ++exp;
	}
	buf.try_resize(num_digits);
  }
  return exp;
}  // namespace detail

template <typename T>
int snprintf_float(T value, int precision, float_specs specs,
				   buffer<char>& buf) {
  // Buffer capacity must be non-zero, otherwise MSVC's vsnprintf_s will fail.
  FMT_ASSERT(buf.capacity() > buf.size(), "empty buffer");
  static_assert(!std::is_same<T, float>::value, "");

  // Subtract 1 to account for the difference in precision since we use %e for
  // both general and exponent format.
  if (specs.format == float_format::general ||
	  specs.format == float_format::exp)
	precision = (precision >= 0 ? precision : 6) - 1;

  // Build the format string.
  enum { max_format_size = 7 };  // The longest format is "%#.*Le".
  char format[max_format_size];
  char* format_ptr = format;
  *format_ptr++ = '%';
  if (specs.showpoint && specs.format == float_format::hex) *format_ptr++ = '#';
  if (precision >= 0) {
	*format_ptr++ = '.';
	*format_ptr++ = '*';
  }
  if (std::is_same<T, long double>()) *format_ptr++ = 'L';
  *format_ptr++ = specs.format != float_format::hex
					  ? (specs.format == float_format::fixed ? 'f' : 'e')
					  : (specs.upper ? 'A' : 'a');
  *format_ptr = '\0';

  // Format using snprintf.
  auto offset = buf.size();
  for (;;) {
	auto begin = buf.data() + offset;
	auto capacity = buf.capacity() - offset;
#ifdef FMT_FUZZ
	if (precision > 100000)
	  throw std::runtime_error(
		  "fuzz mode - avoid large allocation inside snprintf");
#endif
	// Suppress the warning about a nonliteral format string.
	// Cannot use auto because of a bug in MinGW (#1532).
	int (*snprintf_ptr)(char*, size_t, const char*, ...) = FMT_SNPRINTF;
	int result = precision >= 0
					 ? snprintf_ptr(begin, capacity, format, precision, value)
					 : snprintf_ptr(begin, capacity, format, value);
	if (result < 0) {
	  // The buffer will grow exponentially.
	  buf.try_reserve(buf.capacity() + 1);
	  continue;
	}
	auto size = to_unsigned(result);
	// Size equal to capacity means that the last character was truncated.
	if (size >= capacity) {
	  buf.try_reserve(size + offset + 1);  // Add 1 for the terminating '\0'.
	  continue;
	}
	auto is_digit = [](char c) { return c >= '0' && c <= '9'; };
	if (specs.format == float_format::fixed) {
	  if (precision == 0) {
		buf.try_resize(size);
		return 0;
	  }
	  // Find and remove the decimal point.
	  auto end = begin + size, p = end;
	  do {
		--p;
	  } while (is_digit(*p));
	  int fraction_size = static_cast<int>(end - p - 1);
	  std::memmove(p, p + 1, to_unsigned(fraction_size));
	  buf.try_resize(size - 1);
	  return -fraction_size;
	}
	if (specs.format == float_format::hex) {
	  buf.try_resize(size + offset);
	  return 0;
	}
	// Find and parse the exponent.
	auto end = begin + size, exp_pos = end;
	do {
	  --exp_pos;
	} while (*exp_pos != 'e');
	char sign = exp_pos[1];
	assert(sign == '+' || sign == '-');
	int exp = 0;
	auto p = exp_pos + 2;  // Skip 'e' and sign.
	do {
	  assert(is_digit(*p));
	  exp = exp * 10 + (*p++ - '0');
	} while (p != end);
	if (sign == '-') exp = -exp;
	int fraction_size = 0;
	if (exp_pos != begin + 1) {
	  // Remove trailing zeros.
	  auto fraction_end = exp_pos - 1;
	  while (*fraction_end == '0') --fraction_end;
	  // Move the fractional part left to get rid of the decimal point.
	  fraction_size = static_cast<int>(fraction_end - begin - 1);
	  std::memmove(begin + 1, begin + 2, to_unsigned(fraction_size));
	}
	buf.try_resize(to_unsigned(fraction_size) + offset + 1);
	return exp - fraction_size;
  }
}

// A public domain branchless UTF-8 decoder by Christopher Wellons:
// https://github.com/skeeto/branchless-utf8
/* Decode the next character, c, from buf, reporting errors in e.
 *
 * Since this is a branchless decoder, four bytes will be read from the
 * buffer regardless of the actual length of the next character. This
 * means the buffer _must_ have at least three bytes of zero padding
 * following the end of the data stream.
 *
 * Errors are reported in e, which will be non-zero if the parsed
 * character was somehow invalid: invalid byte sequence, non-canonical
 * encoding, or a surrogate half.
 *
 * The function returns a pointer to the next character. When an error
 * occurs, this pointer will be a guess that depends on the particular
 * error, but it will always advance at least one byte.
 */
inline const char* utf8_decode(const char* buf, uint32_t* c, int* e) {
  static const int masks[] = {0x00, 0x7f, 0x1f, 0x0f, 0x07};
  static const uint32_t mins[] = {4194304, 0, 128, 2048, 65536};
  static const int shiftc[] = {0, 18, 12, 6, 0};
  static const int shifte[] = {0, 6, 4, 2, 0};

  int len = code_point_length(buf);
  const char* next = buf + len;

  // Assume a four-byte character and load four bytes. Unused bits are
  // shifted out.
  auto s = reinterpret_cast<const unsigned char*>(buf);
  *c = uint32_t(s[0] & masks[len]) << 18;
  *c |= uint32_t(s[1] & 0x3f) << 12;
  *c |= uint32_t(s[2] & 0x3f) << 6;
  *c |= uint32_t(s[3] & 0x3f) << 0;
  *c >>= shiftc[len];

  // Accumulate the various error conditions.
  *e = (*c < mins[len]) << 6;       // non-canonical encoding
  *e |= ((*c >> 11) == 0x1b) << 7;  // surrogate half?
  *e |= (*c > 0x10FFFF) << 8;       // out of range?
  *e |= (s[1] & 0xc0) >> 2;
  *e |= (s[2] & 0xc0) >> 4;
  *e |= (s[3]) >> 6;
  *e ^= 0x2a;  // top two bits of each tail byte correct?
  *e >>= shifte[len];

  return next;
}

struct stringifier {
  template <typename T> FMT_INLINE std::string operator()(T value) const {
	return to_string(value);
  }
  std::string operator()(basic_format_arg<format_context>::handle h) const {
	memory_buffer buf;
	format_parse_context parse_ctx({});
	format_context format_ctx(buffer_appender<char>(buf), {}, {});
	h.format(parse_ctx, format_ctx);
	return to_string(buf);
  }
};
}  // namespace detail

template <> struct formatter<detail::bigint> {
  format_parse_context::iterator parse(format_parse_context& ctx) {
	return ctx.begin();
  }

  format_context::iterator format(const detail::bigint& n,
								  format_context& ctx) {
	auto out = ctx.out();
	bool first = true;
	for (auto i = n.bigits_.size(); i > 0; --i) {
	  auto value = n.bigits_[i - 1u];
	  if (first) {
		out = format_to(out, "{:x}", value);
		first = false;
		continue;
	  }
	  out = format_to(out, "{:08x}", value);
	}
	if (n.exp_ > 0)
	  out = format_to(out, "p{}", n.exp_ * detail::bigint::bigit_bits);
	return out;
  }
};

FMT_FUNC detail::utf8_to_utf16::utf8_to_utf16(string_view s) {
  auto transcode = [this](const char* p) {
	auto cp = uint32_t();
	auto error = 0;
	p = utf8_decode(p, &cp, &error);
	if (error != 0) FMT_THROW(std::runtime_error("invalid utf8"));
	if (cp <= 0xFFFF) {
	  buffer_.push_back(static_cast<wchar_t>(cp));
	} else {
	  cp -= 0x10000;
	  buffer_.push_back(static_cast<wchar_t>(0xD800 + (cp >> 10)));
	  buffer_.push_back(static_cast<wchar_t>(0xDC00 + (cp & 0x3FF)));
	}
	return p;
  };
  auto p = s.data();
  const size_t block_size = 4;  // utf8_decode always reads blocks of 4 chars.
  if (s.size() >= block_size) {
	for (auto end = p + s.size() - block_size + 1; p < end;) p = transcode(p);
  }
  if (auto num_chars_left = s.data() + s.size() - p) {
	char buf[2 * block_size - 1] = {};
	memcpy(buf, p, to_unsigned(num_chars_left));
	p = buf;
	do {
	  p = transcode(p);
	} while (p - buf < num_chars_left);
  }
  buffer_.push_back(0);
}

FMT_FUNC void format_system_error(detail::buffer<char>& out, int error_code,
								  string_view message) FMT_NOEXCEPT {
  FMT_TRY {
	memory_buffer buf;
	buf.resize(inline_buffer_size);
	for (;;) {
	  char* system_message = &buf[0];
	  int result =
		  detail::safe_strerror(error_code, system_message, buf.size());
	  if (result == 0) {
		format_to(detail::buffer_appender<char>(out), "{}: {}", message,
				  system_message);
		return;
	  }
	  if (result != ERANGE)
		break;  // Can't get error message, report error code instead.
	  buf.resize(buf.size() * 2);
	}
  }
  FMT_CATCH(...) {}
  format_error_code(out, error_code, message);
}

FMT_FUNC void detail::error_handler::on_error(const char* message) {
  FMT_THROW(format_error(message));
}

FMT_FUNC void report_system_error(int error_code,
								  fmt::string_view message) FMT_NOEXCEPT {
  report_error(format_system_error, error_code, message);
}

FMT_FUNC std::string detail::vformat(string_view format_str, format_args args) {
  if (format_str.size() == 2 && equal2(format_str.data(), "{}")) {
	auto arg = args.get(0);
	if (!arg) error_handler().on_error("argument not found");
	return visit_format_arg(stringifier(), arg);
  }
  memory_buffer buffer;
  detail::vformat_to(buffer, format_str, args);
  return to_string(buffer);
}

#ifdef _WIN32
namespace detail {
using dword = conditional_t<sizeof(long) == 4, unsigned long, unsigned>;
extern "C" __declspec(dllimport) int __stdcall WriteConsoleW(  //
	void*, const void*, dword, dword*, void*);
}  // namespace detail
#endif

FMT_FUNC void vprint(std::FILE* f, string_view format_str, format_args args) {
  memory_buffer buffer;
  detail::vformat_to(buffer, format_str,
					 basic_format_args<buffer_context<char>>(args));
#ifdef _WIN32
  auto fd = _fileno(f);
  if (_isatty(fd)) {
	detail::utf8_to_utf16 u16(string_view(buffer.data(), buffer.size()));
	auto written = detail::dword();
	if (!detail::WriteConsoleW(reinterpret_cast<void*>(_get_osfhandle(fd)),
							   u16.c_str(), static_cast<uint32_t>(u16.size()),
							   &written, nullptr)) {
	  FMT_THROW(format_error("failed to write to console"));
	}
	return;
  }
#endif
  detail::fwrite_fully(buffer.data(), 1, buffer.size(), f);
}

#ifdef _WIN32
// Print assuming legacy (non-Unicode) encoding.
FMT_FUNC void detail::vprint_mojibake(std::FILE* f, string_view format_str,
									  format_args args) {
  memory_buffer buffer;
  detail::vformat_to(buffer, format_str,
					 basic_format_args<buffer_context<char>>(args));
  fwrite_fully(buffer.data(), 1, buffer.size(), f);
}
#endif

FMT_FUNC void vprint(string_view format_str, format_args args) {
  vprint(stdout, format_str, args);
}

FMT_END_NAMESPACE

#endif  // FMT_FORMAT_INL_H_

/*** End of inlined file: dmformat-inl.h ***/


#else
#  define FMT_FUNC
#endif

#endif  // FMT_FORMAT_H_

/*** End of inlined file: dmformat.h ***/


/*** Start of inlined file: dmlocale.h ***/
// Formatting library for C++ - std::locale support
//
// Copyright (c) 2012 - present, Victor Zverovich
// All rights reserved.
//
// For the license information refer to format.h.

#ifndef FMT_LOCALE_H_
#define FMT_LOCALE_H_

#include <locale>

FMT_BEGIN_NAMESPACE

namespace detail {
template <typename Char>
std::basic_string<Char> vformat(
	const std::locale& loc, basic_string_view<Char> format_str,
	basic_format_args<buffer_context<type_identity_t<Char>>> args) {
  basic_memory_buffer<Char> buffer;
  detail::vformat_to(buffer, format_str, args, detail::locale_ref(loc));
  return fmt::to_string(buffer);
}
}  // namespace detail

template <typename S, typename Char = char_t<S>>
inline std::basic_string<Char> vformat(
	const std::locale& loc, const S& format_str,
	basic_format_args<buffer_context<type_identity_t<Char>>> args) {
  return detail::vformat(loc, to_string_view(format_str), args);
}

template <typename S, typename... Args, typename Char = char_t<S>>
inline std::basic_string<Char> format(const std::locale& loc,
									  const S& format_str, Args&&... args) {
  return detail::vformat(loc, to_string_view(format_str),
						 fmt::make_args_checked<Args...>(format_str, args...));
}

template <typename S, typename OutputIt, typename... Args,
		  typename Char = char_t<S>,
		  FMT_ENABLE_IF(detail::is_output_iterator<OutputIt, Char>::value)>
inline OutputIt vformat_to(
	OutputIt out, const std::locale& loc, const S& format_str,
	basic_format_args<buffer_context<type_identity_t<Char>>> args) {
  decltype(detail::get_buffer<Char>(out)) buf(detail::get_buffer_init(out));
  vformat_to(buf, to_string_view(format_str), args, detail::locale_ref(loc));
  return detail::get_iterator(buf);
}

template <typename OutputIt, typename S, typename... Args,
		  bool enable = detail::is_output_iterator<OutputIt, char_t<S>>::value>
inline auto format_to(OutputIt out, const std::locale& loc,
					  const S& format_str, Args&&... args) ->
	typename std::enable_if<enable, OutputIt>::type {
  const auto& vargs = fmt::make_args_checked<Args...>(format_str, args...);
  return vformat_to(out, loc, to_string_view(format_str), vargs);
}

FMT_END_NAMESPACE

#endif  // FMT_LOCALE_H_

/*** End of inlined file: dmlocale.h ***/

FMT_BEGIN_NAMESPACE

// Enable safe chrono durations, unless explicitly disabled.
#ifndef FMT_SAFE_DURATION_CAST
#  define FMT_SAFE_DURATION_CAST 1
#endif
#if FMT_SAFE_DURATION_CAST

// For conversion between std::chrono::durations without undefined
// behaviour or erroneous results.
// This is a stripped down version of duration_cast, for inclusion in fmt.
// See https://github.com/pauldreik/safe_duration_cast
//
// Copyright Paul Dreik 2019
namespace safe_duration_cast {

template <typename To, typename From,
		  FMT_ENABLE_IF(!std::is_same<From, To>::value &&
						std::numeric_limits<From>::is_signed ==
							std::numeric_limits<To>::is_signed)>
FMT_CONSTEXPR To lossless_integral_conversion(const From from, int& ec) {
  ec = 0;
  using F = std::numeric_limits<From>;
  using T = std::numeric_limits<To>;
  static_assert(F::is_integer, "From must be integral");
  static_assert(T::is_integer, "To must be integral");

  // A and B are both signed, or both unsigned.
  if (F::digits <= T::digits) {
	// From fits in To without any problem.
  } else {
	// From does not always fit in To, resort to a dynamic check.
	if (from < (T::min)() || from > (T::max)()) {
	  // outside range.
	  ec = 1;
	  return {};
	}
  }
  return static_cast<To>(from);
}

/**
 * converts From to To, without loss. If the dynamic value of from
 * can't be converted to To without loss, ec is set.
 */
template <typename To, typename From,
		  FMT_ENABLE_IF(!std::is_same<From, To>::value &&
						std::numeric_limits<From>::is_signed !=
							std::numeric_limits<To>::is_signed)>
FMT_CONSTEXPR To lossless_integral_conversion(const From from, int& ec) {
  ec = 0;
  using F = std::numeric_limits<From>;
  using T = std::numeric_limits<To>;
  static_assert(F::is_integer, "From must be integral");
  static_assert(T::is_integer, "To must be integral");

  if (detail::const_check(F::is_signed && !T::is_signed)) {
	// From may be negative, not allowed!
	if (fmt::detail::is_negative(from)) {
	  ec = 1;
	  return {};
	}
	// From is positive. Can it always fit in To?
	if (F::digits > T::digits &&
		from > static_cast<From>(detail::max_value<To>())) {
	  ec = 1;
	  return {};
	}
  }

  if (!F::is_signed && T::is_signed && F::digits >= T::digits &&
	  from > static_cast<From>(detail::max_value<To>())) {
	ec = 1;
	return {};
  }
  return static_cast<To>(from);  // Lossless conversion.
}

template <typename To, typename From,
		  FMT_ENABLE_IF(std::is_same<From, To>::value)>
FMT_CONSTEXPR To lossless_integral_conversion(const From from, int& ec) {
  ec = 0;
  return from;
}  // function

// clang-format off
/**
 * converts From to To if possible, otherwise ec is set.
 *
 * input                            |    output
 * ---------------------------------|---------------
 * NaN                              | NaN
 * Inf                              | Inf
 * normal, fits in output           | converted (possibly lossy)
 * normal, does not fit in output   | ec is set
 * subnormal                        | best effort
 * -Inf                             | -Inf
 */
// clang-format on
template <typename To, typename From,
		  FMT_ENABLE_IF(!std::is_same<From, To>::value)>
FMT_CONSTEXPR To safe_float_conversion(const From from, int& ec) {
  ec = 0;
  using T = std::numeric_limits<To>;
  static_assert(std::is_floating_point<From>::value, "From must be floating");
  static_assert(std::is_floating_point<To>::value, "To must be floating");

  // catch the only happy case
  if (std::isfinite(from)) {
	if (from >= T::lowest() && from <= (T::max)()) {
	  return static_cast<To>(from);
	}
	// not within range.
	ec = 1;
	return {};
  }

  // nan and inf will be preserved
  return static_cast<To>(from);
}  // function

template <typename To, typename From,
		  FMT_ENABLE_IF(std::is_same<From, To>::value)>
FMT_CONSTEXPR To safe_float_conversion(const From from, int& ec) {
  ec = 0;
  static_assert(std::is_floating_point<From>::value, "From must be floating");
  return from;
}

/**
 * safe duration cast between integral durations
 */
template <typename To, typename FromRep, typename FromPeriod,
		  FMT_ENABLE_IF(std::is_integral<FromRep>::value),
		  FMT_ENABLE_IF(std::is_integral<typename To::rep>::value)>
To safe_duration_cast(std::chrono::duration<FromRep, FromPeriod> from,
					  int& ec) {
  using From = std::chrono::duration<FromRep, FromPeriod>;
  ec = 0;
  // the basic idea is that we need to convert from count() in the from type
  // to count() in the To type, by multiplying it with this:
  struct Factor
	  : std::ratio_divide<typename From::period, typename To::period> {};

  static_assert(Factor::num > 0, "num must be positive");
  static_assert(Factor::den > 0, "den must be positive");

  // the conversion is like this: multiply from.count() with Factor::num
  // /Factor::den and convert it to To::rep, all this without
  // overflow/underflow. let's start by finding a suitable type that can hold
  // both To, From and Factor::num
  using IntermediateRep =
	  typename std::common_type<typename From::rep, typename To::rep,
								decltype(Factor::num)>::type;

  // safe conversion to IntermediateRep
  IntermediateRep count =
	  lossless_integral_conversion<IntermediateRep>(from.count(), ec);
  if (ec) return {};
  // multiply with Factor::num without overflow or underflow
  if (detail::const_check(Factor::num != 1)) {
	const auto max1 = detail::max_value<IntermediateRep>() / Factor::num;
	if (count > max1) {
	  ec = 1;
	  return {};
	}
	const auto min1 =
		(std::numeric_limits<IntermediateRep>::min)() / Factor::num;
	if (count < min1) {
	  ec = 1;
	  return {};
	}
	count *= Factor::num;
  }

  if (detail::const_check(Factor::den != 1)) count /= Factor::den;
  auto tocount = lossless_integral_conversion<typename To::rep>(count, ec);
  return ec ? To() : To(tocount);
}

/**
 * safe duration_cast between floating point durations
 */
template <typename To, typename FromRep, typename FromPeriod,
		  FMT_ENABLE_IF(std::is_floating_point<FromRep>::value),
		  FMT_ENABLE_IF(std::is_floating_point<typename To::rep>::value)>
To safe_duration_cast(std::chrono::duration<FromRep, FromPeriod> from,
					  int& ec) {
  using From = std::chrono::duration<FromRep, FromPeriod>;
  ec = 0;
  if (std::isnan(from.count())) {
	// nan in, gives nan out. easy.
	return To{std::numeric_limits<typename To::rep>::quiet_NaN()};
  }
  // maybe we should also check if from is denormal, and decide what to do about
  // it.

  // +-inf should be preserved.
  if (std::isinf(from.count())) {
	return To{from.count()};
  }

  // the basic idea is that we need to convert from count() in the from type
  // to count() in the To type, by multiplying it with this:
  struct Factor
	  : std::ratio_divide<typename From::period, typename To::period> {};

  static_assert(Factor::num > 0, "num must be positive");
  static_assert(Factor::den > 0, "den must be positive");

  // the conversion is like this: multiply from.count() with Factor::num
  // /Factor::den and convert it to To::rep, all this without
  // overflow/underflow. let's start by finding a suitable type that can hold
  // both To, From and Factor::num
  using IntermediateRep =
	  typename std::common_type<typename From::rep, typename To::rep,
								decltype(Factor::num)>::type;

  // force conversion of From::rep -> IntermediateRep to be safe,
  // even if it will never happen be narrowing in this context.
  IntermediateRep count =
	  safe_float_conversion<IntermediateRep>(from.count(), ec);
  if (ec) {
	return {};
  }

  // multiply with Factor::num without overflow or underflow
  if (Factor::num != 1) {
	constexpr auto max1 = detail::max_value<IntermediateRep>() /
						  static_cast<IntermediateRep>(Factor::num);
	if (count > max1) {
	  ec = 1;
	  return {};
	}
	constexpr auto min1 = std::numeric_limits<IntermediateRep>::lowest() /
						  static_cast<IntermediateRep>(Factor::num);
	if (count < min1) {
	  ec = 1;
	  return {};
	}
	count *= static_cast<IntermediateRep>(Factor::num);
  }

  // this can't go wrong, right? den>0 is checked earlier.
  if (Factor::den != 1) {
	using common_t = typename std::common_type<IntermediateRep, intmax_t>::type;
	count /= static_cast<common_t>(Factor::den);
  }

  // convert to the to type, safely
  using ToRep = typename To::rep;

  const ToRep tocount = safe_float_conversion<ToRep>(count, ec);
  if (ec) {
	return {};
  }
  return To{tocount};
}
}  // namespace safe_duration_cast
#endif

// Prevents expansion of a preceding token as a function-style macro.
// Usage: f FMT_NOMACRO()
#define FMT_NOMACRO

namespace detail {
inline null<> localtime_r FMT_NOMACRO(...) { return null<>(); }
inline null<> localtime_s(...) { return null<>(); }
inline null<> gmtime_r(...) { return null<>(); }
inline null<> gmtime_s(...) { return null<>(); }
}  // namespace detail

// Thread-safe replacement for std::localtime
inline std::tm localtime(std::time_t time) {
  struct dispatcher {
	std::time_t time_;
	std::tm tm_;

	dispatcher(std::time_t t) : time_(t) {}

	bool run() {
	  using namespace fmt::detail;
	  return handle(localtime_r(&time_, &tm_));
	}

	bool handle(std::tm* tm) { return tm != nullptr; }

	bool handle(detail::null<>) {
	  using namespace fmt::detail;
	  return fallback(localtime_s(&tm_, &time_));
	}

	bool fallback(int res) { return res == 0; }

#if !FMT_MSC_VER
	bool fallback(detail::null<>) {
	  using namespace fmt::detail;
	  std::tm* tm = std::localtime(&time_);
	  if (tm) tm_ = *tm;
	  return tm != nullptr;
	}
#endif
  };
  dispatcher lt(time);
  // Too big time values may be unsupported.
  if (!lt.run()) FMT_THROW(format_error("time_t value out of range"));
  return lt.tm_;
}

inline std::tm localtime(
	std::chrono::time_point<std::chrono::system_clock> time_point) {
  return localtime(std::chrono::system_clock::to_time_t(time_point));
}

// Thread-safe replacement for std::gmtime
inline std::tm gmtime(std::time_t time) {
  struct dispatcher {
	std::time_t time_;
	std::tm tm_;

	dispatcher(std::time_t t) : time_(t) {}

	bool run() {
	  using namespace fmt::detail;
	  return handle(gmtime_r(&time_, &tm_));
	}

	bool handle(std::tm* tm) { return tm != nullptr; }

	bool handle(detail::null<>) {
	  using namespace fmt::detail;
	  return fallback(gmtime_s(&tm_, &time_));
	}

	bool fallback(int res) { return res == 0; }

#if !FMT_MSC_VER
	bool fallback(detail::null<>) {
	  std::tm* tm = std::gmtime(&time_);
	  if (tm) tm_ = *tm;
	  return tm != nullptr;
	}
#endif
  };
  dispatcher gt(time);
  // Too big time values may be unsupported.
  if (!gt.run()) FMT_THROW(format_error("time_t value out of range"));
  return gt.tm_;
}

inline std::tm gmtime(
	std::chrono::time_point<std::chrono::system_clock> time_point) {
  return gmtime(std::chrono::system_clock::to_time_t(time_point));
}

namespace detail {
inline size_t strftime(char* str, size_t count, const char* format,
					   const std::tm* time) {
  return std::strftime(str, count, format, time);
}

inline size_t strftime(wchar_t* str, size_t count, const wchar_t* format,
					   const std::tm* time) {
  return std::wcsftime(str, count, format, time);
}
}  // namespace detail

template <typename Char>
struct formatter<std::chrono::time_point<std::chrono::system_clock>, Char>
	: formatter<std::tm, Char> {
  template <typename FormatContext>
  auto format(std::chrono::time_point<std::chrono::system_clock> val,
			  FormatContext& ctx) -> decltype(ctx.out()) {
	std::tm time = localtime(val);
	return formatter<std::tm, Char>::format(time, ctx);
  }
};

template <typename Char> struct formatter<std::tm, Char> {
  template <typename ParseContext>
  auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
	auto it = ctx.begin();
	if (it != ctx.end() && *it == ':') ++it;
	auto end = it;
	while (end != ctx.end() && *end != '}') ++end;
	tm_format.reserve(detail::to_unsigned(end - it + 1));
	tm_format.append(it, end);
	tm_format.push_back('\0');
	return end;
  }

  template <typename FormatContext>
  auto format(const std::tm& tm, FormatContext& ctx) -> decltype(ctx.out()) {
	basic_memory_buffer<Char> buf;
	size_t start = buf.size();
	for (;;) {
	  size_t size = buf.capacity() - start;
	  size_t count = detail::strftime(&buf[start], size, &tm_format[0], &tm);
	  if (count != 0) {
		buf.resize(start + count);
		break;
	  }
	  if (size >= tm_format.size() * 256) {
		// If the buffer is 256 times larger than the format string, assume
		// that `strftime` gives an empty result. There doesn't seem to be a
		// better way to distinguish the two cases:
		// https://github.com/fmtlib/dmissues/367
		break;
	  }
	  const size_t MIN_GROWTH = 10;
	  buf.reserve(buf.capacity() + (size > MIN_GROWTH ? size : MIN_GROWTH));
	}
	return std::copy(buf.begin(), buf.end(), ctx.out());
  }

  basic_memory_buffer<Char> tm_format;
};

namespace detail {
template <typename Period> FMT_CONSTEXPR const char* get_units() {
  return nullptr;
}
template <> FMT_CONSTEXPR const char* get_units<std::atto>() { return "as"; }
template <> FMT_CONSTEXPR const char* get_units<std::femto>() { return "fs"; }
template <> FMT_CONSTEXPR const char* get_units<std::pico>() { return "ps"; }
template <> FMT_CONSTEXPR const char* get_units<std::nano>() { return "ns"; }
template <> FMT_CONSTEXPR const char* get_units<std::micro>() { return "µs"; }
template <> FMT_CONSTEXPR const char* get_units<std::milli>() { return "ms"; }
template <> FMT_CONSTEXPR const char* get_units<std::centi>() { return "cs"; }
template <> FMT_CONSTEXPR const char* get_units<std::deci>() { return "ds"; }
template <> FMT_CONSTEXPR const char* get_units<std::ratio<1>>() { return "s"; }
template <> FMT_CONSTEXPR const char* get_units<std::deca>() { return "das"; }
template <> FMT_CONSTEXPR const char* get_units<std::hecto>() { return "hs"; }
template <> FMT_CONSTEXPR const char* get_units<std::kilo>() { return "ks"; }
template <> FMT_CONSTEXPR const char* get_units<std::mega>() { return "Ms"; }
template <> FMT_CONSTEXPR const char* get_units<std::giga>() { return "Gs"; }
template <> FMT_CONSTEXPR const char* get_units<std::tera>() { return "Ts"; }
template <> FMT_CONSTEXPR const char* get_units<std::peta>() { return "Ps"; }
template <> FMT_CONSTEXPR const char* get_units<std::exa>() { return "Es"; }
template <> FMT_CONSTEXPR const char* get_units<std::ratio<60>>() {
  return "m";
}
template <> FMT_CONSTEXPR const char* get_units<std::ratio<3600>>() {
  return "h";
}

enum class numeric_system {
  standard,
  // Alternative numeric system, e.g. 十二 instead of 12 in ja_JP locale.
  alternative
};

// Parses a put_time-like format string and invokes handler actions.
template <typename Char, typename Handler>
FMT_CONSTEXPR const Char* parse_chrono_format(const Char* begin,
											  const Char* end,
											  Handler&& handler) {
  auto ptr = begin;
  while (ptr != end) {
	auto c = *ptr;
	if (c == '}') break;
	if (c != '%') {
	  ++ptr;
	  continue;
	}
	if (begin != ptr) handler.on_text(begin, ptr);
	++ptr;  // consume '%'
	if (ptr == end) FMT_THROW(format_error("invalid format"));
	c = *ptr++;
	switch (c) {
	case '%':
	  handler.on_text(ptr - 1, ptr);
	  break;
	case 'n': {
	  const Char newline[] = {'\n'};
	  handler.on_text(newline, newline + 1);
	  break;
	}
	case 't': {
	  const Char tab[] = {'\t'};
	  handler.on_text(tab, tab + 1);
	  break;
	}
	// Day of the week:
	case 'a':
	  handler.on_abbr_weekday();
	  break;
	case 'A':
	  handler.on_full_weekday();
	  break;
	case 'w':
	  handler.on_dec0_weekday(numeric_system::standard);
	  break;
	case 'u':
	  handler.on_dec1_weekday(numeric_system::standard);
	  break;
	// Month:
	case 'b':
	  handler.on_abbr_month();
	  break;
	case 'B':
	  handler.on_full_month();
	  break;
	// Hour, minute, second:
	case 'H':
	  handler.on_24_hour(numeric_system::standard);
	  break;
	case 'I':
	  handler.on_12_hour(numeric_system::standard);
	  break;
	case 'M':
	  handler.on_minute(numeric_system::standard);
	  break;
	case 'S':
	  handler.on_second(numeric_system::standard);
	  break;
	// Other:
	case 'c':
	  handler.on_datetime(numeric_system::standard);
	  break;
	case 'x':
	  handler.on_loc_date(numeric_system::standard);
	  break;
	case 'X':
	  handler.on_loc_time(numeric_system::standard);
	  break;
	case 'D':
	  handler.on_us_date();
	  break;
	case 'F':
	  handler.on_iso_date();
	  break;
	case 'r':
	  handler.on_12_hour_time();
	  break;
	case 'R':
	  handler.on_24_hour_time();
	  break;
	case 'T':
	  handler.on_iso_time();
	  break;
	case 'p':
	  handler.on_am_pm();
	  break;
	case 'Q':
	  handler.on_duration_value();
	  break;
	case 'q':
	  handler.on_duration_unit();
	  break;
	case 'z':
	  handler.on_utc_offset();
	  break;
	case 'Z':
	  handler.on_tz_name();
	  break;
	// Alternative representation:
	case 'E': {
	  if (ptr == end) FMT_THROW(format_error("invalid format"));
	  c = *ptr++;
	  switch (c) {
	  case 'c':
		handler.on_datetime(numeric_system::alternative);
		break;
	  case 'x':
		handler.on_loc_date(numeric_system::alternative);
		break;
	  case 'X':
		handler.on_loc_time(numeric_system::alternative);
		break;
	  default:
		FMT_THROW(format_error("invalid format"));
	  }
	  break;
	}
	case 'O':
	  if (ptr == end) FMT_THROW(format_error("invalid format"));
	  c = *ptr++;
	  switch (c) {
	  case 'w':
		handler.on_dec0_weekday(numeric_system::alternative);
		break;
	  case 'u':
		handler.on_dec1_weekday(numeric_system::alternative);
		break;
	  case 'H':
		handler.on_24_hour(numeric_system::alternative);
		break;
	  case 'I':
		handler.on_12_hour(numeric_system::alternative);
		break;
	  case 'M':
		handler.on_minute(numeric_system::alternative);
		break;
	  case 'S':
		handler.on_second(numeric_system::alternative);
		break;
	  default:
		FMT_THROW(format_error("invalid format"));
	  }
	  break;
	default:
	  FMT_THROW(format_error("invalid format"));
	}
	begin = ptr;
  }
  if (begin != ptr) handler.on_text(begin, ptr);
  return ptr;
}

struct chrono_format_checker {
  FMT_NORETURN void report_no_date() { FMT_THROW(format_error("no date")); }

  template <typename Char> void on_text(const Char*, const Char*) {}
  FMT_NORETURN void on_abbr_weekday() { report_no_date(); }
  FMT_NORETURN void on_full_weekday() { report_no_date(); }
  FMT_NORETURN void on_dec0_weekday(numeric_system) { report_no_date(); }
  FMT_NORETURN void on_dec1_weekday(numeric_system) { report_no_date(); }
  FMT_NORETURN void on_abbr_month() { report_no_date(); }
  FMT_NORETURN void on_full_month() { report_no_date(); }
  void on_24_hour(numeric_system) {}
  void on_12_hour(numeric_system) {}
  void on_minute(numeric_system) {}
  void on_second(numeric_system) {}
  FMT_NORETURN void on_datetime(numeric_system) { report_no_date(); }
  FMT_NORETURN void on_loc_date(numeric_system) { report_no_date(); }
  FMT_NORETURN void on_loc_time(numeric_system) { report_no_date(); }
  FMT_NORETURN void on_us_date() { report_no_date(); }
  FMT_NORETURN void on_iso_date() { report_no_date(); }
  void on_12_hour_time() {}
  void on_24_hour_time() {}
  void on_iso_time() {}
  void on_am_pm() {}
  void on_duration_value() {}
  void on_duration_unit() {}
  FMT_NORETURN void on_utc_offset() { report_no_date(); }
  FMT_NORETURN void on_tz_name() { report_no_date(); }
};

template <typename T, FMT_ENABLE_IF(std::is_integral<T>::value)>
inline bool isnan(T) {
  return false;
}
template <typename T, FMT_ENABLE_IF(std::is_floating_point<T>::value)>
inline bool isnan(T value) {
  return std::isnan(value);
}

template <typename T, FMT_ENABLE_IF(std::is_integral<T>::value)>
inline bool isfinite(T) {
  return true;
}
template <typename T, FMT_ENABLE_IF(std::is_floating_point<T>::value)>
inline bool isfinite(T value) {
  return std::isfinite(value);
}

// Converts value to int and checks that it's in the range [0, upper).
template <typename T, FMT_ENABLE_IF(std::is_integral<T>::value)>
inline int to_nonnegative_int(T value, int upper) {
  FMT_ASSERT(value >= 0 && value <= upper, "invalid value");
  (void)upper;
  return static_cast<int>(value);
}
template <typename T, FMT_ENABLE_IF(!std::is_integral<T>::value)>
inline int to_nonnegative_int(T value, int upper) {
  FMT_ASSERT(
	  std::isnan(value) || (value >= 0 && value <= static_cast<T>(upper)),
	  "invalid value");
  (void)upper;
  return static_cast<int>(value);
}

template <typename T, FMT_ENABLE_IF(std::is_integral<T>::value)>
inline T mod(T x, int y) {
  return x % static_cast<T>(y);
}
template <typename T, FMT_ENABLE_IF(std::is_floating_point<T>::value)>
inline T mod(T x, int y) {
  return std::fmod(x, static_cast<T>(y));
}

// If T is an integral type, maps T to its unsigned counterpart, otherwise
// leaves it unchanged (unlike std::make_unsigned).
template <typename T, bool INTEGRAL = std::is_integral<T>::value>
struct make_unsigned_or_unchanged {
  using type = T;
};

template <typename T> struct make_unsigned_or_unchanged<T, true> {
  using type = typename std::make_unsigned<T>::type;
};

#if FMT_SAFE_DURATION_CAST
// throwing version of safe_duration_cast
template <typename To, typename FromRep, typename FromPeriod>
To fmt_safe_duration_cast(std::chrono::duration<FromRep, FromPeriod> from) {
  int ec;
  To to = safe_duration_cast::safe_duration_cast<To>(from, ec);
  if (ec) FMT_THROW(format_error("cannot format duration"));
  return to;
}
#endif

template <typename Rep, typename Period,
		  FMT_ENABLE_IF(std::is_integral<Rep>::value)>
inline std::chrono::duration<Rep, std::milli> get_milliseconds(
	std::chrono::duration<Rep, Period> d) {
  // this may overflow and/or the result may not fit in the
  // target type.
#if FMT_SAFE_DURATION_CAST
  using CommonSecondsType =
	  typename std::common_type<decltype(d), std::chrono::seconds>::type;
  const auto d_as_common = fmt_safe_duration_cast<CommonSecondsType>(d);
  const auto d_as_whole_seconds =
	  fmt_safe_duration_cast<std::chrono::seconds>(d_as_common);
  // this conversion should be nonproblematic
  const auto diff = d_as_common - d_as_whole_seconds;
  const auto ms =
	  fmt_safe_duration_cast<std::chrono::duration<Rep, std::milli>>(diff);
  return ms;
#else
  auto s = std::chrono::duration_cast<std::chrono::seconds>(d);
  return std::chrono::duration_cast<std::chrono::milliseconds>(d - s);
#endif
}

template <typename Rep, typename Period,
		  FMT_ENABLE_IF(std::is_floating_point<Rep>::value)>
inline std::chrono::duration<Rep, std::milli> get_milliseconds(
	std::chrono::duration<Rep, Period> d) {
  using common_type = typename std::common_type<Rep, std::intmax_t>::type;
  auto ms = mod(d.count() * static_cast<common_type>(Period::num) /
					static_cast<common_type>(Period::den) * 1000,
				1000);
  return std::chrono::duration<Rep, std::milli>(static_cast<Rep>(ms));
}

template <typename Char, typename Rep, typename OutputIt>
OutputIt format_duration_value(OutputIt out, Rep val, int precision) {
  const Char pr_f[] = {'{', ':', '.', '{', '}', 'f', '}', 0};
  if (precision >= 0) return format_to(out, pr_f, val, precision);
  const Char fp_f[] = {'{', ':', 'g', '}', 0};
  const Char format[] = {'{', '}', 0};
  return format_to(out, std::is_floating_point<Rep>::value ? fp_f : format,
				   val);
}
template <typename Char, typename OutputIt>
OutputIt copy_unit(string_view unit, OutputIt out, Char) {
  return std::copy(unit.begin(), unit.end(), out);
}

template <typename OutputIt>
OutputIt copy_unit(string_view unit, OutputIt out, wchar_t) {
  // This works when wchar_t is UTF-32 because units only contain characters
  // that have the same representation in UTF-16 and UTF-32.
  utf8_to_utf16 u(unit);
  return std::copy(u.c_str(), u.c_str() + u.size(), out);
}

template <typename Char, typename Period, typename OutputIt>
OutputIt format_duration_unit(OutputIt out) {
  if (const char* unit = get_units<Period>())
	return copy_unit(string_view(unit), out, Char());
  const Char num_f[] = {'[', '{', '}', ']', 's', 0};
  if (const_check(Period::den == 1)) return format_to(out, num_f, Period::num);
  const Char num_def_f[] = {'[', '{', '}', '/', '{', '}', ']', 's', 0};
  return format_to(out, num_def_f, Period::num, Period::den);
}

template <typename FormatContext, typename OutputIt, typename Rep,
		  typename Period>
struct chrono_formatter {
  FormatContext& context;
  OutputIt out;
  int precision;
  // rep is unsigned to avoid overflow.
  using rep =
	  conditional_t<std::is_integral<Rep>::value && sizeof(Rep) < sizeof(int),
					unsigned, typename make_unsigned_or_unchanged<Rep>::type>;
  rep val;
  using seconds = std::chrono::duration<rep>;
  seconds s;
  using milliseconds = std::chrono::duration<rep, std::milli>;
  bool negative;

  using char_type = typename FormatContext::char_type;

  explicit chrono_formatter(FormatContext& ctx, OutputIt o,
							std::chrono::duration<Rep, Period> d)
	  : context(ctx),
		out(o),
		val(static_cast<rep>(d.count())),
		negative(false) {
	if (d.count() < 0) {
	  val = 0 - val;
	  negative = true;
	}

	// this may overflow and/or the result may not fit in the
	// target type.
#if FMT_SAFE_DURATION_CAST
	// might need checked conversion (rep!=Rep)
	auto tmpval = std::chrono::duration<rep, Period>(val);
	s = fmt_safe_duration_cast<seconds>(tmpval);
#else
	s = std::chrono::duration_cast<seconds>(
		std::chrono::duration<rep, Period>(val));
#endif
  }

  // returns true if nan or inf, writes to out.
  bool handle_nan_inf() {
	if (isfinite(val)) {
	  return false;
	}
	if (isnan(val)) {
	  write_nan();
	  return true;
	}
	// must be +-inf
	if (val > 0) {
	  write_pinf();
	} else {
	  write_ninf();
	}
	return true;
  }

  Rep hour() const { return static_cast<Rep>(mod((s.count() / 3600), 24)); }

  Rep hour12() const {
	Rep hour = static_cast<Rep>(mod((s.count() / 3600), 12));
	return hour <= 0 ? 12 : hour;
  }

  Rep minute() const { return static_cast<Rep>(mod((s.count() / 60), 60)); }
  Rep second() const { return static_cast<Rep>(mod(s.count(), 60)); }

  std::tm time() const {
	auto time = std::tm();
	time.tm_hour = to_nonnegative_int(hour(), 24);
	time.tm_min = to_nonnegative_int(minute(), 60);
	time.tm_sec = to_nonnegative_int(second(), 60);
	return time;
  }

  void write_sign() {
	if (negative) {
	  *out++ = '-';
	  negative = false;
	}
  }

  void write(Rep value, int width) {
	write_sign();
	if (isnan(value)) return write_nan();
	uint32_or_64_or_128_t<int> n =
		to_unsigned(to_nonnegative_int(value, max_value<int>()));
	int num_digits = detail::count_digits(n);
	if (width > num_digits) out = std::fill_n(out, width - num_digits, '0');
	out = format_decimal<char_type>(out, n, num_digits).end;
  }

  void write_nan() { std::copy_n("nan", 3, out); }
  void write_pinf() { std::copy_n("inf", 3, out); }
  void write_ninf() { std::copy_n("-inf", 4, out); }

  void format_localized(const tm& time, char format, char modifier = 0) {
	if (isnan(val)) return write_nan();
	auto locale = context.locale().template get<std::locale>();
	auto& facet = std::use_facet<std::time_put<char_type>>(locale);
	std::basic_ostringstream<char_type> os;
	os.imbue(locale);
	facet.put(os, os, ' ', &time, format, modifier);
	auto str = os.str();
	std::copy(str.begin(), str.end(), out);
  }

  void on_text(const char_type* begin, const char_type* end) {
	std::copy(begin, end, out);
  }

  // These are not implemented because durations don't have date information.
  void on_abbr_weekday() {}
  void on_full_weekday() {}
  void on_dec0_weekday(numeric_system) {}
  void on_dec1_weekday(numeric_system) {}
  void on_abbr_month() {}
  void on_full_month() {}
  void on_datetime(numeric_system) {}
  void on_loc_date(numeric_system) {}
  void on_loc_time(numeric_system) {}
  void on_us_date() {}
  void on_iso_date() {}
  void on_utc_offset() {}
  void on_tz_name() {}

  void on_24_hour(numeric_system ns) {
	if (handle_nan_inf()) return;

	if (ns == numeric_system::standard) return write(hour(), 2);
	auto time = tm();
	time.tm_hour = to_nonnegative_int(hour(), 24);
	format_localized(time, 'H', 'O');
  }

  void on_12_hour(numeric_system ns) {
	if (handle_nan_inf()) return;

	if (ns == numeric_system::standard) return write(hour12(), 2);
	auto time = tm();
	time.tm_hour = to_nonnegative_int(hour12(), 12);
	format_localized(time, 'I', 'O');
  }

  void on_minute(numeric_system ns) {
	if (handle_nan_inf()) return;

	if (ns == numeric_system::standard) return write(minute(), 2);
	auto time = tm();
	time.tm_min = to_nonnegative_int(minute(), 60);
	format_localized(time, 'M', 'O');
  }

  void on_second(numeric_system ns) {
	if (handle_nan_inf()) return;

	if (ns == numeric_system::standard) {
	  write(second(), 2);
#if FMT_SAFE_DURATION_CAST
	  // convert rep->Rep
	  using duration_rep = std::chrono::duration<rep, Period>;
	  using duration_Rep = std::chrono::duration<Rep, Period>;
	  auto tmpval = fmt_safe_duration_cast<duration_Rep>(duration_rep{val});
#else
	  auto tmpval = std::chrono::duration<Rep, Period>(val);
#endif
	  auto ms = get_milliseconds(tmpval);
	  if (ms != std::chrono::milliseconds(0)) {
		*out++ = '.';
		write(ms.count(), 3);
	  }
	  return;
	}
	auto time = tm();
	time.tm_sec = to_nonnegative_int(second(), 60);
	format_localized(time, 'S', 'O');
  }

  void on_12_hour_time() {
	if (handle_nan_inf()) return;
	format_localized(time(), 'r');
  }

  void on_24_hour_time() {
	if (handle_nan_inf()) {
	  *out++ = ':';
	  handle_nan_inf();
	  return;
	}

	write(hour(), 2);
	*out++ = ':';
	write(minute(), 2);
  }

  void on_iso_time() {
	on_24_hour_time();
	*out++ = ':';
	if (handle_nan_inf()) return;
	write(second(), 2);
  }

  void on_am_pm() {
	if (handle_nan_inf()) return;
	format_localized(time(), 'p');
  }

  void on_duration_value() {
	if (handle_nan_inf()) return;
	write_sign();
	out = format_duration_value<char_type>(out, val, precision);
  }

  void on_duration_unit() {
	out = format_duration_unit<char_type, Period>(out);
  }
};
}  // namespace detail

template <typename Rep, typename Period, typename Char>
struct formatter<std::chrono::duration<Rep, Period>, Char> {
 private:
  basic_format_specs<Char> specs;
  int precision;
  using arg_ref_type = detail::arg_ref<Char>;
  arg_ref_type width_ref;
  arg_ref_type precision_ref;
  mutable basic_string_view<Char> format_str;
  using duration = std::chrono::duration<Rep, Period>;

  struct spec_handler {
	formatter& f;
	basic_format_parse_context<Char>& context;
	basic_string_view<Char> format_str;

	template <typename Id> FMT_CONSTEXPR arg_ref_type make_arg_ref(Id arg_id) {
	  context.check_arg_id(arg_id);
	  return arg_ref_type(arg_id);
	}

	FMT_CONSTEXPR arg_ref_type make_arg_ref(basic_string_view<Char> arg_id) {
	  context.check_arg_id(arg_id);
	  return arg_ref_type(arg_id);
	}

	FMT_CONSTEXPR arg_ref_type make_arg_ref(detail::auto_id) {
	  return arg_ref_type(context.next_arg_id());
	}

	void on_error(const char* msg) { FMT_THROW(format_error(msg)); }
	void on_fill(basic_string_view<Char> fill) { f.specs.fill = fill; }
	void on_align(align_t align) { f.specs.align = align; }
	void on_width(int width) { f.specs.width = width; }
	void on_precision(int _precision) { f.precision = _precision; }
	void end_precision() {}

	template <typename Id> void on_dynamic_width(Id arg_id) {
	  f.width_ref = make_arg_ref(arg_id);
	}

	template <typename Id> void on_dynamic_precision(Id arg_id) {
	  f.precision_ref = make_arg_ref(arg_id);
	}
  };

  using iterator = typename basic_format_parse_context<Char>::iterator;
  struct parse_range {
	iterator begin;
	iterator end;
  };

  FMT_CONSTEXPR parse_range do_parse(basic_format_parse_context<Char>& ctx) {
	auto begin = ctx.begin(), end = ctx.end();
	if (begin == end || *begin == '}') return {begin, begin};
	spec_handler handler{*this, ctx, format_str};
	begin = detail::parse_align(begin, end, handler);
	if (begin == end) return {begin, begin};
	begin = detail::parse_width(begin, end, handler);
	if (begin == end) return {begin, begin};
	if (*begin == '.') {
	  if (std::is_floating_point<Rep>::value)
		begin = detail::parse_precision(begin, end, handler);
	  else
		handler.on_error("precision not allowed for this argument type");
	}
	end = parse_chrono_format(begin, end, detail::chrono_format_checker());
	return {begin, end};
  }

 public:
  formatter() : precision(-1) {}

  FMT_CONSTEXPR auto parse(basic_format_parse_context<Char>& ctx)
	  -> decltype(ctx.begin()) {
	auto range = do_parse(ctx);
	format_str = basic_string_view<Char>(
		&*range.begin, detail::to_unsigned(range.end - range.begin));
	return range.end;
  }

  template <typename FormatContext>
  auto format(const duration& d, FormatContext& ctx) -> decltype(ctx.out()) {
	auto begin = format_str.begin(), end = format_str.end();
	// As a possible future optimization, we could avoid extra copying if width
	// is not specified.
	basic_memory_buffer<Char> buf;
	auto out = std::back_inserter(buf);
	detail::handle_dynamic_spec<detail::width_checker>(specs.width, width_ref,
													   ctx);
	detail::handle_dynamic_spec<detail::precision_checker>(precision,
														   precision_ref, ctx);
	if (begin == end || *begin == '}') {
	  out = detail::format_duration_value<Char>(out, d.count(), precision);
	  detail::format_duration_unit<Char, Period>(out);
	} else {
	  detail::chrono_formatter<FormatContext, decltype(out), Rep, Period> f(
		  ctx, out, d);
	  f.precision = precision;
	  parse_chrono_format(begin, end, f);
	}
	return detail::write(
		ctx.out(), basic_string_view<Char>(buf.data(), buf.size()), specs);
  }
};

FMT_END_NAMESPACE

#endif  // FMT_CHRONO_H_

/*** End of inlined file: dmchrono.h ***/


/*** Start of inlined file: dmranges.h ***/
// Formatting library for C++ - experimental range support
//
// Copyright (c) 2012 - present, Victor Zverovich
// All rights reserved.
//
// For the license information refer to format.h.
//
// Copyright (c) 2018 - present, Remotion (Igor Schulz)
// All Rights Reserved
// {fmt} support for ranges, containers and types tuple interface.

#ifndef FMT_RANGES_H_
#define FMT_RANGES_H_

#include <initializer_list>
#include <type_traits>

// output only up to N items from the range.
#ifndef FMT_RANGE_OUTPUT_LENGTH_LIMIT
#  define FMT_RANGE_OUTPUT_LENGTH_LIMIT 256
#endif

FMT_BEGIN_NAMESPACE

template <typename Char> struct formatting_base {
  template <typename ParseContext>
  FMT_CONSTEXPR auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
	return ctx.begin();
  }
};

template <typename Char, typename Enable = void>
struct formatting_range : formatting_base<Char> {
  static FMT_CONSTEXPR_DECL const size_t range_length_limit =
	  FMT_RANGE_OUTPUT_LENGTH_LIMIT;  // output only up to N items from the
									  // range.
  Char prefix;
  Char delimiter;
  Char postfix;
  formatting_range() : prefix('{'), delimiter(','), postfix('}') {}
  static FMT_CONSTEXPR_DECL const bool add_delimiter_spaces = true;
  static FMT_CONSTEXPR_DECL const bool add_prepostfix_space = false;
};

template <typename Char, typename Enable = void>
struct formatting_tuple : formatting_base<Char> {
  Char prefix;
  Char delimiter;
  Char postfix;
  formatting_tuple() : prefix('('), delimiter(','), postfix(')') {}
  static FMT_CONSTEXPR_DECL const bool add_delimiter_spaces = true;
  static FMT_CONSTEXPR_DECL const bool add_prepostfix_space = false;
};

namespace detail {

template <typename RangeT, typename OutputIterator>
OutputIterator copy(const RangeT& range, OutputIterator out) {
  for (auto it = range.begin(), end = range.end(); it != end; ++it)
	*out++ = *it;
  return out;
}

template <typename OutputIterator>
OutputIterator copy(const char* str, OutputIterator out) {
  while (*str) *out++ = *str++;
  return out;
}

template <typename OutputIterator>
OutputIterator copy(char ch, OutputIterator out) {
  *out++ = ch;
  return out;
}

/// Return true value if T has std::string interface, like std::string_view.
template <typename T> class is_like_std_string {
  template <typename U>
  static auto check(U* p)
	  -> decltype((void)p->find('a'), p->length(), (void)p->data(), int());
  template <typename> static void check(...);

 public:
  static FMT_CONSTEXPR_DECL const bool value =
	  is_string<T>::value || !std::is_void<decltype(check<T>(nullptr))>::value;
};

template <typename Char>
struct is_like_std_string<fmt::basic_string_view<Char>> : std::true_type {};

template <typename... Ts> struct conditional_helper {};

template <typename T, typename _ = void> struct is_range_ : std::false_type {};

#if !FMT_MSC_VER || FMT_MSC_VER > 1800
template <typename T>
struct is_range_<
	T, conditional_t<false,
					 conditional_helper<decltype(std::declval<T>().begin()),
										decltype(std::declval<T>().end())>,
					 void>> : std::true_type {};
#endif

/// tuple_size and tuple_element check.
template <typename T> class is_tuple_like_ {
  template <typename U>
  static auto check(U* p) -> decltype(std::tuple_size<U>::value, int());
  template <typename> static void check(...);

 public:
  static FMT_CONSTEXPR_DECL const bool value =
	  !std::is_void<decltype(check<T>(nullptr))>::value;
};

// Check for integer_sequence
#if defined(__cpp_lib_integer_sequence) || FMT_MSC_VER >= 1900
template <typename T, T... N>
using integer_sequence = std::integer_sequence<T, N...>;
template <size_t... N> using index_sequence = std::index_sequence<N...>;
template <size_t N> using make_index_sequence = std::make_index_sequence<N>;
#else
template <typename T, T... N> struct integer_sequence {
  using value_type = T;

  static FMT_CONSTEXPR size_t size() { return sizeof...(N); }
};

template <size_t... N> using index_sequence = integer_sequence<size_t, N...>;

template <typename T, size_t N, T... Ns>
struct make_integer_sequence : make_integer_sequence<T, N - 1, N - 1, Ns...> {};
template <typename T, T... Ns>
struct make_integer_sequence<T, 0, Ns...> : integer_sequence<T, Ns...> {};

template <size_t N>
using make_index_sequence = make_integer_sequence<size_t, N>;
#endif

template <class Tuple, class F, size_t... Is>
void for_each(index_sequence<Is...>, Tuple&& tup, F&& f) FMT_NOEXCEPT {
  using std::get;
  // using free function get<I>(T) now.
  const int _[] = {0, ((void)f(get<Is>(tup)), 0)...};
  (void)_;  // blocks warnings
}

template <class T>
FMT_CONSTEXPR make_index_sequence<std::tuple_size<T>::value> get_indexes(
	T const&) {
  return {};
}

template <class Tuple, class F> void for_each(Tuple&& tup, F&& f) {
  const auto indexes = get_indexes(tup);
  for_each(indexes, std::forward<Tuple>(tup), std::forward<F>(f));
}

template <typename Range>
using value_type = remove_cvref_t<decltype(*std::declval<Range>().begin())>;

template <typename Arg, FMT_ENABLE_IF(!is_like_std_string<
									  typename std::decay<Arg>::type>::value)>
FMT_CONSTEXPR const char* format_str_quoted(bool add_space, const Arg&) {
  return add_space ? " {}" : "{}";
}

template <typename Arg, FMT_ENABLE_IF(is_like_std_string<
									  typename std::decay<Arg>::type>::value)>
FMT_CONSTEXPR const char* format_str_quoted(bool add_space, const Arg&) {
  return add_space ? " \"{}\"" : "\"{}\"";
}

FMT_CONSTEXPR const char* format_str_quoted(bool add_space, const char*) {
  return add_space ? " \"{}\"" : "\"{}\"";
}
FMT_CONSTEXPR const wchar_t* format_str_quoted(bool add_space, const wchar_t*) {
  return add_space ? L" \"{}\"" : L"\"{}\"";
}

FMT_CONSTEXPR const char* format_str_quoted(bool add_space, const char) {
  return add_space ? " '{}'" : "'{}'";
}
FMT_CONSTEXPR const wchar_t* format_str_quoted(bool add_space, const wchar_t) {
  return add_space ? L" '{}'" : L"'{}'";
}
}  // namespace detail

template <typename T> struct is_tuple_like {
  static FMT_CONSTEXPR_DECL const bool value =
	  detail::is_tuple_like_<T>::value && !detail::is_range_<T>::value;
};

template <typename TupleT, typename Char>
struct formatter<TupleT, Char, enable_if_t<fmt::is_tuple_like<TupleT>::value>> {
 private:
  // C++11 generic lambda for format()
  template <typename FormatContext> struct format_each {
	template <typename T> void operator()(const T& v) {
	  if (i > 0) {
		if (formatting.add_prepostfix_space) {
		  *out++ = ' ';
		}
		out = detail::copy(formatting.delimiter, out);
	  }
	  out = format_to(out,
					  detail::format_str_quoted(
						  (formatting.add_delimiter_spaces && i > 0), v),
					  v);
	  ++i;
	}

	formatting_tuple<Char>& formatting;
	size_t& i;
	typename std::add_lvalue_reference<decltype(
		std::declval<FormatContext>().out())>::type out;
  };

 public:
  formatting_tuple<Char> formatting;

  template <typename ParseContext>
  FMT_CONSTEXPR auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
	return formatting.parse(ctx);
  }

  template <typename FormatContext = format_context>
  auto format(const TupleT& values, FormatContext& ctx) -> decltype(ctx.out()) {
	auto out = ctx.out();
	size_t i = 0;
	detail::copy(formatting.prefix, out);

	detail::for_each(values, format_each<FormatContext>{formatting, i, out});
	if (formatting.add_prepostfix_space) {
	  *out++ = ' ';
	}
	detail::copy(formatting.postfix, out);

	return ctx.out();
  }
};

template <typename T, typename Char> struct is_range {
  static FMT_CONSTEXPR_DECL const bool value =
	  detail::is_range_<T>::value && !detail::is_like_std_string<T>::value &&
	  !std::is_convertible<T, std::basic_string<Char>>::value &&
	  !std::is_constructible<detail::std_string_view<Char>, T>::value;
};

template <typename T, typename Char>
struct formatter<
	T, Char,
	enable_if_t<fmt::is_range<T, Char>::value
// Workaround a bug in MSVC 2017 and earlier.
#if !FMT_MSC_VER || FMT_MSC_VER >= 1927
				&&
				(has_formatter<detail::value_type<T>, format_context>::value ||
				 detail::has_fallback_formatter<detail::value_type<T>,
												format_context>::value)
#endif
				>> {
  formatting_range<Char> formatting;

  template <typename ParseContext>
  FMT_CONSTEXPR auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
	return formatting.parse(ctx);
  }

  template <typename FormatContext>
  typename FormatContext::iterator format(const T& values, FormatContext& ctx) {
	auto out = detail::copy(formatting.prefix, ctx.out());
	size_t i = 0;
	auto it = values.begin();
	auto end = values.end();
	for (; it != end; ++it) {
	  if (i > 0) {
		if (formatting.add_prepostfix_space) *out++ = ' ';
		out = detail::copy(formatting.delimiter, out);
	  }
	  out = format_to(out,
					  detail::format_str_quoted(
						  (formatting.add_delimiter_spaces && i > 0), *it),
					  *it);
	  if (++i > formatting.range_length_limit) {
		out = format_to(out, " ... <other elements>");
		break;
	  }
	}
	if (formatting.add_prepostfix_space) *out++ = ' ';
	return detail::copy(formatting.postfix, out);
  }
};

template <typename Char, typename... T> struct tuple_arg_join : detail::view {
  const std::tuple<T...>& tuple;
  basic_string_view<Char> sep;

  tuple_arg_join(const std::tuple<T...>& t, basic_string_view<Char> s)
	  : tuple{t}, sep{s} {}
};

template <typename Char, typename... T>
struct formatter<tuple_arg_join<Char, T...>, Char> {
  template <typename ParseContext>
  FMT_CONSTEXPR auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
	return ctx.begin();
  }

  template <typename FormatContext>
  typename FormatContext::iterator format(
	  const tuple_arg_join<Char, T...>& value, FormatContext& ctx) {
	return format(value, ctx, detail::make_index_sequence<sizeof...(T)>{});
  }

 private:
  template <typename FormatContext, size_t... N>
  typename FormatContext::iterator format(
	  const tuple_arg_join<Char, T...>& value, FormatContext& ctx,
	  detail::index_sequence<N...>) {
	return format_args(value, ctx, std::get<N>(value.tuple)...);
  }

  template <typename FormatContext>
  typename FormatContext::iterator format_args(
	  const tuple_arg_join<Char, T...>&, FormatContext& ctx) {
	// NOTE: for compilers that support C++17, this empty function instantiation
	// can be replaced with a constexpr branch in the variadic overload.
	return ctx.out();
  }

  template <typename FormatContext, typename Arg, typename... Args>
  typename FormatContext::iterator format_args(
	  const tuple_arg_join<Char, T...>& value, FormatContext& ctx,
	  const Arg& arg, const Args&... args) {
	using base = formatter<typename std::decay<Arg>::type, Char>;
	auto out = ctx.out();
	out = base{}.format(arg, ctx);
	if (sizeof...(Args) > 0) {
	  out = std::copy(value.sep.begin(), value.sep.end(), out);
	  ctx.advance_to(out);
	  return format_args(value, ctx, args...);
	}
	return out;
  }
};

/**
  \rst
  Returns an object that formats `tuple` with elements separated by `sep`.

  **Example**::

	std::tuple<int, char> t = {1, 'a'};
	fmt::print("{}", fmt::join(t, ", "));
	// Output: "1, a"
  \endrst
 */
template <typename... T>
FMT_CONSTEXPR tuple_arg_join<char, T...> join(const std::tuple<T...>& tuple,
											  string_view sep) {
  return {tuple, sep};
}

template <typename... T>
FMT_CONSTEXPR tuple_arg_join<wchar_t, T...> join(const std::tuple<T...>& tuple,
												 wstring_view sep) {
  return {tuple, sep};
}

/**
  \rst
  Returns an object that formats `initializer_list` with elements separated by
  `sep`.

  **Example**::

	fmt::print("{}", fmt::join({1, 2, 3}, ", "));
	// Output: "1, 2, 3"
  \endrst
 */
template <typename T>
arg_join<const T*, const T*, char> join(std::initializer_list<T> list,
										string_view sep) {
  return join(std::begin(list), std::end(list), sep);
}

template <typename T>
arg_join<const T*, const T*, wchar_t> join(std::initializer_list<T> list,
										   wstring_view sep) {
  return join(std::begin(list), std::end(list), sep);
}

FMT_END_NAMESPACE

#endif  // FMT_RANGES_H_

/*** End of inlined file: dmranges.h ***/


/*** Start of inlined file: dmscan.h ***/
// Formatting library for C++ - scanning API proof of concept
//
// Copyright (c) 2019 - present, Victor Zverovich
// All rights reserved.
//
// For the license information refer to format.h.

#include <array>
#include <cassert>
#include <climits>

FMT_BEGIN_NAMESPACE
template <typename T, typename Char = char> struct scanner {
  // A deleted default constructor indicates a disabled scanner.
  scanner() = delete;
};

class scan_parse_context {
 private:
  string_view format_;

 public:
  using iterator = string_view::iterator;

  explicit FMT_CONSTEXPR scan_parse_context(string_view format)
	  : format_(format) {}

  FMT_CONSTEXPR iterator begin() const { return format_.begin(); }
  FMT_CONSTEXPR iterator end() const { return format_.end(); }

  void advance_to(iterator it) {
	format_.remove_prefix(detail::to_unsigned(it - begin()));
  }
};

struct scan_context {
 private:
  string_view input_;

 public:
  using iterator = const char*;

  explicit scan_context(string_view input) : input_(input) {}

  iterator begin() const { return input_.data(); }
  iterator end() const { return begin() + input_.size(); }

  void advance_to(iterator it) {
	input_.remove_prefix(detail::to_unsigned(it - begin()));
  }
};

namespace detail {
enum class scan_type {
  none_type,
  int_type,
  uint_type,
  long_long_type,
  ulong_long_type,
  string_type,
  string_view_type,
  custom_type
};

struct custom_scan_arg {
  void* value;
  void (*scan)(void* arg, scan_parse_context& parse_ctx, scan_context& ctx);
};

class scan_arg {
 public:
  scan_type type;
  union {
	int* int_value;
	unsigned* uint_value;
	long long* long_long_value;
	unsigned long long* ulong_long_value;
	std::string* string;
	fmt::string_view* string_view;
	custom_scan_arg custom;
	// TODO: more types
  };

  scan_arg() : type(scan_type::none_type) {}
  scan_arg(int& value) : type(scan_type::int_type), int_value(&value) {}
  scan_arg(unsigned& value) : type(scan_type::uint_type), uint_value(&value) {}
  scan_arg(long long& value)
	  : type(scan_type::long_long_type), long_long_value(&value) {}
  scan_arg(unsigned long long& value)
	  : type(scan_type::ulong_long_type), ulong_long_value(&value) {}
  scan_arg(std::string& value) : type(scan_type::string_type), string(&value) {}
  scan_arg(fmt::string_view& value)
	  : type(scan_type::string_view_type), string_view(&value) {}
  template <typename T> scan_arg(T& value) : type(scan_type::custom_type) {
	custom.value = &value;
	custom.scan = scan_custom_arg<T>;
  }

 private:
  template <typename T>
  static void scan_custom_arg(void* arg, scan_parse_context& parse_ctx,
							  scan_context& ctx) {
	scanner<T> s;
	parse_ctx.advance_to(s.parse(parse_ctx));
	ctx.advance_to(s.scan(*static_cast<T*>(arg), ctx));
  }
};
}  // namespace detail

struct scan_args {
  int size;
  const detail::scan_arg* data;

  template <size_t N>
  scan_args(const std::array<detail::scan_arg, N>& store)
	  : size(N), data(store.data()) {
	static_assert(N < INT_MAX, "too many arguments");
  }
};

namespace detail {

struct scan_handler : error_handler {
 private:
  scan_parse_context parse_ctx_;
  scan_context scan_ctx_;
  scan_args args_;
  int next_arg_id_;
  scan_arg arg_;

  template <typename T = unsigned> T read_uint() {
	T value = 0;
	auto it = scan_ctx_.begin(), end = scan_ctx_.end();
	while (it != end) {
	  char c = *it++;
	  if (c < '0' || c > '9') on_error("invalid input");
	  // TODO: check overflow
	  value = value * 10 + static_cast<unsigned>(c - '0');
	}
	scan_ctx_.advance_to(it);
	return value;
  }

  template <typename T = int> T read_int() {
	auto it = scan_ctx_.begin(), end = scan_ctx_.end();
	bool negative = it != end && *it == '-';
	if (negative) ++it;
	scan_ctx_.advance_to(it);
	const auto value = read_uint<typename std::make_unsigned<T>::type>();
	if (negative) return -static_cast<T>(value);
	return static_cast<T>(value);
  }

 public:
  scan_handler(string_view format, string_view input, scan_args args)
	  : parse_ctx_(format), scan_ctx_(input), args_(args), next_arg_id_(0) {}

  const char* pos() const { return scan_ctx_.begin(); }

  void on_text(const char* begin, const char* end) {
	auto size = to_unsigned(end - begin);
	auto it = scan_ctx_.begin();
	if (it + size > scan_ctx_.end() ||
		!std::equal(begin, end, make_checked(it, size))) {
	  on_error("invalid input");
	}
	scan_ctx_.advance_to(it + size);
  }

  int on_arg_id() { return on_arg_id(next_arg_id_++); }
  int on_arg_id(int id) {
	if (id >= args_.size) on_error("argument index out of range");
	arg_ = args_.data[id];
	return id;
  }
  int on_arg_id(string_view) { return on_error("invalid format"), 0; }

  void on_replacement_field(int, const char*) {
	auto it = scan_ctx_.begin(), end = scan_ctx_.end();
	switch (arg_.type) {
	case scan_type::int_type:
	  *arg_.int_value = read_int();
	  break;
	case scan_type::uint_type:
	  *arg_.uint_value = read_uint();
	  break;
	case scan_type::long_long_type:
	  *arg_.long_long_value = read_int<long long>();
	  break;
	case scan_type::ulong_long_type:
	  *arg_.ulong_long_value = read_uint<unsigned long long>();
	  break;
	case scan_type::string_type:
	  while (it != end && *it != ' ') arg_.string->push_back(*it++);
	  scan_ctx_.advance_to(it);
	  break;
	case scan_type::string_view_type: {
	  auto s = it;
	  while (it != end && *it != ' ') ++it;
	  *arg_.string_view = fmt::string_view(s, to_unsigned(it - s));
	  scan_ctx_.advance_to(it);
	  break;
	}
	case scan_type::none_type:
	case scan_type::custom_type:
	  assert(false);
	}
  }

  const char* on_format_specs(int, const char* begin, const char*) {
	if (arg_.type != scan_type::custom_type) return begin;
	parse_ctx_.advance_to(begin);
	arg_.custom.scan(arg_.custom.value, parse_ctx_, scan_ctx_);
	return parse_ctx_.begin();
  }
};
}  // namespace detail

template <typename... Args>
std::array<detail::scan_arg, sizeof...(Args)> make_scan_args(Args&... args) {
  return { {args...} };
}

string_view::iterator vscan(string_view input, string_view format_str,
							scan_args args) {
  detail::scan_handler h(format_str, input, args);
  detail::parse_format_string<false>(format_str, h);
  return input.begin() + (h.pos() - &*input.begin());
}

template <typename... Args>
string_view::iterator scan(string_view input, string_view format_str,
						   Args&... args) {
  return vscan(input, format_str, make_scan_args(args...));
}
FMT_END_NAMESPACE

/*** End of inlined file: dmscan.h ***/

#endif // __DMFORMATALL_H__
