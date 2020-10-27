#include <cstddef>
#include <type_traits>

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
private:
    size_t chunkSize_;
    Chunk *chunkHead_;
    size_t *copyCnt_;

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
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;
    using rebind = struct rebind {
        typedef ChunkAllocator<T> other;
    };
    using is_always_equal = std::true_type;

    ChunkAllocator(const size_t chunkSize = 1024) :
            chunkSize_(chunkSize * sizeof(T)), chunkHead_(nullptr), copyCnt_(new size_t{0}) {}

    ChunkAllocator(const ChunkAllocator<T> &copy) :
            chunkSize_(copy.chunkSize_),
            chunkHead_(copy.chunkHead_),
            copyCnt_(copy.copyCnt_) {
        ++(*copyCnt_);
    }

    ChunkAllocator<T> &operator=(const ChunkAllocator<T> &copy) {
        if (this == &copy)
            return *this;
        clearDynamicData();
        chunkSize_ = copy.chunkSize_;
        chunkHead_ = copy.chunkHead_;
        copyCnt_ = copy.copyCnt_;
        ++(*copyCnt_);
    }

    ~ChunkAllocator() {
        clearDynamicData();
    }

    T *allocate(const size_t n) {
        size_t needBytes = n * sizeof(T);
        if (needBytes > chunkSize_) {
            chunkHead_ = new Chunk(needBytes, chunkHead_);
            char *memPtr = chunkHead_->allocate(needBytes);
            return reinterpret_cast<T *>(memPtr);
        }
        if (!chunkHead_) {
            chunkHead_ = new Chunk(chunkSize_, chunkHead_);
        }
        char *memPtr = nullptr;
        for (Chunk *chunk = chunkHead_; nullptr != chunk && nullptr == memPtr; chunk = chunk->next()) {
            memPtr = chunk->allocate(needBytes);
        }
        if (!memPtr) {
            chunkHead_ = new Chunk(chunkSize_, chunkHead_);
            memPtr = chunkHead_->allocate(needBytes);
        }
        return (memPtr) ? reinterpret_cast<T *>(memPtr) : nullptr;
    }

    void deallocate(T *, const size_t) {}

    template<typename... Args>
    void construct(T *p, const Args &&... args) {
        new(p) T(args...);
    }

    void destroy(T *p) {
        p->~T();
    }
};


