###############################
# Mouskee_control 2019 29 SEP #
###############################

cmake_minimum_required (VERSION 2.6)
project(mouskee)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

if(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

include_directories(include/)

set (SRCS
	src/main.cpp)

##########################
### --XInputLibrary -- ###
##########################

# Linux
if(UNIX AND NOT APPLE)
  find_library(X_11 X11)
  find_library(X_TST Xtst)
  set(EXTRA_LIBS ${X_11} ${X_TST})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  set(PLATFORM_SOURCE_FILES XInputSimulator/xinputsimulatorimpllinux.cpp XInputSimulator/xinputsimulatorimpllinux.h)
endif(UNIX AND NOT APPLE)
  # Apple
if(APPLE)
  find_library(APP_SERVICES ApplicationServices)
  find_library(CARBON Carbon)
  find_library(CORE_FOUNDATION CoreFoundation)
  set(EXTRA_LIBS ${APP_SERVICES_LIBRARY} ${CARBON} ${CORE_FOUNDATION})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  set(PLATFORM_SOURCE_FILES XInputSimulator/xinputsimulatorimplmacos.cpp XInputSimulator/xinputsimulatorimplmacos.h)
endif(APPLE)

# Windows
if(WIN32)
  #find_library(USER_32 User32.Lib)
  #set(EXTRA_LIBS ${USER_32})
  set(PLATFORM_SOURCE_FILES XInputSimulator/xinputsimulatorimplwin.cpp XInputSimulator/xinputsimulatorimplwin.h)
endif(WIN32)

set(XINPUTLIB_SOURCE_FILES
  XInputSimulator/notimplementedexception.cpp
  XInputSimulator/notimplementedexception.h
  XInputSimulator/xinputsimulator.cpp
  XInputSimulator/xinputsimulator.h
  XInputSimulator/xinputsimulatorimpl.cpp
  XInputSimulator/xinputsimulatorimpl.h
  ${PLATFORM_SOURCE_FILES})

##########################
### --XInputLibrary -- ###
##########################

add_executable(mouskee ${SRCS} ${XINPUTLIB_SOURCE_FILES})

target_include_directories(mouskee PUBLIC XInputSimulator/)
target_link_libraries(mouskee ${EXTRA_LIBS})
