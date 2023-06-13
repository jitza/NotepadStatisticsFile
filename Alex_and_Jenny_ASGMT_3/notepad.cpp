#include "notepad.h"
#include "ui_notepad.h"

Notepad::Notepad(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Notepad)
{
    ui->setupUi(this);

    setCentralWidget(ui->editor);

    //connect actions to slots
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newDocument()));
    connect(ui->actionOpen, &QAction::triggered, this, &Notepad::open);
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exit()));
    connect(ui->actionPrintDoc, SIGNAL(triggered()), this, SLOT(print()));
    connect(ui->actionSave_as_PDF, SIGNAL(triggered()), this, SLOT(saveToPDF()));
    connect(ui->actionFont, SIGNAL(triggered()), this, SLOT(setFont()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));

    connect(ui->actionUpperCase, SIGNAL(triggered()), this, SLOT(toUpperCase()));
    connect(ui->actionLowerCase, SIGNAL(triggered()), this, SLOT(toLowerCase()));
    connect(ui->actionWordCount, SIGNAL(triggered()), this, SLOT(countWords()));

    connect(ui->editor, SIGNAL(textChanged()), this, SLOT(markChange()));

    //ASGMT3
    connect(ui->actionGraph, SIGNAL(triggered()), this, SLOT(showGraphOptions()));
    connect(ui->actionGenerate_Statistical_File, SIGNAL(triggered()), this, SLOT(generateStatFile()));


    savedText = "";
    ui->editor->setText("");
}

Notepad::~Notepad()
{
    delete ui;
}

void Notepad::newDocument()
{
    if(savedText != ui->editor->toPlainText()){
        QMessageBox msgBox;
        msgBox.setText("You have unsaved changes.");
        msgBox.setInformativeText("Do you want to proceed?");
        msgBox.setStandardButtons(QMessageBox::Ok| QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int result = msgBox.exec();

        if(result == QMessageBox::Cancel)
            return;
    }

    savedText = "";
    ui->editor->setText("");
    currentFile = "";
}

void Notepad::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select file");
    QFile file(fileName);

    if(!file.open((QIODevice::ReadOnly) | QFile::Text))
    {
        QMessageBox::warning(this, "..", "File not opened.");
        return;
    }

    currentFile = fileName;

    setWindowTitle(fileName);

    QTextStream fileStream(&file);
    QString content = fileStream.readAll();

    savedText = content;
    ui->editor->setText(content);

    file.close();
}

void Notepad::save()
{
    QString fileName;

    // If we don't have a filename from before, get one.
    if (currentFile.isEmpty()) {

        fileName = QFileDialog::getSaveFileName(this, "Save");

        if (fileName.isEmpty())
            return;

        currentFile = fileName;

    } else {
        fileName = currentFile;
    }

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }

    setWindowTitle(fileName);

    QTextStream fileStream(&file);
    QString text = ui->editor->toPlainText();

    savedText = text;
    fileStream << text;

    file.close();
}

void Notepad::saveAs()
{

    QString fileName = QFileDialog::getSaveFileName(this, "Save as");

    if (fileName.isEmpty())
        return;

    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }

    currentFile = fileName;

    setWindowTitle(fileName);

    QTextStream fileStream(&file);
    QString text = ui->editor->toPlainText();

    savedText = text;
    fileStream << text;

    file.close();
}

void Notepad::exit()
{
    if(savedText != ui->editor->toPlainText()){
        QMessageBox msgBox;
        msgBox.setText("You have unsaved changes.");
        msgBox.setInformativeText("Do you want to exit without Saving?");
        msgBox.setStandardButtons(QMessageBox::Yes| QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int result = msgBox.exec();

        if(result == QMessageBox::Cancel)
            return;
    }

    QCoreApplication::quit();
}

void Notepad::print()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog * printDialog = new QPrintDialog(&printer, this);

    printDialog->setWindowTitle("Print Document");

    if(printDialog->exec() == QDialog::Accepted){
        ui->editor->print(&printer);
    }
}

void Notepad::saveToPDF()
{
    QString filename = "untitled";

    if(!currentFile.isEmpty()){

        //remove extension on file
        QList<QString> nameParts = currentFile.split(".");
        nameParts.removeLast();

        filename = nameParts.join("");
    }

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(QString("%1.pdf").arg(filename));

    ui->editor->print(&printer);
}

