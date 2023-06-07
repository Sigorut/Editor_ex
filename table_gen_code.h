#ifndef TABLE_GEN_CODE_H
#define TABLE_GEN_CODE_H
#include <QString>
#include <QChar>


class Table_gen_code
{
public:
    Table_gen_code();
    QString get_amino_acid(QString triplet);
private:
    void init_table_gen_code_iRNA();
    QString table_gen_code_iRNA[4][4][4];
    int get_index_table(QChar nucleotide);


};

#endif // TABLE_GEN_CODE_H
