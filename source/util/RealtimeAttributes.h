#pragma once

// Function-effect attributes consumed by Clang's RealtimeSanitizer (RTSan),
// upstreamed in LLVM 20. Annotating a function as DUPE_RT_NONBLOCKING tells
// RTSan that the body and its transitive callees must not block (no allocs,
// locks, syscalls, etc). DUPE_RT_BLOCKING marks an explicit boundary where
// blocking is allowed and analysis should stop descending.
//
// Older Clang (and non-Clang compilers) don't recognize the attributes; we
// expand to nothing there so the code still compiles and IDE noise is reduced.

#if defined(__has_cpp_attribute)
#if __has_cpp_attribute(clang::nonblocking)
#define DUPE_RT_NONBLOCKING [[clang::nonblocking]]
#endif
#if __has_cpp_attribute(clang::blocking)
#define DUPE_RT_BLOCKING [[clang::blocking]]
#endif
#endif

#ifndef DUPE_RT_NONBLOCKING
#define DUPE_RT_NONBLOCKING
#endif

#ifndef DUPE_RT_BLOCKING
#define DUPE_RT_BLOCKING
#endif
