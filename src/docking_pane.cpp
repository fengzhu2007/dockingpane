#include "docking_pane.h"
#include "docking_pane_container.h"
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
namespace ady {
class DockingEventFilter : public QObject{
public:
    DockingEventFilter(QWidget* widget){
        m_widget = widget;
    }

protected:
    bool eventFilter(QObject* obj, QEvent* event) override {
        if (event->type() == QEvent::MouseButtonPress) {
            //qDebug() << obj->metaObject()->className() << "clicked";
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QCoreApplication::sendEvent(m_widget, mouseEvent);
            return false;
        }
        return false;
    }

private:
    QWidget* m_widget;

};


    class DockingPanePrivate {
    public:
    ~DockingPanePrivate(){
        qDebug()<<"~DockingPanePrivate";
    }
        QString id;
        QString group;
        QWidget* widget=nullptr;
        bool closeEnable=true;
        float stretch = -10;
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
        QVBoxLayout* layout = (QVBoxLayout* )this->layout();
        if(layout==nullptr){
            layout = new QVBoxLayout(this);
            layout->setMargin(0);
            layout->setSpacing(0);
            this->setLayout(layout);
        }
        widget->setParent(this);
        layout->addWidget(widget);
        //install event filter
        this->installEventFilter(widget);
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

    QString DockingPane::description(){
        return {};
    }

    void DockingPane::setCloseEnable(bool enable){
        d->closeEnable = enable;
    }

    bool DockingPane::closeEnable(){
        return d->closeEnable;
    }

    void DockingPane::activation(){

    }

    void DockingPane::save(bool rename){
        Q_UNUSED(rename);
    }

    void DockingPane::contextMenu(const QPoint& pos){
        Q_UNUSED(pos);
    }

    void DockingPane::undo(){

    }

    DockingPaneContainer* DockingPane::container(){
        if(parentWidget()!=nullptr){
            return static_cast<DockingPaneContainer*>(parentWidget()->parentWidget());
        }else{
            return nullptr;
        }
    }

    void DockingPane::activeToCurrent(){
        auto container = this->container();
        if(container!=nullptr){
            container->setPane(this);
        }
    }

    float DockingPane::stretch(){
        return d->stretch;
    }

    void DockingPane::setStretch(float s){
        d->stretch = s;
    }



    void DockingPane::installEventFilter(QWidget* w){
        const auto children = w->findChildren<QWidget*>();
        for (QWidget* child : children) {
            child->installEventFilter(new DockingEventFilter(this));
            installEventFilter(child);
        }
    }
}
