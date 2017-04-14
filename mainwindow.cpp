#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui -> setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openBtn_clicked()
{
    QSettings settings("settings.ini", QSettings::IniFormat);

    settings.beginGroup("app_settings");

    QString lastPath = settings.value("path").toString(); // Если такой настройки нет / файл не создан, тогда вернется пустая строка, которая
                                                          // интерпретируется как путь к папке по умолчанию (директория приложения)

    QFileDialog fileDialog(Q_NULLPTR, windowTitle(), lastPath, "Файлы макроассемблера UUM32 (*.uum32masm)");

    if(fileDialog.exec() == QDialog::Accepted)
    {
        settings.setValue("path", fileDialog.directory().absolutePath());
        ui -> runBtn -> setEnabled(true);
        ui -> masmFileLbl -> setText(fileDialog.selectedFiles().at(0));
    }

    settings.endGroup();
}

void MainWindow::on_runBtn_clicked()
{
    Parser parser(ui -> out, ui -> masmFileLbl -> text());

    parser.runParser();
}


