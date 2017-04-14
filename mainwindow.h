/*!
 * \file
 * \brief Файл, содержащий класс с пользовательским интерфейсом программы
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QFileDialog>
#include <QSettings>
#include <QDir>
#include "parser.h"

namespace Ui {
    class MainWindow;
}

/*!
 * \brief Класс <b> MainWindow </b> представляет собой логику работы графического интерфейса
 *
 *  Интерфейс программы состоит из 3-х кнопок:
 *  <ul>
 *      <li> Открыть </li>
 *      <li> Запустить </li>
 *      <li> Очистить </li>
 *  </ul>
 *
 *  Одного поля вывода (для хранения и отображения пути к выбранному файлу). <br />
 *  А также из поля для вывода текста, которое используется в качестве консоли.
 */

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    /*!
     * \brief Слот, реализующий действие в ответ на нажатие кнопки "Открыть"
     *
     *  Открывает диалоговое окно, запрашивающее выбор файла с раширением .uum32masm.
     *  Если файл выбран корректно, то делает доступной кнопку "Запустить"
     */

    void on_openBtn_clicked();

    /*!
     * \brief Слот, реализующий действие в ответ на нажатие кнопки "Запустить"
     *
     *  Начинает парсинг файлов.
     */

    void on_runBtn_clicked();

private:
    Ui::MainWindow *ui; ///< Элементы пользовательского интерфейса
};

#endif
