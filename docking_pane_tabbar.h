#ifndef DOCKING_PANE_TABBAR_H
#define DOCKING_PANE_TABBAR_H
#include "docking_pane_manager.h"
#include <QTabBar>
namespace ady{
    class DockingPaneContainer;
    class DockingPaneTabBarPrivate;
    class DockingPaneTabBar : public QWidget {
        Q_OBJECT
    public:
        enum Shape { RoundedNorth, RoundedSouth, RoundedWest, RoundedEast
        };
        DockingPaneTabBar(QWidget* parent);
        ~DockingPaneTabBar();
        DockingPaneManager::Position position();
        void setPosition(DockingPaneManager::Position position);
        void addContainer(DockingPaneContainer* container);
        void removeContainer(DockingPaneContainer* container);
        void removeContainerChild(DockingPaneContainer* container,int index);
        void setShape(Shape shape);
        void addTab(const QString& title);
        void removeTab(int i);

        //QSize sizeHint();


    signals:
        void currentChanged(int i);



    public slots:
        void onCurrentChanged(int i);
        void onItemClicked();

    private:
        DockingPaneTabBarPrivate* d;

    };

}
#endif // DOCKING_PANE_TABBAR_H
