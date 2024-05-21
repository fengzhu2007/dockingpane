#include "docking_client_guide.h"
#include "ui_docking_client_guide.h"


#include "docking_pane_container.h"
#include "docking_workbench.h"

#include <QPixmap>
#include <QBitmap>

#include <QDebug>


namespace ady {
DockingClientGuide::DockingClientGuide(DockingWorkbench* parent)
    :QFrame(parent),
    ui(new Ui::DockingPaneClientGuide){
    ui->setupUi(this);
    setWindowFlags( Qt::FramelessWindowHint| Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground, true);
    //setAttribute(Qt::WA_NoBackground, true);

    ui->left->setPosition(DockingPaneManager::Left);
    ui->right->setPosition(DockingPaneManager::Right);
    ui->top->setPosition(DockingPaneManager::Top);
    ui->bottom->setPosition(DockingPaneManager::Bottom);
    ui->center->setPosition(DockingPaneManager::Center);

    ui->client_left->setPosition(DockingPaneManager::C_Left);
    ui->client_right->setPosition(DockingPaneManager::C_Right);
    ui->client_top->setPosition(DockingPaneManager::C_Top);
    ui->client_bottom->setPosition(DockingPaneManager::C_Bottom);

    QPixmap pixmap(":/images/vs2019/docking_all.png");
    QPalette palette;
    palette.setBrush(this->backgroundRole(),QBrush(pixmap));
    setPalette(palette);
    setMask(pixmap.mask());
    setAutoFillBackground(true);
}

DockingClientGuide::~DockingClientGuide(){
    delete ui;
}

DockingGuideDiamond* DockingClientGuide::updateActive(const QPoint& pos)
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
}
