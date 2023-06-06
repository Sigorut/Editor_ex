#ifndef EDIT_EX_H
#define EDIT_EX_H

#include <QDialog>
#include <QWebEngineView>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QJsonArray>
#include <QJsonObject>
#include <QFileDialog>
#include <QBuffer>

enum Ex{
    number = 1,
    string,
    comp,
    seq,
    tablword,
    treetf
};

namespace Ui {
class Edit_ex;
}

class Edit_ex : public QDialog
{
    Q_OBJECT

public:
    explicit Edit_ex(int name_ex, int parent_item, QWidget *parent = nullptr);
    QJsonObject get_ex();
    void set_ex(QJsonValue selected_ex);
    ~Edit_ex();

private:
    Ui::Edit_ex *ui;
    int name_ex;
    int parent_item;
    QString path_to_image;
    QJsonObject ex;
    QDialogButtonBox *buttons;
    QPushButton *cancel_butt;
    QPushButton *ok_butt;
    QWebEngineView *form;
    void set_view();

    void set_data_number_string_ex(QJsonValue selected_ex);
    void set_data_fields_table_comp_ex(QJsonValue selected_ex);
    void set_data_fields_seq_ex(QJsonValue selected_ex);
    void set_data_fields_tablword_ex(QJsonValue selected_ex);
    void set_data_fields_threetf(QJsonValue selected_ex);


    void get_data_number_string_ex();
    void get_data_comp_ex();
    void get_data_seq_ex();
    void get_data_tablword_ex();
    void get_data_threetf_ex();

    void set_field_string(QString value, QString field_id);
    void set_field_int(int value, QString field_id);

    void parse_field_string(QString &value ,QString field_id);
    void parse_field_int(int &value, QString field_id);
private slots:
    void slot_get_data_form();
    void slot_push_data_to_form();
    void slot_get_path_image();
signals:
    void signal_accept();
};

#endif // EDIT_EX_H
