/*!
 * \file
 * \brief Файл, содержащий класс для разворачивания макроса.
 *
 */

#ifndef MACROCALLDEPLOYER_H
#define MACROCALLDEPLOYER_H

#include "database.h"
#include "console.h"
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QChar>

class MacroCallDeployer
{
    Console*  console;  ///< Консоль для вывода ошибок, если такие появятся.
    Database* database; ///< База данных для извлечения кода и параметров макроопределения.
    static QString additionalPostfix; ///< Добавочный постфикс для замены меток ($loop -> loop + "_" + additionalPostfix)

    /*!
     * \brief Ищет в строке line макроопределение из базы данных и возвращает пару { Индекс в строке, Путь в базе данных }
     * \param[in] line Строка для поиска макроса.
     * \param[in] findSign Признак нахождения вызова макроса в строке (true - найдено / false - не найдено).
     * \return Пара { Индекс в строке, Путь в базе данных }
     */

    QPair<QString::size_type, DBPath> findMacroInDB(const QString& line, bool &findSign);

    /*!
     * \brief Подставляет в строке line формальные параметры фактическими.
     *
     *  Расположение фактических и формальных параметров в списках (QStringList) должны распологаться по одним и тем же индексам
     *  для корректной подстановки.
     *
     * \param[in] line Строка, в которой будут искаться формальные параметры.
     * \param[in] factParamList Список фактических параметров.
     * \param[in] formParamList Список формальных параметров.
     * \return Строка с подставленными фактическими параметрами.
     */

    QString interchangeParameters(const QString& line, const QStringList& factParamList, const QStringList& formParamList);

    /*!
     * \brief Сравниает количество формальных и фактических параметров в списках.
     * \param[in] factParamList Список фактических параметров.
     * \param[in] pathToFormParamList Список формальных параметров.
     * \return <ul>
     *              <li> true  - Количество фактических и формальных параметров совпадает. </li>
     *              <li> false - Количество фактических и формальных параметров не совпадает. </li>
     *         </ul>
     */

    bool compareFactAndFormParameters(const QStringList& factParamList, DBPath pathToFormParamList);

    /*!
     * \brief Извлекает фактические параметры из вызова в строке \a line.
     * \param[in] line Строка, из которой будут извлекаться фактические параметры.
     * \param[in] paramIndex Индекс в строке, начиная с которого идут параметры.
     * \param[out] correctSign Признак успешного извлечения параметров.
     * \return Список параметров.
     */

    QStringList extractMacroParameters(const QString& line, QString::size_type paramIndex, bool& correctSign);

    /*!
     * \brief Удаляет "пустые" параметры из \a paramList.
     * \param[in] paramList Список параметров.
     */

    void deleteEmptyParameters(QStringList& paramList);

    /*!
     * \brief "Увеличивает" текущий подставляемый индекс. (AA -> AB, DZ -> EA)
     */

    static void incAdditionalPostfix()
    {
        if(additionalPostfix[1] == 'Z')
        {
            additionalPostfix[0] = QChar(additionalPostfix[0].unicode() + 1);
            additionalPostfix[1] = 'A';
        }

        else
            additionalPostfix[1] = QChar(additionalPostfix[1].unicode() + 1);
    }

    /*!
     * \brief Записывает обработанный код макроопределения в выходной параметр \a deployedMacroCode
     *
     * Построчно берет код из \a database, находя нужный макрос по replaceMacroPath, в каждой строке ищет метки ($loop), заменяет
     * их на конечные (loop + "_" + additionalPostfix), а также заменяет формальные параметры фактическими.
     *
     * \param[in] replaceMacroPath Путь к макроопределению, которое необходимо обработать.
     * \param[in] factParamList Список фактических параметров (м.б. и пустым, если макрос без параметров).
     * \param[out] deployedMacroCode Строковый список для записи обработанного кода.
     */

    void insertMacroCode(const DBPath& replaceMacroPath, const QStringList &factParamList, QStringList &deployedMacroCode);

public:
    MacroCallDeployer() = delete; ///< Конструктор по умолчанию удалён для избежания создания объектов без передачи в них базы данных и консоли.

    /*!
     * \brief Конструктор, устанавливающий console = _console, а database = _database.
     *
     * \param[in] _console Консоль для вывода.
     * \param[in] _database База данных для извлечения информации.
     *
     * \warning Проверки на Q_NULLPTR не производится.
     */

    MacroCallDeployer(Console* _console, Database* _database) : console(_console), database(_database) { }


    /*!
     * \brief Ищет "вызовы" макросов и возвращает подставленный код в точку их "вызова".
     * \param[in] line Строка кода, в которой ищем макровызов.
     * \param[out] deployedMacroCode Код развернутого макроопределения.
     * \return <ul>
     *              <li> 0 - макро-вызов не найден. </li>
     *              <li> 1 - макро-вызов найден     </li>
     *              <li> 2 - произошла ошибка       </li>
     *         </ul>
     */

    quint16 deployMacroCall(const QString& line, QStringList& deployedMacroCode);

    /*!
     * \brief Сбрасывает добавочных постфикс в значение "AA"
     */

    static void resetAdditionalPostfix()
    {
        additionalPostfix = "AA";
    }
};

#endif
