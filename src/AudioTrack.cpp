#include "AudioTrack.h"
#include <iostream>
#include <cstring>
#include <random>

AudioTrack::AudioTrack(const std::string& title, const std::vector<std::string>& artists, 
                      int duration, int bpm, size_t waveform_samples)
    : title(title), artists(artists), duration_seconds(duration), bpm(bpm), 
      waveform_size(waveform_samples) {

    // Allocate memory for waveform analysis
    waveform_data = new double[waveform_size];

    // Generate some dummy waveform data for testing
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-1.0, 1.0);
    for (size_t i = 0; i < waveform_size; ++i) {
        waveform_data[i] = dis(gen);
    }
    #ifdef DEBUG
    std::cout << "AudioTrack created: " << title << " by " << std::endl;
    for (const auto& artist : artists) {
        std::cout << artist << " ";
    }
    std::cout << std::endl;
    #endif
}

// ========== TODO: STUDENTS IMPLEMENT RULE OF 5 ==========

/*
Destructor for an audio track object.
*/
AudioTrack::~AudioTrack() {
    
    #ifdef DEBUG
    std::cout << "AudioTrack destructor called for: " << title << std::endl;
    #endif
    delete[] this->waveform_data; //deletes the data allocated in the constructor.
}

/*
Copy constructor for an audio track.
*/
AudioTrack::AudioTrack(const AudioTrack& other)
{
    // TODO: Implement the copy constructor
    #ifdef DEBUG
    std::cout << "AudioTrack copy constructor called for: " << other.title << std::endl;
    #endif
   this->copyfrom(other); //helper methods that copy premitive fields.
   
   this->waveform_data=new double[other.waveform_size]; //allocates data array for the copy

   for(int i=0;i<other.waveform_size;i++) {
        this->waveform_data[i]=other.waveform_data[i]; //deep copies the data
   }
}

/*
Helper method that copies other premitive fields.
*/
void AudioTrack::copyfrom(const AudioTrack& other){

    this->title=other.title;
    this->artists=other.artists;
    this->duration_seconds=other.duration_seconds;
    this->bpm=other.bpm;  // beats per minute for mixing
    this->waveform_size=other.waveform_size;

}

/*
Operator= implemintation for the audio track class.
*/
AudioTrack& AudioTrack::operator=(const AudioTrack& other) {
    
    #ifdef DEBUG
    std::cout << "AudioTrack copy assignment called for: " << other.title << std::endl;
    #endif
    if(this!=&other){ //checks for self assignment

        delete[] this->waveform_data; //deletes this data, to deep copy from other
        this->copyfrom(other); 

        this->waveform_data=new double[other.waveform_size];

        for(int i=0;i<other.waveform_size;i++) { //preform the deep copy
            this->waveform_data[i]=other.waveform_data[i];
        }
    }
    return *this;
}

/*
The move copy constructor - from optimizing rvalues usage.
*/
AudioTrack::AudioTrack(AudioTrack&& other) noexcept {
    // TODO: Implement the move constructor
    #ifdef DEBUG
    std::cout << "AudioTrack move constructor called for: " << other.title << std::endl;
    #endif
    this->copyfrom(other);
    this->waveform_data=other.waveform_data; //just coppies other's data array, as he is going to be deleted
    other.waveform_data=nullptr;//avoids other's data deletion.
}

/*
Move = operator for the audio track class
*/
AudioTrack& AudioTrack::operator=(AudioTrack&& other) noexcept {

    #ifdef DEBUG
    std::cout << "AudioTrack move assignment called for: " << other.title << std::endl;
    #endif
     if(this!=&other){ //handles self assignments
        delete[] this->waveform_data;
        this->copyfrom(other);

        this->waveform_data=other.waveform_data; //takes other data array and avoiding deletion
        other.waveform_data=nullptr;
    }
    return *this;
}

void AudioTrack::get_waveform_copy(double* buffer, size_t buffer_size) const {
    if (buffer && waveform_data && buffer_size <= waveform_size) {
        std::memcpy(buffer, waveform_data, buffer_size * sizeof(double));
    }
}