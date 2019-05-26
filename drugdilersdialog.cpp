#include "drugdilersdialog.h"
#include <QSettings>
#include <QGridLayout>
#include <QInputDialog>
#include <QDebug>
#include <QCoreApplication>

DrugDilersDialog::DrugDilersDialog(QWidget *parent) : QDialog(parent)
{
    drugdilers_label = new QLabel(tr("Список фармацевтов:"), this);

    drugdilers = new QListWidget(this);

    add_btn = new QPushButton(tr("Добавить"), this);
    del_btn = new QPushButton(tr("Удалить"), this);
    save_btn = new QPushButton(tr("Сохранить"), this);

    connect(add_btn, SIGNAL(clicked()), this, SLOT(add_drugdiler()));
    connect(del_btn, SIGNAL(clicked()), this, SLOT(del_drugdiler()));
    connect(save_btn, SIGNAL(clicked()), this, SLOT(save()));

    QGridLayout *main_grid = new QGridLayout(this);

    main_grid->addWidget(drugdilers_label,0,0);
    main_grid->addWidget(add_btn,0,1);
    main_grid->addWidget(del_btn,0,2);
    main_grid->addWidget(drugdilers,1,0,1,3);
    main_grid->addWidget(save_btn,2,1);


    resize(400,200);
    setModal(true);

    setLayout(main_grid);
    setWindowTitle(tr("Фармацевты"));


    read_drugdilers();
}

DrugDilersDialog::~DrugDilersDialog()
{

}

void DrugDilersDialog::read_drugdilers()
{
    drugdilers->clear();
    QSettings* settings = new QSettings( QCoreApplication::applicationDirPath() + "/settings.conf",QSettings::IniFormat);
    settings->setIniCodec("UTF-8");



    QStringList drugdilers_list = settings->value("main/drugdilers", "Долгова;Чернышева;Жилкебаева;").toString().split(";");
    foreach(QString drugdiler, drugdilers_list)
    {
        if(drugdiler.isEmpty())
        {
            drugdilers_list.removeOne(drugdiler);
            continue;
        }

    }
    drugdilers->addItems(drugdilers_list);

    delete settings;
}

void DrugDilersDialog::save()
{
    QSettings* settings = new QSettings(QCoreApplication::applicationDirPath() + "/settings.conf",QSettings::IniFormat);
    settings->setIniCodec("UTF-8");

    QString drugdilers_str;
    drugdilers_str="";

    for(int i = 0; i < drugdilers->count(); i++)
        drugdilers_str.append(drugdilers->item(i)->text() + ";");

    settings->setValue("main/drugdilers", drugdilers_str);

    settings->sync();
    delete settings;
}

void DrugDilersDialog::add_drugdiler()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Добавить фармацевта"),
                                            tr("Имя:"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty())
    {
        drugdilers->addItem(text);
    }
}

void DrugDilersDialog::del_drugdiler()
{
    drugdilers->takeItem(drugdilers->currentIndex().row());
}
