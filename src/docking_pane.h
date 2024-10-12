#ifndef DOCKING_PANE_H
#define DOCKING_PANE_H
#include <QWidget>
#include <QJsonObject>
#include "global.h"
namespace ady {
    class DockingEventFilter;
    class DockingPanePrivate;
    class DockingPaneContainer;
    class DockingPaneManager;
    class DOCKINGPANE_EXPORT DockingPane : public QWidget {
        Q_OBJECT
    public:
        //DockingPane(DockingPaneContainer* parent=nullptr);
        DockingPane(QWidget* parent=nullptr);
        virtual ~DockingPane();
        void setCenterWidget(QWidget* widget);
        QWidget* centerWidget();
        void setId(QString id);
        void setGroup(QString group);
        virtual QString id();
        virtual QString group();
        virtual QString description();
        void setCloseEnable(bool enable);
        virtual bool closeEnable();
        virtual void activation();
        virtual void save(bool rename=false);
        virtual void contextMenu(const QPoint& pos);
        virtual void undo();
        virtual QJsonObject toJson();
        DockingPaneContainer* container();
        void activeToCurrent();
        float stretch();
        void setStretch(float s);

    private:
        void installEventFilter(QWidget* w);

    private:
        DockingPanePrivate* d;

    friend class DockingPaneManager;



    };
}
#endif // DOCKING_PANE_H
