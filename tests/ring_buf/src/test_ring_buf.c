/*
 * Ring Buffer Module - Homework Test Skeleton
 *
 * test_fresh_state is provided as a worked example. Fill in the remaining
 * 7 ZTEST bodies according to TEST_SPEC.md. Stubs call ztest_test_skip()
 * so the binary builds and runs cleanly before each test is implemented.
 *
 * Run:
 *   west twister -T tests/ring_buf -p native_sim
 */

#include <zephyr/ztest.h>
#include <errno.h>

#include "ring_buf.h"

/*
 * Shared before hook: every suite reinitialises the ring buffer with a
 * capacity of 4 so tests start from a clean, known state. Capacity 4 is
 * enough to exercise FIFO order (push 1, 2, 3) and overflow (full at 4).
 */
static void before(void *f)
{
	ARG_UNUSED(f);
	rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 *
 * Initial state and re-initialization behaviour.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

/* PROVIDED — study this test before writing the rest. */
ZTEST(ring_buf_init, test_fresh_state)
{
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state)
{
	/* TODO(l8-task1): Push a value, call rb_init(4) again, then
	 * verify the buffer is empty and count is 0.
	 * See TEST_SPEC.md "Suite ring_buf_init" #2.
	 */
	int ret = 0;

	ret = rb_push(0);
	zassert_equal(ret, 0, "Push should succeed");
	ret = rb_init(4);
	zassert_equal(ret, 0, "Re-initialization should succeed");

	zassert_true(rb_is_empty(), "Re-initialized buffer must be empty");
	zassert_equal(rb_count(), 0, "Re-initialized buffer count must be Zero");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 *
 * Single push/pop round-trip, FIFO order, full error path.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop)
{
	/* TODO(l8-task1): rb_push(42), rb_pop(&v) -> v == 42, buffer empty after.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #1.
	 */
	int val = 0;
	int ret = 0;
	ret = rb_push(42);
	zassert_equal(ret, 0, "Push should succeed");
	ret = rb_pop(&val);
	zassert_equal(ret, 0, "Pop should succeed");
	zassert_equal(val, 42, "Popped value should be 42");
	zassert_true(rb_is_empty(), "Buffer should be empty after pop");
}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
	/* TODO(l8-task1): rb_push(1), rb_push(2), rb_push(3); pop three times
	 * and verify the values come out as 1, 2, 3 in that order.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #2.
	 */
	int ret = 0;
	int val = 0;

	ret = rb_push(1);
	zassert_equal(ret, 0, "Push 1 should succeed");
	ret = rb_push(2);
	zassert_equal(ret, 0, "Push 2 should succeed");
	ret = rb_push(3);
	zassert_equal(ret, 0, "Push 3 should succeed");

	ret = rb_pop(&val);
	zassert_equal(ret, 0, "Pop should succeed");
	zassert_equal(val, 1, "First popped value should be 1");

	ret = rb_pop(&val);	
	zassert_equal(ret, 0, "Pop should succeed");
	zassert_equal(val, 2, "Second popped value should be 2");

	ret = rb_pop(&val);
	zassert_equal(ret, 0, "Pop should succeed");
	zassert_equal(val, 3, "Third popped value should be 3");
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
	/* TODO(l8-task1): Fill the buffer to its capacity of 4, then push
	 * one more value -> -ENOSPC.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #3.
	 */
	int ret = 0;
	ret = rb_push(1);
	zassert_equal(ret, 0, "Push 1 should succeed");
	ret = rb_push(2);
	zassert_equal(ret, 0, "Push 2 should succeed");
	ret = rb_push(3);
	zassert_equal(ret, 0, "Push 3 should succeed");
	ret = rb_push(4);
	zassert_equal(ret, 0, "Push 4 should succeed");
	ret = rb_push(5);
	zassert_equal(ret, -ENOSPC, "Push to full buffer should return -ENOSPC");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 *
 * Peek semantics and NULL-pointer boundary conditions.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume)
{
	/* TODO(l8-task1): rb_push(7); rb_peek(&v) -> v == 7; rb_peek(&v) again
	 * -> v == 7; rb_count() still == 1.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #1.
	 */
	int ret = 0;
	int val = 0;

	ret = rb_push(7);
	zassert_equal(ret, 0, "Push should succeed");
	ret = rb_peek(&val);
	zassert_equal(ret, 0, "Peek should succeed");
	zassert_equal(val, 7, "Peeked value should be 7");
	ret = rb_peek(&val);
	zassert_equal(ret, 0, "Peek should succeed");
	zassert_equal(val, 7, "Peeked value should be 7");
	zassert_equal(rb_count(), 1, "Buffer count should be 1");
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
	/* TODO(l8-task1): rb_pop(NULL) -> -EINVAL.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #2.
	 */
	int ret = 0;
	ret = rb_pop(NULL);
	zassert_equal(ret, -EINVAL, "Pop with NULL pointer should return -EINVAL");
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
	/* TODO(l8-task1): push 4 values -> rb_is_full() == true, rb_count() == 4.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #3.
	 */
	int ret = 0;
	ret = rb_push(1);
	zassert_equal(ret, 0, "Push should succeed");
	ret = rb_push(2);
	zassert_equal(ret, 0, "Push should succeed");
	ret = rb_push(3);
	zassert_equal(ret, 0, "Push should succeed");
	ret = rb_push(4);
	zassert_equal(ret, 0, "Push should succeed");
	zassert_equal(rb_is_full(), true, "Buffer should be full");
	zassert_equal(rb_count(), 4, "Buffer count should be 4");
}
