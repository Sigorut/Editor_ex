
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QJsonDocument>
#include "filter.h"
#include "select_type_ex.h"
#include "edit_ex.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QString path_to_bd;
    int selected_ex_id;
    QStandardItemModel* model;
    QStandardItemModel* testmodel;
    Filter *filter_search;
    Select_type_ex *select_type_form;
    Ui::MainWindow *ui;
    void set_enabled_butt(bool flag);
    void open_bd();
    void add_ex_to_bd(QJsonObject record);
    void update_model(QJsonArray ex_all);
private slots:
    void slot_search();
    QJsonArray get_all_ex();
    void slot_filter();
    void slot_create_bd();
    void slot_open_bd();
    void slot_clear_bd();
    void slot_delete_ex();
    void slot_recieve_filter_form(QString str);
    void slot_recieve_select_type_form(int select_item, int parent_item);
    void slot_current_index_model(const QModelIndex &item);
    void slot_edit_current_ex();
    void slot_clone_current_ex();
};

#endif // MAINWINDOW_H
