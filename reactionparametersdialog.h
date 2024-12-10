#ifndef REACTIONPARAMETERSDIALOG_H
#define REACTIONPARAMETERSDIALOG_H

#include <QDialog>
#include <qabstractbutton.h>

namespace Ui {
class ReactionParametersDialog;
}

class ReactionParametersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReactionParametersDialog(double rateConstant, double reactionOrder, double dispersion, double correlation, QWidget *parent = nullptr);
    ~ReactionParametersDialog();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::ReactionParametersDialog *ui;
};

#endif // REACTIONPARAMETERSDIALOG_H
