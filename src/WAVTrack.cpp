#include "WAVTrack.h"
#include <iostream>

WAVTrack::WAVTrack(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int sample_rate, int bit_depth)
    : AudioTrack(title, artists, duration, bpm), sample_rate(sample_rate), bit_depth(bit_depth) {

    std::cout << "WAVTrack created: " << sample_rate << "Hz/" << bit_depth << "bit" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

/*
Method that simulates track loading.
*/
void WAVTrack::load() {
    // TODO: Implement realistic WAV loading simulation
    // NOTE: Use exactly 2 spaces before the arrow (→) character

    std::cout << "[WAVTrack::load] Loading WAV: \"" << title << "\" at " << 
    sample_rate << "Hz/" << bit_depth <<"bit (uncompressed)..." <<std::endl;

    long size = duration_seconds *sample_rate *(bit_depth/8) *2;
    std::cout << "  → Estimated file size: " << size << " bytes" << std::endl;
    std::cout <<"  → Fast loading due to uncompressed format." << std::endl;

}

/*
Method that simulates track betagrid analysis
*/
void WAVTrack::analyze_beatgrid() {
    std::cout << "[WAVTrack::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";

    double beats = (duration_seconds / 60.0) * bpm;
    std::cout << "  → Estimated beats: " << (int)(beats) << "  → Precision factor: 1 (uncompressed audio)" << std::endl;
}

/*
Calculates quality score according to instructions.
*/
double WAVTrack::get_quality_score() const {
    double score = 70.0;

    if(sample_rate >= 44100)
        score+=10;
    if(sample_rate >=96000)
        score+=5;
    if(bit_depth >=16)
        score+=10;
    if(bit_depth >=24)
        score+=15;

    if(score >100)
        score = 100;

    std::cout << "[WAVTrack::get_quality_score] \"" << title << " score = " << (int)(score) <<"/100" << std::endl;
    return score; 
}

/*
Clone method that allowes cloning of a WAVTrack object.
*/
PointerWrapper<AudioTrack> WAVTrack::clone() const {
    
    AudioTrack * copy = new WAVTrack(*this); //using the default copy ctor that uses the base classes
    //copy constructor, then copies the rest.
    return PointerWrapper<AudioTrack>(copy);
}