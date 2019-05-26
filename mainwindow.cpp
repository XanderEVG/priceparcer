#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QTime>
#include <QMessageBox>
//#include <QThread>
#include <QFileDialog>
#include <QCoreApplication>

#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    is_saved=false;
    test=QCoreApplication::applicationDirPath();
    ui->setupUi(this);
    read_settings();
    setWidgets();

    farm_param_dialog = new FirmParamDialog(this);
    drugDilersDialog  = new DrugDilersDialog(this);
    pathParamDialog   = new PathParamDialog(this);
    connect(farm_param_dialog, SIGNAL(firm_param_changed()), this, SLOT(del_files()));

}

MainWindow::~MainWindow()
{
    save_settings();
    delete ui;

    delete vBoxInTable;
    delete vBoxOutTable;
    delete hBoxInputDrugs;
    delete hBoxFiles;
    delete hBoxExport;
    delete mbox;
}


void MainWindow::setWidgets()
{
    mainWidget= new QWidget(this);
    setCentralWidget(mainWidget);

    vBoxInTable = new QVBoxLayout;
    vBoxOutTable = new QVBoxLayout;
    hBoxInputDrugs = new QHBoxLayout;
    hBoxFiles = new QHBoxLayout;
    hBoxExport = new QHBoxLayout;
    mbox = new QVBoxLayout;

    mainWidget->setLayout(mbox);

    inputFilesNameLabel = new QLabel(tr("\nДобавьте .xls файлы \n" ), this);
    inputFilesNameLabel->setStyleSheet("QLabel { background-color : #E1E1E1; border-style: outset; border-width: 1px; border-color: beige; border-radius: 5px;}");
    inputDrugNameLabel  = new QLabel(tr("Введите название лекарства:"), this);

    addFileBtn = new QPushButton(tr("Добавить файл"), this);
    connect(addFileBtn, SIGNAL(clicked()), this, SLOT(add_files()));

    delFileBtn = new QPushButton(tr("Удалить"), this);
    connect(delFileBtn, SIGNAL(clicked()), this, SLOT(del_files()));


    inputDrugs = new QLineEdit(this);
    connect(inputDrugs, SIGNAL(textEdited(QString)), this, SLOT(find_name(QString)));

    inputListLabel  = new QLabel(tr("Таблица выбора:"), this);
    outputListLabel  = new QLabel(tr("Таблица выбранного товара:"), this);

    inTable = new Table(this);
    inModel = new InputModel(inTable);
    inProxyModel = new MyProxyModel(inModel, inTable);
    inTable->setModel(inProxyModel);

    outTable = new Table(this);
    outModel = new OutputModel(outTable);
    outProxyModel = new MyProxyModel(outModel, outTable);
    outTable->setModel(outProxyModel);

    connect(inTable, SIGNAL(doubleClicked(const QModelIndex&)), inProxyModel, SLOT(rowClicked(const QModelIndex&)));
    connect(inProxyModel, SIGNAL(doubleClicked(const QModelIndex)), inModel, SLOT(rowClicked(const QModelIndex)));
    connect(inModel, SIGNAL(addProduct(QList<QString>)), outModel, SLOT(addProduct(QList<QString>)));
    connect(inModel, SIGNAL(addProduct(QList<QString>)), this, SLOT(set_focus_on_input_drug()));

    connect(outTable, SIGNAL(doubleClicked(const QModelIndex&)), outProxyModel, SLOT(rowClicked(const QModelIndex&)));
    connect(outProxyModel, SIGNAL(doubleClicked(const QModelIndex&)), outModel, SLOT(rowClicked(const QModelIndex&)));
    connect(outModel, SIGNAL(unclickRow(QString, QString)), inModel, SLOT(undoAddRow(QString,QString)));


    inTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);




    QList<QString> head_list;
    head_list<<"-"<<tr("Наименование")<<tr("Цена")<<tr("Цена скид.")<<tr("Срок годности")<<tr("Производитель")<<tr("Поставщик");
    inModel->setDefaultHeaderName(head_list);
    inTable->sortByColumn(3,Qt::AscendingOrder);
    inTable->setColumnsWidth(listWidthsIn);

    head_list.clear();
    outTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    head_list<<tr("Наименование")<<tr("Цена")<<tr("Кол-во")<<tr("Стоимость")<<tr("Срок годности")<<tr("Производитель")<<tr("Поставщик");
    outModel->setDefaultHeaderName(head_list);
    outTable->setColumnsWidth(listWidthsOut);
    //outTable->sortByColumn(0,Qt::AscendingOrder);


    QLabel *defNameLabel  = new QLabel(tr("Выберите фармацевта:"), this);
    defNameComboBox = new QComboBox(this);
    defNameComboBox->addItems(drugdilers);
    defNameComboBox->setCurrentIndex(defNameComboBox->count()-1);

    exportBtn = new QPushButton(tr("     Экспорт     "), this);
    connect(exportBtn, SIGNAL(clicked()), this, SLOT(export_data()));

    inModel->clearData();
    outModel->clearData();
    inTable->setDisabled(true);
    outTable->setDisabled(true);
    inputDrugs->setDisabled(true);
    exportBtn->setDisabled(true);

    splitter = new QSplitter(Qt::Vertical, this);
    splitter->setChildrenCollapsible(false);

    frameIn = new QFrame(this);
    frameIn->setFrameShape(QFrame::Panel);
    frameIn->setFrameShadow(QFrame::Sunken);
    frameOut = new QFrame(this);
    frameOut->setFrameShape(QFrame::Panel);
    frameOut->setFrameShadow(QFrame::Sunken);

    splitter->addWidget(frameIn);
    splitter->addWidget(frameOut);
    splitter->setSizes(splitter_sizes_list);

    hBoxFiles->addWidget(inputFilesNameLabel,10);
    hBoxFiles->addWidget(addFileBtn,3);
    hBoxFiles->addWidget(delFileBtn,2);
    mbox->addLayout(hBoxFiles);
    hBoxInputDrugs->addWidget(inputDrugNameLabel,1);
    hBoxInputDrugs->addWidget(inputDrugs,5);
    mbox->addLayout(hBoxInputDrugs);
    vBoxInTable->addWidget(inputListLabel);
    vBoxInTable->addWidget(inTable);
    frameIn->setLayout(vBoxInTable);
    vBoxOutTable->addWidget(outputListLabel);
    vBoxOutTable->addWidget(outTable);
    frameOut->setLayout(vBoxOutTable);
    splitter->addWidget(frameIn);
    splitter->addWidget(frameOut);
    mbox->addWidget(splitter,10);

    hBoxExport->addStretch();
    hBoxExport->addWidget(defNameLabel);
    hBoxExport->addWidget(defNameComboBox);
    hBoxExport->addWidget(exportBtn);
    mbox->addLayout(hBoxExport);

    this->setWindowTitle(tr("Price Parcer"));
    this->setWindowIcon(QIcon(":/images/images/icon.png"));
}



