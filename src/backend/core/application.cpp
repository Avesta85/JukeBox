#include "application.h"
#include "src/ui/createdialog.h"
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

#include <src/backend/network/sessionmanager.h>
#include <windows.h>


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
void Application::show_choiceWindow()
{
    if (!w_choice_window) {
        w_choice_window = new ChoiceWindow();

        connect(w_choice_window,&ChoiceWindow::LoginRequest,this,&Application::show_loginWindow);
        connect(w_choice_window,&ChoiceWindow::SignupRequest,this,&Application::show_signupWindow);
    }
    switchWindow(w_choice_window);
}
void Application::show_loginWindow()
{
    if (!w_login_window) {
        w_login_window = new LoginWindow();

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
    w_playMusic_window(nullptr),w_FavoritSongs(nullptr),w_Friend_Window(nullptr),w_Queue_window(nullptr),
    w_onlineChoice(nullptr),w_onlineChat(nullptr),w_onlineJoin(nullptr),w_onlicechatHost(nullptr)
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

        if(!visualizer)
            visualizer = AudioVisualizer::getInstance();


        ToolBoxWidget* toolbox = w_main_window->getToolBox();
        StageWidget* stage = w_main_window->getStage();


        connect(toolbox, &ToolBoxWidget::movieManagementClicked, stage, &StageWidget::showMovieManagementPage);
        connect(toolbox, &ToolBoxWidget::videoManagementClicked, stage, &StageWidget::showVideoManagementPage);
        connect(toolbox, &ToolBoxWidget::visualizerClicked, stage, &StageWidget::showVisualizerPage);
        connect(toolbox, &ToolBoxWidget::coverArtClicke, stage, &StageWidget::showCoverArtPage);
        connect(toolbox, &ToolBoxWidget::playlistManagementClicked, this, &Application::show_playlistWindow);
        connect(toolbox, &ToolBoxWidget::SongManagementClicked, this, &Application::show_playMusicWindow);
        connect(toolbox, &ToolBoxWidget::FavoriteSongsClicked, this, &Application::show_FavoriteSongWindow);
        connect(toolbox, &ToolBoxWidget::FriendsListClicked, this, &Application::show_FriendWindow);
        connect(toolbox, &ToolBoxWidget::QueueClicked, this, &Application::show_QueueWindow);
        connect(toolbox, &ToolBoxWidget::backCoverArt, stage, &StageWidget::showCoverArtPage);

        connect(toolbox, &ToolBoxWidget::onlineManagmentClicked, this, &Application::show_sessionWindow);
        PlayerManager& playerManager = PlayerManager::getInstance();
        SessionManager& sessionManager = SessionManager::getInstance();
        PlayerControlWidget* playerControls = w_main_window->getPlayerControls();

        QObject::connect(&playerManager, &PlayerManager::playbackStateChanged,
                         visualizer, &AudioVisualizer::setPlaybackState);

        QObject::connect(&playerManager, &PlayerManager::volumeChanged,
                         visualizer, &AudioVisualizer::updateAudioLevel);

        connect(playerControls, &PlayerControlWidget::playPauseClicked, this, &Application::onPlayPauseClicked);
        connect(playerControls, &PlayerControlWidget::seeked, this, &Application::onSeeked);
        connect(playerControls, &PlayerControlWidget::nextClicked, &playerManager, &PlayerManager::next);
        connect(playerControls, &PlayerControlWidget::previousClicked, &playerManager, &PlayerManager::previous);
        connect(this,&Application::control_setSongInfo,playerControls, &PlayerControlWidget::setCurrentSong);
        connect(playerControls, &PlayerControlWidget::volumeChanged, &playerManager, [&playerManager](int volume){
            playerManager.setVolume(static_cast<float>(volume) / 100.0f);
        });

        connect(playerControls, &PlayerControlWidget::addFavorit,this,[](qint64 songid){
            qDebug()<<"added to favorite "<<songid <<"=>"<<UserManager::getInstance().addFavoriteSong(songid);
        });
        connect(playerControls, &PlayerControlWidget::deleteFavorite,this,[](qint64 songid){
            qDebug()<<"removed to favorite "<<songid <<"=>"<< UserManager::getInstance().deleteFavoriteSong(songid);
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
        connect(&playerManager, &PlayerManager::currentSongChanged, this , [this](const Song song){
            auto list = UserManager::getInstance().getUserFavoriteSongs();
            for(auto& s : list){
                if(song.getPath() == s.getPath()){

                    emit control_setSongInfo(song.getID(),true);
                    return;
                }
            }
            emit control_setSongInfo(song.getID(),false);


        });

        connect(stage, &StageWidget::videoFileSelected, this, &Application::onVideoFileSelected);

        connect(this,&Application::play_from_playlist,&playerManager,&PlayerManager::loadPlaylist);

        connect(playerControls, &PlayerControlWidget::repeatModeClicked, this, [playerControls]() {
            PlayerManager::getInstance().changeRepeatMode();
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
                case PlayerManager::RepeatMode::NoRepeat:
                    icon = QIcon(":/icone/arrow.png");
                    break;
                }
            playerControls->setRepeatIcon(icon);
        });
        this->show_online_choice();

        connect(&playerManager, &PlayerManager::RequestForsync, &sessionManager, &SessionManager::RequestsyncSong);
        connect(&sessionManager, &SessionManager::songsynced, &playerManager, &PlayerManager::syncedsussecfully);

        connect(&sessionManager, &SessionManager::remotePlayRequested, this, &Application::onRemotePlay);
        connect(&sessionManager, &SessionManager::remotePauseRequested, this, &Application::onRemotePause);
        connect(&sessionManager, &SessionManager::remoteSeekRequested, this, &Application::onRemoteSeek);
        connect(&sessionManager, &SessionManager::setCurrentSong, this, &Application::onSetCurrentSong);

        connect(&sessionManager, &SessionManager::syncStatusChanged, this, [](const QString& status){
            qDebug() << "[SYNC STATUS]" << status;
        });
        connect(&sessionManager, &SessionManager::syncError, this, [](const QString& error){
            qDebug() << "[SYNC ERROR]" << error;
        });
    }

    switchWindow(w_main_window);
}

void Application::show_playlistCreateWindow()
{

    if(!w_playlist_createWindow){
        w_playlist_createWindow = new mycreateDialog();

        connect(w_playlist_createWindow, &mycreateDialog::CreatePlaylist, this,
                &Application::Create_PlayList);
        connect(this, &Application::Song_view_update, w_playlist_createWindow,
                &mycreateDialog::update_songsList);
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

void Application::show_FavoriteSongWindow()
{
    if(!w_FavoritSongs){
        w_FavoritSongs = new Dialog_FavoriteSongs();

        connect(this,&Application::update_view_favoriteSongs,w_FavoritSongs,&Dialog_FavoriteSongs::update_favoriteSongs);
        connect(w_FavoritSongs,&Dialog_FavoriteSongs::playSongsRequest,&PlayerManager::getInstance(),&PlayerManager::loadSingleMedia);
    }

    emit update_view_favoriteSongs(UserManager::getInstance().getUserFavoriteSongs());
    w_FavoritSongs->show();
}

void Application::show_FriendWindow()
{
    if(!w_Friend_Window){
        w_Friend_Window = new Dialog_Friends();

        connect(this,&Application::update_Friend_view,w_Friend_Window,&Dialog_Friends::update_friend_view);
        connect(w_Friend_Window,&Dialog_Friends::delete_Friend,this,&Application::delete_friend_fromList);
    }

    emit update_Friend_view(UserManager::getInstance().getUserFriend());
    w_Friend_Window->show();
}

void Application::show_QueueWindow()
{
    if(!w_Queue_window){
        w_Queue_window = new Dialog_Queue();

        connect(this,&Application::Queue_update_Window,w_Queue_window,&Dialog_Queue::show_songs_list);
        connect(w_Queue_window,&Dialog_Queue::play_Queue,this,&Application::preparetoPlay_Queue);
    }

    emit Queue_update_Window(DBM::get_instance().getAllSongs());
    w_Queue_window->show();
}

void Application::show_sessionWindow()
{















}

void Application::show_online_choice()
{
    if(!w_onlineChoice){
        w_onlineChoice = new online_choice();
        w_main_window->getStack().addWidget(w_onlineChoice);
        connect(w_onlineChoice,&online_choice::JoinRequest,this,&Application::show_online_join);
        connect(w_onlineChoice,&online_choice::CreateRequest,this,&Application::show_online_chatHost);
    }

    w_main_window->getStack().setCurrentWidget(w_onlineChoice);
}

void Application::show_online_join()
{
    if(!w_onlineJoin){
        w_onlineJoin = new onlineJoin();
        w_main_window->getStack().addWidget(w_onlineJoin);
        connect(w_onlineJoin,&onlineJoin::accepted,this,&Application::show_online_chat);
        connect(w_onlineJoin,&onlineJoin::cancel,this,&Application::show_online_choice);
        connect(w_onlineJoin,&onlineJoin::requestJoin,&SessionManager::getInstance(),&SessionManager::joinSession);
        connect(&SessionManager::getInstance(),&SessionManager::joinRequestaccept,this,[this](){
            emit w_onlineJoin->accepted();
        });
        connect(&SessionManager::getInstance(),&SessionManager::joinRequestReject,this,[this](){
            emit w_onlineJoin->cancel();
        });
    }

    w_main_window->getStack().setCurrentWidget(w_onlineJoin);
}

void Application::show_online_chatHost()
{
    if(!w_onlicechatHost){
        w_onlicechatHost = new onlineChatHost();
        w_main_window->getStack().addWidget(w_onlicechatHost);

        auto&& session = SessionManager::getInstance();
        connect(&session, &SessionManager::newChatMessageForUI,
                w_onlicechatHost, &onlineChatHost::onNewMessage);

        connect(w_onlicechatHost, &onlineChatHost::sendMessage,
                &session, &SessionManager::onChatMessageSendRequested);

        connect(&session, &SessionManager::participantListChanged,
                w_onlicechatHost, [this](const QList<Person>& persons){
                    w_onlicechatHost->updateParticipants(persons);
                });


        connect(w_onlicechatHost, &onlineChatHost::disconnectUsers,
                &session, &SessionManager::kickUsers);

        connect(&session, &SessionManager::participantListChanged,
                w_onlicechatHost, [this](const QList<Person>& persons) {
                    w_onlicechatHost->updateParticipants(persons);
                });

        connect(w_onlicechatHost, &onlineChatHost::endSession,
                &session, &SessionManager::leaveSession);

        connect(w_onlicechatHost, &onlineChatHost::endSession,
                this, &Application::show_online_choice);

        connect(&session, &SessionManager::showInfoMessage,
                w_onlicechatHost, [](const QString& msg) {
                    qDebug() << "Session Info:" << msg;
                });
    }

    w_main_window->getStack().setCurrentWidget(w_onlicechatHost);
}

void Application::show_online_chat()
{
    if(!w_onlineChat){
        w_onlineChat = new onlinechat();
        w_main_window->getStack().addWidget(w_onlineChat);

        auto&& session = SessionManager::getInstance();
        if (session.getSessioonActive() && session.getInstance().ISHost()) {
            connect(w_onlineChat, &onlinechat::sendMessage, &session, &SessionManager::onChatMessageSendRequested);
        } else {
            connect(w_onlineChat, &onlinechat::sendMessage, &session, &SessionManager::sendChatMessageToHost);
        }
        connect(&session, &SessionManager::newChatMessageForUI, w_onlineChat, &onlinechat::onNewMessage);
        connect(&session, &SessionManager::participantListChanged, w_onlineChat, [this](const QList<Person>& persons) {
            QStringList usernames;
            for (const auto& person : persons) {
                usernames.append(person.getUserName());
            }
            w_onlineChat->updateParticipants(usernames);
        });

        connect(w_onlineChat, &onlinechat::leaveSession, &session, &SessionManager::leaveSession);
        connect(w_onlineChat, &onlinechat::leaveSession, this, &Application::show_online_choice);
        connect(&session, &SessionManager::systemMessage, w_onlineChat, &onlinechat::onSystemMessage);
        connect(&session, &SessionManager::kickedFromSession, w_onlineChat, &onlinechat::onKicked);
    }
    w_main_window->getStack().setCurrentWidget(w_onlineChat);
}

void Application::prepertoPlay_RecivedSong(const Song currSong)
{
    PlayerManager::getInstance().loadSingleMedia(currSong.getPath());
}

void Application::preparetoPlay_playList(qint64 playlistID)
{

    auto songs = UserManager::getInstance().getUserPlaylistSong(playlistID);

    emit play_from_playlist(songs);

}

void Application::preparetoPlay_Queue(const QList<qint64> SongsId)
{
    QList<Song> tmp;

    for(const auto& id : SongsId){
        Song themp;
        if((themp = DBM::get_instance().getSongFromID(id)).getName() !=""){
            tmp.append(themp);
        }
    }
    emit play_from_playlist(tmp);
}

void Application::delete_friend_fromList(const QList<Person> deletedFriend)
{
    try{
        auto&& UM= UserManager::getInstance();
        for(auto& p : deletedFriend)
        {
            UM.deleteFriend(p.getUserName());
        }
        emit update_Friend_view(UserManager::getInstance().getUserFriend());
    }
    catch(std::exception& e)
    {
        qDebug() << e.what();
    }
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

void Application::onRemotePlay(qint64 position) {
    if (!m_songReady) {
        m_pendingRemotePlay = true;
        m_pendingRemotePlayPosition = position;
        return;
    }
    PlayerManager::getInstance().loadSingleMedia(m_sessionSong.getPath());
    PlayerManager::getInstance().seek(position);
    PlayerManager::getInstance().play();
}
void Application::onRemotePause() {
    PlayerManager::getInstance().pause();
}
void Application::onRemoteSeek(qint64 position) {
    PlayerManager::getInstance().seek(position);
}

void Application::onSetCurrentSong(const Song& song) {
    m_sessionSong = song;
    m_songReady = !song.getPath().isEmpty();
    if (m_pendingRemotePlay && m_songReady) {
        PlayerManager::getInstance().loadSingleMedia(m_sessionSong.getPath());
        PlayerManager::getInstance().seek(m_pendingRemotePlayPosition);
        PlayerManager::getInstance().play();
        m_pendingRemotePlay = false;
    }
}

void Application::onPlayPauseClicked() {
    PlayerManager& playerManager = PlayerManager::getInstance();
    SessionManager& sessionManager = SessionManager::getInstance();
    bool wasPlaying = (playerManager.getPlaybackState() == QMediaPlayer::PlayingState);
    playerManager.togglePlayPause();
    if (sessionManager.getSessioonActive() && sessionManager.ISHost()) {
        if (wasPlaying) {
            sessionManager.broadcastPauseCommand();
        } else {
            sessionManager.broadcastPlayCommand(playerManager.getCurrentPosition());
        }
    }
}

void Application::onSeeked(qint64 position) {
    PlayerManager& playerManager = PlayerManager::getInstance();
    SessionManager& sessionManager = SessionManager::getInstance();
    playerManager.seek(position);
    if (sessionManager.getSessioonActive() && sessionManager.ISHost()) {
        sessionManager.broadcastSeekCommand(position);
    }
}

Application::~Application() {
    if (w_change_password_window) delete w_change_password_window;
    if (w_choice_window) delete w_choice_window;
    if (w_email_verification_window) delete w_email_verification_window;
    if (w_forgot_password_window) delete w_forgot_password_window;
    if (w_login_window) delete w_login_window;
    if (w_receive_secureWords_window) delete w_receive_secureWords_window;
    if (w_signUp_window) delete w_signUp_window;
    if (w_showKey_Window) delete w_showKey_Window;
    if (w_main_window) delete w_main_window;
    if (w_playlist_choicewindow) delete w_playlist_choicewindow;
    if (w_playlist_createWindow) delete w_playlist_createWindow;
    if (w_playlist_editWindow) delete w_playlist_editWindow;
    if (w_playMusic_window) delete w_playMusic_window;
    if (w_FavoritSongs) delete w_FavoritSongs;
    if (w_Friend_Window) delete w_Friend_Window;
    if (w_Queue_window) delete w_Queue_window;
    if (w_onlineChoice) delete w_onlineChoice;
    if (w_onlineJoin) delete w_onlineJoin;
    if (w_onlineChat) delete w_onlineChat;
    if (w_onlicechatHost) delete w_onlicechatHost;
}

