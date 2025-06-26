#include "showkeywords.h"
#include "ui_showkeywords.h"

ShowKeyWords::ShowKeyWords(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ShowKeyWords)
{
    ui->setupUi(this);
}

ShowKeyWords::~ShowKeyWords()
{
    delete ui;
}