void MainWindow::add_files()
{

    XlReader* xlReader;
    QStringList files = QFileDialog::getOpenFileNames(this, tr(""), default_open_folder, "Excell (*.xls *.xlsx)");

    //qDebug()<<files;

    if(!files.isEmpty())
    {
        if(inputFilesNameLabel->text()==tr("\nДобавьте .xls файлы \n"))
            inputFilesNameLabel->clear();
        inTable->setDisabled(false);
        outTable->setDisabled(false);
        inputDrugs->setDisabled(false);
        exportBtn->setDisabled(false);
        QString label = inputFilesNameLabel->text();
        foreach (QString file, files)
        {
            xlReader = new XlReader(file);
            products += xlReader->getPriceList();

            if(label.size()>0 && xlReader->getFirmName().isEmpty()==false)
                label += ";  ";
            label += xlReader->getFirmName();
            delete xlReader;
        }
        //label.chop(1);
        inputFilesNameLabel->setText(label);
    }
    ui->statusBar->showMessage(tr("Ок"), 500);
    inputDrugs->setFocus();
}

void MainWindow::del_files()
{
    products.clear();
    inModel->clearData();
    outModel->clearData();
    inputDrugs->clear();
    inTable->setDisabled(true);
    outTable->setDisabled(true);
    inputDrugs->setDisabled(true);
    exportBtn->setDisabled(true);

    inputFilesNameLabel->setText(tr("\nДобавьте .xls файлы \n"));
    ui->statusBar->showMessage(tr("Ок"), 500);
}

