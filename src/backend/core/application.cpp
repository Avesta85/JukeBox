#include "application.h"
#include "src/backend/core/UserManager.h"
#include "src/backend/core/playermanager.h"
#include "src/backend/db/DBM.h"
#include "src/ui/changepasswordwindow.h"
#include "src/ui/choicewindow.h"
#include "src/ui/loginwindow.h"
#include "src/ui/mainwindow.h"
#include "src/ui/playercontrolwidget.h"
#include "src/ui/showkeywords.h"
#include "src/ui/signupwindow.h"
#include "src/ui/emailverificationwindow.h"
#include "src/ui/frogotpasswordwindow.h"
#include "src/ui/receivesecurewordswindow.h"
#include "src/ui/stagewidget.h"
#include "src/ui/toolboxwidget.h"

#include <QThread>


std::unique_ptr<Application> Application::s_instance = nullptr;

Application &Application::getInstance()
{
    if(!s_instance)
    {
        s_instance.reset(new Application());
    }

    return *s_instance;
}

void Application::Run()
{

    DBM::get_instance();
    this->show_choiceWindow();
    UserManager::getInstance();
}
//ok
void Application::show_choiceWindow()
{
    if (!w_choice_window) {
        w_choice_window = new ChoiceWindow();

        // signals
        connect(w_choice_window,&ChoiceWindow::LoginRequest,this,&Application::show_loginWindow);
        connect(w_choice_window,&ChoiceWindow::SignupRequest,this,&Application::show_signupWindow);
    }
    switchWindow(w_choice_window);
}
//ok
void Application::show_loginWindow()
{
    if (!w_login_window) {
        w_login_window = new LoginWindow();

        // signals
        connect(w_login_window,&LoginWindow::CancelOperation,this,&Application::show_choiceWindow);
        connect(w_login_window,&LoginWindow::ForgotPassword,this,&Application::show_forgotPassword_window);
        connect(w_login_window, &LoginWindow::LoggedInsuccessfully, this, &Application::showMainWindow);
    }
    switchWindow(w_login_window);
}

void Application::show_signupWindow()
{
    if(!w_signUp_window){
        w_signUp_window = new SignupWindow();

        connect(w_signUp_window,&SignupWindow::backToChoiseWindow,this,&Application::show_choiceWindow);
        connect(w_signUp_window,&SignupWindow::showkey,this,&Application::show_showKeyWindow);
    }
    switchWindow(w_signUp_window);
}
//ok
void Application::show_changePasswordWindow(QString Username)
{
    if(!w_change_password_window){
        w_change_password_window = new ChangePasswordWindow();
        connect(w_change_password_window,&ChangePasswordWindow::goToLoginWindow,this,&Application::show_loginWindow);
    }
    w_change_password_window->set_username(Username);
    switchWindow(w_change_password_window);
}

void Application::show_emailVWindow(QString Email, QString Username)
{
    if(!w_email_verification_window){
        w_email_verification_window = new EmailVerificationWindow();

        connect(w_email_verification_window,&EmailVerificationWindow::backToForgetPassWindow,this,&Application::show_forgotPassword_window);
        connect(w_email_verification_window,&EmailVerificationWindow::passVerification_gotoChangepassword,this,&Application::show_changePasswordWindow);
    }
    w_email_verification_window->setResiverUsername(Username);
    w_email_verification_window->Emailsender(Email);
    switchWindow(w_email_verification_window);
}
//ok
void Application::show_forgotPassword_window()
{
    if(!w_forgot_password_window){
        w_forgot_password_window = new FrogotPasswordWindow();

        connect(w_forgot_password_window ,&FrogotPasswordWindow::backToLoginWindow,this,&Application::show_loginWindow);
        connect(w_forgot_password_window ,&FrogotPasswordWindow::EmailVerification,this,&Application::show_emailVWindow);
        connect(w_forgot_password_window ,&FrogotPasswordWindow::SecurityVerification,this,&Application::show_receiveSWWindow);
    }
    switchWindow(w_forgot_password_window);
}

