#ifndef POINTERWRAPPER_H
#define POINTERWRAPPER_H

#include <utility>
#include <iostream>

/**
 * PointerWrapper - A template class that wraps a raw pointer
 * 
 * This is Phase 3 of the assignment. Students must analyze and implement
 * a complete pointer wrapper class that manages dynamic memory safely.
 * 
 * Refer to the assignment instructions (Phase 3) for detailed guiding questions
 * about resource management, ownership semantics, copy vs move, and interface design.
 */
template<typename T>
class PointerWrapper {
private:
    T* ptr;  // Raw pointer to the managed object

public:
    // ========== CONSTRUCTION AND DESTRUCTION ==========

    /**
     * Default constructor - creates empty wrapper
     */
    PointerWrapper() : ptr(nullptr) {}

    /**
     * Constructor from raw pointer - wraps the pointer
     */
    explicit PointerWrapper(T* p) : ptr(p) {}

    /**
     * RAII - ties the allocated data lifetime to an object on the stack
     * Once out of scope - allocated data gets deleted.
     * 
     * HINT: What should happen to the wrapped pointer when the wrapper is destroyed?
     * Think about ownership and resource management.
     * Is the default destructor sufficient here?
     */
    ~PointerWrapper(){
        delete this->ptr;
    }

    // ========== COPY OPERATIONS (DELETED) ==========

    /**
     * Copy constructor is DELETED
     * Think about why this might be necessary for a pointer wrapper
     */
    PointerWrapper(const PointerWrapper& other) = delete;

    /**
     * Copy assignment is DELETED
     * Consider what problems could arise if copying was allowed
     */
    PointerWrapper& operator=(const PointerWrapper& other) = delete;

    // ========== MOVE OPERATIONS (STUDENTS IMPLEMENT) ==========

    /**
     * Move copy constructor - ensures no 2 wrappers point to the same address
     * What should happen to the source wrapper after the move?
     */
    PointerWrapper(PointerWrapper&& other) noexcept {
        #ifdef DEBUG
        std::cout << "Move constructor called for: " << other.get() << std::endl;
        #endif
        
        this->ptr= other.ptr;
        other.ptr= nullptr;

    }

    /**
     * Move = operator for a wrapper object - deletes current memory in pointed address to avoid leaks
     * also ensures no 2 wrappers point to the same address.
     */
    PointerWrapper& operator=(PointerWrapper&& other) noexcept {
        if (this!=&other){
            delete this->ptr;
            this->ptr= other.ptr;
            other.ptr=nullptr;
        }
        return *this;
    }

    // ========== ACCESS OPERATIONS ==========

    /**
     * Implements *() operator - to treat the wrapper as a real pointer.
     */

    T& operator*() const {
        if (this->ptr==nullptr) {
            throw std::runtime_error("Error: Ptr can't be null.");
        }
        return *ptr;
    };

    /**
     * Implements -> operator - to treat the wrapper as a real pointer.
     */
    T* operator->() const {
        if (this->ptr==nullptr){
            throw std::runtime_error("Error: Ptr can't be null.");
        }
        return this->ptr;
    }

    /**
     *getter for the pointer wrapper class
     returns held pointer, if its nullptr throws an exception.
     */
    T* get() const {
        if (this->ptr==nullptr){
            throw std::runtime_error("Error: Ptr can't be null.");
        }
        return this->ptr;
    }

    // ========== OWNERSHIP MANAGEMENT ==========

    /**
     * Method that release ownership of the wrapper on the allocated memory.
     */
    T* release() {
       T* temp = this->ptr;
       this->ptr= nullptr;
       return temp; // release ownership, as the object dosnt hold the undeleted address.
    }

    /**
     Reset method for wrapper - delete current memory in pointed adrerss, set held pointer to nullptr.
     */
    void reset(T* new_ptr = nullptr) {
        #ifdef DEBUG
        std::cout << "Reset called for pointer: " << this->ptr << std::endl;
        #endif

        delete this->ptr;
        this-> ptr= new_ptr;
    }

    // ========== UTILITY FUNCTIONS ==========

    /**
     * Implemintation for bool casting of a wrapper onbject
     * acts a real pointer, if held pointer is nullptr, convert to false.
     * else converts to true.
     */
    explicit operator bool() const {
        return (this->ptr != nullptr);
    }

    /**
     * Swap two PointerWrapper objects
     * This is implemented for you as a reference
     */
    void swap(PointerWrapper& other) noexcept {
        std::swap(ptr, other.ptr);
    }
};

// ========== NON-MEMBER FUNCTIONS ==========

/**
 * Helper function to create PointerWrapper
 * This is implemented for you as an example
 * Can you figure out when this would be useful in phase 4?
 */
template<typename T, typename... Args>
PointerWrapper<T> make_pointer_wrapper(Args&&... args) {
    return PointerWrapper<T>(new T(std::forward<Args>(args)...));
}

/**
 * TODO: Implement swap for PointerWrapper
 * HINT: How can you swap two wrapper objects?
 * Why might this be useful?
 */
template<typename T>
void swap(PointerWrapper<T>& lhs, PointerWrapper<T>& rhs) noexcept {
    // TODO: Implement global swap function
    // HINT: You can use the member swap function
    lhs.swap(rhs);
}

#endif // POINTERWRAPPER_H