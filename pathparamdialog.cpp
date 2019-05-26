#include "pathparamdialog.h"
#include <QSettings>
#include <QGridLayout>
#include <QInputDialog>
#include <QDebug>
#include <QCoreApplication>

PathParamDialog::PathParamDialog(QWidget *parent) : QDialog(parent)
{


    open_folder_label = new QLabel(tr("Папка загрузки:"), this);
    save_folder_label = new QLabel(tr("Папка сохранения:"), this);
    open_folder_edit = new QLineEdit(this);
    save_folder_edit = new QLineEdit(this);
    save_btn = new QPushButton(tr("Сохранить"), this);


    connect(save_btn, SIGNAL(clicked()), this, SLOT(save()));

    QSizePolicy policy;
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    policy.setVerticalPolicy(QSizePolicy::Minimum);

    open_folder_edit ->setSizePolicy(policy);
    save_folder_edit ->setSizePolicy(policy);



    QGridLayout *main_grid = new QGridLayout(this);



    main_grid->addWidget(open_folder_label,0,0);
    main_grid->addWidget(open_folder_edit,0,1,1,2);

    main_grid->addWidget(save_folder_label,1,0);
    main_grid->addWidget(save_folder_edit,1,1,1,2);

    main_grid->addWidget(save_btn,2,1);
    main_grid->setRowStretch(3,100);

    resize(400,100);
    setModal(true);

    main_grid->setColumnStretch(1, 25);
    main_grid->setColumnStretch(2, 50);
    main_grid->setColumnStretch(2, 10);

    setLayout(main_grid);
    setWindowTitle(tr("Параметры папок"));


    read_paths();

}

PathParamDialog::~PathParamDialog()
{

}

void PathParamDialog::read_paths()
{
    QSettings* settings = new QSettings(QCoreApplication::applicationDirPath() + "/settings.conf",QSettings::IniFormat);
    settings->setIniCodec("UTF-8");

    #ifdef Q_OS_LINUX
        open_folder_edit->setText(settings->value("main/default_open_folder_lin", "~").toString());
        save_folder_edit->setText(settings->value("main/default_save_folder_lin", "~").toString());
    #else
        #ifdef Q_OS_WIN
            open_folder_edit->setText(settings->value("main/default_open_folder_win", "I:/QtProjects").toString());
            save_folder_edit->setText(settings->value("main/default_save_folder_win", "I:/QtProjects").toString());
        #endif
    #endif
    delete settings;
}

void PathParamDialog::save()
{
    QSettings* settings = new QSettings(QCoreApplication::applicationDirPath() + "/settings.conf",QSettings::IniFormat);
    settings->setIniCodec("UTF-8");

    #ifdef Q_OS_LINUX
        settings->setValue("main/default_open_folder_lin", open_folder_edit->text());
        settings->setValue("main/default_save_folder_lin", save_folder_edit->text());
    #else
        #ifdef Q_OS_WIN
            settings->setValue("main/default_open_folder_win", open_folder_edit->text());
            settings->setValue("main/default_save_folder_win", save_folder_edit->text());
        #endif
    #endif

    settings->sync();
    delete settings;
}

