#include "docking_pane.h"
//#include "docking_pane_container.h"

#include <QVBoxLayout>
namespace ady {
    class DockingPanePrivate {
    public:
        QString id;
        QString group;
        QWidget* widget=nullptr;
        bool closeEnable=true;
    };


    DockingPane::DockingPane(QWidget* parent)
        :QWidget(parent){

        d = new DockingPanePrivate;
        setCloseEnable(true);
    }

    DockingPane::~DockingPane()
    {
        delete d;
    }

    void DockingPane::setCenterWidget(QWidget* widget)
    {
        d->widget = widget;
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setMargin(0);
        layout->setSpacing(0);
        widget->setParent(this);
        layout->addWidget(widget);
        this->setLayout(layout);
    }

    QWidget* DockingPane::centerWidget()
    {
        return d->widget;
    }

    void DockingPane::setId(QString id)
    {
        d->id = id;
    }

    void DockingPane::setGroup(QString group)
    {
        d->group = group;
    }

    QString DockingPane::id()
    {
        return d->id;
    }

    QString DockingPane::group()
    {
        return d->group;
    }

    void DockingPane::setCloseEnable(bool enable){
        d->closeEnable = enable;
    }

    bool DockingPane::closeEnable(){
        return d->closeEnable;
    }

}
