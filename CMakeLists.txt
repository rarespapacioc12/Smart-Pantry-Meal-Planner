cmake_minimum_required(VERSION 3.26)

set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")


project(${MAIN_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

###############################################################################

# Tema 3 - dependinte externe descarcate cu FetchContent:
#   * nlohmann/json - stocare/citire JSON (config, preturi, export meal plan)
#   * spdlog        - logging (folosit de Singleton-ul Logger)
#   * cxxopts       - argumente in linia de comanda (folosit in main)
include(FetchContent)

set(FETCHCONTENT_QUIET OFF CACHE INTERNAL "Override verbose output" FORCE)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE INTERNAL "Override update strategy" FORCE)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3   # tag stabil @ 2023-11
    GIT_SHALLOW    1
    GIT_PROGRESS   ON
)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.14.1   # tag stabil @ 2024-04
    GIT_SHALLOW    1
    GIT_PROGRESS   ON
)
FetchContent_Declare(
    cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG        v3.2.0    # tag stabil @ 2023-09
    GIT_SHALLOW    1
    GIT_PROGRESS   ON
)

FetchContent_MakeAvailable(nlohmann_json spdlog cxxopts)

###############################################################################

# external dependencies with find_package

# find_package(Threads REQUIRED)

###############################################################################

# NOTE: update executable name in .github/workflows/cmake.yml:25 when changing name here
add_executable(${MAIN_EXECUTABLE_NAME}
    main.cpp
)
target_sources(${MAIN_EXECUTABLE_NAME}
    PRIVATE
        src/core/Ingredient.cpp
        src/core/Inventory.cpp
        src/core/Recipe.cpp
        src/core/RecipeBook.cpp
        src/core/RecipeLine.cpp
        src/core/ShoppingItem.cpp
        src/core/StandardShoppingItem.cpp
        src/core/PromotedShoppingItem.cpp
        src/core/BulkShoppingItem.cpp
        src/core/OrganicShoppingItem.cpp
        src/core/SeasonalShoppingItem.cpp
        src/core/ShoppingList.cpp
        src/core/StockItem.cpp
        src/core/Types.cpp
        src/core/PriceCatalog.cpp
        src/core/MealPlanner.cpp
        src/config/AppConfig.cpp
        src/factory/ShoppingItemFactory.cpp
        src/utils/Logger.cpp
        src/exceptions/AppError.cpp
        src/exceptions/InvalidQuantityError.cpp
        src/exceptions/ItemNotFoundError.cpp
        src/exceptions/PricingCalculationError.cpp
        src/exceptions/JsonParseError.cpp
        src/io/JsonValue.cpp
        src/io/JsonReader.cpp
        src/io/JsonWriter.cpp
        src/io/DomainJson.cpp
        src/utils/Date.cpp
        src/utils/StringUtil.cpp
)

# NOTE: Add all defined targets (e.g. executables, libraries, etc. )
# NOTE: RUN_SANITIZERS is optional, if it's not present it will default to true
set_compiler_flags(RUN_SANITIZERS ${USE_ASAN} TARGET_NAMES ${MAIN_EXECUTABLE_NAME})
# set_compiler_flags(TARGET_NAMES ${MAIN_EXECUTABLE_NAME} ${FOO} ${BAR})
# where ${FOO} and ${BAR} represent additional executables or libraries
# you want to compile with the set compiler flags

###############################################################################

# use SYSTEM so cppcheck and clang-tidy do not report warnings from these directories
target_include_directories(${MAIN_EXECUTABLE_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE
    ext
)

# Tema 3 - link-uirea bibliotecilor externe (target-urile aduc include-urile ca SYSTEM,
# deci cppcheck/clang-tidy nu raporteaza warning-uri din ele).
target_link_libraries(${MAIN_EXECUTABLE_NAME}
    PRIVATE
        nlohmann_json::nlohmann_json
        spdlog::spdlog
        cxxopts::cxxopts
)

###############################################################################

# copy binaries to "bin" folder; these are uploaded as artifacts on each release
# DESTINATION_DIR is set as "bin" in cmake/Options.cmake:6
install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()

copy_files(FILES tastatura.txt DIRECTORY assets data COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
# copy_files(FILES tastatura.txt config.json DIRECTORY images sounds COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
# copy_files(DIRECTORY images sounds COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
