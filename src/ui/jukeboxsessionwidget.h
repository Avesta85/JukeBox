#ifndef JUKEBOXSESSIONWIDGET_H
#define JUKEBOXSESSIONWIDGET_H

#include <QDockWidget>

namespace Ui {
class JukeBoxSessionWidget;
}

class JukeBoxSessionWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit JukeBoxSessionWidget(QWidget *parent = nullptr);
    ~JukeBoxSessionWidget();



signals:
    void createSessionClicked();
    void joinSessionClicked();
    void connectToHostClicked(const QString& ipAddress);
    void leaveSessionClicked();
    void backFromJoiningClicked();


public slots:
    void showDisconnectedState();
    void showJoiningState();
    void showConnectedState();
    void updateParticipantList(const QStringList& participants);


private slots:

    void on_pushButton_create_session_clicked();
    void on_pushButton_join_session_clicked();
    void on_pushButton_conect_clicked();
    void on_pushButton_backe_clicked();
    void on_pushButton_leave_clicked();
private:
    Ui::JukeBoxSessionWidget *ui;
};

#endif // JUKEBOXSESSIONWIDGET_H
