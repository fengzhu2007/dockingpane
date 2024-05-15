#ifndef DOCKING_GUIDE_DIAMOND_H
#define DOCKING_GUIDE_DIAMOND_H
#include "global.h"
#include "docking_pane_manager.h"

#include <QLabel>
namespace ady {
    class DockingGuideDiamondPrivate;
    class DOCKINGPANE_EXPORT DockingGuideDiamond : public QLabel {
        Q_OBJECT;
    public:
        DockingGuideDiamond(QWidget* parent);
        DockingGuideDiamond(DockingPaneManager::Position position,QWidget* parent);
        ~DockingGuideDiamond();

        DockingPaneManager::Position position();
        void setPosition(DockingPaneManager::Position position);

        bool active();
        void setActive(bool state);

        bool updateActive(const QPoint& pos);


    private:
        DockingGuideDiamondPrivate* d;

    };



}
#endif // DOCKING_GUIDE_DIAMOND_H
