#include "edit_ex.h"
#include "ui_edit_ex.h"

Edit_ex::Edit_ex(int name_ex, int parent_item, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Edit_ex)
{
    ui->setupUi(this);
    this->name_ex = name_ex;
    this->parent_item = parent_item;
    form = new QWebEngineView();
    buttons = new QDialogButtonBox();
    ok_butt = new QPushButton();
    cancel_butt = new QPushButton();
    ok_butt->setText("Сохранить");
    cancel_butt->setText("Отмена");
    buttons->addButton(ok_butt, QDialogButtonBox::AcceptRole);
    buttons->addButton(cancel_butt, QDialogButtonBox::RejectRole);
    set_view();
    ui->verticalLayout->addWidget(form);
    ui->verticalLayout_t->addWidget(buttons);
    connect(ok_butt, SIGNAL(clicked()), SLOT(slot_get_data_form()));
    connect(ui->path_butt, SIGNAL(clicked()), SLOT(slot_get_path_image()));
    connect(cancel_butt, SIGNAL(clicked()), SLOT(reject()));
    connect(this, SIGNAL(signal_accept()), SLOT(accept()));


}

Edit_ex::~Edit_ex()
{
    delete form;
    delete ui;
}

void Edit_ex::set_view()
{
    switch (name_ex) {
    case Ex::number:
        form->load(QUrl("qrc:/number_response.html"));
        break;
    case Ex::string:
        form->load(QUrl("qrc:/string_response.html"));
        break;
    case Ex::comp:
        form->load(QUrl("qrc:/table_comp_response.html"));
        break;
    case Ex::seq:
        form->load(QUrl("qrc:/table_seq_response.html"));
        break;
    case Ex::tablword:
        form->load(QUrl("qrc:/table_word_response.html"));
        break;
    case Ex::treetf:
        form->load(QUrl("qrc:/threetf.html"));
        break;
    default:
        break;
    }
}

void Edit_ex::set_data_number_string_ex(QJsonValue selected_ex)
{
    set_field_string(selected_ex["text_ex"].toString(), "input_text_ex2");
    set_field_string(selected_ex["solution"].toString(), "solution_text_ex2");
    qDebug() << name_ex << Ex::string;
    switch (name_ex) {
    case Ex::number:
        set_field_int(selected_ex["answer"].toString().toInt(), "answer2");
        break;
    case Ex::string:
        set_field_string(selected_ex["answer"].toString(), "answer2");
        break;
    default:
        break;
    }
}

void Edit_ex::set_data_fields_table_comp_ex(QJsonValue selected_ex)
{
    bool flg = false;
    form->page()->runJavaScript("set_fields(" + QString::number(selected_ex["answer"].toArray().size()) + ")",
            [&](QVariant result)->void{
        flg = true;
    });
    while(!flg){
        QApplication::processEvents();
    }
    QJsonArray arr_answ = selected_ex["answer"].toArray();
    QJsonObject single_answ;
    for(int i = 0; i < arr_answ.size(); i++){
        single_answ = arr_answ[i].toObject();
        foreach(const QString& key, single_answ.keys()) {
            QJsonValue value = single_answ.value(key);
            set_field_string(key, "input_A_" + QString::number(i));
            set_field_string(value.toString(), "input_1_" + QString::number(i));
        }
    }
    set_field_int(selected_ex["answer"].toArray().size(), "count_options3");
    set_field_string(selected_ex["text_ex"].toString(), "input_text_ex3");
    set_field_string(selected_ex["solution"].toString(), "solution_text_ex3");
}

void Edit_ex::set_data_fields_seq_ex(QJsonValue selected_ex)
{
    bool flg = false;
    form->page()->runJavaScript("set_fields(" + QString::number(selected_ex["options"].toArray().size()) + ")",
            [&](QVariant result)->void{
        flg = true;
    });
    while(!flg){
        QApplication::processEvents();
    }
    QJsonArray arr_options = selected_ex["options"].toArray();
    QJsonObject single_option;
    for(int i = 0; i < arr_options.size(); i++){
        single_option = arr_options[i].toObject();
        foreach(const QString& key, single_option.keys()) {
            QJsonValue value = single_option.value(key);
            set_field_string(value.toString(), "input_1_" + QString::number(i));
        }
        //set_field_string(options[i]., "input_1_" + QString::number(i+1));
    }
    set_field_int(selected_ex["options"].toArray().size(), "count_options5");
    set_field_int(selected_ex["answer"].toString().toInt(), "answer5");
    set_field_string(selected_ex["text_ex"].toString(), "input_text_ex5");
    set_field_string(selected_ex["solution"].toString(), "solution_text_ex5");
}

