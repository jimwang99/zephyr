#include <zephyr/logging/log.h>
#include <zephyr/ztest.h>

#include "circular_buffer.hpp"

LOG_MODULE_REGISTER(circular_buffer);

void TestCircularBuffer()
{
	const int NUM_ENTRY = 3;
	CircularBuffer<int> cb(NUM_ENTRY);
	cb.Open();

	LOG_INF("Testing writing CircularBuffer");
	for (int i = 0; i < NUM_ENTRY; ++i) {
		auto err = cb.Write(i);
		zassert_equal(err, 0);
	}
	for (int i = 0; i < NUM_ENTRY; ++i) {
		auto err = cb.Write(i);
		zassert_equal(err, -ENOBUFS);
	}

	LOG_INF("Testing reading CircularBuffer");
	for (int i = 0; i < NUM_ENTRY; ++i) {
		int j;
		auto err = cb.Read(j);
		zassert_equal(err, 0);
		zassert_equal(i, j);
	}
	for (int i = 0; i < NUM_ENTRY; ++i) {
		int j;
		auto err = cb.Read(j);
		zassert_equal(err, -ENODATA);
	}

	cb.Close();
}

void ThreadProducer(void *circular_buffer, void *unused2, void *unused3)
{
	LOG_INF("ThreadProducer starts ...");
	// k_sleep(K_MSEC(1));
	// LOG_INF("sleep done");
	auto cb = reinterpret_cast<CircularBuffer<int> *>(circular_buffer);
	// for (int i = 0; i < 20; ++i) {
	// 	while (cb->Write(i) == -ENOBUFS) {
	// 		// k_sleep(K_CYC(1000));
	// 	}
	// 	LOG_DBG("Write %d", i);
	// }
	LOG_INF("ThreadProducer ends ...");
}

void ThreadConsumer(void *circular_buffer, void *unused2, void *unused3)
{
	LOG_INF("ThreadConsumer starts ...");
	auto cb = reinterpret_cast<CircularBuffer<int> *>(circular_buffer);
	// for (int i = 0; i < 20; ++i) {
	// 	int j;
	// 	while (cb->Read(j) == -ENODATA) {
	// 		// k_sleep(K_CYC(1000));
	// 	}
	// 	LOG_DBG("Read %d", i);
	// 	zassert_equal(i, j);
	// }
	LOG_INF("ThreadConsumer ends ...");
}

void TestCircularBufferMultithreading(k_thread_stack_t *stack1, k_thread_stack_t *stack2)
{
	zassert_not_null(stack1);
	zassert_not_null(stack2);
	// create circular buffer
	const int NUM_ENTRY = 3;
	CircularBuffer<int> cb(NUM_ENTRY);
	cb.Open();

	// create thread
	k_thread producer;
	k_thread consumer;

	auto tid1 =
		k_thread_create(&producer, stack1, K_THREAD_STACK_SIZEOF(stack1), ThreadProducer,
				static_cast<void *>(&cb), NULL, NULL, 5, 0, K_NO_WAIT);
	auto tid2 =
		k_thread_create(&consumer, stack2, K_THREAD_STACK_SIZEOF(stack2), ThreadConsumer,
				static_cast<void *>(&cb), NULL, NULL, 5, 0, K_NO_WAIT);

	k_thread_start(tid1);
	k_thread_start(tid2);

	k_thread_join(tid1, K_FOREVER);
	k_thread_join(tid2, K_FOREVER);

	cb.Close();
}