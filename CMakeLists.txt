cmake_minimum_required(VERSION 3.10)
project(AIGameEnvironment)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# Source files
set(SOURCES
    main.cpp
    PenaltyScoreboard.cpp
    Challenge.cpp
    ConcreteChallenges.cpp
    AIAgent.cpp
    Game.cpp
)

# Header files (for IDE organization)
set(HEADERS
    PenaltyScoreboard.h
    Challenge.h
    ConcreteChallenges.h
    AIAgent.h
    Game.h
)

# Create executable
add_executable(ai_game_env ${SOURCES} ${HEADERS})

# Optional: Add compiler warnings
if(MSVC)
    target_compile_options(ai_game_env PRIVATE /W4)
else()
    target_compile_options(ai_game_env PRIVATE -Wall -Wextra -pedantic)
endif()

# Set output directory
set_target_properties(ai_game_env PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Print project info
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
