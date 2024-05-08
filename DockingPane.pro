QT += widgets core

TARGET = DockingPane
TEMPLATE = lib

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


DEFINES += QT_DEPRECATED_WARNINGS DOCKINGPANE_LIBRARY

HEADERS += \
    docking_guide.h \
    docking_guide_cover.h \
    docking_guide_diamond.h \
    docking_pane.h \
    docking_pane_client.h \
    docking_pane_container.h \
    docking_pane_container_nclient.h \
    docking_pane_container_tabbar.h \
    docking_pane_fixed_window.h \
    docking_pane_float_window.h \
    docking_pane_handle.h \
    docking_pane_layout.h \
    docking_pane_layout_item_info.h \
    docking_pane_manager.h \
    docking_pane_tabbar.h \
    docking_pane_tabbar_item.h \
    docking_pane_tabbar_item_1.h \
    docking_pane_window_resizer.h \
    docking_workbench.h \
    global.h

SOURCES += \
    docking_guide.cpp \
    docking_guide_cover.cpp \
    docking_guide_diamond.cpp \
    docking_pane.cpp \
    docking_pane_client.cpp \
    docking_pane_container.cpp \
    docking_pane_container_nclient.cpp \
    docking_pane_container_tabbar.cpp \
    docking_pane_fixed_window.cpp \
    docking_pane_float_window.cpp \
    docking_pane_handle.cpp \
    docking_pane_layout.cpp \
    docking_pane_layout_item_info.cpp \
    docking_pane_manager.cpp \
    docking_pane_tabbar.cpp \
    docking_pane_tabbar_item.cpp \
    docking_pane_tabbar_item_1.cpp \
    docking_pane_window_resizer.cpp \
    docking_workbench.cpp

FORMS += \
    docking_guide.ui \
    docking_pane_container_nclient.ui

RESOURCES += \
    res.qrc
