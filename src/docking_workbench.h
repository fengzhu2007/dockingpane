#ifndef DOCKING_WORKBENCH_H
#define DOCKING_WORKBENCH_H
#include <QWidget>
#include <QFrame>
#include "global.h"
namespace ady {
    class DockingWorkbenchPrivate;
    class DockingPaneTabBar;
    class DockingPaneClient;
    class DockingPaneManager;
    class DockingPaneContainer;
    class DockingPaneFloatWindow;
    class DockingPaneFixedWindow;
    class DockingPane;
    class DOCKINGPANE_EXPORT DockingWorkbench : public QFrame {
        Q_OBJECT
    public:
        DockingWorkbench(QWidget* parent,DockingPaneManager* manager);
        ~DockingWorkbench();
        void initClient();

        void showGuide(DockingPaneContainer* widget,QRect rect);
        void showSiderGuide();
        void hideGuide();

        void hideSiderGuide();
        int activeGuide(const QPoint& pos );
        int activeSiderGuide(const QPoint& pos);
        void startLookup();
        void endLookup();
        DockingPaneContainer* lookup(const QPoint& pos,QRect& rect,bool &guide_visibility,int* tab);

        void showGuideCover(DockingPaneContainer* container,int position,const QRect& rect,int tab=-1);
        void hideGuideCover();

        void lockContainer(DockingPaneFloatWindow* window,DockingPaneContainer* container,int position,int tab=-1);
        void restoreWidget(DockingPaneContainer* widget,int position);
        void restorePane(DockingPane* pane,int position);

        DockingPaneTabBar* tabBar(int position);
        QSize tabBarSize(int position);
        void siderFixed(DockingPaneContainer* container,int position);

        void updateTabBars(const QSize& size);

        QList<DockingPaneContainer*> containers();
        int clientCount();
        DockingPaneClient* client(int index=0);

        DockingPaneManager* manager();

        void showFixedWindow(DockingPaneContainer* container,int position);
        void resizeFixedWindow(const QSize& size);
        void hideFixedWindow();
        void unActiveAll();
        void updateLayout();
        void dump(QString prefix);


        //void updateGuidePosition();

    signals:
        void beforePaneClose(DockingPane* pane,bool isClient,bool* ret);
        void paneClosed(QString& id,QString&group,bool isClient);
        //void paneCloseRequested(DockingPane* pane,bool isClient);
        void paneCurrentChanged(int index,DockingPane* pane);





    protected:
        virtual void resizeEvent(QResizeEvent *e) override;
        virtual void paintEvent(QPaintEvent *e) override;
        virtual void mousePressEvent(QMouseEvent *e)override;
        virtual void showEvent(QShowEvent* e) override;

    private:
        DockingWorkbenchPrivate* d;

    friend class DockingPaneManager;

    };
}
#endif // DOCKING_WORKBENCH_H
