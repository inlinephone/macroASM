/*!
 *  \file
 *  \brief Файл, содержащий основной класс для рабора файлов.
 */

#ifndef PARSER_H
#define PARSER_H

#include "database.h"
#include "console.h"
#include "macrokeywordparser.h"
#include "macrocalldeployer.h"
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QPair>

/*!
 * \brief Класс <b> Parser </b> является основным для разбора файлов.
 *
 * Данный класс предоставляет удобный интерфейс для разбора masm, mlb файлов.
 * Работает с вложенными mlb - файлами, а также проверяет исходный текст на наличие
 * некоторых ошибок.
 */

class Parser
{
    Database database;      ///< База данных, хранящая всю информация о библиотеках
    MacroConsole console;   ///< Консоль для вывода ошибок, логов, предупреждений и т.д.
    QString masmFilePath;   ///< Путь к masm - файлу

    /*!
     * \brief Вытаскивает из пути к файлу путь к директории
     * \param[in] path Путь к файлу в директории
     * \return Путь к директории
     */

    QString getDirectory(const QString &path);

    /*!
        \brief Метод, осуществляющий основной разбор masm - файла и запись обработанных строк кода в новый файл.
        \param[in] stream Открытый поток на начале masm - файла

        Принцип работы цикла разбора:

            1) Если строка пустая или состоит только из комментария - разбор не проводится
            2) Ищем в текущей строке ключевое слово "include"
                2.1) Если слово "include" найдено - извлекаем путь к mlb - файлу, открываем
                     и разбираем его.
                2.2) Иначе ищем в текущей строке макрос для подстановки среди тех, что есть в базе
    */

    bool parseMasmFile(QTextStream& stream);

    /*!
     * \brief Метод, удаляющий из строки комментарий и пустые пробелы.
     * \param[in, out] line Строка исходного текста
     */

    void purifyLine(QString &line);

    /*!
     * \brief Находит ключевое слово <b><i> include </i></b>
     * \param[in] line Строка исходного текста
     * \return <ul>
     *              <li> Пустую строку, если include не найден </li>
     *              <li> Путь к masm - файлу, если include найден </li>
     *         </ul>
     * \warning Если после закрывающей кавычки пути подключаемого файла будут стоять
     * символы, то программа не обратит на них никакого внимания, например:
     * \code include "Путь к файлу" incorrect symbols \endcode
     *
     */

    QString parseIncludeKeyword(const QString& line);

    /*!
     * \brief Добавляет макросы mlb - библиотеки в базу данных
     * \param[in] path Путь к mlb - библиотеке
     * \param[in] isFirstEntry - проверяет, входим ли мы с основным masm файлом (первое вхождение) или с mlb (true - masm / false - mlb).
     * \return <ul>
     *              <li> true  - если нет ошибок </li>
     *              <li> false - если ошибки есть </li>
     *         </ul>
     */

    bool addMlbToBase(const QString& path, bool isFirstEntry = false);

    /*!
     * \brief Удаляет пустые библиотеки из <b> database </b>
     */

    void deleteEmptyLibraries();
public:
    /*!
     * \brief Конструктор, инициализирующий поля консоли и пути к masm - файлу.
     * \param[in] _console <b>TextBrowser</b>, который преобразуется в консоль.
     * \param[in] _masmFilePath Путь к masm - файлу.
     */

    Parser(QTextBrowser *_console, const QString& _masmFilePath);

    /*!
     * \brief Удалённый конструктор по умолчанию (Во избежание создания парсера без консоли и пути к masm - файлу.
     */

    Parser() = delete;

    /*!
     * \brief Запускает основной цикл разбора masm - файла.
     */

    void runParser();
};

#endif
