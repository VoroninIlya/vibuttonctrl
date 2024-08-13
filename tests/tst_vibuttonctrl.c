#include "unity.h"
#include "unity_fixture.h"
#include "vibuttonctrl.h"
#include "stdio.h"

TEST_GROUP(TST_VIBUTTCTRL);

TEST_GROUP_RUNNER(TST_VIBUTTCTRL) {
  RUN_TEST_CASE(TST_VIBUTTCTRL, VIBTCTR_Init);
  RUN_TEST_CASE(TST_VIBUTTCTRL, VIBTCTR_Create);
}

TEST_SETUP(TST_VIBUTTCTRL) {

}

TEST_TEAR_DOWN(TST_VIBUTTCTRL) {
}

TEST(TST_VIBUTTCTRL, VIBTCTR_Init)
{
  // parametrs == NULL
  {
    TEST_ASSERT_EQUAL(false, VIBTCTR_Init(NULL, NULL, NULL));
  }
  // normal conditions
  {
    TEST_ASSERT_EQUAL(true, VIBTCTR_Init(
      (VIBTCTR_getPortState_t)1, (VIBTCTR_getTick_t)1, 
      (VIBTCTR_IsTickOverflowed_t)1));
  }
}

TEST(TST_VIBUTTCTRL, VIBTCTR_Create)
{
  // parametrs == NULL
  {
    TEST_ASSERT_EQUAL(false, VIBTCTR_Create(NULL, NULL, 0));
  }
  // normal conditions
  {
    TEST_ASSERT_EQUAL(true, VIBTCTR_Create("1", (void*)1, 0));
  }
  // with the same name
  {
    TEST_ASSERT_EQUAL(false, VIBTCTR_Create("1", (void*)1, 0));
  }
  // if VIBTCTR_MAX_BUTTONS > buttons.initializedNumber
  {
    TEST_ASSERT_EQUAL(true, VIBTCTR_Create("2", (void*)1, 0));
    TEST_ASSERT_EQUAL(false, VIBTCTR_Create("3", (void*)1, 0));
  }
}