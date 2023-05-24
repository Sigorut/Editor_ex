#ifndef FILTER_H
#define FILTER_H

#include <QDialog>
#include <QTreeWidgetItem>
namespace Ui {
class Filter;
}

class Filter : public QDialog
{
    Q_OBJECT

public:
    explicit Filter(QWidget *parent = nullptr);
    ~Filter();

private:
    Ui::Filter *ui;
    QTreeWidget* clone_tree;
    void updateParentItem(QTreeWidgetItem *item);
    //void clone_tree_widget();
private slots:
    void treeItemChanged(QTreeWidgetItem *item, int column);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

signals:
    void signal_send_data(QString str);
};

#endif // FILTER_H
