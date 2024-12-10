#include "reactionparametersdialog.h"
#include "ui_reactionparametersdialog.h"

ReactionParametersDialog::ReactionParametersDialog(double rateConstant, double reactionOrder, double dispersion, double correlation, QWidget *parent):
    QDialog(parent),
    ui(new Ui::ReactionParametersDialog)
{
    ui->setupUi(this);
    ui->rateConstantLabel->setText(QString("Константа скорости: %1").arg(rateConstant));
    ui->reactionOrderLabel->setText(QString("Порядок реакции: %1").arg(reactionOrder));
    ui->dispersionLabel->setText(QString("Дисперсия: %1").arg(dispersion));
    ui->correlationLabel->setText(QString("Коэффициент корреляции: %1").arg(correlation));

    ui->rateConstantLabel->adjustSize();
    ui->reactionOrderLabel->adjustSize();
    ui->dispersionLabel->adjustSize();
    ui->correlationLabel->adjustSize();
}

ReactionParametersDialog::~ReactionParametersDialog()
{
    delete ui;
}


void ReactionParametersDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    ReactionParametersDialog::close();
}

