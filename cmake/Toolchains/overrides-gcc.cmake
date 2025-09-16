#
# GCC Compiler Configuration Overrides
#

# Base warning flags for all configurations
set(BASE_WARNING_FLAGS
  # Baseline warnings
  -Wall -Wextra -Wpedantic -Wuninitialized -Wmissing-include-dirs
  -Wshadow -Wundef -Winvalid-pch

  # Control flow
  -Winit-self -Wswitch-enum -Wswitch-default -Wformat=2
  -Wformat-nonliteral -Wformat-security -Wformat-y2k

  # Arithmetic and conversion
  -Wdouble-promotion -Wfloat-equal -Wpointer-arith -Wstrict-overflow=5
  -Wcast-qual -Wcast-align -Wconversion -Wpacked

  # Advanced GCC warnings
  -Wshift-overflow=2 -Wshift-negative-value -Wnull-dereference
  -Wduplicated-cond -Wunused-macros -Wstringop-overflow=4
  -Wduplicated-branches -Walloc-zero -Walloca -Wcast-align=strict
  -Wstringop-truncation

  # Security and safety
  -Wstrict-aliasing -Wredundant-decls -Wmissing-declarations
  -Wmissing-field-initializers -Wwrite-strings -Wstack-protector
  -Wpadded -Winline -Wdisabled-optimization -Wlogical-op
  -Wstack-usage=1024 -Wframe-larger-than=1024 -Wtrampolines
  -Wvector-operation-performance
)

# Security and optimization flags
set(BASE_SECURITY_FLAGS
  -D_FORTIFY_SOURCE=3 -D_GLIBCXX_ASSERTIONS
  -fstrict-aliasing -fstack-protector -fstack-protector-strong
)

# Visibility and code generation
set(BASE_CODEGEN_FLAGS
  -fvisibility=hidden -fdata-sections -ffunction-sections
  -fPIC -fPIE -pie
)

# C-specific flags
set(C_ONLY_FLAGS
  -Waggregate-return -Wbad-function-cast -Wc++-compat
)

# C++-specific flags
set(CXX_ONLY_FLAGS
  -Wzero-as-null-pointer-constant -Wctor-dtor-privacy -Wold-style-cast
  -Woverloaded-virtual -Wsuggest-final-types -Wsuggest-final-methods
  -Wsuggest-override -Wuseless-cast -Wnoexcept -Wstrict-null-sentinel
  -Wvirtual-inheritance -Wmultiple-inheritance -Wextra-semi
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
string(JOIN " " CMAKE_C_FLAGS_ASAN "-DNDEBUG" "-D_ASAN" "-O1" "-g" "-fno-omit-frame-pointer" "-fsanitize=address")
string(JOIN " " CMAKE_CXX_FLAGS_ASAN "-DNDEBUG" "-D_ASAN" "-O1" "-g" "-fno-omit-frame-pointer" "-fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS_ASAN "-fsanitize=address -Wl,-z,separate-code -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")
set(CMAKE_SHARED_LINKER_FLAGS_ASAN "-fsanitize=address -Wl,-z,separate-code -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")
set(CMAKE_MODULE_LINKER_FLAGS_ASAN "-fsanitize=address -Wl,-z,separate-code -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")

# Thread Sanitizer configuration
string(JOIN " " CMAKE_C_FLAGS_TSAN "-DNDEBUG" "-D_TSAN" "-O1" "-g" "-fno-omit-frame-pointer" "-fsanitize=thread")
string(JOIN " " CMAKE_CXX_FLAGS_TSAN "-DNDEBUG" "-D_TSAN" "-O1" "-g" "-fno-omit-frame-pointer" "-fsanitize=thread")
set(CMAKE_EXE_LINKER_FLAGS_TSAN "-fsanitize=thread -Wl,-z,separate-code -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")
set(CMAKE_SHARED_LINKER_FLAGS_TSAN "-fsanitize=thread -Wl,-z,separate-code -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")
set(CMAKE_MODULE_LINKER_FLAGS_TSAN "-fsanitize=thread -Wl,-z,separate-code -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")

# Undefined Behavior Sanitizer configuration
string(JOIN " " CMAKE_C_FLAGS_UBSAN "-DNDEBUG" "-D_UBSAN" "-O1" "-g" "-fno-omit-frame-pointer" "-fsanitize=undefined")
string(JOIN " " CMAKE_CXX_FLAGS_UBSAN "-DNDEBUG" "-D_UBSAN" "-O1" "-g" "-fno-omit-frame-pointer" "-fsanitize=undefined")
set(CMAKE_EXE_LINKER_FLAGS_UBSAN "-fsanitize=undefined -Wl,-z,separate-code -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")
set(CMAKE_SHARED_LINKER_FLAGS_UBSAN "-fsanitize=undefined -Wl,-z,separate-code -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")
set(CMAKE_MODULE_LINKER_FLAGS_UBSAN "-fsanitize=undefined -Wl,-z,separate-code -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")

#
# Diagnostic output
#

foreach(lang IN ITEMS C CXX)
  foreach(cfg IN ITEMS DEBUG RELEASE PROFILE ASAN TSAN UBSAN)
    message(STATUS "CMAKE_${lang}_FLAGS_${cfg}: ${CMAKE_${lang}_FLAGS_${cfg}}")
  endforeach()
  message(STATUS "CMAKE_${lang}_FLAGS: ${CMAKE_${lang}_FLAGS}")
endforeach()

foreach(cfg IN ITEMS DEBUG RELEASE PROFILE ASAN TSAN UBSAN)
  foreach(ld IN ITEMS EXE SHARED MODULE)
    message(STATUS "CMAKE_${ld}_LINKER_FLAGS_${cfg}: ${CMAKE_${ld}_LINKER_FLAGS_${cfg}}")
  endforeach()
endforeach()

foreach(ld IN ITEMS EXE SHARED MODULE)
  message(STATUS "CMAKE_${ld}_LINKER_FLAGS: ${CMAKE_${ld}_LINKER_FLAGS}")
endforeach()