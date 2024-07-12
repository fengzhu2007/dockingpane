#ifndef DOCKING_PANE_CONTAINER_TABBAR_H
#define DOCKING_PANE_CONTAINER_TABBAR_H
#include <QTabBar>
namespace ady {
    class DockingPaneContainerTabBarPrivate;
    class DockingPaneContainerTabBar : public QTabBar{
        Q_OBJECT
    public:
        DockingPaneContainerTabBar(QWidget* parent);

    public slots:
        void showContextMenu(const QPoint &pos);
        void onFloat(int i=-1,bool moving=false);
        void onFloatRelease();

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
