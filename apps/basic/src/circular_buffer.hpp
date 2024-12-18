#pragma once
#include <string.h>
#include <zephyr/kernel.h>

template <typename T> class CircularBuffer
{
      private:
	T *buffer_;        // buffer pointer
	int size_;         // total size of the buffer
	int write_idx_;    // write pointer's index
	int read_idx_;     // read pointer's index
	int nelem_;        // number of elements in the buffer
	struct k_sem sem_; // semaphore to protect the pointers

      public:
	CircularBuffer(int size) : size_(size), write_idx_(0), read_idx_(0), nelem_(0)
	{
		buffer_ = static_cast<T *>(k_malloc(size * sizeof(T)));
		k_sem_init(&sem_, 1, 1);
	}

	~CircularBuffer()
	{
		k_free(static_cast<void *>(buffer_));
	}

	int get_size()
	{
		return size_;
	}

	int get_nelem()
	{
		return nelem_;
	}

	bool write(T &elem)
	{
		if (nelem_ == size_) {
			return false;
		}
		memcpy(&buffer_[write_idx_], &elem, sizeof(T));
		write_idx_ = (write_idx_ + 1) % size_;
		k_sem_take(&sem_, K_FOREVER);
		nelem_++;
		k_sem_give(&sem_);
		return true;
	}

	bool read(T &elem)
	{
		if (nelem_ == 0) {
			return false;
		}
		memmove(&elem, &buffer_[read_idx_], sizeof(T));
		read_idx_ = (read_idx_ + 1) % size_;
		k_sem_take(&sem_, K_FOREVER);
		nelem_--;
		k_sem_give(&sem_);
		return true;
	}
};