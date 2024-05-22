#include "docking_guide.h"
#include "ui_docking_guide.h"
//#include "docking_pane_container.h"
#include "docking_workbench.h"

#include <QPixmap>
#include <QBitmap>

#include <QDebug>


namespace ady {
    class DockingGuidePrivate{
    public:
        int size_mode;
    };


    DockingGuide::DockingGuide(DockingWorkbench* parent)
        :QFrame(parent),
        ui(new Ui::DockingPaneGuide){
        ui->setupUi(this);
        d = new DockingGuidePrivate();
        setWindowFlags( Qt::FramelessWindowHint| Qt::ToolTip);
        setAttribute(Qt::WA_TranslucentBackground, true);

        ui->left->setPosition(DockingPaneManager::Left);
        ui->right->setPosition(DockingPaneManager::Right);
        ui->top->setPosition(DockingPaneManager::Top);
        ui->bottom->setPosition(DockingPaneManager::Bottom);
        ui->center->setPosition(DockingPaneManager::Center);

        ui->client_left->setPosition(DockingPaneManager::C_Left);
        ui->client_right->setPosition(DockingPaneManager::C_Right);
        ui->client_top->setPosition(DockingPaneManager::C_Top);
        ui->client_bottom->setPosition(DockingPaneManager::C_Bottom);

        /*QPixmap pixmap(":/images/vs2019/docking_all.png");
        QPalette palette;
        palette.setBrush(this->backgroundRole(),QBrush(pixmap));
        setPalette(palette);
        setMask(pixmap.mask());
        setAutoFillBackground(true);*/
    }

    DockingGuide::~DockingGuide(){
        delete ui;
        delete d;
    }

    DockingGuideDiamond* DockingGuide::updateActive(const QPoint& pos)
    {
        QRect rc = geometry();
        QPoint p (pos.x() - rc.x(),pos.y() - rc.y());
        bool ret = ui->left->updateActive(p);
        if(ret){
            return ui->left;
        }
        ret = ui->top->updateActive(p);
        if(ret){
            return ui->top;
        }
        ret = ui->center->updateActive(p);
        if(ret){
            return ui->center;
        }
        ret = ui->right->updateActive(p);
        if(ret){
            return ui->right;
        }
        ret = ui->bottom->updateActive(p);
        if(ret){
            return ui->bottom;
        }
        ret = ui->client_left->updateActive(p);
        if(ret){
            return ui->client_left;
        }
        ret = ui->client_top->updateActive(p);
        if(ret){
            return ui->client_top;
        }
        ret = ui->client_right->updateActive(p);
        if(ret){
            return ui->client_right;
        }
        ret = ui->client_bottom->updateActive(p);
        if(ret){
            return ui->client_bottom;
        }
        return nullptr;
    }



    void DockingGuide::setSizeMode(int mode){
        if(d->size_mode==mode){
            return ;
        }
        d->size_mode = mode;
        //qDebug()<<"mode:"<<mode;
        if((d->size_mode&DockingGuide::Normal)>0){
            ui->label->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/docking.png")));
            this->setGeometry(QRect(0,0,112,112));
            ui->label->setGeometry(QRect(0,0,112,112));

            ui->left->setGeometry(QRect(4,40,32,32));
            ui->right->setGeometry(QRect(76,40,32,32));
            ui->center->setGeometry(QRect(40,40,32,32));
            ui->top->setGeometry(QRect(40,4,32,32));
            ui->bottom->setGeometry(QRect(40,76,32,32));
        }else if((d->size_mode &DockingGuide::Large)>0 ){
            ui->label->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/docking_all.png")));
            this->setGeometry(QRect(0,0,184,184));
            ui->label->setGeometry(QRect(0,0,184,184));
            //set buttons
            ui->left->setGeometry(QRect(4,76,32,32));
            ui->client_left->setGeometry(QRect(40,76,32,32));
            ui->center->setGeometry(QRect(76,76,32,32));
            ui->client_right->setGeometry(QRect(112,76,32,32));
            ui->right->setGeometry(QRect(148,76,32,32));

            ui->top->setGeometry(QRect(76,4,32,32));
            ui->client_top->setGeometry(QRect(76,40,32,32));
            ui->client_bottom->setGeometry(QRect(76,112,32,32));
            ui->bottom->setGeometry(QRect(76,148,32,32));
        }
        ui->center->show();
        if((d->size_mode&DockingGuide::Left)>0){
             ui->left->show();
        }else{
             ui->left->hide();
        }
        if((d->size_mode&DockingGuide::Top)>0){
             ui->top->show();
        }else{
             ui->top->hide();
        }
        if((d->size_mode&DockingGuide::Right)>0){
             ui->right->show();
        }else{
             ui->right->hide();
        }
        if((d->size_mode&DockingGuide::Bottom)>0){
             ui->bottom->show();
        }else{
             ui->bottom->hide();
        }
        if((d->size_mode&DockingGuide::C_Left)>0){
             ui->client_left->show();
        }else{
             ui->client_left->hide();
        }
        if((d->size_mode&DockingGuide::C_Top)>0){
             ui->client_top->show();
        }else{
             ui->client_top->hide();
        }
        if((d->size_mode&DockingGuide::C_Right)>0){
             ui->client_right->show();
        }else{
             ui->client_right->hide();
        }
        if((d->size_mode&DockingGuide::C_Bottom)>0){
             ui->client_bottom->show();
        }else{
             ui->client_bottom->hide();
        }

    }
    int DockingGuide::sizeMode(){
        return d->size_mode;
    }
}
