/*!
 * \file
 * \brief Данный файл содержит определения базы данных
 *
 *  База данных представляет собой вектор из библиотек, каждая из которых содержит имя библиотеки
 *  и её код в виде вектора макроопределений, которые включают в себя исходный код метки, список параметров и имя
 *  макроса.
 */

#ifndef MACROITEM_H
#define MACROITEM_H

#include <QStringList>
#include <QString>
#include <QVector>

/*!
 * \brief Структура MacroItem описывает одно макроопределение библиотеки.
 */

struct MacroItem
{
    QStringList sourceCode; ///< Исходный код
    QStringList paramList;  ///< Список параметров
    QString     macroLabel; ///< Имя макроса
};

using LibVector = QVector <MacroItem>; ///< Вектор, описывающий одну библиотеку (состоит из макроопределений)

/*!
 * \brief Структура Library описывает одну подключаемую библиотеку.
 */

struct Library
{
    QString   libName;       ///< Имя библиотеки (точнее, полный путь к ней и её имя)
    LibVector macroLabelVec; ///< Вектор макроопределений
};

using Database  = QVector <Library>; ///< Вектор из всех подключаемых библиотек

/*!
 * \brief Структура DBPath представляет собой путь к макроопределению,
 * то есть индекс библиотеки в базе данных и индекс макроопределения в библиотеке.
 */

struct DBPath
{
    Database::size_type  libIndex;   ///< Индекс библиотеки.
    LibVector::size_type macroIndex; ///< Индекс макроопределения.
};

#endif
