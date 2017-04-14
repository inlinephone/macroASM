#include "macrokeywordparser.h"

quint16 MacroKeywordParser::parseMacroKeyword(const QString &line, Database::size_type currLibIndex)
{
    // Общий вид: |label: macro [&par1, &par2, ...]|
    // Коды возврата:
    //                  0 - macro не найдено
    //                  1 - macro найдено
    //                  2 - произошла ошибка

    QRegularExpression regExp("\\smacro", QRegularExpression::CaseInsensitiveOption);

    int macroIndex = line.indexOf(regExp, 0);

    if(macroIndex == -1)
        return 0;

    // Вытащим имя макроса, находящееся до ключ. слова macro

    bool isMacroDefineCorrect;
    QString macroLabel = extractMacroName(macroIndex, line, isMacroDefineCorrect);

    if(isMacroDefineCorrect == false)
        return 2;

    // Теперь проверим, не было ли такой метки раньше в текущей библиотеке
    if(compareWithOthers(macroLabel, currLibIndex) == true)
    {
        *console < "Повторное определение макроса с именем \"" + macroLabel + '\"';
        return 2;
    }

    // Добавим имя метки в базу (т.е. создадим в текущей библиотеке ещё один макрос и добавим в него только имя)
    (*database)[currLibIndex].macroLabelVec.push_back({QStringList(), QStringList(), macroLabel});

    // Теперь извлечем параметры

    QStringList paramList = extractMacroParam(macroIndex, line, isMacroDefineCorrect);

    if(isMacroDefineCorrect == false)
        return 2; // Сообщение об ошибке выводится в extractMacroParams

    // Проверим, нет ли в списке извлеченных параметров пустых строк
    for(QStringList::size_type i = 0; i < paramList.size(); ++i)
        if(paramList[i].isEmpty())
        {
            *console < "Некорректное определение параметров";
            return 2;
        }

    (*database)[currLibIndex].macroLabelVec.last().paramList = std::move(paramList);

    return 1;
}

QStringList MacroKeywordParser::extractMacroParam(int macroIndex, const QString &line, bool &correctSign)
{
    QStringList paramList;      // Список вытащенных парамеров
    bool paramAreaSign = false; // Показывает, находимся ли мы на имени параметра, или между ними
    int i = macroIndex + QString("macro").size() + 1; // Перепрыгиваем за "macro" и пробельный символ

    correctSign = true; // Признак корректного определения параметров

    while(i < line.size() && correctSign)
    {
        if(paramAreaSign == false)
        {
            if(line[i] == '&')
            {
                paramAreaSign = true;
                paramList.push_back(QString());
            }

            else if(!line[i].isSpace()) // Кроме пробельных символов в определении параметров ничего не может быть
            {
                *console < "Некорректное определение параметров";
                correctSign = false;
            }
        }

        else
        {
            if(line[i].isLetterOrNumber() || line[i] == '_')
                paramList.last() += line[i];
            else if(line[i] == ',')
                paramAreaSign = false;
            else if(line[i].isSpace())
                /*Ничего не делать*/;
            else
            {
                *console < "Некорректное определение параметров";
                correctSign = false;
            }

        }

        ++i;
    }

    return paramList;
}

QString MacroKeywordParser::extractMacroName(int macroIndex, const QString &line, bool &correctSign)
{
    bool macroLabelEndSign = false; // Флаг, определяющий, найден ли в строке признак конца имени макроса (:)
    QString macroLabel;             // Строка, в которую будет записано имя макроса

    correctSign = true;

    QString::size_type i;
    for(i = 0; i < macroIndex && !macroLabelEndSign; ++i) // Идем вплоть до macro
    {
        if(line[i] == ':')
            macroLabelEndSign = true;
        else
        {
            macroLabel += line[i];

            if(line[i].isSpace()) // Если до ':' нашелся пробельный символ, значит некорректно определено macro
            {
                *console < "Некорректное определение <b>macro</b>";
                correctSign = false;
            }
        }
    }

    if(macroLabelEndSign == false ||
       !line[i].isSpace()) // То есть, если символ между ':' и macro не пробел (он там точно д.б., причем только один, из - за simplified() в purifyLine())
    {
        *console < "Некорректное определение <b>macro</b>";
        correctSign = false;
    }

    if(macroLabel.isEmpty())
    {
        *console < "Некорректное определение <b>macro</b>";
        correctSign = false;
    }

    return macroLabel;
}

bool MacroKeywordParser::compareWithOthers(const QString& macroLabel, Database::size_type currLibIndex)
{
    for(auto iter : (*database)[currLibIndex].macroLabelVec)
        if(iter.macroLabel == macroLabel)
            return true;

    return false;
}