void MainWindow::export_data()
{
    if(is_saved)
    {
        bool ok;

        int btn = QMessageBox::information( this, "Внимание", "Эти данные уже выгружались в файл. Продолжить?","Да", "Отмена", 0, 1 );

        if (ok && btn==0)
        {
            return;
        }

    }

    QString fileout = QString("%1/%2_output.xlsx").arg(default_save_folder).arg(defNameComboBox->currentText());
    XlWriter xlsxWriter(fileout, outModel->export_data());
    ui->statusBar->showMessage(tr("Сохранено в " ) + fileout, 1000);
    is_saved=true;
}



void MainWindow::find_name(QString text)
{
    inModel->clearData();

    if(text.size()<3) return;

    //QTime start = QTime::currentTime();


    text=text.replace(",", ".");
    QStringList values = text.split(" ");
    QList<Product> find_products;


    find_products=products;

    //удаляем лишние пробелы в поисковой строке
    foreach (QString val, values)
    {
        if(val=="")
            values.removeOne(val);

    }


    for(int i = 0; i < values.size(); i++) //цикл по поисковым словам
    {

        QList<Product> temp_products;
        for(int j = 0; j < find_products.size(); ++j) //цикл по массиву элементов прайсов
        {


            if(i==0 && find_first_pos) //если ищем 1 слово и если включена опция "искать с начала строки" то ищем с начала строки )))
            {
                if(find_products.at(j).name.indexOf(values.at(i), 0,Qt::CaseInsensitive)==0) //если строка которую нужно найти находится на позиции 0 то:
                {
                    temp_products<<find_products.at(j);
                }
            }
            else
            {
                if(find_products.at(j).name.contains(values.at(i), Qt::CaseInsensitive)) //если строка находится в любом месте
                {
                    temp_products<<find_products.at(j);
                }
            }

        }
        find_products=temp_products;


    }


    foreach (Product prod, find_products)
    {
        inModel->appendRowData(prod);
    }

    //qDebug() << "прошло " << start.elapsed() << " милисекунд\n\n";



}



void MainWindow::read_settings()
{
    QSettings* settings = new QSettings(QCoreApplication::applicationDirPath() + "/settings.conf",QSettings::IniFormat);
    settings->setIniCodec("UTF-8");
    bool firstTime = settings->value("main/firstTime", true).toBool();
    drugdilers = settings->value("main/drugdilers", "").toString().split(";");

    #ifdef Q_OS_LINUX
        default_open_folder = settings->value("main/default_open_folder_lin", "~").toString();
        default_save_folder = settings->value("main/default_save_folder_lin", "~").toString();
    #else
        #ifdef Q_OS_WIN
            default_open_folder = settings->value("main/default_open_folder_win", "I:/QtProjects").toString();
            default_save_folder = settings->value("main/default_save_folder_win", "I:/QtProjects").toString();
        #else
            default_open_folder = "";
            default_save_folder = "";
        #endif
    #endif

    setGeometry(settings->value("main/window_size", QRect(200, 200, 800, 500)).toRect());

    QString sizes_str;
    sizes_str = settings->value("main/splitter_size", QString("500*300")).toString();
    QStringList sizes_list = sizes_str.split("*");
    splitter_sizes_list<<sizes_list[0].toInt()<<sizes_list[1].toInt();

    QString inTable_sizes_str;
    QString outTable_sizes_str;
    inTable_sizes_str = settings->value("main/intable_columns_size", QString("25*512*70*80*128*132*100")).toString(); //TODO not work
    outTable_sizes_str = settings->value("main/outtable_columns_size", QString("453*78*80*100*119*117*100")).toString();


    QStringList inTable_sizes_list = inTable_sizes_str.split("*");
    QStringList outTable_sizes_list = outTable_sizes_str.split("*");




    listWidthsIn<<inTable_sizes_list[0].toInt()<<inTable_sizes_list[1].toInt()<<inTable_sizes_list[2].toInt()<<inTable_sizes_list[3].toInt()
                <<inTable_sizes_list[4].toInt()<<inTable_sizes_list[5].toInt()<<inTable_sizes_list[6].toInt();
    listWidthsOut<<outTable_sizes_list[0].toInt()<<outTable_sizes_list[1].toInt()<<outTable_sizes_list[2].toInt()<<outTable_sizes_list[3].toInt()
                <<outTable_sizes_list[4].toInt()<<outTable_sizes_list[5].toInt();

    find_first_pos = settings->value("main/find_first_pos", false).toBool();
    ui->find_first_action->setChecked(find_first_pos);
    //TODO

    if(firstTime)
    {
        //MSGBOX ATTENTION
        QMessageBox::information(NULL, tr("Price Parcer"), tr("Данная программа написана непрофессионалами и не прошла тестироание. К ошибкам прошу относится с пониманием. О каждой сообщать на XanderEVG@gmail.com"));
        first_save_settings();
    }
    delete settings;
}

