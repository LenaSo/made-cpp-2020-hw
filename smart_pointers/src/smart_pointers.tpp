namespace task {

    template<class T>
    UniquePtr<T>::UniquePtr(pointer ptr) : ptr_(ptr) {}

    template<class T>
    UniquePtr<T>::UniquePtr(UniquePtr <element_type> &&other) : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    template<class T>
    UniquePtr <T> &UniquePtr<T>::operator=(UniquePtr <element_type> &&other) {
        if (ptr_ != other.ptr_) {
            if (ptr_) {
                delete ptr_;
            }
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    template<class T>
    UniquePtr<T>::~UniquePtr() {
        if (ptr_)
            delete ptr_;
    }

    template<class T>
    typename std::add_lvalue_reference<T>::type UniquePtr<T>::operator*() const {
        return *ptr_;
    }

    template<class T>
    T *UniquePtr<T>::operator->() const noexcept {
        return get();
    }

    template<class T>
    T *UniquePtr<T>::get() const noexcept {
        return ptr_;
    }

    template<class T>
    T *UniquePtr<T>::release() noexcept {
        pointer out = ptr_;
        ptr_ = nullptr;
        return out;
    }

    template<class T>
    void UniquePtr<T>::reset(pointer ptr) noexcept {
        pointer old_ptr = ptr_;
        ptr_ = ptr;
        if (old_ptr) {
            delete old_ptr;
        }
    }

    template<class T>
    void UniquePtr<T>::swap(UniquePtr <T> &other) noexcept {
        std::swap(ptr_, other.ptr_);
    }

//-------SharedPtr---------------------------------------------------
    template<class T>
    constexpr SharedPtr<T>::SharedPtr() noexcept : cb_(nullptr) {}

    template<class T>
    SharedPtr<T>::SharedPtr(T *ptr) : cb_(new ControlBlock{ptr, 1}) {}

    template<class T>
    SharedPtr<T>::SharedPtr(const SharedPtr <element_type> &other) : cb_(other.cb_) {
        if (cb_)
            cb_->cnt++;
    }

    template<class T>
    SharedPtr<T>::SharedPtr(SharedPtr <element_type> &&other) : cb_(other.cb_) {
        other.cb_ = nullptr;
    }

    template<class T>
    SharedPtr<T>::SharedPtr(const WeakPtr <T> &weakPtr) : SharedPtr<T>(weakPtr.lock()) {}

    template<class T>
    SharedPtr<T> &SharedPtr<T>::operator=(const SharedPtr <element_type> &other) {
        if (cb_ != other.cb_) {
            release();
            cb_ = other.cb_;
            if (cb_)
                cb_->cnt++;
        }
        return *this;
    }

    template<class T>
    SharedPtr<T> &SharedPtr<T>::operator=(SharedPtr <element_type> &&other) {
        if (cb_ != other.cb_) {
            release();
            cb_ = other.cb_;
            other.cb_ = nullptr;
        }
        return *this;
    }

    template<class T>
    SharedPtr<T>::~SharedPtr() {
        release();
    }

    template<class T>
    void SharedPtr<T>::release() {
        if (cb_) {
            cb_->cnt--;
            if (cb_->cnt == 0) {
                if (cb_->ptr)
                    delete cb_->ptr;
                delete cb_;
                cb_ = nullptr;
            }
        }
    }

    template<class T>
    T &SharedPtr<T>::operator*() const noexcept {
//        if (!cb_ || !cb_->cnt) {
//            return *(T*)0;
//        }
        return *(cb_->ptr);
    }

    template<class T>
    T *SharedPtr<T>::operator->() const noexcept {
        get();
    }

    template<class T>
    T *SharedPtr<T>::get() const noexcept {
        if (!cb_ || !cb_->cnt) {
            return nullptr;
        }
        return cb_->ptr;
    }

    template<class T>
    long SharedPtr<T>::use_count() const noexcept {
        return static_cast<long>((cb_) ? cb_->cnt : 0);
    }

    template<class T>
    void SharedPtr<T>::reset() noexcept {
        SharedPtr<T>().swap(*this);
    }

    template<class T>
    void SharedPtr<T>::reset(T *ptr) noexcept {
        SharedPtr<T>(ptr).swap(*this);
    }

    template<class T>
    void SharedPtr<T>::swap(SharedPtr <T> &other) noexcept {
        std::swap(cb_, other.cb_);
    }

    //-----------WeakPtr------------------------------------------------------------------
    template<class T>
    constexpr WeakPtr<T>::WeakPtr() noexcept : cb_(nullptr) {}

    template<class T>
    WeakPtr<T>::WeakPtr(const SharedPtr <T> &r) noexcept : cb_(new ControlBlock{r.cb_, 1}) {}

    template<class T>
    WeakPtr<T>::WeakPtr(const WeakPtr <T> &r) noexcept : cb_(r.cb_) {
        cb_->cnt++;
    }

    template<class T>
    WeakPtr<T>::WeakPtr(WeakPtr <T> &&r) noexcept : cb_(r.cb_) {
        r.cb_ = nullptr;
    }

    template<class T>
    WeakPtr<T>::~WeakPtr() {
        release();
    }

    template<class T>
    void WeakPtr<T>::release() {
        if (cb_) {
            cb_->cnt--;
            if (cb_->cnt == 0) {
                delete cb_;
                cb_ = nullptr;
            }
        }
    }

    template<class T>
    WeakPtr<T> &WeakPtr<T>::operator=(const WeakPtr <T> &other) noexcept {
        if (cb_ != other.cb_) {
            release();
            cb_ = other.cb_;
            cb_->cnt++;
        }
        return *this;
    }

    template<class T>
    WeakPtr<T> &WeakPtr<T>::operator=(const SharedPtr <T> &shared) noexcept {
        release();
        *this = WeakPtr(shared);
        return *this;
    }

    template<class T>
    WeakPtr<T> &WeakPtr<T>::operator=(WeakPtr <T> &&other) noexcept {
        if (cb_ != other.cb_) {
            release();
            cb_ = other.cb_;
            other.cb_ = nullptr;
        }
        return *this;
    }

    template<class T>
    bool WeakPtr<T>::expired() const noexcept {
        return !(cb_ && cb_->sharedCb_ && cb_->sharedCb_->cnt);
    }

    template<class T>
    long WeakPtr<T>::use_count() const noexcept {
        return static_cast<long>((cb_ && cb_->sharedCb_) ? cb_->sharedCb_->cnt : 0);
    }

    template<class T>
    SharedPtr<T> WeakPtr<T>::lock() const noexcept {
        SharedPtr <T> out(nullptr);
        if (cb_) {
            cb_->sharedCb_->cnt++;
            out.cb_ = cb_->sharedCb_;
        }
        return out;
    }

    template<class T>
    void WeakPtr<T>::reset() noexcept {
        release();
    }

    template<class T>
    void WeakPtr<T>::swap(WeakPtr <T> &other) noexcept {
        std::swap(cb_, other.cb_);
    }

}