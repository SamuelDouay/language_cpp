cmake_minimum_required(VERSION 3.20)
project(language_cpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# version Catch:3.15.3
add_subdirectory(externals/Catch2)

add_library(project_warnings INTERFACE)
target_compile_options(project_warnings INTERFACE -Wall -Wextra -Wpedantic -Werror)

# Macro pour créer un exécutable en scannant automatiquement les fichiers
function(add_game TARGET_NAME SOURCE_DIR)
    # Récupère TOUS les fichiers .cpp dans le dossier (y compris sous-dossiers)
    file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/src/*.cpp)
    # Récupère TOUS les fichiers .hpp dans le sous-dossier include
    file(GLOB_RECURSE HEADERS ${SOURCE_DIR}/include/*.hpp)

    # Crée l'exécutable avec les sources ET les headers (pour les afficher dans CLion)
    add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS})

    # Ajoute le dossier include au chemin de recherche des headers
    target_include_directories(${TARGET_NAME} PRIVATE ${SOURCE_DIR}/include)

    # Lie les warnings communs
    target_link_libraries(${TARGET_NAME} PRIVATE project_warnings)

    # (Optionnel) Si tu utilises <print> ou stdc++exp, décommente :
    target_link_libraries(${TARGET_NAME} PRIVATE stdc++exp)

    # Affiche un message dans la console CMake pour vérification
    message(STATUS "${TARGET_NAME} compilé avec : ${SOURCES}")
endfunction()

add_game(minesweeper minesweeper)
add_game(sudoku sudoku)

add_executable(tests tests/example/test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)