#include "filedialog.h"
#include <QCompleter>

FileDialog::FileDialog(QWidget *_parent)
     : QDialog(_parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{

    QLabel* lFilePath { new QLabel("&File path: ") } ;
    lFilePath->setBuddy(editFilePath);

    QPushButton* btnConnect { new QPushButton("&Open") };
    QPushButton* btnCancel { new QPushButton("&Cancel") };

    connect(btnConnect, SIGNAL(clicked()), SLOT(accept()));
    connect(btnCancel, SIGNAL(clicked()), SLOT(reject()));

    QGridLayout* ptopLayout { new QGridLayout };
    ptopLayout->addWidget(lFilePath, 0, 0);
    ptopLayout->addWidget(editFilePath, 0, 1);
    ptopLayout->addWidget(btnConnect, 0, 2);
    ptopLayout->addWidget(btnCancel, 1, 2);
    setLayout(ptopLayout);

    openFile();

    QTextStream ts(requestHistory);
    completedRequests = ts.readAll().split('\n');

    completer = new QCompleter(completedRequests, lFilePath);
    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    editFilePath->setCompleter(completer);
}

FileDialog::~FileDialog()
{
    if(requestHistory) {
        auto newRequest = editFilePath->text();
        if(completedRequests.indexOf(newRequest) < 0) {
            requestHistory->resize(0);
            completedRequests.push_front(newRequest);
            if(completedRequests.size() > 50) {
                completedRequests.pop_back();
            }
            for(qint64 i = 0; i < completedRequests.size() - 1; ++i) {
                requestHistory->write((completedRequests[i] + '\n').toUtf8());
            }
            if(completedRequests.size() > 0) {
                requestHistory->write(completedRequests.back().toUtf8());
            }
        }
        requestHistory->close();
        delete requestHistory;
        delete completer;
    }
}

QString FileDialog::filePath() const
{
    return editFilePath->text();
}

void FileDialog::openFile()
{
    requestHistory = new QFile("../requesthistory.txt");
    if(!requestHistory || !requestHistory->open(QIODevice::ReadWrite)) {
        QFile("../requesthistory.txt").open(QIODevice::ReadOnly);
        return openFile();
    }
}
