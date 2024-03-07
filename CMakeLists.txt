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
add_library(H1_Login_Module SHARED "src/main_login.cpp" "src/app_memory.hpp" "src/win32_sockets.hpp" "src/misc.cpp" "src/win32_sockets.cpp" "src/SOE/session.cpp" "src/SOE/core_protocol.hpp" "src/SOE/core_protocol.cpp" "src/binary.hpp" "src/binary.cpp" "src/Packets/LoginUdp_11/login_packets.hpp" "src/Packets/LoginUdp_11/login_packets.cpp" "src/Login_Server/login.hpp" "src/Login_Server/login.cpp")

# Link against the necessary libraries
target_link_libraries(H1_Login_Module PRIVATE ws2_32 PRIVATE winmm.lib)

# Add source to this project's executable.
add_executable (H1Z1-Client-Server "src/win32_login.cpp" "src/app_memory.hpp" "src/win32_sockets.hpp" "src/misc.cpp" "src/win32_sockets.cpp" "src/SOE/session.cpp" "src/SOE/core_protocol.hpp" "src/SOE/core_protocol.cpp" "src/binary.hpp" "src/binary.cpp" "src/Packets/LoginUdp_11/login_packets.hpp" "src/Packets/LoginUdp_11/login_packets.cpp" "src/Login_Server/login.hpp" "src/Login_Server/login.cpp")

# Link against the necessary libraries
target_link_libraries(H1Z1-Client-Server PRIVATE ws2_32 PRIVATE winmm.lib PUBLIC H1_Login_Module)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET H1_Login_Module PROPERTY CXX_STANDARD 20)
  set_property(TARGET H1Z1-Client-Server PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
