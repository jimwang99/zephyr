#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/ztest.h>

LOG_MODULE_REGISTER(testsuite_basic_main);

#include "circular_buffer.hpp"
#include "event_logger.hpp"

ZTEST(basic, test_read_and_write)
{
	const int NUM_ENTRY = 3;
	CircularBuffer<int> cb(NUM_ENTRY);

	LOG_INF("Testing writing CircularBuffer");
	for (int i = 0; i < NUM_ENTRY; ++i) {
		bool ret = cb.write(i);
		zassert_equal(ret, true);
	}
	for (int i = 0; i < NUM_ENTRY; ++i) {
		bool ret = cb.write(i);
		zassert_equal(ret, false);
	}

	LOG_INF("Testing reading CircularBuffer");
	for (int i = 0; i < NUM_ENTRY; ++i) {
		int j;
		bool ret = cb.read(j);
		zassert_equal(ret, true);
		zassert_equal(i, j);
	}
	for (int i = 0; i < NUM_ENTRY; ++i) {
		int j;
		bool ret = cb.read(j);
		zassert_equal(ret, false);
	}
}

ZTEST(basic, test_log_and_get)
{
	const int NUM_ENTRY = 16;
	EventLogger evlog(NUM_ENTRY);

	LOG_INF("Testing adding events to the EventLogger");
	zassert_equal(evlog.log(EventId::START_UP), true);
	k_sleep(K_CYC(1000));
	zassert_equal(evlog.log(EventId::START_READING), true);
	k_sleep(K_CYC(1000));
	zassert_equal(evlog.log(EventId::START_WRITING), true);
	k_sleep(K_CYC(1000));
	zassert_equal(evlog.log(EventId::STOP_READING), true);
	k_sleep(K_CYC(1000));
	zassert_equal(evlog.log(EventId::STOP_WRITING), true);
	k_sleep(K_CYC(1000));
	zassert_equal(evlog.log(EventId::SHUT_DOWN), true);
}

ZTEST_SUITE(basic, NULL, NULL, NULL, NULL, NULL);
