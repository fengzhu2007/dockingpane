#ifndef DOCKING_PANE_CONTAINER_H
#define DOCKING_PANE_CONTAINER_H
#include "docking_pane_manager.h"
#include "global.h"
#include <QStackedWidget>
namespace ady{
    class DockingPane;
    class DockingPaneContainerTabBar;
    class DockingPaneLayoutItemInfo;
    class DockingWorkbench;
    class DockingPaneContainerPrivate;

    class DOCKINGPANE_EXPORT DockingPaneContainer : public QWidget {
        Q_OBJECT
        //Q_PROPERTY(bool activeState READ activeState WRITE setActiveState)
    public:
        enum State{
            Inner,
            Fixed,
            Float
        };
        DockingPaneContainer(QWidget* parent,DockingPaneManager::Position position=DockingPaneManager::S_Left);
        virtual ~DockingPaneContainer();


        virtual void initView();
        bool isClient();
        virtual bool isClientRegion();
        void appendPane(DockingPane* pane,bool active=false);
        void insertPane(int index,DockingPane* pane,bool active=false);
        void setPane(int index);
        void setPane(DockingPane* pane);

        void setState(State state);
        State state();
        bool activeState();
        void activeWidget(bool active);

        void setItemInfo(DockingPaneLayoutItemInfo* info);
        DockingPaneLayoutItemInfo* itemInfo();

        int paneCount();
        int current();
        DockingPane* pane(int i);
        DockingPane* currentPane();
        DockingPane* takeAt(int i);
        DockingPane* takeCurrent();
        int indexOf(DockingPane* pane);

        DockingPaneContainerTabBar* tabBar();
        QStackedWidget* stacked();
        bool closeCurrent();
        bool closePane(DockingPane* pane,bool force=false);
        bool closePane(int i,bool force=false);
        void fixedPane(int i);
        void floatPane(int i);

        void visibleTabBar(bool visible);
        void setOriPosition(DockingPaneManager::Position position);
        DockingPaneManager::Position oriPosition();
        void setOriRect(QRect rc);
        void setOriRect(int w,int h);
        QRect oriRect();
        void setMoving(bool state);
        int guideFlags();
        float stretch();

        DockingWorkbench* workbench();




    public slots:
        void onCurrentChanged(int i);
        void onWidgetRemoved(int i);


    protected:
        DockingPaneContainer(QWidget* parent,bool client);
        DockingPaneContainer(QWidget* parent,bool client,bool init_view);
        virtual void focusInEvent(QFocusEvent *event) override;
        //virtual void focusOutEvent(QFocusEvent *event) override;
        virtual void paintEvent(QPaintEvent *e) override;


    protected:
        DockingPaneContainerPrivate* d;


    friend class DockingPaneManager;


    };
}


#endif // DOCKING_PANE_CONTAINER_H
