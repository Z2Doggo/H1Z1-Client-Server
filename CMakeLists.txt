# CMakeList.txt : CMake project for H1Z1-Client-Server, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("H1Z1-Client-Server")

# Add source to this project's shared executable.
add_library(H1_Login_Module SHARED "src/main_login.cpp" "src/win32_sockets.hpp" "src/misc.cpp" "src/win32_sockets.cpp" "src/binary.hpp" "src/binary.cpp" "src/Login_Server/login.hpp"  "src/OLD_endian.cpp" "src/Packets/LoginUdp_11/LoginUdp_11.hpp" "src/Packets/LoginUdp_11/LoginUdp_11.cpp" "src/SOE/soe_protocol.hpp" "src/SOE/soe_protocol.cpp" "src/SOE/session.cpp")

# Link against the necessary libraries
target_link_libraries(H1_Login_Module PRIVATE ws2_32.lib PRIVATE winmm.lib PRIVATE kernel32.lib)

# Add source to this project's executable.
add_executable (H1Z1-Client-Server "src/win32_login.cpp" "src/win32_sockets.hpp" "src/misc.cpp" "src/win32_sockets.cpp" "src/binary.hpp" "src/binary.cpp" "src/Login_Server/login.hpp"  "src/OLD_endian.cpp" "src/Packets/LoginUdp_11/LoginUdp_11.hpp" "src/Packets/LoginUdp_11/LoginUdp_11.cpp" "src/SOE/soe_protocol.hpp" "src/SOE/soe_protocol.cpp" "src/SOE/session.cpp")

# Link against the necessary libraries
target_link_libraries(H1Z1-Client-Server PRIVATE ws2_32.lib PRIVATE winmm.lib PRIVATE kernel32.lib PUBLIC H1_Login_Module)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET H1_Login_Module PROPERTY CXX_STANDARD 20)
  set_property(TARGET H1Z1-Client-Server PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
