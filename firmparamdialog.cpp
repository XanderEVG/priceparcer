#include "firmparamdialog.h"
#include <QSettings>
#include <QGridLayout>
#include <QInputDialog>
#include <QDebug>
#include <QCoreApplication>

FirmParamDialog::FirmParamDialog(QWidget *parent) : QDialog(parent)
{



    firm = new QComboBox(this);
    add_firm_btn = new QPushButton("+", this);

    runame_label = new QLabel(tr("Название:"), this);
    description_label = new QLabel(tr("Строка идентификации:"), this);
    headers_label = new QLabel(tr("Заголовки:"), this);
    head_name_label = new QLabel(tr("Наименование:"), this);
    head_price_label = new QLabel(tr("Цена:"), this);
    head_expire_data_label = new QLabel(tr("Срок годности:"), this);
    head_manufacturer_label = new QLabel(tr("Производитель:"), this);
    discount_label = new QLabel(tr("Скидка, % :"), this);
    caution_label = new QLabel(tr("Внимание, сохранение приведет к удалению выбранных товаров  "), this);


    runame_edit = new QLineEdit(this);
    description_edit = new QLineEdit(this);

    head_name_edit = new QLineEdit(this);
    head_price_edit = new QLineEdit(this);
    head_expire_data_edit = new QLineEdit(this);
    head_manufacturer_edit = new QLineEdit(this);
    discount_edit = new QLineEdit(this);

    save_btn = new QPushButton(tr("Сохранить"), this);

    connect(add_firm_btn, SIGNAL(clicked()), this, SLOT(add_firm()));
    connect(save_btn, SIGNAL(clicked()), this, SLOT(save()));
    connect(firm, SIGNAL(currentTextChanged(QString)), this, SLOT(firm_select(QString)));

    firm->setEditable(false);
    add_firm_btn ->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Minimum);
    add_firm_btn->setFixedWidth(30);

    QSizePolicy policy;
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    policy.setVerticalPolicy(QSizePolicy::Preferred);

    runame_edit ->setSizePolicy(policy);
    description_edit ->setSizePolicy(policy);
    head_name_edit ->setSizePolicy(policy);
    head_price_edit ->setSizePolicy(policy);
    head_expire_data_edit ->setSizePolicy(policy);
    head_manufacturer_edit ->setSizePolicy(policy);
    discount_edit ->setSizePolicy(policy);

    QValidator *validator = new QDoubleValidator(0, 100, 2, this);
    discount_edit->setValidator(validator);


    QGridLayout *main_grid = new QGridLayout(this);

    main_grid->addWidget(firm,0,0,1,2);
    main_grid->addWidget(add_firm_btn,0,2);

    main_grid->addWidget(runame_label,1,0);
    main_grid->addWidget(runame_edit,1,1,1,2);

    main_grid->addWidget(description_label,2,0);
    main_grid->addWidget(description_edit,2,1,1,2);

    main_grid->setRowStretch(3,10);
    main_grid->addWidget(headers_label,4,0,1,3);

    main_grid->addWidget(head_name_label,5,0);
    main_grid->addWidget(head_name_edit,5,1,1,2);

    main_grid->addWidget(head_price_label,6,0);
    main_grid->addWidget(head_price_edit,6,1,1,2);

    main_grid->addWidget(head_expire_data_label,7,0);
    main_grid->addWidget(head_expire_data_edit,7,1,1,2);

    main_grid->addWidget(head_manufacturer_label,8,0);
    main_grid->addWidget(head_manufacturer_edit,8,1,1,2);

    main_grid->setRowStretch(9,10);

    main_grid->addWidget(discount_label,10,0);
    main_grid->addWidget(discount_edit,10,1,1,2);

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(caution_label);
    hlayout->addWidget(save_btn);

    main_grid->addLayout(hlayout,11,0,1,3);


    resize(450,300);
    setModal(true);

    main_grid->setColumnStretch(1, 25);
    main_grid->setColumnStretch(2, 50);
    main_grid->setColumnStretch(2, 10);

    setLayout(main_grid);
    setWindowTitle(tr("Параметры фирмы"));


    read_farms();
    read_farm_param(firm->itemText(0));

}

FirmParamDialog::~FirmParamDialog()
{

}

void FirmParamDialog::read_farms()
{
    firm->clear();
    QSettings* settings = new QSettings(QCoreApplication::applicationDirPath() + "/settings.conf",QSettings::IniFormat);
    settings->setIniCodec("UTF-8");
    QStringList farmnames = settings->value("main/farmnames", "").toString().split(";");

    foreach(QString name, farmnames)
    {
        if(name.isEmpty())
        {
            farmnames.removeOne(name);
            continue;
        }
        firm->addItem(name);
   }

    delete settings;
}

void FirmParamDialog::read_farm_param(QString farm_name)
{
    QSettings* settings = new QSettings(QCoreApplication::applicationDirPath() + "/settings.conf",QSettings::IniFormat);
    settings->setIniCodec("UTF-8");

    if(farm_name.isEmpty())
        farm_name=firm->itemText(0);

    QStringList headers = settings->value(farm_name+"/headers", "Наименование;Цена;Срок годности;Производитель").toString().split(";");
    foreach(QString head, headers)
    {
        if(head.isEmpty())
        {
            headers.removeOne(head);
            continue;
        }
    }
    runame_edit->setText(settings->value(farm_name+"/runame", "").toString());
    description_edit->setText(settings->value(farm_name+"/description", "").toString());

    head_name_edit->setText(headers[0]);
    head_price_edit->setText(headers[1]);
    head_expire_data_edit->setText(headers[2]);
    head_manufacturer_edit->setText(headers[3]);
    discount_edit->setText(settings->value(farm_name+"/discount", "0").toString());

    delete settings;
}

void FirmParamDialog::save_farm(QString farm_name)
{
    QSettings* settings = new QSettings(QCoreApplication::applicationDirPath() + "/settings.conf",QSettings::IniFormat);
    settings->setIniCodec("UTF-8");

    QString farmnames_temp;
    QStringList farmnames = settings->value("main/farmnames", "").toString().split(";");


    foreach(QString name, farmnames)
    {
        if(name.isEmpty())
        {
            farmnames.removeOne(name);
            continue;
        }

       farmnames_temp.append(name);
       farmnames_temp.append(";");
   }
    if(!farmnames.contains(farm_name))
        farmnames_temp+=farm_name + ";";


    settings->setValue("main/farmnames",farmnames_temp);
    settings->setValue(farm_name+"/runame", runame_edit->text());
    settings->setValue(farm_name+"/description", description_edit->text());
    settings->setValue(farm_name+"/headers", QString("%1;%2;%3;%4;").arg(head_name_edit->text()).arg(head_price_edit->text()).arg(head_expire_data_edit->text()).arg(head_manufacturer_edit->text()));
    settings->setValue(farm_name+"/discount", discount_edit->text());

    settings->sync();
    delete settings;
}

void FirmParamDialog::add_firm()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Создать фирму"),
                                            tr("Название фирмы(по англ):"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty())
    {
       firm->addItem(text);
       firm->setCurrentText(text);
       runame_edit->clear();
       description_edit->clear();

       head_name_edit ->clear();
       head_price_edit->clear();
       head_expire_data_edit->clear();
       head_manufacturer_edit->clear();
       discount_edit->setText("0");

    }
}

void FirmParamDialog::save()
{
    save_farm(firm->currentText());
    emit firm_param_changed();
}

void FirmParamDialog::firm_select(QString name)
{
    read_farm_param(name);
}