void Edit_ex::set_data_fields_tablword_ex(QJsonValue selected_ex)
{
    int rows = selected_ex["count_rows"].toInt();
    int cols = selected_ex["count_cols"].toInt();
    bool flg = false;
    form->page()->runJavaScript("set_fields(" + QString::number(rows) + "," +
                                QString::number(cols) + ")",
                                [&](QVariant result)->void{
        flg = true;
    });
    while(!flg){
        QApplication::processEvents();
    }
    QJsonObject arr_table = selected_ex["table"].toObject();
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(arr_table[QString::number(i) + QString::number(j)].toString() != "~ans~"){
                set_field_string(arr_table[QString::number(i) + QString::number(j)].toString(), "input_" + QString::number(i) + QString::number(j));
            }
        }
    }
    set_field_int(rows, "count_rows3");
    set_field_int(cols, "count_cols3");
    set_field_string(selected_ex["answer"].toString(), "answer3");
    set_field_string(selected_ex["text_ex"].toString(), "input_text_ex3");
    set_field_string(selected_ex["solution"].toString(), "solution_text_ex3");
}

void Edit_ex::set_data_fields_threetf(QJsonValue selected_ex)
{
    bool flg = false;
    form->page()->runJavaScript("set_fields(" + QString::number(selected_ex["options"].toArray().size()) + ")",
            [&](QVariant result)->void{
        flg = true;
    });
    while(!flg){
        QApplication::processEvents();
    }
    QJsonArray options = selected_ex["options"].toArray();
    QJsonArray answers = selected_ex["answer"].toArray();
    QStringList list_options, list_answers;
    for(int i = 0; i < options.size(); i++){
        list_options << options[i].toString();
    }
    for(int i = 0; i < answers.size(); i++){
        list_answers << answers[i].toString();
    }
    qDebug() << list_answers << list_options;
    for(int i = 0; i < list_options.size(); i++){
        set_field_string(list_options[i], "input_text_" + QString::number(i));
        for(int j = 0; j < list_answers.size(); j++){
            if(list_answers[j] == list_options[i]){
                bool flg_temp = false;
                form->page()->runJavaScript("document.getElementById(\"checkbox_input_" + QString::number(i) + "\").checked = true;",
                                            [&](QVariant result)->void{
                    flg_temp = true;
                });
                while(!flg_temp){
                    QApplication::processEvents();
                }
            }
        }
    }
    //input_text_
    //checkbox_input_
    set_field_int(selected_ex["options"].toArray().size(), "count_options4");
    set_field_string(selected_ex["text_ex"].toString(), "input_text_ex4");
    set_field_string(selected_ex["solution"].toString(), "solution_text_ex4");
}


void Edit_ex::set_ex(QJsonValue selected_ex)
{
    ex = selected_ex.toObject();
    connect(form, SIGNAL(loadFinished(bool)), SLOT(slot_push_data_to_form()));
}

void Edit_ex::get_data_number_string_ex()
{
    QString text_ex, solution, answer;

    parse_field_string(text_ex, "input_text_ex2");
    parse_field_string(answer, "answer2");
    parse_field_string(solution, "solution_text_ex2");
    ex.insert("text_ex", text_ex);
    ex.insert("answer", answer);
    ex.insert("solution", solution);
    ex.insert("type", name_ex);
    ex.insert("parent_type", parent_item);

    emit signal_accept();
}

