#ifndef DOCKING_PANE_CLIENT_REGION_H
#define DOCKING_PANE_CLIENT_REGION_H
#include "docking_pane_container.h"
#include "global.h"
namespace ady {
class DockingWorkbench;
class DOCKINGPANE_EXPORT DockingPaneClientRegion : public DockingPaneContainer {
    Q_OBJECT
public:
    DockingPaneClientRegion(DockingWorkbench* parent);
    virtual bool isClientRegion() override;
protected:
    virtual void paintEvent(QPaintEvent *e) override;

};
}

#endif // DOCKING_PANE_CLIENT_REGION_H
