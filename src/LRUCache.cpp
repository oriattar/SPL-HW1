#include "LRUCache.h"
#include <iostream>

LRUCache::LRUCache(size_t capacity)
    : slots(capacity), max_size(capacity), access_counter(0) {}

bool LRUCache::contains(const std::string& track_id) const {
    return findSlot(track_id) != max_size;
}

AudioTrack* LRUCache::get(const std::string& track_id) {
    size_t idx = findSlot(track_id);
    if (idx == max_size) return nullptr;
    return slots[idx].access(++access_counter);
}

/**
 * Method that handles putting a track to cahce memory.
 */
bool LRUCache::put(PointerWrapper<AudioTrack> track) {
    if(!track) { //track validity check
        return false;
    }
        
    if(this->get(track->get_title()))
    { // if the track was found increase the slot access time, increase access counter
        this->access_counter++;
        return false;
    }

    this->access_counter++; //increase access counter, as a slot is about to be accessed.
    if(this->isFull()) //all slots are taken
    {
        if(this->evictLRU()) // eveacts the LRU slot
        {
            size_t index = this->findEmptySlot();
            
            slots[index].store(std::move(track),this->access_counter); // adding a track to an free solts
            return true; //eviction occured
        }
    }
    else
    {
        slots[this->findEmptySlot()].store(std::move(track), this->access_counter);
        return false;
    }
    return false;
}

bool LRUCache::evictLRU() {
    size_t lru = findLRUSlot();
    if (lru == max_size || !slots[lru].isOccupied()) return false;
    slots[lru].clear();
    return true;
}

size_t LRUCache::size() const {
    size_t count = 0;
    for (const auto& slot : slots) if (slot.isOccupied()) ++count;
    return count;
}

void LRUCache::clear() {
    for (auto& slot : slots) {
        slot.clear();
    }
}

void LRUCache::displayStatus() const {
    std::cout << "[LRUCache] Status: " << size() << "/" << max_size << " slots used\n";
    for (size_t i = 0; i < max_size; ++i) {
        if(slots[i].isOccupied()){
            std::cout << "  Slot " << i << ": " << slots[i].getTrack()->get_title()
                      << " (last access: " << slots[i].getLastAccessTime() << ")\n";
        } else {
            std::cout << "  Slot " << i << ": [EMPTY]\n";
        }
    }
}

size_t LRUCache::findSlot(const std::string& track_id) const {
    for (size_t i = 0; i < max_size; ++i) {
        if (slots[i].isOccupied() && slots[i].getTrack()->get_title() == track_id) return i;
    }
    return max_size;

}

/**
 * Finds the LRU slots, finding the minimal access time in the array
 */
size_t LRUCache::findLRUSlot() const {
    size_t minIndex = max_size; //first init
    uint64_t minValue = 0; //min value is at least 1
    for(int i=0;i<max_size;i++) {
        if(slots[i].isOccupied())
        {
            if(slots[i].getLastAccessTime()<minValue || minValue==0) {
                minValue=slots[i].getLastAccessTime();
                minIndex=i;
           }
        }
    }
    return minIndex;
}

size_t LRUCache::findEmptySlot() const {
    for (size_t i = 0; i < max_size; ++i) {
        if (!slots[i].isOccupied()) return i;
    }
    return max_size;
}

void LRUCache::set_capacity(size_t capacity){
    if (max_size == capacity)
        return;
    //udpate max size
    max_size = capacity;
    //update the slots vector
    slots.resize(capacity);
}