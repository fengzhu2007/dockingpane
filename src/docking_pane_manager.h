#ifndef DOCKING_PANE_MANAGER_H
#define DOCKING_PANE_MANAGER_H
#include "global.h"
#include <QObject>
#include <QWidget>
namespace ady {
    class DockingWorkbench;
    class DockingPane;
    class DockingPaneContainer;
    class DockingPaneManagerPrivate;
    class DOCKINGPANE_EXPORT DockingPaneManager :public QObject{
    public:
        enum Position {
            S_Left = 0,
            S_Top,
            S_Right,
            S_Bottom,
            Left,
            Top,
            Right,
            Bottom,
            Center
        };
        DockingPaneManager(QWidget* parent);
        ~DockingPaneManager();
        DockingWorkbench* workbench();
        QWidget* widget();
        void createPane(DockingPane* pane,Position position);
        void createPane(DockingPane* pane,DockingPaneContainer* container,Position position);

        DockingPane* createPane(QString id,QString group,QString title,QWidget* widget,Position position);
        DockingPane* createPane(QString id,QString group,QString title,QWidget* widget,DockingPaneContainer* target,Position position);
        //void addItem(QWidget* widget,Position position);

        void dump();

    private:
        DockingPaneManagerPrivate* d;

    };
}
#endif // DOCKING_PANE_MANAGER_H
