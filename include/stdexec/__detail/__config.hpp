#ifndef STDEXEC___CONFIG_HPP
#define STDEXEC___CONFIG_HPP

#if __has_include(<version>)
#include <version>
#else
#include <ciso646>
#endif

#include <cassert>
#include <version>

#define STDEXEC_STRINGIZE(_ARG) #_ARG

#define STDEXEC_EXPAND(...) __VA_ARGS__
#define STDEXEC_EAT(...)

#define STDEXEC_HEAD_OR_TAIL(_XP, ...) STDEXEC_EXPAND __VA_OPT__((__VA_ARGS__) STDEXEC_EAT)(_XP)
#define STDEXEC_HEAD_OR_NULL(_XP, ...) STDEXEC_EXPAND __VA_OPT__(() STDEXEC_EAT)(_XP)

#if defined(__GNUG__)
#define STDEXEC_GUNG(...) 1 /* STDEXEC_HEAD_OR_TAIL(1, __VA_ARGS__) */
#endif

#ifndef STDEXEC_MSVC
#define STDEXEC_MSVC(...) 0 /* STDEXEC_HEAD_OR_NULL(0, __VA_ARGS__) */
#endif
#ifndef STDEXEC_GUNG
#define STDEXEC_GUNG(...) STDEXEC_HEAD_OR_NULL(0, __VA_ARGS__)
#endif

#if STDEXEC_GUNG()
#define STDEXEC_PRAGMA_IGNORE_GNU(...) \
  _Pragma(STDEXEC_STRINGIZE(GCC diagnostic ignored __VA_ARGS__))
#define STDEXEC_PRAGMA_PUSH()     \
  _Pragma("GCC dianosttic push")  \
  STDEXEC_PRAGMA_IGNORE_GNU("-Wpragmas")\
  STDEXEC_PRAGMA_IGNORE_GNU("-Wunknown-pragmas") \
  STDEXEC_PRAGMA_IGNORE_GNU("-Wunknown-attributes") \
  STDEXEC_PRAGMA_IGNORE_GNU("-Wattributes")
#define STDEXEC_PRAGMA_POP() _Pragma("GCC diagnostic pop")
#else
#define STDEXEC_PRAGMA_PUSH()
#define STDEXEC_PRAGMA_POP()
#endif

#if !STDEXEC_MSVC() && defined(__has_builtin)
#define STDEXEC_HAS_BUILTIN __has_builtin
#else
#define STDEXEC_HAS_BUILTIN(...) 0
#endif

#ifdef STDEXEC_ASSERT
#error "Redefinition of STDEXEC_ASSERT is not permitted. Define STDEXEC_ASSERT_FN instead."
#endif

#define STDEXEC_ASSERT(_XP)       \
  do {                            \
    static_assert(noexcept(_XP)); \
    STDEXEC_ASSERT_FN(_XP);       \
  } while (false)

#ifndef STDEXEC_ASSERT_FN
#define STDEXEC_ASSERT_FN assert
#endif



#endif //! STDEXEC___CONFIG_HPP
