#ifndef DOCKING_PANE_CONTAINER_H
#define DOCKING_PANE_CONTAINER_H
#include "docking_pane_manager.h"
#include "global.h"
#include <QStackedWidget>
namespace ady{
    class DockingPane;
    class DockingPaneContainerTabBar;
    class DockingPaneContainerPrivate;
    class DockingPaneLayoutItemInfo;
    class DOCKINGPANE_EXPORT DockingPaneContainer : public QWidget {
        Q_OBJECT
    public:
        enum State{
            Inner,
            Fixed,
            Float
        };
        DockingPaneContainer(QWidget* parent,DockingPaneManager::Position position=DockingPaneManager::S_Left);
        ~DockingPaneContainer();


        bool isClient();
        void appendPane(DockingPane* pane);
        void insertPane(int index,DockingPane* pane);
        void setPane(int index);

        void setState(State state);
        State state();
        void activeWidget(bool active);

        void setItemInfo(DockingPaneLayoutItemInfo* info);
        DockingPaneLayoutItemInfo* itemInfo();

        int paneCount();
        int current();
        DockingPane* pane(int i);
        DockingPane* takeAt(int i);
        DockingPane* takeCurrent();

        DockingPaneContainerTabBar* tabBar();
        QStackedWidget* stacked();
        void closeCurrent();

        void visibleTabBar(bool visible);
        void setOriPosition(DockingPaneManager::Position position);
        DockingPaneManager::Position oriPosition();
        void setOriRect(QRect rc);
        void setOriRect(int w,int h);
        void setMoving(bool state);


    public slots:
        void onCurrentChanged(int i);
        void onWidgetRemoved(int i);


    protected:
        DockingPaneContainer(QWidget* parent,bool client);
        virtual void focusInEvent(QFocusEvent *event) override;
        //virtual void focusOutEvent(QFocusEvent *event) override;
        virtual void paintEvent(QPaintEvent *e) override;


    protected:
        DockingPaneContainerPrivate* d;


    friend class DockingPaneManager;


    };
}


#endif // DOCKING_PANE_CONTAINER_H