void Notepad::setFont()
{
    bool fontSelected;

    QFont font = QFontDialog::getFont(&fontSelected, this);

    if(fontSelected){
        ui->editor->setFont(font);
    }

}

void Notepad::about()
{
    QString message = "<h2>- - Notepad - -</h2>";
    message.append("<p>Copyright &copy; 2022 CMPS2212 - GUI Class</p>");
    message.append("<p>Name: My Name<p>");
    message.append("<p>ID: 101<p>");

    QMessageBox::about(this, "My Notepad", message);
}

void Notepad::toUpperCase()
{
    QString text = ui->editor->toPlainText();
    ui->editor->setText(text.toUpper());
}

void Notepad::toLowerCase()
{
    QString text = ui->editor->toPlainText();
    ui->editor->setText(text.toLower());
}

void Notepad::countWords()
{
    QString text = ui->editor->toPlainText();

    // split the editor text into a word list
    QStringList wordList = text.split(QRegularExpression("[\r\n\t ;:,.!?]+"), Qt::SkipEmptyParts);

    QMessageBox::information(this, tr("Word Count"), QString("The total number of words is: %1").arg(wordList.count()));

}

void Notepad::markChange()
{
    QString file = "New";
    QString prefix = "";

    if(savedText != ui->editor->toPlainText()){
        prefix = "*";
    }

    if(!currentFile.isEmpty()){
        file = currentFile;
    }

    setWindowTitle(prefix + file);
}

/*-----------------------------------------------ASGMT3---------------------------------------------*/
void Notepad::showGraphOptions()
{
    QList<Word> wordList;

    Dialog * newDialog = new Dialog();
    newDialog->exec();                                               //showing graph dialog

    if(newDialog->getDataSource() == "Auto Generate"){
        if(ui->editor->toPlainText() == ""){                        //checking if the Notepad's content is empty
            QMessageBox::warning(this, tr("Text Editor is Empty"),
                                 tr("Populate the text editor or load a file."),
                                 QMessageBox::Ok);
        }
        else{
            QList<Word> wordList;                         //Creating a list to store the words
            QString text = ui->editor->toPlainText();     //Saving the notepad's content
            QStringList list = text.split(QRegularExpression("[\r\n\t ;:,.?!]+"), Qt::SkipEmptyParts); //Splitting the text and saving it inside the list

            // Unique words
            for (int i = 0; i < list.size(); i++)
            {
                bool flag = true;

                Word currentWord(list.at(i).toLower());

                for (int i = 0; i < wordList.size(); i++)
                {
                    if (wordList[i] == currentWord)
                    {
                        flag = false;
                        ++wordList[i];
                        break;
                    }
                }

                if (flag)
                    wordList << currentWord;
            }

            //Sort the QList according to the frequency
            std::sort(wordList.begin(), wordList.end());

            generateGraph(newDialog->getchartType(), newDialog->getGraphQuantity(), wordList); //Generating graph base on the other user's options
        }
        return;
    }
    else if (newDialog->getDataSource() == "File"){  //If user chooses to create a graph base on a new file's content

        QString fileName = QFileDialog::getOpenFileName(this, "Select file",QDir::currentPath(),"*.sta"); //opening file dialog
        QFile file(fileName);

        if(!file.open((QIODevice::ReadOnly) | QFile::Text)) //If the opening of the file wasn't successfull
        {
            QMessageBox::warning(this, tr("Error"),tr("No file was opened")); //we display a warning message
            return;
        }

        QTextStream fileStream(&file);
        QStringList stringList;

        if(fileStream.readLine().isEmpty()){  //If the file's contnet is empty, we display a warning
            QMessageBox::warning(this, tr("Empty File"),tr("Fill does not contain any text."
                                                           "\nCould not generate chart."));
            return;
        }

        while(!fileStream.atEnd()){
            stringList.append(fileStream.readLine());  //Saving each text line inside the list
        }

        for(int i=4; i > 0; i--){   //Removing the first five lines
            stringList.removeFirst();
        }

        int j = 0;
        for(int i=stringList.size()-1; i >= 0; i--, j++){ //Looping through the list
            QStringList string;
            string = stringList[i].split(" ");   //separating the words in the list
            wordList.append(string[0]);          //appending it to the wordList
            wordList[j].setCount(string[1].toInt());
        }

        file.close(); //Closing the file
        generateGraph(newDialog->getchartType(), newDialog->getGraphQuantity(), wordList); //Generating the graph base on the other user's options
        return;
    }

}

