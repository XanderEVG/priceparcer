#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QFrame>
#include <QProgressBar>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "xlreader.h"
#include "xlwriter.h"

#include "table.h"
#include "model.h"
#include "inputmodel.h"
#include "myproxymodel.h"
#include "outputmodel.h"

#include "firmparamdialog.h"
#include "drugdilersdialog.h"
#include "pathparamdialog.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QList<Product> list;


public slots:
    void add_files();
    void del_files();
    void export_data();
    void find_name(QString text);

private slots:


    void on_close_action_triggered();

    void on_open_action_triggered();

    void on_erase_action_triggered();

    void on_firm_param_action_triggered();

    void on_folders_action_triggered();

    void on_about_action_triggered();

    void on_drugdilers_action_triggered();

    void on_find_first_action_toggled(bool arg1);

    void set_focus_on_input_drug();

    void on_action_open_settings_folder_triggered();

private:
    Ui::MainWindow *ui;
    QWidget *mainWidget;


    QVBoxLayout* vBoxInTable;
    QVBoxLayout* vBoxOutTable;
    QHBoxLayout* hBoxInputDrugs;
    QHBoxLayout* hBoxFiles;
    QHBoxLayout* hBoxExport;
    QVBoxLayout* mbox;
    QSplitter* splitter;
    QList<int> splitter_sizes_list;
    QList<int> listWidthsIn;
    QList<int> listWidthsOut;



    QLabel* inputFilesNameLabel;
    QPushButton* addFileBtn;
    QPushButton* delFileBtn;
    QLabel* inputDrugNameLabel;
    QLineEdit* inputDrugs;
    QLabel* inputListLabel;
    Table* inTable;
    InputModel* inModel;
    MyProxyModel* inProxyModel;
    QLabel* outputListLabel;
    Table* outTable;
    OutputModel* outModel;
    MyProxyModel* outProxyModel;
    QComboBox* defNameComboBox;
    QPushButton* exportBtn;
    QFrame* frameIn;
    QFrame* frameOut;
    QProgressBar *progressBar;


    QList<Product> products;
    QStringList drugdilers;
    QString default_open_folder;
    QString default_save_folder;

    QString test;


    FirmParamDialog *farm_param_dialog;
    DrugDilersDialog *drugDilersDialog;
    PathParamDialog *pathParamDialog;
    bool find_first_pos;

    bool is_saved;


    void setWidgets();













    void read_settings();
    void first_save_settings();
    void save_settings();


};

#endif // MAINWINDOW_H
