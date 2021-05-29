#include "stylesheet.h"
#include <QFile>

StyleSheet::StyleSheet()
{
    GetStyleFromFile();
}

QString StyleSheet::GetBlueStyleBtn()
{
    switch(GetStyleFromFile())
    {
    case 1:
    {
        return "QPushButton {background-color: #bcd1e0;"
               "border: 1px solid black;"
               "border-radius: 4px;}";
    }
    case 2:
    {
        return "QPushButton {background-color: #b5b5b5;"
               "border: 1px solid black;"
               "border-radius: 4px;}";
    }
    }


}

QString StyleSheet::GetBlueStyleWidget()
{
    switch(GetStyleFromFile())
    {
    case 1:
    {

        return "QWidget {background-color: #e4e9ed; }";
    }
    case 2:
    {

        return "QWidget {background-color: #dbdbdb; }";
    }
    }
}

QString StyleSheet::GetBlueStyleScrollArea()
{
    return "QScrollArea {background-color: #bcd1e0; }"
            "border: 1px solid black;";
}

QString StyleSheet::GetBlueStyleDatePicker()
{
    return "QDateTimeEdit {background-color: #ffffff; }"
           "border: 1px solid black;";
}

QString StyleSheet::GetBlueStyleComboBox()
{
    switch(GetStyleFromFile())
    {
    case 1:
    {
        return "QComboBox {background-color: #bcd1e0;"
               "border: 1px solid black;"
               "border-radius: 4px;}";
    }
    case 2:
    {
        return "QComboBox {background-color: #b5b5b5;"
               "border: 1px solid black;"
               "border-radius: 4px;}";
    }
    }
}

QString StyleSheet::GetBlueLightStyleWidget()
{
    return "QWidget {background-color: #ffffff; }";
}

QString StyleSheet::GetBlueDarkStyleWidget()
{
    switch(GetStyleFromFile())
    {
    case 1:
    {

        return "QWidget {background-color: #d7dce0; }";
    }
    case 2:
    {

        return "QWidget {background-color: #bfbfbf; }";
    }
    }

}

QString StyleSheet::GetBlueStyleMenuBar()
{
    switch(GetStyleFromFile())
    {
    case 1:
    {

        return "QMenuBar {background-color: #bcd1e0; }";
    }
    case 2:
    {

        return "QMenuBar {background-color: #b5b5b5; }";
    }
    }

}

void StyleSheet::SetStyleToFile(int styleNumber)
{
    QFile file("../Style.txt");
    if ((file.exists())&&(file.open(QIODevice::WriteOnly)))
    {
        file.write(QString::number(styleNumber).toUtf8());
        file.close();
    }
}

int StyleSheet::GetStyleFromFile()
{
    int style = 0;
    QFile file("../Style.txt");
    if ((file.exists())&&(file.open(QIODevice::ReadOnly)))
    {
        style = file.readAll().toInt();
        file.close();
    }
    return style;
}
