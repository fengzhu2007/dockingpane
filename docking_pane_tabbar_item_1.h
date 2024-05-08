#ifndef DOCKING_PANE_TABBAR_ITEM_H
#define DOCKING_PANE_TABBAR_ITEM_H
#include <QFrame>
#include <QLabel>
namespace ady{
    /*class DockingPaneTabBarItemPrivate;
    class DockingPaneTabBarItem : public QFrame
    {
        Q_OBJECT
    public:
        DockingPaneTabBarItem(QWidget* parent);
        ~DockingPaneTabBarItem();
        void setText(QString text);
        QString text();

    private:
        DockingPaneTabBarItemPrivate* d;
    };*/

    class DockingPaneTabBarItemV2 : public QLabel {

    public:
        DockingPaneTabBarItemV2(QWidget* parent);

    protected:
        void initPainter(QPainter *painter) const override;

    };




}
#endif // DOCKING_PANE_TABBAR_ITEM_H
