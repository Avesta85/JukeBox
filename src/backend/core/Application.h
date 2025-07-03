#ifndef APPLICATION_H
#define APPLICATION_H
#include <QObject>
#include <QHostAddress>
#include "src/backend/core/playlist.h"
#include "src/backend/core/song.h"
#include "src/ui/dialog_favoritesongs.h"
#include "src/ui/dialog_friends.h"
#include "src/ui/dialog_queue.h"
#include "src/ui/editplaylist.h"
#include "src/ui/online_choice.h"
#include "src/ui/onlinechat.h"
#include "src/ui/onlinechathost.h"
#include "src/ui/onlinejoin.h"
#include "src/ui/playlistchoicewindow.h"
#include "src/ui/playmusicwindow.h"
#include "src/ui/jukeboxsessionwidget.h"

#include <QTimer>
#include <QMap>

class ChangePasswordWindow;
class ChoiceWindow;
class EmailVerificationWindow;
class FrogotPasswordWindow;
class LoginWindow;
class ReceiveSecureWordsWindow;
class SignupWindow;
class ShowKeyWords;
class MainWindow;
class mycreateDialog;

class Application : public QObject
{
    Q_OBJECT
public:
    static Application& getInstance();


    void Run();
    ///// window getters

    void show_choiceWindow();
    void show_loginWindow();
    void show_signupWindow();
    void show_changePasswordWindow(QString Username);
    void show_emailVWindow(QString Email ,QString Username);
    void show_forgotPassword_window();
    void show_receiveSWWindow(QString key , QString username);
    void show_showKeyWindow(QString key);
    void show_playlistWindow();
    void showMainWindow();
    void show_playlistCreateWindow();
    void show_playlistEditWindow(qint64 playlistID);
    void show_playMusicWindow();
    void show_FavoriteSongWindow();
    void show_FriendWindow();
    void show_QueueWindow();
    void show_sessionWindow();



    void show_online_choice();
    void show_online_join();
    void show_online_chatHost();
    void show_online_chat();


signals:
    void update_Friend_view(const QList<Person> FriendsList);
    void Playlist_view_updated(const QList<Playlist> playlist_list);
    void update_view_favoriteSongs(const QList<Song> FavoroteSongs);
    void Song_view_update(const QList<Song>Songlist);
    void Song_view_updated();
    void edit_Playlist_Song_view(const QList<Song>allSong , const QList<Song>playlistSong, qint64 playlistId, const QString& playlistName);
    void music_updateSong(const QList<Song>allSong);
    void play_from_playlist(const QList<Song> songs);
    void control_setSongInfo(qint64 songID,bool isLiked);
    void Queue_update_Window(const QList<Song> allSong);
public slots:

    void prepertoPlay_RecivedSong(const Song currSong);
    void preparetoPlay_playList(qint64 playlistID);
    void preparetoPlay_Queue(const QList<qint64> playlistID);
    void delete_friend_fromList(const QList<Person> deletedFriend);
    void Create_PlayList(const QString name , const QList<qint64>songs);
    void Delete_PlayList(qint64 Playlistid);
    void ADD_Songs(const QString& pahts);
    void Edit_PlaylistName(qint64 playListId,const QString& newName);
    void edite_update_playlistSongs(qint64 playListId,const QList<qint64>& newSongsId,const QList<qint64>& rmSongsId);
    void onRemotePlay(qint64 position);
    void onRemotePause();
    void onRemoteSeek(qint64 position);
    void onSetCurrentSong(const Song& song);
    void onPlayPauseClicked();
    void onSeeked(qint64 position);

private:
    explicit Application(QObject *parent = nullptr);


    // static V
    static std::unique_ptr<Application> s_instance;


    //member V
    QWidget* m_currentWindow;
    // member Window

    ChangePasswordWindow* w_change_password_window;
    ChoiceWindow* w_choice_window;
    EmailVerificationWindow* w_email_verification_window;
    FrogotPasswordWindow* w_forgot_password_window;
    LoginWindow* w_login_window;
    ReceiveSecureWordsWindow* w_receive_secureWords_window;
    SignupWindow* w_signUp_window;
    ShowKeyWords* w_showKey_Window;
    PlaylistChoiceWindow* w_playlist_choicewindow;
    mycreateDialog* w_playlist_createWindow;
    EditPlayList* w_playlist_editWindow;
    playmusicwindow* w_playMusic_window;
    MainWindow* w_main_window;
    Dialog_FavoriteSongs* w_FavoritSongs;
    Dialog_Friends* w_Friend_Window;
    Dialog_Queue* w_Queue_window;
    JukeBoxSessionWidget* w_session_window;



    /////online part
    online_choice* w_onlineChoice;
    onlineJoin* w_onlineJoin;
    onlinechat* w_onlineChat;
    onlineChatHost* w_onlicechatHost;
    // function
    void switchWindow(QWidget* nextWindow);

    Song m_sessionSong;

    bool m_songReady = false;
    bool m_pendingRemotePlay = false;
    qint64 m_pendingRemotePlayPosition = 0;

private slots:
    void onVideoFileSelected(const QString& filePath);


};

#endif // APPLICATION_H