void MainWindow::first_save_settings()
{
    QSettings* settings = new QSettings(QCoreApplication::applicationDirPath() + "/settings.conf",QSettings::IniFormat);
    settings->setIniCodec("UTF-8");

    settings->setValue("main/firstTime", false);
    settings->setValue("main/farmnames", "GELIKA;FEB;MEDSERVIS;ROSFARM;");
    settings->setValue("main/drugdilers", "Долгова;Чернышева;Жилкебаева;");
    settings->setValue("main/window_size", QRect(200, 200, 500, 500));

    settings->setValue("main/default_open_folder_lin", "/home");
    settings->setValue("main/default_save_folder_lin", "/home");
    settings->setValue("main/default_open_folder_win", "C:/");
    settings->setValue("main/default_save_folder_win", "C:/");



    settings->sync();
    delete settings;
}

void MainWindow::save_settings()
{
    QSettings* settings = new QSettings(QCoreApplication::applicationDirPath() + "/settings.conf",QSettings::IniFormat);
    settings->setIniCodec("UTF-8");

    settings->setValue("main/window_size", geometry());
    settings->setValue("main/splitter_size", QString("%1*%2").arg(splitter->sizes()[0]).arg(splitter->sizes()[1]));


    QList<int> listWidthsIn_save = inTable->ColumnsWidth();
    QList<int> listWidthsOut_save = outTable->ColumnsWidth();

    settings->setValue("main/intable_columns_size", QString("%1*%2*%3*%4*%5*%6*%7").arg(listWidthsIn_save[0]).arg(listWidthsIn_save[1]).arg(listWidthsIn_save[2]).arg(listWidthsIn_save[3])
                                                                                  .arg(listWidthsIn_save[4]).arg(listWidthsIn_save[5]).arg(listWidthsIn_save[6]));
    settings->setValue("main/outtable_columns_size", QString("%1*%2*%3*%4*%5*%6").arg(listWidthsOut_save[0]).arg(listWidthsOut_save[1]).arg(listWidthsOut_save[2]).arg(listWidthsOut_save[3])
                                                                                   .arg(listWidthsOut_save[4]).arg(listWidthsOut_save[5]));
    settings->setValue("main/find_first_pos", find_first_pos);

    settings->sync();
    delete settings;
}



void MainWindow::on_close_action_triggered()
{
    close();
}

void MainWindow::on_open_action_triggered()
{
    add_files();
}

void MainWindow::on_erase_action_triggered()
{
    del_files();
}

void MainWindow::on_firm_param_action_triggered()
{
    farm_param_dialog->read_farms();
    farm_param_dialog->read_farm_param();
    farm_param_dialog->show();
}

void MainWindow::on_folders_action_triggered()
{
    pathParamDialog->read_paths();
    pathParamDialog->show();
}

void MainWindow::on_drugdilers_action_triggered()
{
    drugDilersDialog->read_drugdilers();
    drugDilersDialog->show();
}
void MainWindow::on_find_first_action_toggled(bool arg1)
{
    find_first_pos = arg1;
}

void MainWindow::on_about_action_triggered()
{
    QString text;
    text = tr("Простая, быстрая и надежная(хе-хе) программа\n");
    text+=tr("по поиску товара из нескольких прайсов\n\n");
    text+=tr("Версия 1.1.2019.05.25.0 \n\n");
    text+=tr("©XanderEVG Inc, 2015. Все права защищены.");

    QMessageBox::about(this,tr("О программе"), text );
}

void MainWindow::set_focus_on_input_drug()
{
   inputDrugs->setFocus();
   inputDrugs->selectAll();
   outTable->scrollToBottom();
}


void MainWindow::on_action_open_settings_folder_triggered()
{
    QDesktopServices::openUrl(QUrl("file:///" + QCoreApplication::applicationDirPath() + "/", QUrl::TolerantMode));
}