void Application::show_receiveSWWindow(QString key, QString username)
{
    if(!w_receive_secureWords_window){
        w_receive_secureWords_window = new ReceiveSecureWordsWindow();
        connect(w_receive_secureWords_window ,&ReceiveSecureWordsWindow::backToForgetPassWindow,this,&Application::show_forgotPassword_window);
        connect(w_receive_secureWords_window ,&ReceiveSecureWordsWindow::passed,this,&Application::show_changePasswordWindow);

    }
    w_receive_secureWords_window->setkey(key);
    w_receive_secureWords_window->setusername(username);
    switchWindow(w_receive_secureWords_window);
}

void Application::show_showKeyWindow(QString key)
{
    if(!w_showKey_Window){
        w_showKey_Window = new ShowKeyWords();
        connect(w_showKey_Window ,&ShowKeyWords::signup_successfully,this,&Application::show_loginWindow);
    }
    w_showKey_Window->set_key(key);
    switchWindow(w_showKey_Window);
}

void Application::show_playlistWindow()
{
    if(!w_playlist_choicewindow){
        w_playlist_choicewindow = new PlaylistChoiceWindow();

        connect(w_playlist_choicewindow,&PlaylistChoiceWindow::deletePlaylistRequested,this,&Application::Delete_PlayList);
        connect(this,&Application::Playlist_view_updated,w_playlist_choicewindow,&PlaylistChoiceWindow::updatePlaylistView);
        connect(w_playlist_choicewindow,&PlaylistChoiceWindow::createPlaylistRequested,this,&Application::show_playlistCreateWindow);
        connect(w_playlist_choicewindow,&PlaylistChoiceWindow::editPlaylistRequested,this,&Application::show_playlistEditWindow);

        connect(w_playlist_choicewindow,&PlaylistChoiceWindow::playPlaylistRequested,this,&Application::preparetoPlay_playList);
    }


    emit Playlist_view_updated(UserManager::getInstance().getUserPLaylist());
    w_playlist_choicewindow->show();
}

Application::Application(QObject *parent)
    :QObject(parent),m_currentWindow(nullptr),w_change_password_window(nullptr)
    ,w_choice_window(nullptr),w_email_verification_window(nullptr),w_forgot_password_window(nullptr)
    ,w_login_window(nullptr),w_receive_secureWords_window(nullptr),w_signUp_window(nullptr),w_showKey_Window(nullptr),
    w_main_window(nullptr),w_playlist_choicewindow(nullptr),w_playlist_createWindow(nullptr),w_playlist_editWindow(nullptr),
    w_playMusic_window(nullptr)
{

}

void Application::switchWindow(QWidget *nextWindow)
{
    if(!nextWindow) return;
    nextWindow->show();
    if(m_currentWindow)
    {
        m_currentWindow->hide();
    }
    m_currentWindow = nextWindow;
}

void Application::onVideoFileSelected(const QString &filePath)
{
    if (!w_main_window) return;
    StageWidget* stage = w_main_window->getStage();
    QVideoWidget* videoWidget = stage->getVideoManagementWidget();
    if (!videoWidget) return;
    PlayerManager::getInstance().loadSingleVideo(filePath, videoWidget);
}


