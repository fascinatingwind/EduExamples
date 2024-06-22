cmake_minimum_required(VERSION 3.20)

include(../global.cmake)
include(../qtglobal.cmake)

project(PlotExample)

include_directories(include)
link_directories(src)

set(CMAKE_AUTOUIC_SEARCH_PATHS "ui")

set(HEADER_FILES ${HEADER_FILES}
		"include/ChartModel.h"
		"include/ChartRecord.h"
		"include/ChartWindow.h"
)

set(SOURCE_FILES ${SOURCE_FILES}
		"src/ChartModel.cpp"
		"src/ChartRecord.cpp"
		"src/ChartWindow.cpp"
		"main.cpp"
)

qt_add_resources(SOURCE_FILES Resource.qrc)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
	Qt${QT_VERSION_MAJOR}::Core
	Qt${QT_VERSION_MAJOR}::Gui
	Qt${QT_VERSION_MAJOR}::Widgets
	Qt${QT_VERSION_MAJOR}::Charts
)

if(WIN32)
add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD COMMAND
	"${CMAKE_COMMAND}" -E env PATH="${QT_BIN_DIR}" "${PLATFORM_DEPLOYQT_EXECUTABLE}" "$<TARGET_FILE:PlotExample>" "--no-compiler-runtime"
	COMMENT "Running windeployqt..."
)
endif()
