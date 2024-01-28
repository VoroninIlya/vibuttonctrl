#include "unity.h"
#include "unity_fixture.h"
#include "vibuttonctrl.h"
#include "stdio.h"

TEST_GROUP(TST_VIBUTTCTRL);

TEST_GROUP_RUNNER(TST_VIBUTTCTRL) {
  RUN_TEST_CASE(TST_VIBUTTCTRL, VIBCORE_Runtime);
}

TEST_SETUP(TST_VIBUTTCTRL) {
}

TEST_TEAR_DOWN(TST_VIBUTTCTRL) {
}

TEST(TST_VIBUTTCTRL, VIBTCTR_Runtime)
{
  printf("Test: VIBTCTR_Runtime");
  TEST_ASSERT_EQUAL(0, VIBTCTR_Runtime());
}