void Application::showMainWindow()
{
    if(!w_main_window)
    {
        w_main_window = new MainWindow();

        ToolBoxWidget* toolbox = w_main_window->getToolBox();
        StageWidget* stage = w_main_window->getStage();

        connect(toolbox, &ToolBoxWidget::movieManagementClicked, stage, &StageWidget::showMovieManagementPage);
        connect(toolbox, &ToolBoxWidget::videoManagementClicked, stage, &StageWidget::showVideoManagementPage);
        connect(toolbox, &ToolBoxWidget::playlistManagementClicked, this, &Application::show_playlistWindow);
        connect(toolbox, &ToolBoxWidget::SongManagementClicked, this, &Application::show_playMusicWindow);
        connect(toolbox, &ToolBoxWidget::onlineManagmentClicked, this, &Application::show_sessionWindow);

        PlayerManager& playerManager = PlayerManager::getInstance();
        PlayerControlWidget* playerControls = w_main_window->getPlayerControls();

        connect(playerControls, &PlayerControlWidget::playPauseClicked, &playerManager, &PlayerManager::togglePlayPause);
        connect(playerControls, &PlayerControlWidget::nextClicked, &playerManager, &PlayerManager::next);
        connect(playerControls, &PlayerControlWidget::previousClicked, &playerManager, &PlayerManager::previous);
        connect(playerControls, &PlayerControlWidget::seeked, &playerManager, &PlayerManager::seek);

        connect(playerControls, &PlayerControlWidget::volumeChanged, &playerManager, [&playerManager](int volume){
            playerManager.setVolume(static_cast<float>(volume) / 100.0f);
        });

        connect(playerControls, &PlayerControlWidget::muteClicked, &playerManager, &PlayerManager::setMuted);

        connect(&playerManager, &PlayerManager::positionChanged, playerControls, &PlayerControlWidget::updatePosition);
        connect(&playerManager, &PlayerManager::durationChanged, playerControls, &PlayerControlWidget::updateDuration);
        connect(&playerManager, &PlayerManager::mutedChanged, playerControls, &PlayerControlWidget::setMuted);

        connect(&playerManager, &PlayerManager::playbackStateChanged, playerControls, [playerControls](QMediaPlayer::PlaybackState state){
            playerControls->updatePlaybackState(state == QMediaPlayer::PlayingState);
        });
        connect(&playerManager, &PlayerManager::volumeChanged, playerControls, &PlayerControlWidget::updateVolume);
        connect(&playerManager, &PlayerManager::currentSongChanged, w_main_window, &MainWindow::updateSongInfo);

        connect(stage, &StageWidget::videoFileSelected, this, &Application::onVideoFileSelected);

        connect(this,&Application::play_from_playlist,&playerManager,&PlayerManager::loadPlaylist);

        connect(playerControls, &PlayerControlWidget::repeatModeClicked, this, [playerControls]() {
            PlayerManager::getInstance().changeRepeatMode();
            // Update icon based on new mode
            auto mode = PlayerManager::getInstance().getRepeatMode();
            QIcon icon;
            switch (mode) {
                case PlayerManager::RepeatMode::Shuffle:
                    icon = QIcon(":/icone/shuffle.png");
                    break;
                case PlayerManager::RepeatMode::RepeatOne:
                    icon = QIcon(":/icone/repeatone.png");
                    break;
                case PlayerManager::RepeatMode::RepeatAll:
                    icon = QIcon(":/icone/repeat.png");
                    break;
            }
            playerControls->setRepeatIcon(icon);
        });
    }

    switchWindow(w_main_window);
}

void Application::show_playlistCreateWindow()
{

    if(!w_playlist_createWindow){
        w_playlist_createWindow = new CreateDialog();

        connect(w_playlist_createWindow,&CreateDialog::CreatePlaylist,this,&Application::Create_PlayList);
        connect(this,&Application::Song_view_update,w_playlist_createWindow,&CreateDialog::update_songsList);
    }

    emit Song_view_update(DBM::get_instance().getAllSongs());
    w_playlist_createWindow->show();
}

void Application::show_playlistEditWindow(qint64 playlistID)
{
    if(!w_playlist_editWindow){
        w_playlist_editWindow = new EditPlayList();

        connect(w_playlist_editWindow,&EditPlayList::update_playlistName,this,&Application::Edit_PlaylistName);
        connect(w_playlist_editWindow,&EditPlayList::update_songToPlaylist,this,&::Application::edite_update_playlistSongs);
        connect(this,&Application::edit_Playlist_Song_view,w_playlist_editWindow,&EditPlayList::update_SongView);


        connect(w_playlist_editWindow,&EditPlayList::accepted,this,[this](){
            emit Playlist_view_updated(UserManager::getInstance().getUserPLaylist());
            w_playlist_editWindow->close();
        });
        connect(w_playlist_editWindow,&EditPlayList::rejected,this,[this](){
            w_playlist_editWindow->close();
        });
    }

    Playlist tmp = DBM::get_instance().selectPlaylist(playlistID);


    emit edit_Playlist_Song_view(DBM::get_instance().getAllSongs(),DBM::get_instance().getSongsInPlaylist(playlistID),playlistID,tmp.getName());
    w_playlist_editWindow->show();

}

