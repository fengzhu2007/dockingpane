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
            Gone,
            Visible,
            Other
        };

        DockingPaneContainerNClient(DockingPaneContainer* parent);
        ~DockingPaneContainerNClient();
        void updateTitle(QString title);
        void setButtonState(Button b,State s);
        void setActive(bool active);

    public slots:
        void onMenuRequested(const QPoint &pos);
        void onClose();
        void onAutoHide();


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
