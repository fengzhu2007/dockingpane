#ifndef DOCKING_PANE_MANAGER_H
#define DOCKING_PANE_MANAGER_H
#include "global.h"
#include <QObject>
#include <QWidget>
namespace ady {
    class DockingWorkbench;
    class DockingPane;
    class DockingPaneFloatWindow;
    class DockingPaneContainer;
    class DockingPaneLayoutItemInfo;
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
            Center,
            C_Left,
            C_Top,
            C_Right,
            C_Bottom
        };
        DockingPaneManager(QWidget* parent);
        ~DockingPaneManager();
        DockingWorkbench* workbench();
        QWidget* widget();
        DockingPaneLayoutItemInfo* createPane(DockingPane* pane,Position position,bool active=false);
        void createPane(DockingPane* pane,DockingPaneContainer* container,Position position);

        DockingPane* createPane(QString id,QString group,QString title,QWidget* widget,Position position);
        DockingPane* createPane(QString id,QString group,QString title,QWidget* widget,DockingPaneContainer* target,Position position);
        DockingPane* createFixedPane(const QString& id,const QString& group,const QString& title,QWidget* widget,Position position);
        DockingPaneFloatWindow* createFloatPane(const QString& id,const QString& group,const QString& title,QWidget* widget);
        //void addItem(QWidget* widget,Position position);

        void dump();


    private:
        DockingPaneManagerPrivate* d;

    };
}
#endif // DOCKING_PANE_MANAGER_H
