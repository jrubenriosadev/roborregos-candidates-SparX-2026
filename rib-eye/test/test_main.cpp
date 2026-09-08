#include <unity.h>
#include "Arduino.h"

void test_chip_model_not_null(void) {
    TEST_ASSERT_NOT_NULL(ESP.getChipModel());
}

void setup() {
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_chip_model_not_null);
    UNITY_END();
}

void loop() {}