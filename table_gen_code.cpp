
#include "table_gen_code.h"

Table_gen_code::Table_gen_code()
{
    init_table_gen_code_iRNA();
}

QString Table_gen_code::get_amino_acid(QString triplet)
{
    return table_gen_code_iRNA[get_index_table(triplet[0])][get_index_table(triplet[1])][get_index_table(triplet[2])];
}

void Table_gen_code::init_table_gen_code_iRNA()
{
    table_gen_code_iRNA[0][0][0] = "Фен";
    table_gen_code_iRNA[0][0][1] = "Фен";
    table_gen_code_iRNA[0][0][2] = "Лей";
    table_gen_code_iRNA[0][0][3] = "Лей";

    table_gen_code_iRNA[0][1][0] = "Сер";
    table_gen_code_iRNA[0][1][1] = "Сер";
    table_gen_code_iRNA[0][1][2] = "Сер";
    table_gen_code_iRNA[0][1][3] = "Сер";

    table_gen_code_iRNA[0][2][0] = "Тир";
    table_gen_code_iRNA[0][2][1] = "Тир";
    table_gen_code_iRNA[0][2][2] = "---";
    table_gen_code_iRNA[0][2][3] = "---";

    table_gen_code_iRNA[0][3][0] = "Цис";
    table_gen_code_iRNA[0][3][1] = "Цис";
    table_gen_code_iRNA[0][3][2] = "---";
    table_gen_code_iRNA[0][3][3] = "Три";

    table_gen_code_iRNA[1][0][0] = "Лей";
    table_gen_code_iRNA[1][0][1] = "Лей";
    table_gen_code_iRNA[1][0][2] = "Лей";
    table_gen_code_iRNA[1][0][3] = "Лей";

    table_gen_code_iRNA[1][1][0] = "Про";
    table_gen_code_iRNA[1][1][1] = "Про";
    table_gen_code_iRNA[1][1][2] = "Про";
    table_gen_code_iRNA[1][1][3] = "Про";

    table_gen_code_iRNA[1][2][0] = "Гис";
    table_gen_code_iRNA[1][2][1] = "Гис";
    table_gen_code_iRNA[1][2][2] = "Глн";
    table_gen_code_iRNA[1][2][3] = "Глн";

    table_gen_code_iRNA[1][3][0] = "Арг";
    table_gen_code_iRNA[1][3][1] = "Арг";
    table_gen_code_iRNA[1][3][2] = "Арг";
    table_gen_code_iRNA[1][3][3] = "Арг";

    table_gen_code_iRNA[2][0][0] = "Иле";
    table_gen_code_iRNA[2][0][1] = "Иле";
    table_gen_code_iRNA[2][0][2] = "Иле";
    table_gen_code_iRNA[2][0][3] = "Мет";

    table_gen_code_iRNA[2][1][0] = "Тре";
    table_gen_code_iRNA[2][1][1] = "Тре";
    table_gen_code_iRNA[2][1][2] = "Тре";
    table_gen_code_iRNA[2][1][3] = "Тре";

    table_gen_code_iRNA[2][2][0] = "Асн";
    table_gen_code_iRNA[2][2][1] = "Асн";
    table_gen_code_iRNA[2][2][2] = "Лиз";
    table_gen_code_iRNA[2][2][3] = "Лиз";

    table_gen_code_iRNA[2][3][0] = "Сер";
    table_gen_code_iRNA[2][3][1] = "Сер";
    table_gen_code_iRNA[2][3][2] = "Арг";
    table_gen_code_iRNA[2][3][3] = "Арг";

    table_gen_code_iRNA[3][0][0] = "Вал";
    table_gen_code_iRNA[3][0][1] = "Вал";
    table_gen_code_iRNA[3][0][2] = "Вал";
    table_gen_code_iRNA[3][0][3] = "Вал";

    table_gen_code_iRNA[3][1][0] = "Ала";
    table_gen_code_iRNA[3][1][1] = "Ала";
    table_gen_code_iRNA[3][1][2] = "Ала";
    table_gen_code_iRNA[3][1][3] = "Ала";

    table_gen_code_iRNA[3][2][0] = "Асп";
    table_gen_code_iRNA[3][2][1] = "Асп";
    table_gen_code_iRNA[3][2][2] = "Глу";
    table_gen_code_iRNA[3][2][3] = "Глу";

    table_gen_code_iRNA[3][3][0] = "Гли";
    table_gen_code_iRNA[3][3][1] = "Гли";
    table_gen_code_iRNA[3][3][2] = "Гли";
    table_gen_code_iRNA[3][3][3] = "Гли";
}

int Table_gen_code::get_index_table(QChar nucleotide)
{
    QString temp_str = "УЦАГ";
    return temp_str.indexOf(nucleotide);
}

