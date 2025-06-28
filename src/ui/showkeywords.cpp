#include "showkeywords.h"
#include "ui_showkeywords.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QDebug> // برای qWarning

ShowKeyWords::ShowKeyWords(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ShowKeyWords)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icone/musicplayer"));
}

ShowKeyWords::~ShowKeyWords()
{
    delete ui;
}

