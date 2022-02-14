#pragma once

#include "minunit.h"

void mock_queue_tests();
void soya_ring_tests();
void soya_disk_tests();

#define RUN_TESTS				\
   mu_run_suite(mock_queue_tests);		\
   mu_run_suite(soya_ring_tests);		\
   mu_run_suite(soya_disk_tests);
