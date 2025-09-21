#
# LLVM/Clang Compiler Configuration Overrides
#

# Base warning flags for all configurations
set(BASE_WARNING_FLAGS
  # Baseline warnings
  -Werror
  -Wpedantic -Wall -Wextra -Wuninitialized -Wmissing-include-dirs
  -Wshadow -Wundef -Winvalid-pch
  -Werror=option-ignored -Warc-repeated-use-of-weak
  -Wbitfield-enum-conversion -Wclass-varargs
  -Wconditional-uninitialized -Wthread-safety

  # Control flow
  -Winit-self -Wswitch-enum -Wswitch-default -Wformat=2
  -Wformat-nonliteral -Wformat-security -Wformat-y2k

  # Arithmetic and conversion
  -Wdouble-promotion -Wfloat-equal -Wpointer-arith -Wstrict-overflow=5
  -Wcast-qual -Wcast-align -Wconversion -Wpacked

  # Advanced GCC warnings
  -Wshift-negative-value -Wnull-dereference -Wunused-macros -Walloca

  # Security and safety
  -Wstrict-aliasing -Wredundant-decls -Wmissing-declarations
  -Wmissing-field-initializers -Wwrite-strings -Wstack-protector
  -Wpadded -Winline -Wdisabled-optimization -Wframe-larger-than=1024

  # Mistakes
  -Wconsumed -Wdirect-ivar-access -Wdisabled-macro-expansion
  -Wembedded-directive -Wexit-time-destructors
  -Wexpansion-to-defined -Wformat-pedantic -Widiomatic-parentheses
  -Winconsistent-missing-destructor-override -Winfinite-recursion
  -Wloop-analysis -Wmethod-signatures
  -Wmismatched-tags -Wmissing-braces -Wmissing-prototypes
  -Wmissing-variable-declarations -Wmost -Wmove
  -Wnonportable-system-include-path -Wnull-pointer-arithmetic
  -Wover-aligned -Woverriding-method-mismatch -Wpch-date-time
  -Wpragmas -Wreserved-id-macro -Wreserved-user-defined-literal
  -Wretained-language-linkage -Wsemicolon-before-method-body
  -Wsometimes-uninitialized -Wstring-conversion
  -Wsuper-class-method-mismatch -Wtautological-compare
  -Wundefined-reinterpret-cast
  -Wunreachable-code -Wunreachable-code-break
  -Wunreachable-code-loop-increment -Wunreachable-code-return
  -Wvector-conversion
)

# Security and optimization flags
set(BASE_SECURITY_FLAGS
  # Common
  -D_FORTIFY_SOURCE=3 -D_GLIBCXX_ASSERTIONS
  -fstrict-aliasing
  -fstack-protector-strong -fstack-clash-protection
  -fsanitize=bounds -fsanitize-undefined-trap-on-error
  # Code Sanitization
  -Wcomma -Wduplicate-enum -Wduplicate-method-arg
  -Wduplicate-method-match -Wdynamic-exception-spec
  -Wempty-translation-unit -Wexplicit-ownership-type
  -Wignored-qualifiers -Wimplicit -Wkeyword-macro
  -Wnewline-eof -Wredundant-parens -Wstatic-in-inline
  -Wstrict-prototypes -Wweak-template-vtables
  -Wweak-vtables -Wzero-length-array
### Arrays
  -Warray-bounds-pointer-arithmetic
  #-Wextended-offsetof
  -Wflexible-array-extensions
### Arithmetics
  -Wfloat-conversion -Wfloat-overflow-conversion
  -Wfloat-zero-conversion -Wshorten-64-to-32
  -Wsign-compare -Wsign-conversion
### Advices
  -Wcomment -Wdocumentation -Wdocumentation-pedantic
  -Wglobal-constructors -Wgnu -Wheader-hygiene
  -Wunneeded-internal-declaration -Wunneeded-member-function
  -Wvla -Wsuggest-override -Wnull-dereference -Wunused-macros
  -Wshift-overflow -Wshift-negative-value
)

# Visibility and code generation
set(BASE_CODEGEN_FLAGS
  -fvisibility=hidden -fdata-sections -ffunction-sections
  -fPIC -fPIE
  #-pie
)

