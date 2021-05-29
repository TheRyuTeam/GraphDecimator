#pragma once
#include <QtGui>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QDialog>
#include <QCompleter>

class FileDialog : public QDialog
{
    Q_OBJECT
private:
    QLineEdit*  editFilePath { new QLineEdit };
    QFile*      requestHistory;
    QStringList completedRequests;
    QCompleter* completer;
public:
    FileDialog(QWidget *_parent = nullptr);
    ~FileDialog();

    QString filePath() const;
    void openFile();
};
