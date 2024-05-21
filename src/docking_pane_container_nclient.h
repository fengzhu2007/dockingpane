#ifndef DOCKING_PANE_CONTAINER_NCLIENT_H
#define DOCKING_PANE_CONTAINER_NCLIENT_H
#include <QFrame>

namespace Ui {
    class DockingPaneContainerNClient;
}

namespace ady {
    class DockingPaneContainer;
    class DockingPaneContainerNClientPrivate;
    class DockingPaneContainerNClient : public QFrame{
        Q_OBJECT


    public:
        enum Button {
            Dropdown,
            Pin,
            Max,
            Close,
        };
        enum State {
            Inner,
            Fixed,
            Float
        };

        DockingPaneContainerNClient(DockingPaneContainer* parent);
        ~DockingPaneContainerNClient();
        void updateTitle(QString title);
        void setButtonState(Button b,State s);
        void setActive(bool active);
        void setMoving(bool state);

        bool activeState();
        void setActiveState(bool active_state);

        void stylePolish();


    public slots:
        void onMenuRequested();
        void onClose();
        void onAutoHide();
        void onFloat(bool moving=false);
        void onDock();
        void onTabDock();


    protected:
        virtual void mousePressEvent(QMouseEvent *e) override;
        virtual void mouseMoveEvent(QMouseEvent *e) override;
        virtual void mouseReleaseEvent(QMouseEvent *e) override;
        virtual void paintEvent(QPaintEvent *e) override;


    private:
        Ui::DockingPaneContainerNClient* ui;
        DockingPaneContainerNClientPrivate* d;
    };

}
#endif // DOCKING_PANE_CONTAINER_NCLIENT_H
