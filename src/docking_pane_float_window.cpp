#include "docking_pane_float_window.h"
#include "docking_pane_container.h"
#include "docking_pane_window_resizer.h"
#include "docking_workbench.h"
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QDebug>
namespace ady{
    class DockingPaneFloatWindowPrivate {
    public:
        DockingPaneWindowResizer* regions[8];
        int resizer_size = 0;
        bool moving = false;

    };

    DockingPaneFloatWindow::DockingPaneFloatWindow(QWidget* parent,int margin)
        :QWidget(parent)
    {
        setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
        d = new DockingPaneFloatWindowPrivate;
        d->resizer_size = margin;
        initResizer();
        this->setAttribute(Qt::WA_TranslucentBackground);


    }

    DockingPaneFloatWindow::~DockingPaneFloatWindow(){
        delete d;
    }

    void DockingPaneFloatWindow::setCenterWidget(DockingPaneContainer* container)
    {
        QVBoxLayout* layout = (QVBoxLayout*)this->layout();
        if(layout==nullptr){
            layout = new QVBoxLayout(this);
            layout->setMargin(6);
            layout->setSpacing(0);
        }
        layout->addWidget((QWidget*)container);
        container->setParent(this);
        container->setState(DockingPaneContainer::Float);

        QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
        shadow_effect->setOffset(0, 0);
        shadow_effect->setColor(Qt::black);
        shadow_effect->setBlurRadius(10);
        container->setGraphicsEffect(shadow_effect);
    }

    void DockingPaneFloatWindow::setCenterWidget(DockingPane* pane)
    {

    }

    DockingPaneContainer* DockingPaneFloatWindow::centerWidget()
    {
        return (DockingPaneContainer*)layout()->itemAt(0)->widget();
    }



    void DockingPaneFloatWindow::initResizer()
    {

        for(int i=0;i<8;i++){
            DockingPaneWindowResizer::Region region = (DockingPaneWindowResizer::Region)i;
            d->regions[i] = new DockingPaneWindowResizer(this,region);
        }
        updateResizer();
    }

    void DockingPaneFloatWindow::updateResizer()
    {
        QRect rc = geometry();
        for(int i=0;i<8;i++){
            DockingPaneWindowResizer::Region region = (DockingPaneWindowResizer::Region)i;
            if(region==DockingPaneWindowResizer::Left){
                d->regions[i]->setGeometry(0,d->resizer_size,d->resizer_size,rc.height() - 2*d->resizer_size);
            }else if(region==DockingPaneWindowResizer::Right){
                d->regions[i]->setGeometry(rc.width() - d->resizer_size,d->resizer_size,d->resizer_size,rc.height() - 2*d->resizer_size);
            }else if(region==DockingPaneWindowResizer::Top){
                d->regions[i]->setGeometry(d->resizer_size,0,rc.width() - 2*d->resizer_size,d->resizer_size);
            }else if(region==DockingPaneWindowResizer::Bottom){
                d->regions[i]->setGeometry(d->resizer_size,rc.height() - d->resizer_size,rc.width() - 2*d->resizer_size,d->resizer_size);
            }else if(region==DockingPaneWindowResizer::LeftTop){
                d->regions[i]->setGeometry(0,0,d->resizer_size,d->resizer_size);
            }else if(region==DockingPaneWindowResizer::LeftBottom){
                d->regions[i]->setGeometry(0,rc.height() - d->resizer_size,d->resizer_size,d->resizer_size);
            }else if(region==DockingPaneWindowResizer::RightTop){
                d->regions[i]->setGeometry(rc.width() - d->resizer_size,0,d->resizer_size,d->resizer_size);
            }else if(region==DockingPaneWindowResizer::RightBottom){
                d->regions[i]->setGeometry(rc.width() - d->resizer_size,rc.height() - d->resizer_size,d->resizer_size,d->resizer_size);
            }
        }
    }

    void DockingPaneFloatWindow::showMaximized(){
        this->layout()->setMargin(0);
        QWidget::showMaximized();
    }

    void DockingPaneFloatWindow::showNormal(){
        this->layout()->setMargin(6);
        QWidget::showNormal();
    }

    void DockingPaneFloatWindow::resizeEvent(QResizeEvent *event)
    {
        QWidget::resizeEvent(event);
        updateResizer();
    }

    void DockingPaneFloatWindow::mousePressEvent(QMouseEvent *event){
        QWidget::mousePressEvent(event);
        DockingWorkbench* workbench = (DockingWorkbench*)parentWidget();
        workbench->unActiveAll();
        workbench->hideFixedWindow();
        centerWidget()->activeWidget(true);
    }





}
