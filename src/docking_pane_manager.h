#ifndef DOCKING_PANE_MANAGER_H
#define DOCKING_PANE_MANAGER_H
#include "global.h"
#include <QObject>
#include <QWidget>
#include <QJsonObject>



namespace ady {


    class DockingPaneManager;
    class DockingWorkbench;
    class DockingPane;
    class DockingPaneLayout;
    class DockingPaneFloatWindow;
    class DockingPaneContainer;
    class DockingPaneLayoutItemInfo;
    class DockingPaneManagerPrivate;

    typedef DockingPane* (*InitPaneFuncPtr)(DockingPaneManager* ,const QString&,const QJsonObject&);

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
        void initClient();
        DockingWorkbench* workbench();
        QWidget* widget();
        DockingPaneLayoutItemInfo* createPane(DockingPane* pane,Position position,bool active=false);
        DockingPaneLayoutItemInfo* createPane(DockingPane* pane,DockingPaneContainer* target,Position position);
        DockingPane* createPane(QString id,QString group,QString title,QWidget* widget,Position position);
        DockingPane* createPane(QString id,QString group,QString title,QWidget* widget,DockingPaneContainer* target,Position position);


        DockingPaneLayoutItemInfo* createPane(DockingPane* pane,DockingPaneLayoutItemInfo* parent,Position position);
        DockingPaneLayoutItemInfo* restorePane(DockingPane* pane,Position position,DockingPaneLayoutItemInfo* parent,DockingPaneLayoutItemInfo* previous=nullptr);


        DockingPane* createFixedPane(const QString& id,const QString& group,const QString& title,QWidget* widget,Position position);
        DockingPaneContainer* createFixedPane(DockingPane* pane,Position position);
        void createFixedPane(DockingPaneContainer* container,Position position);

        DockingPaneFloatWindow* createFloatPane(const QString& id,const QString& group,const QString& title,QWidget* widget);
        //DockingPaneContainer* createFloatPane(DockingPane* pane,const QRect rect);
        //DockingPaneContainer* createFloatPane(DockingPane* pane,int x,int y,int w,int h);
        DockingPaneFloatWindow* createFloatPane(DockingPaneContainer* container,const QRect& rect);


        DockingPaneLayout* layout();
        //void addItem(QWidget* widget,Position position);
        QJsonObject toJson();
        QJsonArray toJsonOne(DockingPaneLayoutItemInfo* layouItem);

        void restore(QJsonObject dockpanes,InitPaneFuncPtr func);
        bool close();



        void dump();

    private:
        void restoreContainers(QJsonArray& list,int orientation,DockingPaneLayoutItemInfo* parent,InitPaneFuncPtr func);
        int restoreTabs(QJsonArray& list,DockingPaneLayoutItemInfo* info,InitPaneFuncPtr func);


    private:
        DockingPaneManagerPrivate* d;

    };
}
#endif // DOCKING_PANE_MANAGER_H
