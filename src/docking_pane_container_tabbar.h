#ifndef DOCKING_PANE_CONTAINER_TABBAR_H
#define DOCKING_PANE_CONTAINER_TABBAR_H
#include <QTabBar>
namespace ady {
    class DockingPaneContainerTabBarPrivate;
    class DockingPaneContainerTabBar : public QTabBar{
        Q_OBJECT
    public:
        DockingPaneContainerTabBar(QWidget* parent);

    protected:
        virtual void mousePressEvent(QMouseEvent *e) override;
        virtual void mouseMoveEvent(QMouseEvent *e) override;
        virtual void mouseReleaseEvent(QMouseEvent *e) override;
        virtual void paintEvent(QPaintEvent* event) override;
    private:
        DockingPaneContainerTabBarPrivate* d;
    };
}
#endif // DOCKING_PANE_CONTAINER_TABBAR_H
