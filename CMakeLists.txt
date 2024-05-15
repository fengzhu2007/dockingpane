cmake_minimum_required(VERSION 3.14)

project(dockingpane)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core)


message("dirss :${AUTOGEN_BUILD_DIR}")
set(RESOURCES
    res.qrc
)

qt5_add_resources(RESOURCE_ADDED ${RESOURCES})


add_library(dockingpane SHARED
    ${RESOURCE_ADDED}
    docking_guide.h
    docking_guide_cover.h
    docking_guide_diamond.h
    docking_pane.h
    docking_pane_client.h
    docking_pane_container.h
    docking_pane_container_nclient.h
    docking_pane_container_tabbar.h
    docking_pane_fixed_window.h
    docking_pane_float_window.h
    docking_pane_handle.h
    docking_pane_layout.h
    docking_pane_layout_item_info.h
    docking_pane_manager.h
    docking_pane_tabbar.h
    docking_pane_tabbar_item.h
    docking_pane_tabbar_item_1.h
    docking_pane_window_resizer.h
    docking_workbench.h
    global.h
    docking_guide.cpp
    docking_guide_cover.cpp
    docking_guide_diamond.cpp
    docking_pane.cpp
    docking_pane_client.cpp
    docking_pane_container.cpp
    docking_pane_container_nclient.cpp
    docking_pane_container_tabbar.cpp
    docking_pane_fixed_window.cpp
    docking_pane_float_window.cpp
    docking_pane_handle.cpp
    docking_pane_layout.cpp
    docking_pane_layout_item_info.cpp
    docking_pane_manager.cpp
    docking_pane_tabbar.cpp
    docking_pane_tabbar_item.cpp
    docking_pane_tabbar_item_1.cpp
    docking_pane_window_resizer.cpp
    docking_workbench.cpp
    docking_guide.ui
    docking_pane_container_nclient.ui


)

target_link_libraries(dockingpane PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(dockingpane PRIVATE DOCKINGPANE_LIBRARY)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy  ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}.dll        ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}.dll
    COMMENT "Copying file to binary directory"
)

