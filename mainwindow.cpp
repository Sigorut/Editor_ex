
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
    //Открытие базы данных
    QFile bd(path_to_bd);
    QString text_content;
    QJsonDocument json_content;
    QStringList labels;
    selected_ex_id = -1;
    model->clear();
    labels << "Тип задания" << "Вид Задания" << "Текст задания" << "id";
    model->setHorizontalHeaderLabels(labels);
    if(bd.open(QIODevice::ReadOnly)){
        //Считывается весь файл
        text_content = bd.readAll();
        json_content = QJsonDocument::fromJson(text_content.toUtf8());
        //Конвертирование строки в QJsonDocument
        QJsonArray json_all_ex = json_content.array();
        //Добавление заданий в модель
        update_model(json_all_ex);
        ui->table_view->setModel(model);
        ui->table_view->setColumnHidden(3, true);
        ui->table_view->sortByColumn(0, Qt::SortOrder::AscendingOrder);
        bd.close();
    }
}

void MainWindow::add_ex_to_bd(QJsonObject record)
{
    //Добавление задания в базу данных
    QJsonArray json_all_ex;
    json_all_ex = get_all_ex();
    json_all_ex << record;
    QFile bd(path_to_bd);
    if(bd.open(QIODevice::WriteOnly)){
        QJsonDocument jsonDoc;
        jsonDoc.setArray(json_all_ex);
        //Запись заданий в файл
        bd.write(jsonDoc.toJson());
        update_model(json_all_ex);

    }
}

void MainWindow::update_model(QJsonArray ex_all)
{
    //Запись заданий в таблицу представления
    int i = 0;
    int sort_column = ui->table_view->horizontalHeader()->sortIndicatorSection();
    int sort_order = ui->table_view->horizontalHeader()->sortIndicatorOrder();
    QStandardItem *item;
    QStringList labels;
    model->clear();
    //Задаем заголовки в таблице
    labels << "Тип задания" << "Вид Задания" << "Текст задания" << "id";
    model->setHorizontalHeaderLabels(labels);
    ui->table_view->clearMask();
    QString type, parent_type;
    //Проход по заданиям и добавление определенных полей в таблицу представления
    foreach (const QJsonValue & value, ex_all) {
        QJsonObject obj = value.toObject();
        qDebug() << obj.value("type").toInt();
        type = get_type(obj.value("type").toInt());
        item = new QStandardItem(QString("%1").arg(type));
        model->setItem(i,0,item);
        parent_type = get_parent_type(obj.value("parent_type").toInt());
        item = new QStandardItem(QString("%1").arg(parent_type));
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
    //Скрытие id задания
    ui->table_view->setColumnHidden(3, true);
    //Сортировка заданий
    ui->table_view->sortByColumn(sort_column, Qt::SortOrder(sort_order));
}

QString MainWindow::get_type(int type)
{
    //Получение типа задания
    switch (type) {
    case 1:
        return "Числовой ответ";
        break;
    case 2:
        return "Строковый ответ";
        break;
    case 3:
        return "Сопоставление";
        break;
    case 4:
        return "Последовательность";
        break;
    case 5:
        return "Пропущенное слово";
        break;
    case 6:
        return "Выбор верных/неверных";
        break;
    case 7:
        return "28line";
        break;
    default:
        break;
    }
    return "";
}

QString MainWindow::get_parent_type(int parent_type)
{
    //Получение родительского типа заданий
    switch (parent_type) {
    case 1:
        return "Числовой";
        break;
    case 2:
        return "Строковый";
        break;
    case 3:
        return "Табличный";
        break;
    case 4:
        return "Множественный";
        break;
    case 5:
        return "28line";
        break;
    default:
        break;
    }
    return "";
}

void MainWindow::slot_search()
{
    //Поиск по базе данных и вывод найденных записей в таблицу
    QString data;
    QString search_line = ui->lineEdit_search->text();
    search_line = search_line.toLower();
    for(int i = 0; i < model->rowCount(); i++){
        ui->table_view->hideRow(i);
        for(int j = 0; j < model->columnCount(); j++){
            qDebug() << model->item(i,j)->text();
            data = model->item(i,j)->text();
            data = data.toLower();
            if(data.contains(search_line)){
                ui->table_view->showRow(i);
            }
        }
    }
}

QJsonArray MainWindow::get_all_ex()
{
    //Получение всех заданий из базы данных
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
    //Создание базы данных
    //Вызов диалогового окна для выбора пути до файла
    QString path_to_file = QFileDialog::getSaveFileName(this, tr("Сохранить"),
                                                        "/",
                                                        tr("JSON (*.json)"));
    //Если путь выбран, то создается бд
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
        //Разблокировка кнопок
        set_enabled_butt(true);

        open_bd();
    }
}

