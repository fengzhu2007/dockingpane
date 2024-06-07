#ifndef DOCKING_PANE_H
#define DOCKING_PANE_H
#include <QWidget>
#include "global.h"
namespace ady {
    class DockingPanePrivate;
    class DockingPaneContainer;
    class DockingPaneManager;
    class DOCKINGPANE_EXPORT DockingPane : public QWidget {
        Q_OBJECT
    public:
        DockingPane(DockingPaneContainer* parent);
        ~DockingPane();
        void setCenterWidget(QWidget* widget);
        QWidget* centerWidget();
        void setId(QString id);
        void setGroup(QString group);
        QString id();
        QString group();
        void setCloseEnable(bool enable);
        bool closeEnable();



    protected:
        DockingPanePrivate* d;




    friend class DockingPaneManager;



    };
}
#endif // DOCKING_PANE_H
