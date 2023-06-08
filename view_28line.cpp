#include "view_28line.h"
#include "ui_view_28line.h"

View_28line::View_28line(QJsonObject ex, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::View_28line)
{
    ui->setupUi(this);
    this->ex = ex;
    form = new QWebEngineView();
    ok = new QPushButton();
    ok->setText("Ок");
    ui->verticalLayout->addWidget(form);
    ui->verticalLayout->addWidget(ok,0, Qt::AlignRight);
    form->load(QUrl("qrc:/28line.html"));
    connect(form, SIGNAL(loadFinished(bool)), SLOT(slot_set_data()));
    connect(ok, SIGNAL(clicked()), SLOT(accept()));
}

View_28line::~View_28line()
{
    delete ui;
}

void View_28line::slot_set_data()
{
    set_field_string(ex["text_ex"].toString(), "text_ex");
    qDebug() << ex["codogenic"].toString() << ex["transcribed"].toString();
    set_field_string("3\'- " + ex["codogenic"].toString() + " -5\'<br>", "text_data");
    set_field_string("5\'- " + ex["transcribed"].toString() + " -3\'", "text_data");
    set_field_value(ex["iRNA"].toString(), "answer_iRNA");
    set_field_value(ex["polypeptide"].toString(), "answer_poly");
    if(ex["image"].toString().size()){
        set_field_image();
    }
}

void View_28line::set_field_string(QString value, QString field_id)
{
    bool flag_finish = false;
    qDebug() << field_id;
    form->page()->runJavaScript("document.getElementById(\"" + field_id + "\").innerHTML += \"" + value + "\";",
                                [&](QVariant result)->void{
        flag_finish = true;
    });
    while(!flag_finish){
        QApplication::processEvents();
    }
}
void View_28line::set_field_value(QString value, QString field_id)
{
    bool flag_finish = false;
    qDebug() << field_id;
    form->page()->runJavaScript("document.getElementById(\"" + field_id + "\").value = \"" + value + "\";",
                                [&](QVariant result)->void{
        flag_finish = true;
    });
    while(!flag_finish){
        QApplication::processEvents();
    }
}

void View_28line::set_field_image()
{
    QByteArray base64 = ex.value("image").toString().toUtf8();
    if(base64.size()!= 0){
        QPixmap testt;
        testt.loadFromData(QByteArray::fromBase64(base64), ex.value("image_type").toString().toLocal8Bit().data());
        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        testt.save(&buffer, "PNG");
        // to base64
        QByteArray ba = bytes.toBase64();

        QString html;
        html += QString("<img src='data:image/png;base64,%1'/>").arg(QString(ba));
        bool flag_finish = false;
        form->page()->runJavaScript("document.getElementById(\"image\").innerHTML += \"" + html + "\";",
                                    [&](QVariant result)->void{
            flag_finish = true;
        });
        while(!flag_finish){
            QApplication::processEvents();
        }
    }
}
