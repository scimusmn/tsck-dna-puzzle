# minunit project template

(c) sanine (Kate Swanson) 2021

This is intended to be a quick-start template for my ever-growing `minunit`
test system. It's designed to be very simple and require no dependencies,
although it has most definitely strayed from the original minunit's extreme
minimalism.

## getting started

Clone and enter the repository, and then run

```bash
mkdir build
cd build
cmake ..
make test
```

This will create an executable called `test` that actually runs all of your
tests. Right now, it should only be running the minunit self-tests.

## writing new tests

All minunit tests are organized into *suites*, which are simply functions that
call out to other test functions. To create a suite, you must declare it in `
src/test/suites.h` and then add it to the RUN_TESTS macro. For example:

```c
#pragma once

#include "minunit.h"

void mock_queue_tests();
void my_new_suite();

#define RUN_TESTS                               \
   mu_run_suite(mock_queue_tests);              \
   mu_run_suite(my_new_suite);
```

Make sure your suite is in `RUN_TESTS` as it won't run otherwise!

Now you can actually start writing tests. The convention is, if you're testing
a file called `my_file.c`, you should create a file called `my_file.test.c` to
store the tests. Here's an example test file:

```c
#include "test/minunit.h"
#include "test/mock_queue.h"

mu_test my_new_test_1();
mu_test my_new_test_2();


void my_new_suite()
{
   mu_run_test("this test should succeed", my_new_test_1);
   mu_run_test("this test should fail", my_new_test_2);
}


mu_test my_new_test_1()
{
   // all of these should succeed
   mu_assert(3 < 4);
   mu_assert_equal(15, 5*3);
   mu_assert_unequal(3, 4);
   mu_assert_streq("hello", "hello");

   return 0;
}


mu_test my_new_test_2()
{
   // all of these should fail
   mu_assert(3 > 4);
   mu_assert_equal(15, 5*5);
   mu_assert_unequal(3, 3);
   mu_assert_streq("hello", "world");

   return 0;
}
```

Each test must have the signature `mu_test test()` and each test must `return 0`
at the end. Within each test, there are four different assert types provided
which, if the asserts fail, will return from the function immediately and when
called within `mu_run_test()` will print a helpful warning message, including
the filename and line of the failed assertion.

Now you just need to add your files (`my_file.c` and `my_file.test.c`) to 
`TEST_SOURCES` in the CMakeLists.txt file. Then, in the build folder, you can
run `make test && ./test` to run the tests.


## development & contributing

This is a fairly motley collection of testing macros and functions that I've
assembled as I've worked on various projects, and unless someone out there
feels very passionate about making this a proper library in its own right,
that's probably how it will stay.

I've been working on a system for creating mocks -- that's the aspect of this
repository that's likely to see the most changes, as the rest of this library
has about a year and a half of battle testing and I feel fairly okay with it.

Eventually, I'd also like to add code coverage reporting so that this system
could be used with a CI system, but that's a ways off at this point.


## license

All of the code in this repository, unless otherwise marked, is licensed under
the Anti-Capitalist Software License.