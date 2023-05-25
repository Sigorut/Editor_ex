
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Редактор заданий");
    filter_search = new Filter();
    select_type_form = new Select_type_ex();
    model = new QStandardItemModel();
    testmodel  = new QStandardItemModel();
    set_enabled_butt(false);
    ui->table_view->horizontalHeader()->setStretchLastSection(true);
    ui->table_view->setSortingEnabled(true);
    ui->table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_view->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->table_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->butt_search, SIGNAL(clicked()), SLOT(slot_search()));
    connect(ui->butt_filter, SIGNAL(clicked()), filter_search, SLOT(show()));
    connect(ui->butt_del, SIGNAL(clicked()), SLOT(slot_delete_ex()));
    connect(filter_search, SIGNAL(signal_send_data(QString)), this, SLOT(slot_recieve_filter_form(QString)));
    connect(select_type_form, SIGNAL(signal_send_data(int, int)), this, SLOT(slot_recieve_select_type_form(int, int)));
    connect(ui->butt_add, SIGNAL(clicked()), select_type_form, SLOT(show()));
    connect(ui->action_new, SIGNAL(triggered()), SLOT(slot_create_bd()));
    connect(ui->action_open, SIGNAL(triggered()), SLOT(slot_open_bd()));
    connect(ui->table_view, SIGNAL(clicked(const QModelIndex &)),SLOT(slot_current_index_model(const QModelIndex &)));
    connect(ui->table_view, SIGNAL(doubleClicked(QModelIndex)),SLOT(slot_edit_current_ex()));
    connect(ui->butt_edit, SIGNAL(clicked()), SLOT(slot_edit_current_ex()));
    connect(ui->butt_clone, SIGNAL(clicked()), SLOT(slot_clone_current_ex()));
    connect(ui->action_clear, SIGNAL(triggered()), SLOT(slot_clear_bd()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_enabled_butt(bool flag)
{
    ui->butt_clone->setEnabled(flag);
    ui->butt_add->setEnabled(flag);
    ui->butt_del->setEnabled(flag);
    ui->butt_edit->setEnabled(flag);
    ui->butt_filter->setEnabled(flag);
    ui->butt_search->setEnabled(flag);
}

void MainWindow::open_bd()
{
    QFile bd(path_to_bd);
    QString text_content;
    QJsonDocument json_content;
    QStringList labels;
    selected_ex_id = -1;
    model->clear();
    labels << "Тип задания" << "Вид Задания" << "Текст задания" << "id";
    model->setHorizontalHeaderLabels(labels);
    if(bd.open(QIODevice::ReadOnly)){
        text_content = bd.readAll();
        json_content = QJsonDocument::fromJson(text_content.toUtf8());
        QJsonArray json_all_ex = json_content.array();
        qDebug() << text_content;
        update_model(json_all_ex);
        ui->table_view->setModel(model);
        ui->table_view->setColumnHidden(3, true);
        ui->table_view->sortByColumn(0, Qt::SortOrder::AscendingOrder);
        bd.close();
    }
}

void MainWindow::add_ex_to_bd(QJsonObject record)
{
    QFile bd(path_to_bd);
    QString text_content;
    QJsonDocument json_content;
    QJsonArray json_all_ex;
    if(bd.open(QIODevice::ReadOnly)){
        text_content = bd.readAll();
        json_content = QJsonDocument::fromJson(text_content.toUtf8());
        json_all_ex = json_content.array();
        json_all_ex << record;
        qDebug() << json_all_ex;
        bd.close();
    }
    if(bd.open(QIODevice::WriteOnly)){
        QJsonDocument jsonDoc;
        jsonDoc.setArray(json_all_ex);
        bd.write(jsonDoc.toJson());
        update_model(json_all_ex);

    }
    //    open_bd();
}

void MainWindow::update_model(QJsonArray ex_all)
{
    int i = 0;
    int sort_column = ui->table_view->horizontalHeader()->sortIndicatorSection();

    int sort_order = ui->table_view->horizontalHeader()->sortIndicatorOrder();
    QStandardItem *item;
    QStringList labels;
    QStringList temp;
    qDebug() << "update" << ex_all.size();
    model->clear();
    labels << "Тип задания" << "Вид Задания" << "Текст задания" << "id";
    model->setHorizontalHeaderLabels(labels);
    ui->table_view->clearMask();
    foreach (const QJsonValue & value, ex_all) {
        QJsonObject obj = value.toObject();
        qDebug() << obj.value("type").toInt();
        item = new QStandardItem(QString("%1").arg(obj.value("type").toInt()));
        model->setItem(i,0,item);
        item = new QStandardItem(QString("%1").arg(obj.value("parent_type").toInt()));
        model->setItem(i,1,item);
        QString valuetemp = obj.value("text_ex").toString();
        if(valuetemp.contains("\\n")){
            QStringList temp;
            temp = valuetemp.split("\\n");
            valuetemp.clear();
            for(int i = 0; i < temp.size(); i++){
                valuetemp += temp[i];
                if(i != temp.size()-1){
                    valuetemp += + "\n";
                }
            }
        }
        item = new QStandardItem(QString("%1").arg(valuetemp));
        model->setItem(i,2,item);
        item = new QStandardItem(QString("%1").arg(obj.value("id").toInt()));
        model->setItem(i,3,item);
        i++;
    }
    ui->table_view->setColumnHidden(3, true);
    ui->table_view->sortByColumn(sort_column, Qt::SortOrder(sort_order));
}

void MainWindow::slot_search()
{
    for(int i = 0; i < model->rowCount(); i++){
        ui->table_view->hideRow(i);
        for(int j = 0; j < model->columnCount(); j++){
            qDebug() << model->item(i,j)->text();
            if(model->item(i,j)->text().contains(ui->lineEdit_search->text())){
                ui->table_view->showRow(i);
            }
        }
    }
}

QJsonArray MainWindow::get_all_ex()
{
    QFile bd(path_to_bd);
    QString text_content;
    QJsonDocument json_content;
    QJsonArray json_all_ex;
    if(bd.open(QIODevice::ReadOnly)){
        text_content = bd.readAll();
        json_content = QJsonDocument::fromJson(text_content.toUtf8());
        json_all_ex = json_content.array();
        bd.close();
    }
    return json_all_ex;
}

void MainWindow::slot_filter()
{
    Filter test;
    test.exec();
}

void MainWindow::slot_create_bd()
{
    QString path_to_file = QFileDialog::getSaveFileName(this, tr("Сохранить"),
                                                        "/",
                                                        tr("JSON (*.json)"));
    qDebug() << path_to_file;

    if(!path_to_file.isEmpty()){

        path_to_bd = path_to_file;
        QFile filejson(path_to_bd);
        if(filejson.open(QIODevice::WriteOnly)){
            QJsonArray json_all_ex;
            QJsonDocument jsonDoc;
            jsonDoc.setArray(json_all_ex);
            filejson.write(jsonDoc.toJson());
            filejson.close();
        }
        set_enabled_butt(true);
        open_bd();
    }
}

void MainWindow::slot_open_bd()
{
    QString path_to_file = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        "/",
                                                        tr("*.json"));
    if(!path_to_file.isEmpty()){
        set_enabled_butt(true);
        path_to_bd = path_to_file;
        open_bd();
    }
}

void MainWindow::slot_clear_bd()
{
    QMessageBox msgBox;
    msgBox.setText("Вы действительно хотите очистить весь банк заданий?.");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    msgBox.setButtonText(QMessageBox::Yes, tr("Да"));
    msgBox.setButtonText(QMessageBox::Cancel, tr("Отмена"));
    int ret = msgBox.exec();
    QJsonArray all_ex = get_all_ex();
    QFile bd(path_to_bd);
    switch (ret) {
    case QMessageBox::Yes:
        while(all_ex.size()>0){
                all_ex.pop_back();
        }
        if(bd.open(QIODevice::WriteOnly)){
            QJsonDocument jsonDoc;
            jsonDoc.setArray(all_ex);
            bd.write(jsonDoc.toJson());
            update_model(all_ex);

        }
        break;
    case QMessageBox::Cancel:
        qDebug() << "Ну и ладна";
        break;
    default:
        // should never be reached
        break;
    }
}

void MainWindow::slot_delete_ex()
{
    QMessageBox msgBox;
    msgBox.setText("Удаление задания.");
    msgBox.setInformativeText("Вы действительно хотите удалить задание?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    msgBox.setButtonText(QMessageBox::Yes, tr("Да"));
    msgBox.setButtonText(QMessageBox::Cancel, tr("Отмена"));
    int ret = msgBox.exec();
    QJsonArray all_ex = get_all_ex();
    QJsonValue single_ex;

    switch (ret) {
    case QMessageBox::Yes:

        for(int i = 0; i < all_ex.size(); i++){
            single_ex = all_ex[i];
            qDebug() << single_ex["id"].toInt() << selected_ex_id;
            if(single_ex["id"].toInt() == selected_ex_id){
                qDebug() << "delete";
                all_ex.removeAt(i);
                QFile bd(path_to_bd);
                if(bd.open(QIODevice::WriteOnly)){
                    QJsonDocument jsonDoc;
                    jsonDoc.setArray(all_ex);
                    bd.write(jsonDoc.toJson());
                    update_model(all_ex);

                }
                break;
            }
        }
        break;
    case QMessageBox::Cancel:
        qDebug() << "Ну и ладна";
        break;
    default:
        // should never be reached
        break;
    }
}

void MainWindow::slot_recieve_filter_form(QString str)
{
    qDebug() << "main" << str << "tes";
}

void MainWindow::slot_recieve_select_type_form(int select_item, int parent_item)
{
    Edit_ex edit_ex_form(select_item, parent_item, this);
    if(edit_ex_form.exec()){
        QJsonObject ex = edit_ex_form.get_ex();
        QJsonArray json_all_ex = get_all_ex();
        QVector<bool> free_slot(1000,true);
        qDebug() << free_slot;
        QJsonValue single;
        for(int i = 0; i < json_all_ex.size(); i++){
            for(int j = 0; free_slot.size(); j++){
                single = json_all_ex[i];
                if(single["id"].toInt() == j){
                    free_slot[j] = false;
                    break;
                }
            }
        }
        for(int i = 0; i < free_slot.size(); i++){
            if(free_slot[i]){
                ex.insert("id", i);
                break;
            }
        }
        add_ex_to_bd(ex);
    }
}

void MainWindow::slot_current_index_model(const QModelIndex &item)
{

    qDebug() << item.row();
    QJsonArray all_ex = get_all_ex();
    QJsonValue single_ex;
    for(int i = 0; i < all_ex.size(); i++){
        single_ex = all_ex[i];
        if(single_ex["id"].toInt() == model->item(item.row(),3)->text().toInt()){
            qDebug() << single_ex;
            selected_ex_id = single_ex["id"].toInt();
            break;
        }
    }

}

void MainWindow::slot_edit_current_ex()
{
    qDebug() << "edit";
    QJsonArray all_ex = get_all_ex();
    QJsonValue single_ex;
    int i = 0;
    for(;i < all_ex.size(); i++){
        single_ex = all_ex[i];
        if(single_ex["id"].toInt() == selected_ex_id){
            break;
        }
    }

    Edit_ex edit_ex_form(single_ex["type"].toInt(), single_ex["parent_type"].toInt(), this);
    edit_ex_form.set_ex(single_ex);
    if(edit_ex_form.exec()){
        single_ex = edit_ex_form.get_ex();
        all_ex[i] = single_ex;
        QFile bd(path_to_bd);
        if(bd.open(QIODevice::WriteOnly)){
            QJsonDocument jsonDoc;
            jsonDoc.setArray(all_ex);
            bd.write(jsonDoc.toJson());
            update_model(all_ex);
        }
    }
}

void MainWindow::slot_clone_current_ex()
{

    QJsonArray all_ex = get_all_ex();
    QJsonObject single_ex;
    int i = 0;
    for(;i < all_ex.size(); i++){
        single_ex = all_ex[i].toObject();
        if(single_ex["id"].toInt() == selected_ex_id){
            break;
        }
    }
    QVector<bool> free_slot(1000,true);
    qDebug() << free_slot;
    QJsonValue temp;
    for(int i = 0; i < all_ex.size(); i++){
        for(int j = 0; free_slot.size(); j++){
            temp = all_ex[i];
            if(temp["id"].toInt() == j){
                free_slot[j] = false;
                break;
            }
        }
    }
    for(int i = 0; i < free_slot.size(); i++){
        if(free_slot[i]){
            single_ex.insert("id", i);
            break;
        }
    }
    add_ex_to_bd(single_ex);
    Edit_ex edit_ex_form(single_ex["type"].toInt(), single_ex["parent_type"].toInt(), this);
    edit_ex_form.set_ex(single_ex);
    if(edit_ex_form.exec()){
        single_ex = edit_ex_form.get_ex();
        all_ex[i] = single_ex;
        QFile bd(path_to_bd);
        if(bd.open(QIODevice::WriteOnly)){
            QJsonDocument jsonDoc;
            jsonDoc.setArray(all_ex);
            bd.write(jsonDoc.toJson());
            update_model(all_ex);
        }
    }
}



