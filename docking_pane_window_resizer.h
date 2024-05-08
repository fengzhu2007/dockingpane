#ifndef DOCKING_PANE_WINDOW_RESIZER_H
#define DOCKING_PANE_WINDOW_RESIZER_H
#include <QFrame>
namespace ady {
    class DockingPaneWindowResizerPrivate;
    class DockingPaneWindowResizer : public QFrame{
        Q_OBJECT
    public:
        enum Region{
            Left=0,
            Top,
            Right,
            Bottom,
            LeftTop,
            LeftBottom,
            RightTop,
            RightBottom
        };
        DockingPaneWindowResizer(QWidget* parent,Region region);
        ~DockingPaneWindowResizer();

        void setRegion(Region region);
        Region region();

    protected:
        virtual void mousePressEvent(QMouseEvent *e) override;
        virtual void mouseMoveEvent(QMouseEvent *e) override;
        virtual void mouseReleaseEvent(QMouseEvent *e) override;

    private:
        DockingPaneWindowResizerPrivate* d;


    };
}
#endif // DOCKING_PANE_WINDOW_RESIZER_H
