project(acritoxinstaller)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

if(${CMAKE_BUILD_TYPE} MATCHES "Release")
  add_definitions(-DNDEBUG)
  add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif(${CMAKE_BUILD_TYPE} MATCHES "Release")

set(LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)")
set(EXEC_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX} CACHE PATH "Installation prefix for executables and object code libraries" FORCE)
set(BIN_INSTALL_DIR ${EXEC_INSTALL_PREFIX}/bin CACHE PATH "Installation prefix for user executables" FORCE)
set(LIB_INSTALL_DIR ${EXEC_INSTALL_PREFIX}/lib${LIB_SUFFIX} CACHE PATH  "Installation prefix for object code libraries" FORCE)
set(DATA_INSTALL_DIR ${EXEC_INSTALL_PREFIX}/share/acritoxinstaller CACHE PATH  "Installation prefix where shared data will be installed" FORCE)
set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include CACHE PATH "Installation prefix for C header files" FORCE)

set(BACKEND_DIR ${DATA_INSTALL_DIR})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/backend/backend.sh.in ${CMAKE_CURRENT_BINARY_DIR}/backend.sh @ONLY)
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/backend.sh DESTINATION "${BACKEND_DIR}")
install(DIRECTORY backend/modules DESTINATION "${DATA_INSTALL_DIR}")

add_subdirectory( qtermwidget )

file(GLOB wizard_SRCS wizard/[a-z]*.cpp)
file(GLOB wizard_HDRS wizard/[a-z]*.h)
file(GLOB wizard_UIS  wizard/[a-z]*.ui)

configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
set(acritoxinstaller_SRCS mainwizard.cpp lxdesupport.cpp main.cpp backend.cpp listitem.cpp listdelegate.cpp busyappfilter.cpp ${wizard_SRCS})
set(acritoxinstaller_MOC_HDRS mainwizard.h lxdesupport.cpp backend.h listdelegate.h ${wizard_HDRS})
set(acritoxinstaller_UIS ${wizard_UIS})

set(FILES_TO_TRANSLATE ${acritoxinstaller_SRCS} ${acritoxinstaller_MOC_HDRS} ${acritoxinstaller_UIS})
file (GLOB TRANSLATIONS_FILES translations/*.ts)

qt4_wrap_cpp(acritoxinstaller_MOC_SRCS ${acritoxinstaller_MOC_HDRS})
qt4_wrap_ui(acritoxinstaller_UIS_H ${acritoxinstaller_UIS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(acritoxinstaller ${acritoxinstaller_SRCS} ${acritoxinstaller_MOC_SRCS} ${acritoxinstaller_UIS_H})
target_link_libraries(acritoxinstaller ${QT_LIBRARIES} qtermwidget crypt)

option (UPDATE_TRANSLATIONS "Update source translation translations/*.ts
files (WARNING: make clean will delete the source .ts files! Danger!)")
if (UPDATE_TRANSLATIONS)
  qt4_create_translation(QM_FILES ${FILES_TO_TRANSLATE}
${TRANSLATIONS_FILES})
else (UPDATE_TRANSLATIONS)
  qt4_add_translation(QM_FILES ${TRANSLATIONS_FILES})
endif (UPDATE_TRANSLATIONS) 
add_custom_target (translations_target DEPENDS ${QM_FILES})
install(FILES ${QM_FILES} DESTINATION ${QT_TRANSLATIONS_DIR})
add_dependencies(acritoxinstaller translations_target)

install(TARGETS acritoxinstaller RUNTIME DESTINATION "${BIN_INSTALL_DIR}" LIBRARY DESTINATION "${LIB_INSTALL_DIR}")

