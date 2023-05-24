#include "select_type_ex.h"
#include "ui_select_type_ex.h"

Select_type_ex::Select_type_ex(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Select_type_ex)
{
    ui->setupUi(this);
    connect(ui->treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), SLOT(slot_choose_item(QTreeWidgetItem*, int)));
    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), SLOT(slot_selected_item(QTreeWidgetItem*, int)));
}

Select_type_ex::~Select_type_ex()
{
    delete ui;
}

void Select_type_ex::send_data(QString item_text, QString parent_text)
{
    qDebug() << parent_text;
    if(item_text == "Числовой ответ" || item_text == "Числовой ответ с рисунком"){
        emit signal_send_data(1,1);
    }else if(item_text == "Строковый ответ" ||
             item_text == "Строковый ответ с рисунком" ||
             (item_text == "Вставить пропущенное слово" && parent_text == "Строковый ответ")){
        emit signal_send_data(2,2);
    }else if(item_text == "Сопоставление" || item_text == "Сопоставление с рисунком"){
        emit signal_send_data(3,3);
    }else if(item_text == "Последовательность"){
        emit signal_send_data(4,1);
    }else if(item_text == "Вставить пропущенное слово" && parent_text == "Табличный ответ"){
        emit signal_send_data(5,3);
    }else{
        emit signal_send_data(6,4);
    }
    this->close();
}

void Select_type_ex::slot_choose_item(QTreeWidgetItem* item, int column)
{
    for(int i = 0; i < ui->treeWidget->topLevelItemCount(); i++){
        if(item->text(0) == ui->treeWidget->topLevelItem(i)->text(0)){
            item = ui->treeWidget->topLevelItem(i)->child(0);
            break;
        }
    }
    send_data(item->text(0), item->parent()->text(0));

}

void Select_type_ex::slot_selected_item(QTreeWidgetItem *item, int column)
{
    for(int i = 0; i < ui->treeWidget->topLevelItemCount(); i++){
        if(ui->treeWidget->topLevelItem(i)->isSelected()){
            item->child(0)->setSelected(true);
        }
    }
}
