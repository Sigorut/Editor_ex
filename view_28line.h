#ifndef VIEW_28LINE_H
#define VIEW_28LINE_H

#include <QDialog>
#include <QWebEngineView>
#include <QPushButton>
#include <QSpacerItem>
#include <QJsonValue>
#include <QByteArray>
#include <QJsonObject>
#include <QBuffer>

namespace Ui {
class View_28line;
}

class View_28line : public QDialog
{
    Q_OBJECT

public:
    explicit View_28line(QJsonObject ex, QWidget *parent = nullptr);
    ~View_28line();

private:
    Ui::View_28line *ui;
    QWebEngineView *form;
    QPushButton *ok;
    QJsonObject ex;
    void set_field_image();
    void set_field_string(QString value, QString field_id);
    void set_field_value(QString value, QString field_id);
private slots:
    void slot_set_data();
};

#endif // VIEW_28LINE_H
