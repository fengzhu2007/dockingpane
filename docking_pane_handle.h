#ifndef DOCKING_PANE_HANDLE_H
#define DOCKING_PANE_HANDLE_H
#include "global.h"
#include <QFrame>
namespace ady {
    class DockingPaneLayoutItemInfo;
    class DOCKINGPANE_EXPORT DockingPaneHandle : public QFrame{
        Q_OBJECT
    public:
        enum Orientation {
            Unkown,
            Horizontal,
            Vertical
        };
        DockingPaneHandle(QWidget* parent,DockingPaneLayoutItemInfo*itemInfo);

        void setOrientation(Orientation orient);
        Orientation orientation();

        inline DockingPaneLayoutItemInfo* itemInfo(){return m_itemInfo;};


    protected:
        virtual void mouseMoveEvent(QMouseEvent *event) override;
        virtual void mousePressEvent(QMouseEvent *event) override;
        virtual void mouseReleaseEvent(QMouseEvent *event) override;



    private:
        DockingPaneLayoutItemInfo* m_itemInfo;
        Orientation m_ori;
        bool m_start_moving = false;
        int m_offset = 0;


    };
}
#endif // DOCKING_PANE_HANDLE_H