# C-specific flags
set(C_ONLY_FLAGS
  -Waggregate-return -Wbad-function-cast -Wc++-compat
)

# C++-specific flags
set(CXX_ONLY_FLAGS
  -Wzero-as-null-pointer-constant -Wctor-dtor-privacy -Wold-style-cast
  -Woverloaded-virtual
  #-Wsuggest-final-types -Wsuggest-final-methods
  -Wsuggest-override
  #-Wuseless-cast
  #-Wnoexcept -Wstrict-null-sentinel
  #-Wvirtual-inheritance -Wmultiple-inheritance
  -Wextra-semi
  -fvisibility-inlines-hidden -fno-exceptions -fno-unwind-tables
  -fno-asynchronous-unwind-tables -fno-rtti
)
# Combine flags for each language
string(JOIN " " CMAKE_C_FLAGS
  ${BASE_WARNING_FLAGS} ${BASE_SECURITY_FLAGS} ${BASE_CODEGEN_FLAGS} ${C_ONLY_FLAGS}
)
string(JOIN " " CMAKE_CXX_FLAGS
  ${BASE_WARNING_FLAGS} ${BASE_SECURITY_FLAGS} ${BASE_CODEGEN_FLAGS} ${CXX_ONLY_FLAGS}
)

#
# Configuration-specific flags
#

# Debug configuration
string(JOIN " " CMAKE_C_FLAGS_DEBUG "-DDEBUG" "-D_DEBUG" "-O0" "-g")
string(JOIN " " CMAKE_CXX_FLAGS_DEBUG "-DDEBUG" "-D_DEBUG" "-O0" "-g")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "")
set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "")

