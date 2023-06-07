
#ifndef GEN_28LINE_H
#define GEN_28LINE_H

#include <QDialog>
#include <QWebEngineView>
#include <QRandomGenerator>
#include <QJsonObject>
#include "table_gen_code.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Gen_28line; }
QT_END_NAMESPACE

class Gen_28line : public QDialog

{
    Q_OBJECT

public:
    Gen_28line(QWidget *parent = nullptr);

    ~Gen_28line();

private:
    Ui::Gen_28line *ui;
    QString nucleotides = "АТГЦ";
    QJsonObject gen_28line_ex();
    QString get_codogenic_chain();
    QString get_transcribed_chain(QString codogenic_chain);
    QString get_iRNA_chain(QString transcribed_chain);
    bool check_stop_codon(QString single_triplet);
    QChar get_complementarity_DNA(QChar nucleotide);
    QChar get_complementarity_iRNA(QChar nucleotide);
    QString get_polypeptide(QString iRNA_chain);
    Table_gen_code table;


};

#endif // GEN_28LINE_H
