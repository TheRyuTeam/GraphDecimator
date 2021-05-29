#include "util.h"

void messageBox(const QString& text)
{
    QMessageBox mb;
    mb.setText(text);
    mb.show();
    mb.exec();
}
