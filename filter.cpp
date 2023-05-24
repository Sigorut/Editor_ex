#include "filter.h"
#include "ui_filter.h"

Filter::Filter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Filter)
{
    ui->setupUi(this);
    connect(ui->treeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this,SLOT(treeItemChanged(QTreeWidgetItem*, int )));
    clone_tree = new QTreeWidget;
    //clone_tree_widget();
}

void Filter::treeItemChanged(QTreeWidgetItem *item, int column)
{

    if(Qt::Checked == item->checkState(0))//The parent node is selected, and all child nodes are selected
    {
        int count = item->childCount();
        if(count>0)
        {
            for(int i=0;i<count;i++)
            {
                item->child(i)->setCheckState(0,Qt::Checked);
            }
        }
        else
        {
            updateParentItem(item);//Change the parent node
        }
    }
    else if(Qt::Unchecked==item->checkState(0))//Uncheck the parent node, uncheck all the child nodes
    {
        int count = item->childCount();
        if(count>0)
        {
            for(int i=0;i<count;i++)
            {
                item->child(i)->setCheckState(0,Qt::Unchecked);
            }
        }
        else
        {
            updateParentItem(item);
        }
    }
}

void Filter::updateParentItem(QTreeWidgetItem *item)
{
    QTreeWidgetItem* parent=item->parent();//Get the parent node
    if(parent==NULL)
    {
        return;
    }
    int selectedCount=0;
    int childCount = parent->childCount();
    for(int i=0;i<childCount;i++)//Check if all child nodes are selected
    {
        QTreeWidgetItem*childItem=parent->child(i);
        if(childItem->checkState(0)==Qt::Checked)
        {
            selectedCount++;
        }
    }
    if(selectedCount<=0)
    {
        parent->setCheckState(0,Qt::Unchecked);//The parent node is not selected
    }
    else if(selectedCount>0 && selectedCount<childCount)
    {
        parent->setCheckState(0,Qt::PartiallyChecked);//The parent node is partially selected
    }
    else if(selectedCount==childCount)
    {
        parent->setCheckState(0,Qt::Checked);//The parent node is selected
    }
}

Filter::~Filter()
{
    delete ui;
}

//void Filter::clone_tree_widget()
//{
//    for(int i = 0; i < ui->treeWidget->topLevelItemCount(); i++){
////        ui->treeWidget->top
//        QTreeWidgetItem *item;
//        item = ui->treeWidget->topLevelItem(i);
//        clone_tree->addTopLevelItem(item);
//        qDebug() << clone_tree->topLevelItem(i)->text(0);
//        for(int j = 0; j < ui->treeWidget->topLevelItem(i)->childCount(); j++){
//            //clone_tree->topLevelItem(i)->addChild(ui->treeWidget->topLevelItem(i)->child(j));
//        }
//    }
//   // ui->gridLayout->addWidget(&clone_tree,2,0);
//}


void Filter::on_buttonBox_accepted()
{
    emit signal_send_data("Hello");
}

void Filter::on_buttonBox_rejected()
{
    //ui->treeWidget->clear();
}

