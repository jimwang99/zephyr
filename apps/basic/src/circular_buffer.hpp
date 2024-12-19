#pragma once

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <zephyr/kernel.h>

template <typename T> class CircularBuffer
{
      private:
	T *buffer_; // buffer pointer
	size_t capacity_;
	size_t write_index_;
	size_t read_index_;
	bool is_initialized_;

      public:
	CircularBuffer(size_t capacity)
		: capacity_(capacity), write_index_(0), read_index_(0), is_initialized_(false)
	{
	}

	int Open() noexcept
	{
		if (is_initialized_) {
			return -EEXIST;
		}
		write_index_ = 0;
		read_index_ = 0;
		is_initialized_ = true;

		buffer_ = static_cast<T *>(k_malloc(capacity_ * sizeof(T)));
		if (buffer_ == NULL) {
			return -EAGAIN;
		}
		return 0;
	}

	int Close() noexcept
	{
		if (!is_initialized_) {
			return -ENOENT;
		}
		is_initialized_ = false;
		k_free(static_cast<void *>(buffer_));
		return 0;
	}

	int Write(T &elem) noexcept
	{
		if (!IsInitialized()) {
			return -EEXIST;
		}
		if (IsFull()) {
			return -ENOBUFS;
		}

		memcpy(&buffer_[write_index_], &elem, sizeof(T));
		write_index_++;
		return 0;
	}

	ssize_t Write(T *elem, size_t num) noexcept
	{
		if (!IsInitialized()) {
			return -EEXIST;
		}

		int i;
		for (i = 0; i < num; ++i) {
			auto err = Write(elem[i]);
			if (err != 0) {
				assert(err == -ENOBUFS);
				break;
			}
		}
		return i + 1;
	}

	int Read(T &elem) noexcept
	{
		if (!IsInitialized()) {
			return -EEXIST;
		}
		if (IsEmpty()) {
			return -ENODATA;
		}

		memmove(&elem, &buffer_[read_index_], sizeof(T));
		read_index_++;
		return 0;
	}

	ssize_t Read(T *elem, size_t num) noexcept
	{
		if (!IsInitialized()) {
			return -EEXIST;
		}

		int i;
		for (i = 0; i < num; ++i) {
			auto err = Read(elem[i]);
			if (err != 0) {
				assert(err == -ENODATA);
				break;
			}
		}
		return i + 1;
	}

	size_t GetCapacity() const noexcept
	{
		return capacity_;
	}

	size_t GetSize() const noexcept
	{
		return write_index_ - read_index_;
	}

	bool IsFull() const noexcept
	{
		return GetSize() == GetCapacity();
	}

	bool IsEmpty() const noexcept
	{
		return GetSize() == 0;
	}

	bool IsInitialized() const noexcept
	{
		return is_initialized_;
	}
};

void TestCircularBuffer();
void TestCircularBufferMultithreading(k_thread_stack_t *, k_thread_stack_t *);