#ifndef HISTORYFORM_H
#define HISTORYFORM_H

#include <QWidget>
#include "daofactory.h"
#include "clockentity.h"

namespace Ui {
class HistoryForm;
}

class HistoryForm : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryForm(QWidget *parent = 0);
    ~HistoryForm();

private slots:

    void on_dateEdit_start_dateChanged(const QDate &date);

    void on_dateEdit_end_dateChanged(const QDate &date);

private:
    Ui::HistoryForm *ui;
};

#endif // HISTORYFORM_H
