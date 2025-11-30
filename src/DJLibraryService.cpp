#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */

 /*
 The class destructor.
 Iterates over the library tracks it created and deletes them.
 */
DJLibraryService::~DJLibraryService()
{
    for(int i =0;i<this->library.size();i++)
    {
        delete this->library[i];
    }
}

/*
Method that builds library of tracks, given track info vector parsed from the config.txt file.
*/
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    
    for(int i=0;i<library_tracks.size();i++)
    {
        AudioTrack * track;
        SessionConfig::TrackInfo curr = library_tracks[i]; // takes current track info

        if(library_tracks[i].type == "MP3") //if mp3 track creates an mp3 object otherwise create wav object
            track = new MP3Track(curr.title,curr.artists,curr.duration_seconds,curr.bpm,curr.extra_param1,curr.extra_param2);
        else
            track = new WAVTrack(curr.title,curr.artists,curr.duration_seconds,curr.bpm,curr.extra_param1,curr.extra_param2);

        this->library.push_back(track);//appending track to lib vector

    }

    std::cout << "[INFO] Track library built: "<<this->library.size() << " tracks loaded" << std::endl;
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    return playlist;
}

/**
 * Locates a track in the playlist and returns a pointer to it.
 * not releasing ownership of the track.
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    
    return this->playlist.find_track(track_title); 
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    
    if(!this->playlist.is_empty())
        this->playlist.Clear(); // cleaning prevoiusly allocated tracks

    this->playlist = Playlist(playlist_name);

    int count_track=0;
    std::cout << "[INFO] Loading playlist: " << playlist_name <<std::endl;

    for(int i=0; i < track_indices.size(); i++){
        int currIndex = track_indices[i];
        if(currIndex >0 && currIndex <= this->library.size())//if the index is in library bounds
        {
            AudioTrack * track = this->library[currIndex-1]; //selects the track from library
            AudioTrack * copy = track->clone().release(); // creates new copy in memo
            if(copy)
            {
                copy->load();
                copy->analyze_beatgrid(); //simulates data vis
                count_track++;
                this->playlist.add_track(copy);

            }
            else
            {
                std::cout << "[ERROR] Failed to clone the track " << currIndex << std::endl;
            }
        }
        else //invalid index
        {
            std::cout << "[WARNING] Invalid track index: "<< currIndex << std::endl;
        }
    }
     std::cout << "Log summary: [INFO] Playlist loaded: "<< playlist_name << " (" << count_track << "  tracks) " << std::endl;
}
/**
 * Method that collects the titles of the playlist field.
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {

    std::vector<std::string> titles = std::vector<std::string>();

    std::vector<AudioTrack *> tracks = this->playlist.getTracks(); //getting the tracks vector

    for(int i=0;i<tracks.size();i++)
    {
        titles.push_back(tracks[i]->get_title());//collecting titles
    }
    return titles;
}
