#include <Arduino.h>
#include <unity.h>

#include "rolling_array.h"
#include "short_float.h"

void test_less_than() {
  byte a = 5;
  byte b = a + 2;
  TEST_ASSERT_EQUAL(a, lesser(a, b));
  TEST_ASSERT_EQUAL(a, lesser(b, a));
  TEST_ASSERT_EQUAL(a, lesser(a, a));

  TEST_ASSERT_EQUAL(b, bigger(a, b));
  TEST_ASSERT_EQUAL(b, bigger(b, a));
  TEST_ASSERT_EQUAL(b, bigger(b, b));

  byte z = 0;
  TEST_ASSERT_EQUAL(a, lesser(z, a));
  TEST_ASSERT_EQUAL(a, lesser(a, z));
  TEST_ASSERT_EQUAL(a, bigger(z, a));
  TEST_ASSERT_EQUAL(a, bigger(a, z));
}

void test_rolling_array() {
  rolling_array<float, 5> ra;

  ra.insert(1);
  ra.insert(2);
  ra.insert(3);
  TEST_ASSERT_EQUAL(2, ra.get_avg());
  TEST_ASSERT_EQUAL(1, ra.get_min());
  TEST_ASSERT_EQUAL(3, ra.get_max());

  ra.insert(4);
  ra.insert(5);
  TEST_ASSERT_EQUAL(3, ra.get_avg());
  TEST_ASSERT_EQUAL(1, ra.get_min());
  TEST_ASSERT_EQUAL(5, ra.get_max());

  ra.insert(6);
  ra.insert(7);
  TEST_ASSERT_EQUAL(5, ra.get_avg());
  TEST_ASSERT_EQUAL(3, ra.get_min());
  TEST_ASSERT_EQUAL(7, ra.get_max());
}

void test_rolling_minmax() {
  // with only 3 slots
  rolling_minmax<float, unsigned long, 3000, 1000> rm;

  TEST_ASSERT_EQUAL(true, rm.empty());

  // first slot
  rm.add_val(900000, 20);
  rm.add_val(910000, 22);
  rm.add_val(920000, 24);
  TEST_ASSERT_EQUAL(20, rm.get_min());
  TEST_ASSERT_EQUAL(24, rm.get_max());
  TEST_ASSERT_EQUAL(false, rm.empty());

  // next slot
  rm.add_val(1010000, 25.8);
  rm.add_val(1020000, 24.9);
  rm.add_val(1030000, 25.4);
  TEST_ASSERT_EQUAL(20, rm.get_min());
  TEST_ASSERT_EQUAL(25.8, rm.get_max());

  // next slot
  rm.add_val(3100000, 26);
  rm.add_val(3200000, 27);
  TEST_ASSERT_EQUAL(20, rm.get_min());
  TEST_ASSERT_EQUAL(27, rm.get_max());

  // next slot (with array full, but this is still using buffer)
  rm.add_val(4200000, 28);
  TEST_ASSERT_EQUAL(20, rm.get_min());
  TEST_ASSERT_EQUAL(28, rm.get_max());

  // next slot, this should loop and erase the first slot with last buffer
  rm.add_val(5210000, 26);
  TEST_ASSERT_EQUAL(24.9, rm.get_min());
  TEST_ASSERT_EQUAL(28, rm.get_max());
}

void test_short_float() {
  short_float<3> a(23.5);
  short_float<3> b(16.5);

  TEST_ASSERT_EQUAL(a.val(), 23.5);
  TEST_ASSERT_EQUAL(b.val(), 16.5);
  TEST_ASSERT_EQUAL(true, a > b);
  TEST_ASSERT_EQUAL(true, a == a);
  TEST_ASSERT_EQUAL(true, b < a);
}

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    Serial.begin(115200);
    Serial.println("Start tests!");

    UNITY_BEGIN();    // IMPORTANT LINE!
    RUN_TEST(test_less_than);
    RUN_TEST(test_rolling_array);
    RUN_TEST(test_rolling_minmax);
    RUN_TEST(test_short_float);
}

void loop() {
  UNITY_END();
}