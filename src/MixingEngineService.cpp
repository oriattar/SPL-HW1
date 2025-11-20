#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : active_deck(0), auto_sync(false), bpm_tolerance(0)
{
this->decks[0]= nullptr;
this->decks[1]= nullptr;
std::cout << "[MixingEngineService] Initialized with 2 empty decks."<< std:: endl;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
std::cout << "[MixingEngineService] Cleaning up decks...." << std:: endl;
this->ClearDeck(0);
this->ClearDeck(1);
this->decks[0]= nullptr;
this->decks[1]= nullptr;
 }

void MixingEngineService::ClearDeck (int i){
   int size= sizeof(this->decks[i])/ sizeof(decks[i][0]);
    for (int j=0; j<size; j++ ){
        if (&this->decks[i][j]!= nullptr){
            delete &this->decks[i][j];
        }
    }
}
    


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
if (decks[0] == nullptr && decks[1]== nullptr){
    decks[0][0]= track;
    this->active_deck=0;
}
else {
    std::cout << "\n=== Loading Track to Deck ===" << std:: endl;
PointerWrapper<AudioTrack> copy = track.clone();
if (!copy){
std::cout << "[ERROR] Track: "<< track.get_title() << " failed to clone" << std:: endl;
    return -1;
}
int index = 1- this->active_deck;
 std::cout << "[Deck Switch] Target deck: <index>" << std:: endl;
}
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
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    return false; // Placeholder
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
}