void Notepad::generateStatFile()
{
    QList<Word> wordList;

    if(savedText.isEmpty()){ //If the notepad's content is empty, we display a warning message
        QMessageBox::warning(this, tr("Error"),tr("No Opened Document."));
        return;
    }


    QStringList currentName = currentFile.split("/");
    QStringList name = currentName.last().split(".");

    QFile file(name[0] + "_statistics.sta");  //Creating file with current file name and appending "_statistics.sta"
    QTextStream stream( &file );

    if( file.open(QIODevice::ReadWrite) ) //We open the file for writting
    {
        QString text = ui->editor->toPlainText();
        QStringList list = text.split(QRegularExpression("[\r\n\t ;:,.?!]+"), Qt::SkipEmptyParts);

        // Unique words
        for (int i = 0; i < list.size(); i++)
        {
            bool flag = true;
            Word currentWord(list.at(i).toLower());

            for (int i = 0; i < wordList.size(); i++) {
                if (wordList[i] == currentWord) {
                    flag = false;
                    ++wordList[i];
                    break;
                }
            }

            if (flag)
                wordList << currentWord;
        }

        //Sort the QList according to the frequency
        std::sort(wordList.begin(), wordList.end());

        //Writting to the file

        stream << "Statistical Data for '"<< currentName.last()<<"'\n";
        stream << "Generated Date: "<< QDate::currentDate().toString("dd-MM-yyyy") <<"\n";
        stream << "Total Word Count: "<< list.size() <<"\n";
        stream << "Unique Word Count: "<< wordList.size() <<"\n";
        stream << "Word Frequency:" <<"\n";

        for (int i = wordList.size() - 1; i >= 0 ; i--){
            stream << wordList[i].getText() <<" " <<wordList[i].getCount() <<"\n";
        }

        QMessageBox msgBox;
        msgBox.setText("The Statistical Data File was successfully generated.");
        msgBox.exec();

    }
}

void Notepad::generateGraph(QString chartType, QString graphQuantity, QList<Word> wordList)
{
    int quantity;

    if(graphQuantity == "Show Top 10")
        quantity = 10;
    else if(graphQuantity == "Show Top 20")
        quantity = 20;
    else
        quantity = wordList.size();

    if(chartType == "Line"){

        //Create the Line Chart
        QLineSeries *series = new QLineSeries();

        for (int i = wordList.size() - 1, counter = 0; i >= 0 && counter < quantity; ++counter, i--){
            series->append(counter+1,wordList[i].getCount());
        }

        QChart *chart = new QChart();
        chart->legend()->hide();
        chart->addSeries(series);
        chart->createDefaultAxes();
        chart->setTitle("Most Frequent Words");

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        QMainWindow *window = new QMainWindow(this);
        window->setCentralWidget(chartView);
        window->resize(700,600);
        window->show();
    }
    else if(chartType == "Bar"){

        //Create the Bar Chart
        QChart *chart = new QChart();
        QBarSeries *series = new QBarSeries();

        for (int i = wordList.size() - 1, counter = 0; i >= 0 && counter < quantity; counter++, i--){
            QBarSet *set = new QBarSet(wordList[i].getText());
            *set << wordList[i].getCount();
            series->append(set);
        }

        chart->addSeries(series);
        chart->setTitle("Most Frequent Words");
        chart->setAnimationOptions(QChart::AllAnimations);

        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);
        chart->createDefaultAxes();

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->resize(700,600);
        chartView->show();
    }
    else{
        //Create the Pie Chart

        QPieSeries *series = new QPieSeries();

        for (int i = wordList.size() - 1, counter = 0; i >= 0 && counter < quantity; counter++, i--){
            series->append(wordList[i].getText(),wordList[i].getCount());
        }

        QPieSlice *slice = series->slices().at(0);
        slice->setExploded();
        slice->setLabelVisible();
        slice->setPen(QPen(Qt::darkGreen, 2));
        slice->setBrush(Qt::green);

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Most Frequent Words");
        chart->legend();

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        QMainWindow *window = new QMainWindow(this);
        window->setCentralWidget(chartView);
        window->resize(700,600);
        window->show();
    }
}
