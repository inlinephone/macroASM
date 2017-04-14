/*!
 * \file
 * \brief Данный файл описывает классы консоли.
 *
 *  Классы консоли используются для упрощения работы с <b> QTextBrowser </b> и берут
 *  логику её работы на себя.
 */

#ifndef CONSOLE_H
#define CONSOLE_H

#include <QTextBrowser>

/*!
 *  \brief Класс <b> Console </b> является обёрткой над классом <b> QTextBrowser </b>
 *   и используется для эмуляции консольного ввода - вывода.
 *
 *   Данный класс предоставляет интерфейс ввода - вывода, похожий на
 *   STL IO, но не использующий буффер, а выводящий непосредственно в виджет <b> QTextBrowser </b>.
 */

class Console
{
protected:
    QTextBrowser *_console; ///< Многостроное текстовое окно для вывода.
public:
    /*!
     * \brief Удалённый конструктор предотвращает создание объектов без передачи <b> QTextBrowser* </b>.
     */
    Console() = delete;

    /*!
     * \brief Единственный конструктор класса, принимающий <b> QTextBrowser* </b>, который будет использоваться для вывода.
     * \param[in] console Виджет для работы с текстом, который будет использоваться как консоль.
     *
     * \warning Проверки на Q_NULLPTR не производится.
     */

    Console(QTextBrowser *console) : _console(console) {}

    /*!
     * \brief Метод, осуществляющий стандартный вывод.
     * \param[in] text Текст для вывода.
     */

    virtual void cout  (const QString& text) { _console -> append(text); }

    /*!
     * \brief Метод для вывода ошибок, цвет текста: красный.
     * \param[in] text Текст ошибки для вывода.
     */

    virtual void cerr  (const QString& text) { _console -> append("<font color = red>"     + text + "</font>"); }

    /*!
     * \brief Метод для вывода логов, цвет текста: зеленый.
     * \param[in] text Текст лога для вывода.
     */

    virtual void clog  (const QString& text) { _console -> append("<font color = green>"   + text + "</font>"); }

    /*!
     * \brief Метод для вывода предупреждений, цвет текста: оранжевый.
     * \param[in] text Текст предупреждения для вывода.
     */

    virtual void cwarn (const QString& text) { _console -> append("<font color = orange>"  + text + "</font>"); }

    void operator <=(const QString &text) { clog  (text); } ///< Оператор, являющийся оболочкой над clog.
    void operator <<(const QString &text) { cout  (text); } ///< Оператор, являющийся оболочкой над cout.
    void operator < (const QString &text) { cerr  (text); } ///< Оператор, являющийся оболочкой над cerr.
    void operator |=(const QString &text) { cwarn (text); } ///< Оператор, являющийся оболочкой над cwarn.
};

/*!
 *  \brief Класс <b> MacroConsole </b> является расширенем класса <b> Console </b>
 *   добавляющий к нему функционал для работы с файлами и их нумерацией.
 *
 *   Данный класс содержит счётчик строк и имя текущего файла. При выводе на экран он отображает
 *   сначала имя файла, затем номер строки и только потом сообщение / ошибку / лог / предупреждение.
 */

class MacroConsole final : public Console
{
    QString currFile; ///< Имя файла.
    quint32 currLine; ///< Счётчик строк.
public:
    /*!
     * \brief Удалённый конструктор предотвращает создание объектов без передачи <b> QTextBrowser* </b>
     */

    MacroConsole() = delete;

    /*!
     * \brief Единственный конструктор класса <b> MacroConsole </b>.
     *  Инициализирует \a currFile пустой строкой, \a currLine нулём, а \a Console::_console параметром - \a console.
     * \param console Инициализирует \a Console::_console  с помощью параметра \a console.
     */

    MacroConsole(QTextBrowser *console) : Console(console), currFile(), currLine(0) {}

    /*!
     * \brief Выводит текст в консоль, добавляя в начало имя файла и номер текущей строки.
     * \param[in] text Текст для вывода.
     */

    virtual void cout(const QString &text) override
    {
        _console -> append(currFile + " [" + QString::number(currLine) + "] | " + text);
    }

    /*!
     * \brief Выводит текст ошибки в консоль, добавляя в начало имя файла и номер текущей строки.
     * \param[in] text Текст ошибки для вывода.
     */

    virtual void cerr(const QString &text) override
    {
        _console -> append(currFile + " [" + QString::number(currLine) + "] | Ошибка: <font color = red>" + text + "</font>");
    }

    /*!
     * \brief Выводит текст лога в консоль, добавляя в начало имя файла и номер текущей строки.
     * \param[in] text Текст лога для вывода.
     */

    virtual void clog(const QString &text) override
    {
        _console -> append(currFile + " [" + QString::number(currLine) + "] | <font color = green>" + text + "</font>");
    }

    /*!
     * \brief Выводит текст предупреждения в консоль, добавляя в начало имя файла и номер текущей строки.
     * \param[in] text Текст предупреждения для вывода.
     */

    virtual void cwarn (const QString &text) override
    { 
        _console -> append(currFile + " [" + QString::number(currLine) + "] | <font color = orange>" + text + "</font>");
    }

    void    prim_cout  (const QString& text) { Console::cout(text);  } ///< Вызывает метод <i> Console::cout(text) </i>.
    void    prim_clog  (const QString& text) { Console::clog(text);  } ///< Вызывает метод <i> Console::clog(text) </i>.
    void    prim_cerr  (const QString& text) { Console::cerr(text);  } ///< Вызывает метод <i> Console::cerr(text) </i>.
    void    prim_cwarn (const QString& text) { Console::cwarn(text); } ///< Вызывает метод <i> Console::cwarn(text) </i>.

    void    inc             () { ++currLine;   } ///< Увеличивает текущий счётчик строк на 1.
    void    resetCurrentLine() { currLine = 0; } ///< Сбрасывает текущий счётчик в 0.

    /*!
     * \brief Устанавливает текущий счётчик в \a linePos
     * \param[in] linePos Номер строки
     */

    void setCurrentLine(quint32 linePos) { currLine = linePos; }

    /*!
     * \brief Возвращает текущую позицию файла.
     * \return Номер строки mlb - файла.
     */

    quint32 getCurrentLine() const { return currLine; }

    /*!
     * \brief Устанавливает имя текущего (обрабатываемого) файла.
     * \param[in] filename Имя нового файла.
     */

    void setCurrentFile(const QString &filename) { currFile = filename; }

    /*!
     * \brief Возвращает текущий файл.
     * \return Имя текущего файла.
     */

    QString getCurrentFile() const { return currFile; }
};

#endif // CONSOLE_H
