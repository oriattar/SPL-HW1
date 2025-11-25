#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

#define ERROR_CLONE 400

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * Handles track loading to cache.
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    if(this->cache.contains(track.get_title())) { //checks if its already on cache , HIT
        this->cache.get(track.get_title());

    return 1;

    }
    AudioTrack* copy=track.clone().release();
    if(!copy) {
        std::cout <<"[ERROR] Track:" <<track.get_title() << "failed to clone" <<std::endl;
        return ERROR_CLONE;
    }

    copy->load();
    copy->analyze_beatgrid();

   if(this->cache.put(PointerWrapper<AudioTrack> (copy)))
        return -1;
        
    return 0;

}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * Method return tracks from cache, if exist in one of the slots, else returns nullptr.
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    return this->cache.get(track_title);
}
