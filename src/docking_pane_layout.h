#ifndef DOCKING_PANE_LAYOUT_H
#define DOCKING_PANE_LAYOUT_H
#include <QLayout>
#include "docking_pane_manager.h"
#include "global.h"
namespace ady {
    class DockingPaneLayoutItemInfo;
    class DOCKINGPANE_EXPORT DockingPaneLayout : public QLayout
    {
    public:
        DockingPaneLayout(QWidget *parent, int margin = 0, int spacing = -1);
        ~DockingPaneLayout();

        void addItem(QLayoutItem *item) override;
        //void addWidget(QWidget *widget, Position position);
        Qt::Orientations expandingDirections() const override;
        bool hasHeightForWidth() const override;
        int count() const override;
        QLayoutItem *itemAt(int index) const override;
        QSize minimumSize() const override;
        void setGeometry(const QRect &rect) override;
        QSize sizeHint() const override;
        QLayoutItem *takeAt(int index) override;



        void addPane(DockingPane* pane,DockingPaneManager::Position position);
        void addPane(DockingPane* pane,QLayoutItem* relation,DockingPaneManager::Position position);

        DockingPaneLayoutItemInfo* addItem(DockingPaneContainer* widget,DockingPaneManager::Position position);
        DockingPaneLayoutItemInfo* addItem(QLayoutItem* itemInfo,DockingPaneManager::Position position);
        DockingPaneLayoutItemInfo* addItem(DockingPaneContainer* widget,DockingPaneLayoutItemInfo* relation,DockingPaneManager::Position position);
        //void addItem(DockingPaneLayoutItemInfo* itemInfo,DockingPaneLayoutItemInfo* parent,DockingPaneManager::Position position);

        DockingPaneLayoutItemInfo* rootItem();
        void dump(QString prefix);

    private:
        DockingPaneLayoutItemInfo* m_rootItem;


        friend class DockingPaneManager;
    };
}
#endif // DOCKING_PANE_LAYOUT_H
