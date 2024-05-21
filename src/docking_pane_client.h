#ifndef DOCKING_PANE_CLIENT_H
#define DOCKING_PANE_CLIENT_H
#include "docking_pane_container.h"
#include "global.h"
namespace ady {
    class DockingWorkbench;
    class DOCKINGPANE_EXPORT DockingPaneClient : public DockingPaneContainer {
        Q_OBJECT
    public:
        DockingPaneClient(DockingWorkbench* parent,bool init_view=true);
        virtual void initView() override;
    protected:
        virtual void focusInEvent(QFocusEvent *event) override;
        virtual void focusOutEvent(QFocusEvent *event) override;
        virtual void paintEvent(QPaintEvent *e) override;

    };
}
#endif // DOCKING_PANE_CLIENT_H