void Edit_ex::get_data_threetf_ex()
{
    int count_options = 0;
    QString text_ex, solution, single_options, single_checkbox_value;

    QJsonArray options, answer;
    //Тянем задание, количество вариантов и обьяснение
    parse_field_int(count_options, "count_options4");
    parse_field_string(text_ex, "input_text_ex4");
    parse_field_string(solution, "solution_text_ex4");
    //Тянем варианты и правильные ответы
    for(int i = 0; i < count_options; i++){
        qDebug() << single_checkbox_value;
        parse_field_string(single_options, "input_text_" + QString::number(i));
        form->page()->runJavaScript("document.getElementById(\"checkbox_input_" + QString::number(i) + "\").checked;",
                                    [&](QVariant result)->void{
            single_checkbox_value = result.toString();
        });
        while(single_checkbox_value.isEmpty()){
            QApplication::processEvents();
        }
        qDebug() << single_checkbox_value;
        if(single_checkbox_value == "true"){
            answer << single_options;
        }
        options << single_options;
        single_options.clear();
        single_checkbox_value.clear();
    }
    ex.insert("text_ex", text_ex);
    ex.insert("solution", solution);
    ex.insert("type", name_ex);
    ex.insert("parent_type", parent_item);
    ex.insert("options", options);
    ex.insert("answer", answer);

    emit signal_accept();
}


void Edit_ex::get_data_comp_ex()
{
    QJsonObject single_answer;
    QJsonArray answer;
    int count_options = 0;
    QString text_ex, solution, single_option_str, single_answer_str;
    parse_field_string(text_ex, "input_text_ex3");
    parse_field_string(solution, "solution_text_ex3");
    parse_field_int(count_options, "count_options3");

    for(int i = 0; i < count_options; i++){
        parse_field_string(single_option_str, "input_A_" + QString::number(i));
        parse_field_string(single_answer_str, "input_1_" + QString::number(i));
        single_answer.insert(single_option_str, single_answer_str);
        answer << single_answer;
        single_answer = QJsonObject();
        single_answer_str.clear();
        single_option_str.clear();
    }
    ex.insert("answer", answer);
    ex.insert("text_ex", text_ex);
    ex.insert("parent_type", parent_item);
    ex.insert("type", name_ex);
    ex.insert("solution", solution);

    emit signal_accept();
}

void Edit_ex::get_data_seq_ex()
{
    QJsonObject single_option;
    QJsonArray options;
    int count_options = 0;
    QString text_ex, solution, single_option_str, answer_str;
    parse_field_string(text_ex, "input_text_ex5");
    parse_field_string(answer_str, "answer5");
    parse_field_string(solution, "solution_text_ex5");
    parse_field_int(count_options, "count_options5");
    for(int i = 0; i < count_options; i++){
        parse_field_string(single_option_str, "input_1_" + QString::number(i));
        single_option.insert(QString::number(i+1), single_option_str);
        options << single_option;
        single_option = QJsonObject();
        single_option_str.clear();
    }
    ex.insert("text_ex", text_ex);
    ex.insert("parent_type", parent_item);
    ex.insert("type", name_ex);
    ex.insert("options", options);
    ex.insert("answer", answer_str);
    ex.insert("solution", solution);

    emit signal_accept();

}

void Edit_ex::get_data_tablword_ex()
{
    QJsonObject table;
    int count_rows = 0, count_cols = 0;
    QString text_ex, solution, table_item, answer;
    parse_field_int(count_rows, "count_rows3");
    parse_field_int(count_cols, "count_cols3");
    parse_field_string(text_ex, "input_text_ex3");
    parse_field_string(solution, "solution_text_ex3");
    parse_field_string(answer, "answer3");
    for(int i = 0; i < count_rows; i++){
        for(int j = 0; j < count_cols; j++){
            parse_field_string(table_item, "input_" + QString::number(i) + QString::number(j));
            if(table_item.isEmpty()){
                table.insert(QString::number(i) + QString::number(j), "~ans~");
            }else{
                table.insert(QString::number(i) + QString::number(j), table_item);
            }
        }
    }
    ex.insert("count_rows", count_rows);
    ex.insert("count_cols", count_cols);
    ex.insert("text_ex", text_ex);
    ex.insert("solution", solution);
    ex.insert("type", name_ex);
    ex.insert("parent_type", parent_item);
    ex.insert("answer", answer);
    ex.insert("table", table);
    emit signal_accept();
}

