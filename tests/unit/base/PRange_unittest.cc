#include <cmath>
#include <gtest/gtest.h>
#include <shogun/base/progress.h>
#include <shogun/io/SGIO.h>
#include <thread>

using namespace shogun;

const int millis = 10;

auto range_test = progress("PROGRESS: ", range(0, 100));
TEST(PRange, basic_upper)
{
	int other_i = 0;
	int count = 10;
	for (auto i : progress("PROGRESS: ", range(count)))
	{
		EXPECT_EQ(i, other_i);
		other_i++;
	}
	EXPECT_EQ(count, other_i);
}

TEST(PRange, basic_lower_upper)
{
	int count = 10;
	int start = std::rand();
	int other_i = start;
	for (auto i : progress("PROGRESS: ", range(start, start + count)))
	{
		EXPECT_EQ(i, other_i);
		other_i++;
	}
	EXPECT_EQ(start + count, other_i);
}

TEST(PRange, zero)
{
	int actual_count = 0;
	int count = 0;
	for (auto i : progress("PROGRESS: ", range(count)))
	{
		(void)i;
		actual_count++;
	}
	EXPECT_EQ(count, actual_count);
}

TEST(PRange, identical_bounds)
{
	int actual_count = 0;
	int b = std::rand();
	for (auto i : progress("PROGRESS: ", range(b, b)))
	{
		(void)i;
		actual_count++;
	}
	EXPECT_EQ(0, actual_count);
}

TEST(PRange, progress_correct_bounds_positive)
{
	env()->io()->enable_progress();
	range_test = progress("PROGRESS: ", range(0, 10));
	for (int i = 0; i < 10; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(millis));
		EXPECT_EQ(std::ceil(range_test.get_current_progress()), i);
		range_test.print_progress();
	}
	range_test.complete();
	EXPECT_EQ(std::ceil(range_test.get_current_progress()), 11);
}

TEST(PRange, progress_correct_bounds_negative)
{
	env()->io()->enable_progress();
	range_test = progress("PROGRESS: ", range(-10, 0));
	for (int i = -10; i > 0; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(millis));
		EXPECT_EQ(std::ceil(range_test.get_current_progress()), i);
		range_test.print_progress();
	}
	range_test.complete();
	EXPECT_EQ(std::ceil(range_test.get_current_progress()), 1);
}

TEST(PRange, progress_iterator_correct_bounds_positive)
{
	env()->io()->enable_progress();
	range_test = progress("PROGRESS: ", range(0, 10));
	for (auto i : range_test)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(millis));
		EXPECT_EQ(std::ceil(range_test.get_current_progress()), i);
	}
	EXPECT_EQ(std::ceil(range_test.get_current_progress()), 11);
}

TEST(PRange, progress_iterator_correct_bounds_negative)
{
	env()->io()->enable_progress();
	range_test = progress("PROGRESS: ", range(-10, 0));
	for (auto i : range_test)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(millis));
		EXPECT_EQ(std::ceil(range_test.get_current_progress()), i);
	}
	EXPECT_EQ(std::ceil(range_test.get_current_progress()), 1);
}

TEST(PRange, lambda_stop)
{
	int test = 6;
	/* Stops before the 4th iteration */
	for (auto i : progress("PROGRESS: ", range(0, 6), UTF8, [&]() {
		     return test > 3;
		 }))
	{
		(void)i;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		test--;
	}
	EXPECT_EQ(test, 3);
}

TEST(PRange, lambda_stop_shorter_method)
{
	int test = 6;
	/* Stops before the 4th iteration */
	for (auto i :
	     progress("PROGRESS: ", range(0, 6), [&]() { return test > 3; }))
	{
		(void)i;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		test--;
	}
	EXPECT_EQ(test, 3);
}

TEST(PRange, DISABLED_progress_incorrect_bounds_positive)
{
	env()->io()->enable_progress();
	range_test = progress("PROGRESS: ", range(100, 0));
	for (auto i : range_test)
	{
		(void)i;
		std::this_thread::sleep_for(std::chrono::milliseconds(millis));
	}
	EXPECT_FLOAT_EQ(range_test.get_current_progress(), (float64_t)0);
}

TEST(PRange, DISABLED_progress_incorrect_bounds_negative)
{
	env()->io()->enable_progress();
	range_test = progress("PROGRESS: ", range(100, 0));
	for (auto i : range_test)
	{
		(void)i;
		std::this_thread::sleep_for(std::chrono::milliseconds(millis));
	}
	EXPECT_FLOAT_EQ(range_test.get_current_progress(), (float64_t)0);
}

TEST(PRange, DISABLED_progress_incorrect_bounds_equal)
{
	env()->io()->enable_progress();
	range_test = progress("PROGRESS: ", range(1, 1));
	for (auto i : range_test)
	{
		(void)i;
		std::this_thread::sleep_for(std::chrono::milliseconds(millis));
	}
	EXPECT_FLOAT_EQ(range_test.get_current_progress(), (float64_t)0);
}