void Application::show_playMusicWindow()
{
    if(!w_playMusic_window){
        w_playMusic_window = new playmusicwindow();

        connect(this,&Application::music_updateSong,w_playMusic_window,&playmusicwindow::updateSongList);
        connect(w_playMusic_window,&playmusicwindow::songFileSelected,&PlayerManager::getInstance(),&PlayerManager::loadSingleMedia);
    }

    emit music_updateSong(DBM::get_instance().getAllSongs());
    w_playMusic_window->show();
}

void Application::show_sessionWindow()
{
    if (!w_session_window)
    {
        w_session_window = new JukeBoxSessionWidget();

        // اتصال یک‌باره به سیگنال Connect

        connect(w_session_window, &JukeBoxSessionWidget::createSessionClicked,
                this, [this]() {
                    SessionManager::getInstance().startNewSession(
                        UserManager::getInstance().getUserName(),
                        QHostAddress::Any,  // یا QHostAddress::LocalHost یا IP دلخواه
                        UDP_PORT
                        );


                    QStringList list;
                    list << UserManager::getInstance().getUserName();
                    w_session_window->updateParticipantList(list);
                    w_session_window->showParticipantListState();
                });


        disconnect(&SessionManager::getInstance(), nullptr, w_session_window, nullptr);

        connect(w_session_window, &JukeBoxSessionWidget::connectToHostClicked,
                this, [this](const QString& ip) {

                    SessionManager::getInstance().joinSession(QHostAddress(ip), UDP_PORT,
                                                              UserManager::getInstance().getUserName());

                    connect(&SessionManager::getInstance(), &SessionManager::showInfoMessage,
                            w_session_window, [=](const QString& msg) {
                               // w_session_window->setStatusMessage(msg);
                            });

                    connect(&SessionManager::getInstance(), &SessionManager::participantListChanged,
                            w_session_window, [=](const QList<Person>& persons) {
                                if (!SessionManager::getInstance().getSessioonActive())
                                    return;

                                w_session_window->onConnectionSuccess(persons);
                            });

                    // ⛔️ Timeout ایمن: بعد ۱۰ ثانیه اگر اتصال نگرفت، خطا
                    QTimer::singleShot(10000, w_session_window, [=]() {
                        if (!SessionManager::getInstance().getSessioonActive()) {
                            w_session_window->onConnectionFailed();
                        }
                    });


                });
    }

    w_session_window->show();
}


void Application::preparetoPlay_playList(qint64 playlistID)
{

    auto songs = UserManager::getInstance().getUserPlaylistSong(playlistID);

    emit play_from_playlist(songs);

}

void Application::Create_PlayList(const QString name , const QList<qint64>songs)
{
    qint64 playlistID = UserManager::getInstance().addPlaylist(Playlist(name));
    if(playlistID != -1){
        for (const qint64& id : songs){
            UserManager::getInstance().addSongToPlaylist(id,playlistID);
        }
        emit Playlist_view_updated(UserManager::getInstance().getUserPLaylist());
    }
}

void Application::Delete_PlayList(qint64 Playlistid)
{
    UserManager::getInstance().deletePlaylist(Playlistid);
    emit Playlist_view_updated(UserManager::getInstance().getUserPLaylist());
}

void Application::ADD_Songs(const QString &pahts)
{
    ;
}

void Application::Edit_PlaylistName(qint64 playListId, const QString &newName)
{
    UserManager::getInstance().UpdatePlaylistName(playListId,newName);
}

void Application::edite_update_playlistSongs(qint64 playListId, const QList<qint64> &newSongsId, const QList<qint64> &rmSongsId)
{
    try{
        auto&& um =UserManager::getInstance();

        for(const auto& id : newSongsId)
        {
            um.addSongToPlaylist(id,playListId);
        }
        for(const auto& id: rmSongsId)
        {
            um.deleteSongfromPlaylist(playListId,id);
        }
    }
    catch(std::exception& e)
    {
        qDebug()<<e.what();
    }

}