QJsonObject Edit_ex::get_ex()
{
    return ex;
}

void Edit_ex::slot_get_data_form()
{
    if(!path_to_image.isEmpty()){
        QFile file(path_to_image);
        QByteArray data;
        file.open(QIODevice::ReadOnly);
        data = file.readAll();
        QByteArray base64 = data.toBase64();
        QStringList type;
        type = path_to_image.split(".");

        ex.insert("image_type", type[1]);
        ex.insert("image", QString(base64));
    }

    switch (name_ex) {
    case Ex::number:
        get_data_number_string_ex();
        break;
    case Ex::string:
        get_data_number_string_ex();
        break;
    case Ex::comp:
        get_data_comp_ex();
        break;
    case Ex::seq:
        get_data_seq_ex();
        break;
    case Ex::tablword:
        get_data_tablword_ex();
        break;
    case Ex::treetf:
        get_data_threetf_ex();
        break;
    default:
        break;
    }
}

void Edit_ex::slot_push_data_to_form()
{

        QByteArray base64 = ex.value("image").toString().toUtf8();
        if(base64.size()!= 0){
            QPixmap testt;
            testt.loadFromData(QByteArray::fromBase64(base64), ex.value("image_type").toString().toLocal8Bit().data());
            ui->label_2->setPixmap(testt.scaled(150, 150, Qt::KeepAspectRatio,  Qt::SmoothTransformation));
        }

    switch (name_ex) {
    case Ex::number:
        set_data_number_string_ex(ex);
        break;
    case Ex::string:
        set_data_number_string_ex(ex);
        break;
    case Ex::comp:
        set_data_fields_table_comp_ex(ex);
        break;
    case Ex::seq:
        set_data_fields_seq_ex(ex);
        break;
    case Ex::tablword:
        set_data_fields_tablword_ex(ex);
        break;
    case Ex::treetf:
        set_data_fields_threetf(ex);
        break;
    default:
        break;
    }
}

void Edit_ex::slot_get_path_image()
{
    QString temp = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                "/",
                                                tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
    if(!temp.isEmpty()){
        path_to_image = temp;
        ui->lineEdit_path->setText(path_to_image);

        ui->label_2->setPixmap(QPixmap(temp).scaled(150, 150, Qt::KeepAspectRatio,  Qt::SmoothTransformation));
    }
}

void Edit_ex::set_field_string(QString value, QString field_id)
{
    bool flag_finish = false;
    qDebug() << field_id;
    form->page()->runJavaScript("document.getElementById(\"" + field_id + "\").value += \"" + value + "\";",
                                [&](QVariant result)->void{
        flag_finish = true;
    });
    while(!flag_finish){
        QApplication::processEvents();
    }
}

void Edit_ex::set_field_int(int value, QString field_id)
{
    bool flag_finish = false;
    qDebug() << field_id;
    form->page()->runJavaScript("document.getElementById(\"" + field_id + "\").value = " + QString::number(value) + ";",
                                [&](QVariant result)->void{
        flag_finish = true;
    });
    while(!flag_finish){
        QApplication::processEvents();
    }
}

void Edit_ex::parse_field_string(QString &value, QString field_id)
{
    value.clear();
    value = "~~";
    form->page()->runJavaScript("document.getElementById(\"" + field_id + "\").value;",
                                [&](QVariant result)->void{
        value = result.toString().toHtmlEscaped();
        if(value.contains("\n")){
            QStringList temp;
            temp = value.split("\n");
            value.clear();
            for(int i = 0; i < temp.size(); i++){
                value += temp[i];
                if(i != temp.size()-1){
                    value += + "\\n";
                }
            }
        }
    });
    while(value == "~~"){
        QApplication::processEvents();
    }
}

void Edit_ex::parse_field_int(int &value, QString field_id)
{
    value = 0;
    form->page()->runJavaScript("document.getElementById(\"" + field_id + "\").value;",
                                [&](QVariant result)->void{
        value = result.toInt();
    });
    while(!value){
        QApplication::processEvents();
    }
}
