#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

#define ERROR_CLONE 400
DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    if(this->cache.contains(track.get_title())) {
    this->cache.get(track.get_title());
    return 1;
    }
        AudioTrack* copy=track.clone().release();
        if(!copy) {
             std::cout <<"[error] Track:" <<track.get_title() << "failed to clone" <<std::endl;
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
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    return this->cache.get(track_title);
}
