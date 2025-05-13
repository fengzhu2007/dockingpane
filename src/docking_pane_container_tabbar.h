#ifndef DOCKING_PANE_CONTAINER_TABBAR_H
#define DOCKING_PANE_CONTAINER_TABBAR_H
#include <QTabBar>
#include <QRect>
#include <QMouseEvent>
#include <QDebug>
class QDropEvent;
namespace ady {
typedef void (*DropFunc) (QDropEvent*);
    class DockingPaneContainerTabBarPrivate;
    class DockingPaneContainerTabBar : public QTabBar{
        Q_OBJECT
    public:
        DockingPaneContainerTabBar(QWidget* parent);
        void setDropCallback(std::function<void(QDropEvent*)> func);
        std::function<void(QDropEvent*)> dropCallback();
        void setState(int state);


    public slots:
        void showContextMenu(const QPoint &pos);
        void onFloat(int i=-1,bool moving=false);
        void onFloatRelease();

    protected:

        virtual void mousePressEvent(QMouseEvent *e) override;
        virtual void mouseMoveEvent(QMouseEvent *e) override;
        virtual void mouseReleaseEvent(QMouseEvent *e) override;
        virtual void paintEvent(QPaintEvent* event) override;
        virtual void dragEnterEvent(QDragEnterEvent *event) override;
        virtual void dragMoveEvent(QDragMoveEvent *event) override;
        virtual void dropEvent(QDropEvent *event) override;

    private:
        DockingPaneContainerTabBarPrivate* d;
    };
}
#endif // DOCKING_PANE_CONTAINER_TABBAR_H
