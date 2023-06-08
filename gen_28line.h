
#ifndef GEN_28LINE_H
#define GEN_28LINE_H

#include <QDialog>
#include <QWebEngineView>
#include <QRandomGenerator>
#include <QJsonObject>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QRadioButton>
#include <QFile>
#include "table_gen_code.h"

struct Chains
{
    QString codogenic_chain, transcribed_chain, iRNA_chain, polypeptide;
    Chains(){}
    Chains(QString codogenic_chain, QString transcribed_chain, QString iRNA_chain, QString polypeptide) {
        this->codogenic_chain = codogenic_chain;
        this->transcribed_chain = transcribed_chain;
        this->iRNA_chain = iRNA_chain;
        this->polypeptide = polypeptide;
    }
};

QT_BEGIN_NAMESPACE
namespace Ui { class Gen_28line; }
QT_END_NAMESPACE

class Gen_28line : public QDialog

{
    Q_OBJECT

public:
    Gen_28line(int name_ex, int parent_item, QWidget *parent = nullptr);
    QJsonObject get_ex();
    ~Gen_28line();

private:
    Ui::Gen_28line *ui;
    int name_ex;
    int parent_item;
    QWebEngineView *forms[3];
    QRadioButton *butt_forms[3];
    Chains fields_for_forms[3];

    QString nucleotides = "АТГЦ";
    QJsonObject ex;
    Chains gen_28line_ex();
    QString get_codogenic_chain();
    QString get_transcribed_chain(QString codogenic_chain);
    QString get_iRNA_chain(QString transcribed_chain);
    bool check_stop_codon(QString single_triplet);
    QChar get_complementarity_DNA(QChar nucleotide);
    QChar get_complementarity_iRNA(QChar nucleotide);
    QString get_polypeptide(QString iRNA_chain);
    Table_gen_code table;
private slots:
    void slot_set_data_to_form();
};

#endif // GEN_28LINE_H
