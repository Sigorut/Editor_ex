
#include "gen_28line.h"
#include "ui_gen_28line.h"


Gen_28line::Gen_28line(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Gen_28line)
{
    ui->setupUi(this);
    qDebug() << gen_28line_ex();
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
    if(single_triplet == "ТАА" || single_triplet == "ТАТ" || single_triplet == "ТГА"){
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


QJsonObject Gen_28line::gen_28line_ex()
{
    QJsonObject ex;
    QString codogenic_chain = get_codogenic_chain();
    QString transcribed = get_transcribed_chain(codogenic_chain);
    QString iRNA_chain = get_iRNA_chain(transcribed);
    QString polypeptide = get_polypeptide(iRNA_chain);
    qDebug() << codogenic_chain << "\n" << transcribed << "\n" << iRNA_chain<< "\n" << polypeptide;
    return ex;
}


