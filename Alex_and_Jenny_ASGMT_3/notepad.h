#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QFontDialog>
#include <QRegularExpression>
#include <QDateTime>
#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QChartView>
#include <QValueAxis>
#include <QLineSeries>
#include <QPieSeries>
#include <QIODevice>

#include "word.h"
#include "dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Notepad; }
QT_END_NAMESPACE

class Notepad : public QMainWindow
{
    Q_OBJECT

public:
    Notepad(QWidget *parent = nullptr);
    ~Notepad();

private slots:
    void newDocument();
    void open();
    void save();
    void saveAs();
    void exit();
    void print();
    void saveToPDF();
    void setFont();
    void about();
    void toUpperCase();
    void toLowerCase();
    void countWords();
    void markChange();
    void showGraphOptions();
    void generateStatFile();
    void generateGraph(QString, QString, QList<Word>);


private:
    Ui::Notepad *ui;
    QString currentFile;
    QString savedText;
};
#endif // NOTEPAD_H
