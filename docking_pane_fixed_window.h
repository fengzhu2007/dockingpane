#ifndef DOCKING_PANE_FIXED_WINDOW_H
#define DOCKING_PANE_FIXED_WINDOW_H
#include "global.h"
#include <QWidget>
namespace ady {
    class DockingPane;
    class DockingPaneContainer;
    class DockingPaneWindowResizer;
    class DockingPaneFixedWindowPrivate;
    class DockingPaneFixedWindow : public QWidget{
        Q_OBJECT
    public:
        DockingPaneFixedWindow(QWidget* parent,int margin);
        ~DockingPaneFixedWindow();
        void setCenterWidget(DockingPaneContainer* container);
        DockingPaneContainer* centerWidget();

        void updateResizer();

        void setFixedPosition(int position);
        int fixedPosition();

    protected:
        virtual void resizeEvent(QResizeEvent *event) override;
        virtual void changeEvent(QEvent *event)override;

    private:
        void initResizer();

    private:
        DockingPaneFixedWindowPrivate* d;

    };
}
#endif // DOCKING_PANE_FIXED_WINDOW_H
