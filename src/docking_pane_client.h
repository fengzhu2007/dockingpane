#ifndef DOCKING_PANE_CLIENT_H
#define DOCKING_PANE_CLIENT_H
#include "docking_pane_container.h"
#include "global.h"
#include <functional>
class QDropEvent;


namespace ady {
typedef void (*DropFunc) (QDropEvent*);
class DockingWorkbench;
class DockingPaneClientPrivate;
    class DOCKINGPANE_EXPORT DockingPaneClient : public DockingPaneContainer {
        Q_OBJECT
    public:
        DockingPaneClient(DockingWorkbench* parent,bool init_view=true);
        virtual ~DockingPaneClient();
        virtual void initView() override;
        void setDropCallback(std::function<void(QDropEvent*)> func);
        std::function<void(QDropEvent*)> dropCallback();
    public slots:
        void onTabClose(int i);
    protected:
        virtual void focusInEvent(QFocusEvent *event) override;
        virtual void focusOutEvent(QFocusEvent *event) override;
        virtual void paintEvent(QPaintEvent *e) override;
        virtual void dragEnterEvent(QDragEnterEvent *event) override;
        virtual void dragMoveEvent(QDragMoveEvent *event) override;
        virtual void dropEvent(QDropEvent *event) override;
    private:
        DockingPaneClientPrivate* d;
    };
}
#endif // DOCKING_PANE_CLIENT_H
