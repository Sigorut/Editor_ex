#ifndef SELECT_TYPE_EX_H
#define SELECT_TYPE_EX_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class Select_type_ex;
}

class Select_type_ex : public QDialog
{
    Q_OBJECT

public:
    explicit Select_type_ex(QWidget *parent = nullptr);
    ~Select_type_ex();

private:
    Ui::Select_type_ex *ui;
    void send_data(QString, QString);
private slots:
    void slot_choose_item(QTreeWidgetItem* item,int column);
    void slot_selected_item(QTreeWidgetItem* item,int column);
signals:
    void signal_send_data(int,int);
};

#endif // SELECT_TYPE_EX_H
