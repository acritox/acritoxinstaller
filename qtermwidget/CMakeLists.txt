# Set version number
#set(QTERMWIDGET_VERSION_MAJOR 0)
#set(QTERMWIDGET_VERSION_MINOR 1)
#set(QTERMWIDGET_VERSION_RELEASE 0)
#set(QTERMWIDGET_VERSION "${QTERMWIDGET_VERSION_MAJOR}.${QTERMWIDGET_VERSION_MINOR}.${QTERMWIDGET_VERSION_RELEASE}")

add_definitions(${QT_DEFINITIONS})
add_definitions(-DHAVE_POSIX_OPENPT)
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)

set(qtermwidget_SRCS TerminalCharacterDecoder.cpp KeyboardTranslator.cpp Screen.cpp History.cpp BlockArray.cpp konsole_wcwidth.cpp ScreenWindow.cpp Emulation.cpp Vt102Emulation.cpp TerminalDisplay.cpp Filter.cpp Pty.cpp kpty.cpp k3process.cpp k3processcontroller.cpp Session.cpp ShellCommand.cpp qtermwidget.cpp)
set(qtermwidget_MOC_HDRS ScreenWindow.h Emulation.h Vt102Emulation.h TerminalDisplay.h Filter.h Pty.h k3process.h k3processcontroller.h Session.h qtermwidget.h)


qt4_wrap_cpp(qtermwidget_MOC_SRCS ${qtermwidget_MOC_HDRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(qtermwidget SHARED ${qtermwidget_SRCS} ${qtermwidget_MOC_SRCS})
target_link_libraries(qtermwidget ${QT_LIBRARIES})
#set_target_properties(qtermwidget PROPERTIES
#    VERSION ${QTERMWIDGET_VERSION}
#    SOVERSION ${QTERMWIDGET_VERSION_MAJOR}
#    )

install(TARGETS qtermwidget RUNTIME DESTINATION "${BIN_INSTALL_DIR}" LIBRARY DESTINATION "${LIB_INSTALL_DIR}")

