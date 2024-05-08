#ifndef DOCKING_GUIDE_H
#define DOCKING_GUIDE_H
#include <QFrame>
#include "global.h"
namespace Ui {
    class DockingPaneGuide;
}
namespace ady {
    class DockingWorkbench;
    class DockingGuideDiamond;
    class DOCKINGPANE_EXPORT DockingGuide : public QFrame
    {
        Q_OBJECT
    public:
        DockingGuide(DockingWorkbench* parent);
        ~DockingGuide();
        DockingGuideDiamond* updateActive(const QPoint& pos);
    private:
        Ui::DockingPaneGuide* ui;


    };
}
#endif // DOCKING_GUIDE_H