void MainWindow::slot_open_bd()
{
    //Открытие созданной базы данных
    //Получение пути до бд
    QString path_to_file = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        "/",
                                                        tr("*.json"));
    //Если путь до файла выбран, то открывается бд
    if(!path_to_file.isEmpty()){
        set_enabled_butt(true);
        path_to_bd = path_to_file;
        open_bd();
    }
}

void MainWindow::slot_clear_bd()
{
    //Очищение базы данных
    QMessageBox msgBox;
    msgBox.setText("Вы действительно хотите очистить весь банк заданий?.");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    msgBox.setButtonText(QMessageBox::Yes, tr("Да"));
    msgBox.setButtonText(QMessageBox::Cancel, tr("Отмена"));
    int ret = msgBox.exec();
    QFile bd(path_to_bd);
    switch (ret) {
    case QMessageBox::Yes:
        if(bd.open(QIODevice::WriteOnly)){
            QJsonDocument jsonDoc;
            QJsonArray empty;
            jsonDoc.setArray(empty);
            bd.write(jsonDoc.toJson());
            update_model(empty);
        }
        break;
    case QMessageBox::Cancel:
        qDebug() << "Ну и ладна";
        break;
    default:
        break;
    }
}

void MainWindow::slot_delete_ex()
{
    //Удаление задания из бд
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
        //Проход по бд до соответствия id
        for(int i = 0; i < all_ex.size(); i++){
            single_ex = all_ex[i];
            qDebug() << single_ex["id"].toInt() << selected_ex_id;
            if(single_ex["id"].toInt() == selected_ex_id){
                //Удаление задания
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
    //Получение типа задания из другой формы
    //Создается форма для создания задания
    QJsonObject ex;
    if(select_item > 6){
        Gen_28line ex_gen_form(select_item, parent_item);
        if(ex_gen_form.exec()){
            ex = ex_gen_form.get_ex();
            qDebug() << "aAAAAAFWFWNFAKWFNAWLKFAWLKFNAWLKFNAWKLFNAWLKFNAWLKFNAWLKFNAWLFKN";
        }
    }else{
        Edit_ex edit_ex_form(select_item, parent_item, this);
        if(edit_ex_form.exec()){
            ex = edit_ex_form.get_ex();
        }
    }
    QJsonArray json_all_ex = get_all_ex();
    //Задание уникального id созданному заданию
    QVector<bool> free_slot(1000,true);
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
    //Добавление задания в бд
    qDebug() << ex;
    add_ex_to_bd(ex);
}

void MainWindow::slot_current_index_model(const QModelIndex &item)
{
    //Получение id задания, при нажатии на него в таблице
    QJsonArray all_ex = get_all_ex();
    QJsonValue single_ex;
    for(int i = 0; i < all_ex.size(); i++){
        single_ex = all_ex[i];
        if(single_ex["id"].toInt() == model->item(item.row(),3)->text().toInt()){
            selected_ex_id = single_ex["id"].toInt();
            break;
        }
    }

}

void MainWindow::slot_edit_current_ex()
{
    //Редактирование задания
    //Определение выбранного задания
    QJsonArray all_ex = get_all_ex();
    QJsonValue single_ex;
    int i = 0;
    for(;i < all_ex.size(); i++){
        single_ex = all_ex[i];
        if(single_ex["id"].toInt() == selected_ex_id){
            break;
        }
    }
    if(single_ex["type"].toInt() > 6){
        View_28line ex_form(single_ex.toObject(), this);
        if(ex_form.exec()){

        }
    }else{
        //Вызов формы для редактирования
        Edit_ex edit_ex_form(single_ex["type"].toInt(), single_ex["parent_type"].toInt(), this);
        edit_ex_form.set_ex(single_ex);
        //Если пользователь нажал сохранить, то в бд и таблице изменится задание
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
}

void MainWindow::slot_clone_current_ex()
{
    //Дублирование задания
    //Получение выбранного задания для дублирования
    QJsonArray all_ex = get_all_ex();
    QJsonObject single_ex;
    int i = 0;
    for(;i < all_ex.size(); i++){
        single_ex = all_ex[i].toObject();
        if(single_ex["id"].toInt() == selected_ex_id){
            break;
        }
    }
    //Задается новый id для дублированного задания
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



