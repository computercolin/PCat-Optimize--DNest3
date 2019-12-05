project (DNest3 C CXX)
cmake_minimum_required (VERSION 2.8.0)

# Setting this explicitly avoids warnings from versions of CMake >= 3
# regarding policy CMP0042; it should be ignored versions of CMake which don't
# support it. (Setting the policy explicitly will break with old CMakes.)
set (CMAKE_MACOSX_RPATH TRUE)

set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set (CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -ansi -Wno-unused-parameter")
# set (CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -flto -march=native -g -fno-omit-frame-pointer")
set (CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto -march=native -g -fno-omit-frame-pointer")
set (CMAKE_CXX_FLAGS_DEBUG "-O0 -DNDEBUG -g -fno-omit-frame-pointer")
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "release")
endif (NOT CMAKE_BUILD_TYPE)
message(STATUS "INFO Build is type ${CMAKE_BUILD_TYPE}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/buildflags.in
        ${CMAKE_BINARY_DIR}/buildflags_dnest3)

# Initialize the dependencies to an empty list.
set (DNEST_DEPS)

# ==========================================================================
#                                                                      BOOST
# ==========================================================================

set( Boost_USE_MULTITHREADED ON )
set( Boost_NO_BOOST_CMAKE 1 ) #  Bug where FindBoost replaces the system MT boot with single threaded
find_package (Boost COMPONENTS thread system iostreams)
if (Boost_FOUND)
    list (APPEND DNEST_DEPS ${Boost_LIBRARIES})
    include_directories (SYSTEM ${Boost_INCLUDES})
    message(STATUS "INFO Boost libraries found. Building DNest3 w/ libboost.")

    # try and find zlib if available
    find_package (ZLIB)
    if (ZLIB_FOUND)
        add_definitions (-DDNest3_zlib)
	message(STATUS "INFO ZLib found. Building DNest3 w/ ZLib.")
    endif (ZLIB_FOUND)
else (Boost_FOUND)
	message(STATUS "WARNING WARNING Boost libraries were NOT found. Non-boost version will be compiled. May have problems.")
    add_definitions (-DDNest3_No_Boost)
endif (Boost_FOUND)

# ==========================================================================
#                                                                        GSL
# ==========================================================================

find_package (GSL REQUIRED)
list (APPEND DNEST_DEPS ${GSL_LIBRARIES})
include_directories (SYSTEM ${GSL_INCLUDES})

# ==========================================================================
#                                                                THE LIBRARY
# ==========================================================================

include_directories (include)

set (DNEST_SRC
     src/CommandLineOptions.cpp
     src/Level.cpp
     src/LikelihoodType.cpp
     src/Model.cpp
     src/Options.cpp
     src/RandomNumberGenerator.cpp
     src/Utils.cpp)

set (DNEST_INCLUDES
     include/CommandLineOptions.h
     include/Level.h
     include/LikelihoodType.h
     include/MTSampler.h
     include/MTSamplerImpl.h
     include/Model.h
     include/Options.h
     include/RandomNumberGenerator.h
     include/Sampler.h
     include/SamplerImpl.h
     include/Start.h
     include/StartImpl.h
     include/Utils.h
)

add_library (dnest3 ${DNEST_SRC})
add_library (lib::dnest3 ALIAS dnest3)
target_link_libraries (dnest3 ${DNEST_DEPS})

install (TARGETS dnest3 DESTINATION lib)
install (FILES ${DNEST_INCLUDES} DESTINATION include/dnest3)


# DELETEME
RETURN()
# ==========================================================================
#                                                               THE EXAMPLES
# ==========================================================================

add_subdirectory (Examples)

# ==========================================================================
#                                                          THE DOCUMENTATION
# ==========================================================================

if (BUILD_MANUAL)
    add_subdirectory (doc)
endif (BUILD_MANUAL)