# Profile configuration
string(JOIN " " CMAKE_C_FLAGS_PROFILE "-DNDEBUG" "-D_PROFILE" "-O2" "-fno-omit-frame-pointer")
string(JOIN " " CMAKE_CXX_FLAGS_PROFILE "-DNDEBUG" "-D_PROFILE" "-O2" "-fno-omit-frame-pointer")
string(JOIN " " PROFILE_LINK_FLAGS
  "-Wl,-z,separate-code" "-Wl,-z,nodlopen"
  "-Wl,-z,noexecstack" "-Wl,-z,now" "-Wl,-z,relro"
)
set(CMAKE_EXE_LINKER_FLAGS_PROFILE "${PROFILE_LINK_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS_PROFILE "${PROFILE_LINK_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS_PROFILE "${PROFILE_LINK_FLAGS}")

# Release configuration
string(JOIN " " CMAKE_C_FLAGS_RELEASE "-DNDEBUG" "-D_RELEASE" "-O3")
string(JOIN " " CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG" "-D_RELEASE" "-O3")
string(JOIN " " RELEASE_LINK_FLAGS
  "-Wl,--gc-sections" "-Wl,-z,separate-code" "-Wl,-z,nodlopen"
  "-Wl,-z,noexecstack" "-Wl,-z,now" "-Wl,-z,relro"
  "-Wl,--no-copy-dt-needed-entries" "-Wl,--strip-all"
)
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${RELEASE_LINK_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${RELEASE_LINK_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${RELEASE_LINK_FLAGS}")

# Address Sanitizer configuration
string(JOIN " " CMAKE_C_FLAGS_ASAN "-DNDEBUG" "-D_ASAN" "-O1" "-g" "-fno-omit-frame-pointer" "-fsanitize=address" "-fsanitize=leak")
string(JOIN " " CMAKE_CXX_FLAGS_ASAN "-DNDEBUG" "-D_ASAN" "-O1" "-g" "-fno-omit-frame-pointer" "-fsanitize=address" "-fsanitize=leak")
set(CMAKE_EXE_LINKER_FLAGS_ASAN "-fsanitize=address -fsanitize=leak -Wl,-z,separate-code -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")
set(CMAKE_SHARED_LINKER_FLAGS_ASAN "-fsanitize=address -fsanitize=leak -Wl,-z,separate-code -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")
set(CMAKE_MODULE_LINKER_FLAGS_ASAN "-fsanitize=address -fsanitize=leak -Wl,-z,separate-code -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")

# Thread Sanitizer configuration
string(JOIN " " CMAKE_C_FLAGS_TSAN "-DNDEBUG" "-D_TSAN" "-O1" "-g" "-fno-omit-frame-pointer" "-fsanitize=thread")
string(JOIN " " CMAKE_CXX_FLAGS_TSAN "-DNDEBUG" "-D_TSAN" "-O1" "-g" "-fno-omit-frame-pointer" "-fsanitize=thread")
set(CMAKE_EXE_LINKER_FLAGS_TSAN "-fsanitize=thread -Wl,-z,separate-code -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")
set(CMAKE_SHARED_LINKER_FLAGS_TSAN "-fsanitize=thread -Wl,-z,separate-code -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")
set(CMAKE_MODULE_LINKER_FLAGS_TSAN "-fsanitize=thread -Wl,-z,separate-code -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")

# Undefined Behavior Sanitizer configuration
string(JOIN " " CMAKE_C_FLAGS_UBSAN "-DNDEBUG" "-D_UBSAN" "-O1" "-g" "-fno-omit-frame-pointer" "-fsanitize=undefined" "-fsanitize=float-divide-by-zero" "-fsanitize=float-cast-overflow" "-fsanitize=integer")
string(JOIN " " CMAKE_CXX_FLAGS_UBSAN "-DNDEBUG" "-D_UBSAN" "-O1" "-g" "-fno-omit-frame-pointer" "-fsanitize=undefined" "-fsanitize=float-divide-by-zero" "-fsanitize=float-cast-overflow" "-fsanitize=integer")
set(CMAKE_EXE_LINKER_FLAGS_UBSAN "-fsanitize=undefined -Wl,-z,separate-code -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")
set(CMAKE_SHARED_LINKER_FLAGS_UBSAN "-fsanitize=undefined -Wl,-z,separate-code -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")
set(CMAKE_MODULE_LINKER_FLAGS_UBSAN "-fsanitize=undefined -Wl,-z,separate-code -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")

# Real-Time sanitizing configuration
string(JOIN " " CMAKE_C_FLAGS_RTSAN "-DNDEBUG" "-D_RTSAN" "-O1" "-g" "-fno-omit-frame-pointer" "-fsanitize=realtime")
string(JOIN " " CMAKE_CXX_FLAGS_RTSAN "-DNDEBUG" "-D_RTSAN" "-O1" "-g" "-fno-omit-frame-pointer" "-fsanitize=realtime")
set(CMAKE_EXE_LINKER_FLAGS_RTSAN "-fsanitize=realtime -Wl,-z,separate-code -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")
set(CMAKE_SHARED_LINKER_FLAGS_RTSAN "-fsanitize=realtime -Wl,-z,separate-code -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")
set(CMAKE_MODULE_LINKER_FLAGS_RTSAN "-fsanitize=realtime -Wl,-z,separate-code -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")


#
# Diagnostic output
#

foreach(lang IN ITEMS C CXX)
  foreach(cfg IN ITEMS DEBUG RELEASE PROFILE ASAN TSAN UBSAN RTSAN)
    message(STATUS "CMAKE_${lang}_FLAGS_${cfg}: ${CMAKE_${lang}_FLAGS_${cfg}}")
  endforeach()
  message(STATUS "CMAKE_${lang}_FLAGS: ${CMAKE_${lang}_FLAGS}")
endforeach()

foreach(cfg IN ITEMS DEBUG RELEASE PROFILE ASAN TSAN UBSAN RTSAN)
  foreach(ld IN ITEMS EXE SHARED MODULE)
    message(STATUS "CMAKE_${ld}_LINKER_FLAGS_${cfg}: ${CMAKE_${ld}_LINKER_FLAGS_${cfg}}")
  endforeach()
endforeach()

foreach(ld IN ITEMS EXE SHARED MODULE)
  message(STATUS "CMAKE_${ld}_LINKER_FLAGS: ${CMAKE_${ld}_LINKER_FLAGS}")
endforeach()
