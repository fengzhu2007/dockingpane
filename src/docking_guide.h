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
    class DockingGuidePrivate;
    class DOCKINGPANE_EXPORT DockingGuide : public QFrame
    {
        Q_OBJECT
    public:
        enum SizeMode{
            Normal=1,
            Large=2,
            Left=4,
            Top=8,
            Right=16,
            Bottom=32,
            C_Left=64,
            C_Top=128,
            C_Right=256,
            C_Bottom=512,
            Center=1024,

            //neighbor
            C_Parent_Horizontal=2048,
            C_Parent_Vertical = 4096

        };


        DockingGuide(DockingWorkbench* parent);
        ~DockingGuide();
        DockingGuideDiamond* updateActive(const QPoint& pos);
        void setSizeMode(int mode);
        int sizeMode();

    private:
        Ui::DockingPaneGuide* ui;
        DockingGuidePrivate* d;


    };
}
#endif // DOCKING_GUIDE_H
