#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/ztest.h>

LOG_MODULE_REGISTER(testsuite_basic_main);

#include "circular_buffer.hpp"
// #include "event_logger.hpp"

K_THREAD_STACK_DEFINE(stack1, 1024);
K_THREAD_STACK_DEFINE(stack2, 1024);

ZTEST(basic, test_circular_buffer)
{
	TestCircularBuffer();
}

ZTEST(basic, test_circular_buffer_multithreading)
{
	TestCircularBufferMultithreading(stack1, stack2);
}

// ZTEST(basic, test_log_and_get)
// {
// 	const int NUM_ENTRY = 16;
// 	EventLogger evlog(NUM_ENTRY);

// 	LOG_INF("Testing adding events to the EventLogger");
// 	zassert_equal(evlog.log(EventId::START_UP), true);
// 	k_sleep(K_CYC(1000));
// 	zassert_equal(evlog.log(EventId::START_READING), true);
// 	k_sleep(K_CYC(1000));
// 	zassert_equal(evlog.log(EventId::START_WRITING), true);
// 	k_sleep(K_CYC(1000));
// 	zassert_equal(evlog.log(EventId::STOP_READING), true);
// 	k_sleep(K_CYC(1000));
// 	zassert_equal(evlog.log(EventId::STOP_WRITING), true);
// 	k_sleep(K_CYC(1000));
// 	zassert_equal(evlog.log(EventId::SHUT_DOWN), true);
// }

ZTEST_SUITE(basic, NULL, NULL, NULL, NULL, NULL);
