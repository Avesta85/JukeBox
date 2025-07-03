#ifndef JUKEBOXSESSIONWIDGET_H
#define JUKEBOXSESSIONWIDGET_H

#include <QDockWidget>
#include <QTimer>
#include "src/backend/core/person.h"

namespace Ui {
class JukeBoxSessionWidget;
}

class JukeBoxSessionWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit JukeBoxSessionWidget(QWidget *parent = nullptr);
    ~JukeBoxSessionWidget();
    QTimer* m_connectionTimeoutTimer = nullptr;
    bool& getConnecting();



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
    void showParticipantListState();
    void enableConnectButten();
    void resetJoinForm();
    void onConnectionSuccess(const QList<Person>& persons);
    void onConnectionFailed();

private slots:

    void on_pushButton_create_session_clicked();
    void on_pushButton_join_session_clicked();
    void on_pushButton_conect_clicked();
    void on_pushButton_backe_clicked();
    void on_pushButton_leave_clicked();
private:
    Ui::JukeBoxSessionWidget *ui;
    bool m_isConnecting = false;


};

#endif // JUKEBOXSESSIONWIDGET_H

