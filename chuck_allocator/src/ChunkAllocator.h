#include <cstddef>
#include <type_traits>
#include <utility>

class Chunk {
private:
    char *data_;
    size_t size_;
    size_t offset_;
    Chunk *next_;
public:
    Chunk(size_t size = 1024, Chunk *next = nullptr) :
            data_(new char[size]), size_(size), offset_(0), next_(next) {}

    ~Chunk() {
        delete[] data_;
    }

    char *allocate(const size_t n) {
        if (offset_ + n <= size_) {
            char *ptr = data_ + offset_;
            offset_ += n;
            return ptr;
        }
        return nullptr;
    }

    Chunk *next() {
        return next_;
    }
};


template<typename T>
class ChunkAllocator {
public:
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    template<class U> struct rebind {
        typedef ChunkAllocator<U> other;
    };
private:
    static const size_type CHANK_SIZE = 128 * 1024; // bytes
    Chunk *chunkHead_;
    size_type *copyCnt_;

    void clearDynamicData() {
        if (*copyCnt_) {
            *copyCnt_--;
            return;
        }
        delete copyCnt_;
        while (chunkHead_) {
            Chunk *head = chunkHead_;
            chunkHead_ = chunkHead_->next();
            delete head;
        }
    }

public:
    ChunkAllocator() : chunkHead_(nullptr), copyCnt_(new size_type{0})
    {}

    ChunkAllocator(const ChunkAllocator<T> &copy) :
            chunkHead_(copy.chunkHead_),
            copyCnt_(copy.copyCnt_) {
        ++(*copyCnt_);
    }

    ChunkAllocator<T> &operator=(const ChunkAllocator<T> &copy) {
        if (this == &copy)
            return *this;
        clearDynamicData();
        chunkHead_ = copy.chunkHead_;
        copyCnt_ = copy.copyCnt_;
        ++(*copyCnt_);
    }

    bool operator==(const ChunkAllocator<T> &other) const {
        if (this == other)
            return true;
        if (!this->chunkHead_)
            return false;
        return (this->chunkHead_ == other.chunkHead_);
    }

    ~ChunkAllocator() {
        clearDynamicData();
    }

    pointer allocate(const size_type n) {
        size_type needBytes = n * sizeof(T);
        if (needBytes > CHANK_SIZE) {
            chunkHead_ = new Chunk(needBytes, chunkHead_);
            char *memPtr = chunkHead_->allocate(needBytes);
            return reinterpret_cast<pointer>(memPtr);
        }
        if (!chunkHead_) {
            chunkHead_ = new Chunk(CHANK_SIZE, chunkHead_);
        }
        char *memPtr = nullptr;
        for (Chunk *chunk = chunkHead_; nullptr != chunk && nullptr == memPtr; chunk = chunk->next()) {
            memPtr = chunk->allocate(needBytes);
        }
        if (!memPtr) {
            chunkHead_ = new Chunk(CHANK_SIZE, chunkHead_);
            memPtr = chunkHead_->allocate(needBytes);
        }
        return (memPtr) ? reinterpret_cast<pointer>(memPtr) : nullptr;
    }

    void deallocate(pointer, const size_type) {}

    template<typename... Args>
    void construct(pointer p, Args&&... args) {
        new(p) T(std::forward<Args>(args)...);
    }

    void destroy(pointer p) {
        p->~T();
    }
};


