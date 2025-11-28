#include "MixingEngineService.h"
#include <iostream>
#include <memory>

#define FIRST_DECK 1
#define SECOND_DECK 0

#define ERROR_CLONE -1


/**
 * Constructor
 */
MixingEngineService::MixingEngineService()
     : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    this->decks[FIRST_DECK]= nullptr; // sets bouth decks to nullptr as the inital state.
    this->decks[SECOND_DECK]= nullptr; 
    std::cout << "[MixingEngineService] Initialized with 2 empty decks."<< std:: endl;
}

/**
 * Destructor
 */
MixingEngineService::~MixingEngineService() {

    std::cout << "[MixingEngineService] Cleaning up decks...." << std:: endl;

    if(decks[FIRST_DECK]) //if the first deck contains a song delete it
        delete decks[FIRST_DECK];
    decks[FIRST_DECK] = nullptr;

    if(decks[SECOND_DECK]) //if the second deck has a song delete it
        delete decks[SECOND_DECK];
    decks[SECOND_DECK]= nullptr;
 }

    


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    
    std::cout << "\n=== Loading Track to Deck ===" << std:: endl;

    PointerWrapper<AudioTrack> copy = track.clone();
        if (!copy){
        std::cout << "[ERROR] Track: "<< track.get_title() << " failed to clone" << std:: endl;
            return ERROR_CLONE;
    }
    
    if (decks[FIRST_DECK] == nullptr && decks[SECOND_DECK]== nullptr){ //in initial state

        copy->load();
        copy->analyze_beatgrid();

        decks[FIRST_DECK] = copy.release();
        this->active_deck=0;

         std::cout << "[Load Complete] "<< track.get_title() <<" is now loaded on deck " << 0 << std::endl;
    }
     else {

    int target = 1- this->active_deck; //calculates target deck
    std::cout << "[Deck Switch] Target deck:" << target << std:: endl;

    AudioTrack * curr =decks[target];
    if(curr)
    {
        delete curr;
        decks[target] = nullptr;
    }

    copy->load();
    copy->analyze_beatgrid(); //simulates load logs

    AudioTrack * activeSong = this->decks[this->active_deck];

    if(activeSong && this->auto_sync) //BPM Management
    {
        if(this->can_mix_tracks(copy))
        {
            this->sync_bpm(copy);
        }
    }
    this->decks[target] = copy.release();

    std::cout << "[Load Complete] "<< track.get_title() <<" is now loaded on deck " << target << std::endl;

    if(activeSong) //unloading handling
    {
        std::cout << "[Unload] Unloading previous deck "<< this->active_deck <<"(<" <<activeSong->get_title() << ">) " << std::endl;
        delete activeSong;
        this->decks[this->active_deck] = nullptr;
    }

    this->active_deck = target;
    std::cout << "[Active Deck] Switched to deck " << target << std::endl;
    return target; 
    }
    
    return 0;
}



/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    AudioTrack * activeSong = this->decks[this->active_deck];
    if(!activeSong || !track)
        return false;
    
    int diff = std::abs(track->get_bpm() - activeSong->get_bpm()); // calculates abs val of difference
    if(diff<=this->bpm_tolerance)
        return true;

    return false;
}

/**
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {

    AudioTrack * activeSong = this->decks[this->active_deck];
    if(!activeSong || !track)
        return;
    
    int org_bpm = track->get_bpm();
    int new_bpm = (org_bpm + activeSong->get_bpm()) /2;
    track->set_bpm(new_bpm); //updates bpm of the new track

    std::cout << "[Sync BPM] Syncing BPM from "<<org_bpm <<" to "<< new_bpm << std::endl;

}
