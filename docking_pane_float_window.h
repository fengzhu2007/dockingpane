#ifndef DOCKING_PANE_FLOAT_WINDOW_H
#define DOCKING_PANE_FLOAT_WINDOW_H
#include "global.h"
#include <QWidget>
namespace ady {
    class DockingPane;
    class DockingPaneContainer;
    class DockingPaneWindowResizer;
    class DockingPaneFloatWindowPrivate;
    class DockingPaneFloatWindow : public QWidget{
        Q_OBJECT
    public:
        DockingPaneFloatWindow(QWidget* parent,int margin);
        ~DockingPaneFloatWindow();
        void setCenterWidget(DockingPaneContainer* container);
        void setCenterWidget(DockingPane* pane);
        DockingPaneContainer* centerWidget();
        void updateResizer();
    protected:
        virtual void resizeEvent(QResizeEvent *event) override;
    private:
        void initResizer();

    private:
        DockingPaneFloatWindowPrivate* d;

    };
}
#endif // DOCKING_PANE_FLOAT_WINDOW_H
