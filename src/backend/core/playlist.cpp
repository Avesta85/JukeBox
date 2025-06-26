#include "playlist.h"



Playlist::Playlist(const QString& name) : name(name), playlistId(-1), currentMediaIndex(-1)
{
    qDebug() << "Playlist created:" << name;
}

Playlist::Playlist(const QString& name, size_t id) : name(name), playlistId(id), currentMediaIndex(-1)
{
    qDebug() << "Playlist created:" << name << "with ID:" << id;
}

Playlist::Playlist(const Playlist& other) : name(other.name), playlistId(other.playlistId), currentMediaIndex(other.currentMediaIndex)
{
    // these are shallow copy , if you want to have a sprated source must use deep copy wich is not commen
    for (Media* item : other.mediaItems)
        mediaItems.push_back(item);

    qDebug() << "Playlist copy constructor called for:" << name;
}

Playlist::~Playlist()
{
    qDebug() << "Playlist destructor called for:" << name;
    mediaItems.clear();
}

void Playlist::setName(const QString& NewName)
{this->name = NewName;}

void Playlist::setPlaylistId(const size_t NewID)
{this->playlistId = NewID;}

void Playlist::setCurrentIndex(const int& newCurrIndex)
{this->currentMediaIndex = newCurrIndex;}

void Playlist::setNewListOfMedia(const std::vector<Media*>& NewMediaList)
{this->mediaItems = NewMediaList;}

void Playlist::addMedia(Media* const media)
{
    if (media)
    {
        mediaItems.push_back(media);
        if (currentMediaIndex == -1) // if list is empty choose it as first item
            currentMediaIndex = 0;
        qDebug() << "Added media to" << name << ":" << media->getName();
    }
}

bool Playlist::removeMedia(Media* const media)
{
    if (!media)
        return false;

    auto it = std::remove(mediaItems.begin(), mediaItems.end(), media);
    if (it != mediaItems.end())
    {
        mediaItems.erase(it, mediaItems.end());
        qDebug() << "Removed media from" << name << ":" << media->getName();

        // منطق برای به روز رسانی currentMediaIndex پس از حذف  ////////////////////////////////
        if (currentMediaIndex >= mediaItems.size() && !mediaItems.empty())
            currentMediaIndex = mediaItems.size() - 1;
        else if (mediaItems.empty())
            currentMediaIndex = -1;

        return true;
    }
    return false;
}

bool Playlist::removeMediaAt(int index)
{
    if (index >= 0 && index < mediaItems.size())
    {
        qDebug() << "Removed media at index" << index << "from" << name << ":" << mediaItems[index]->getName();
        mediaItems.erase(mediaItems.begin() + index);

        // منطق برای به روز رسانی currentMediaIndex پس از حذف
        if (currentMediaIndex >= mediaItems.size() && !mediaItems.empty())
            currentMediaIndex = mediaItems.size() - 1;
        else if (mediaItems.empty())
            currentMediaIndex = -1;
        else if (index < currentMediaIndex)
            currentMediaIndex--; // اگر آیتمی قبل از ایندکس فعلی حذف شد، ایندکس فعلی رو کم کن

        return true;
    }
    return false;
}

void Playlist::clear()
{
    mediaItems.clear();
    currentMediaIndex = -1;
    qDebug() << "Playlist cleared:" << name;
}

// --- Navigation and Access Methods ---

Media* Playlist::getCurrentMedia() const
{
    if (currentMediaIndex >= 0 && currentMediaIndex < mediaItems.size())
        return mediaItems[currentMediaIndex];
    return nullptr;
}

Media* Playlist::nextMedia() {
    if (currentMediaIndex != -1 && currentMediaIndex < mediaItems.size() - 1) {
        currentMediaIndex++;
        return mediaItems[currentMediaIndex];
    }
    return nullptr; // به انتهای لیست رسیدیم یا لیست خالی است
}

Media* Playlist::prevMedia() {
    if (currentMediaIndex > 0) {
        currentMediaIndex--;
        return mediaItems[currentMediaIndex];
    }
    return nullptr; // به ابتدای لیست رسیدیم یا لیست خالی است
}

void Playlist::setCurrentMedia(Media* media) {
    if (!media) {
        currentMediaIndex = -1;
        return;
    }
    for (int i = 0; i < mediaItems.size(); ++i) {
        if (mediaItems[i] == media) {
            currentMediaIndex = i;
            qDebug() << "Current media set to:" << media->getName();
            return;
        }
    }
    qDebug() << "Media not found in playlist:" << media->getName();
    currentMediaIndex = -1; // اگر پیدا نشد
}

void Playlist::setCurrentMediaByIndex(int index) {
    if (index >= 0 && index < mediaItems.size()) {
        currentMediaIndex = index;
        qDebug() << "Current media set to index:" << index;
    } else {
        qDebug() << "Invalid index for setting current media:" << index;
        currentMediaIndex = -1;
    }
}



const QString& Playlist::getName() const
{return this->name;}

const size_t& Playlist::getId() const
{return this->playlistId;}

const int& Playlist::size() const
{return this->mediaItems.size();}

bool Playlist::isEmpty() const
{return this->mediaItems.empty();}

