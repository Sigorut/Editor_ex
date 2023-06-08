
#include "gen_28line.h"
#include "ui_gen_28line.h"


Gen_28line::Gen_28line(int name_ex, int parent_item, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Gen_28line)
{
    ui->setupUi(this);
    this->name_ex = name_ex;
    this->parent_item = parent_item;
    for(int i = 0; i < 3; i++){
        forms[i] = new QWebEngineView();
        forms[i]->load(QUrl("qrc:/28line.html"));

        butt_forms[i] = new QRadioButton();
        butt_forms[i]->setMaximumWidth(30);
        ui->gridLayout_3->addWidget(forms[i], i, 0);
        ui->gridLayout_3->addWidget(butt_forms[i], i, 1);
        connect(forms[i], SIGNAL(loadFinished(bool)), SLOT(slot_set_data_to_form()));
    }
    butt_forms[0]->setChecked(true);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Сохранить");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Отмена");
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), SLOT(accept()));
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), SLOT(reject()));
    connect(ui->butt_refresh, SIGNAL(clicked()), SLOT(slot_set_data_to_form()));

}

Gen_28line::~Gen_28line()
{
    delete ui;
}

QString Gen_28line::get_codogenic_chain()
{
    QString codogenic_chain;
    QString single_triplet;
    int count_triplets = QRandomGenerator::global()->bounded(5, 7);
    QString temp = "АГЦ";
    for(int i = 0; i < count_triplets; i++){
        single_triplet.clear();
        for(int j = 0; j < 3; j++){
            single_triplet.append(nucleotides[QRandomGenerator::global()->bounded(0, 4)]);
        }
        if(!check_stop_codon(single_triplet)){
            single_triplet[0] = temp[QRandomGenerator::global()->bounded(0, 3)];
        }
        codogenic_chain.append(single_triplet);
    }
    return codogenic_chain;
}

bool Gen_28line::check_stop_codon(QString single_triplet)
{
    if(single_triplet == "ТАА" || single_triplet == "ТАГ" || single_triplet == "ТГА"){
        return false;
    }
    return true;
}

QChar Gen_28line::get_complementarity_DNA(QChar nucleotide)
{
    if(nucleotide == QChar(u'А')){
        return QChar(u'Т');
    }
    if(nucleotide == QChar(u'Т')){
        return QChar(u'А');
    }
    if(nucleotide == QChar(u'Г')){
        return QChar(u'Ц');
    }
    if(nucleotide == QChar(u'Ц')){
        return QChar(u'Г');
    }
    return QChar(' ');
}

QChar Gen_28line::get_complementarity_iRNA(QChar nucleotide)
{
    if(nucleotide == QChar(u'А')){
        return QChar(u'У');
    }
    if(nucleotide == QChar(u'Т')){
        return QChar(u'А');
    }
    if(nucleotide == QChar(u'Г')){
        return QChar(u'Ц');
    }
    if(nucleotide == QChar(u'Ц')){
        return QChar(u'Г');
    }
    return QChar(' ');
}

QString Gen_28line::get_polypeptide(QString iRNA_chain)
{
    QString polypeptide;
    QString triplet;
    for(int i = 0; i < iRNA_chain.size(); i+=3){
        triplet = iRNA_chain.right(iRNA_chain.size() - i).left(3);
        polypeptide += table.get_amino_acid(triplet);
        if(i < iRNA_chain.size()-4){
            polypeptide += "-";
        }
    }
    return polypeptide;
}

void Gen_28line::slot_set_data_to_form()
{
    for(int i = 0; i < 3; i++){
        fields_for_forms[i] = gen_28line_ex();
        bool flg = false;
        forms[i]->page()->runJavaScript("set_fields(\"" + fields_for_forms[i].codogenic_chain + "\",\"" +
                                        fields_for_forms[i].transcribed_chain + "\",\"" +
                                        fields_for_forms[i].iRNA_chain + "\",\"" +
                                        fields_for_forms[i].polypeptide + "\")",
                [&](QVariant result)->void{
            flg = true;
        });
        while(!flg){
            QApplication::processEvents();
        }
    }
}

QJsonObject Gen_28line::get_ex()
{
    for(int i = 0; i < 3; i++){
        if(butt_forms[i]->isChecked()){
            ex.insert("text_ex", "Определите последовательность аминокислот во фрагменте полипептидной цепи.");
            ex.insert("iRNA", fields_for_forms[i].iRNA_chain);
            ex.insert("codogenic", fields_for_forms[i].codogenic_chain);
            ex.insert("transcribed", fields_for_forms[i].transcribed_chain);
            ex.insert("polypeptide", fields_for_forms[i].polypeptide);
            QString path_to_image = ":/table_gen_code.jpg";
            QFile file(path_to_image);
            QByteArray data;
            file.open(QIODevice::ReadOnly);
            data = file.readAll();
            QByteArray base64 = data.toBase64();
            QStringList type;
            type = path_to_image.split(".");
            ex.insert("image_type", type[1]);
            ex.insert("image", QString(base64));
            ex.insert("type", name_ex);
            ex.insert("parent_type", parent_item);
        }
    }
    qDebug() << ex;
    return ex;
}

QString Gen_28line::get_transcribed_chain(QString codogenic_chain)
{
    QString transcribed_chain;
    for(int i = 0; i < codogenic_chain.size(); i++){
        transcribed_chain.append(get_complementarity_DNA(codogenic_chain[i]));
    }
    return transcribed_chain;
}

QString Gen_28line::get_iRNA_chain(QString transcribed_chain)
{
    QString iRNA_chain;
    for(int i = 0; i < transcribed_chain.size(); i++){
        iRNA_chain.append(get_complementarity_iRNA(transcribed_chain[i]));
    }
    return iRNA_chain;
}


Chains Gen_28line::gen_28line_ex()
{
    QString codogenic_chain = get_codogenic_chain();
    QString transcribed_chain = get_transcribed_chain(codogenic_chain);
    QString iRNA_chain = get_iRNA_chain(transcribed_chain);
    QString polypeptide = get_polypeptide(iRNA_chain);

    qDebug() << codogenic_chain << "\n" << transcribed_chain << "\n" << iRNA_chain<< "\n" << polypeptide;
    return Chains(codogenic_chain, transcribed_chain, iRNA_chain, polypeptide);
}


