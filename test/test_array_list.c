#include <cengine/cengine.h>
#include "test.h"


int CENGINE_TEST_CORE_ArrayList() 
{
    IntegerArrayList arr;
    IntegerArrayList_Init(&arr, 10);

    for (size_t i = 0; i < 10; i++) { 
        IntegerArrayList_Push(&arr, 10);
        printf("item: %d \r\n", arr.data[i]);
    }

    CENGINE_TEST_ASSERT_TRUE(arr.len == 10);

    IntegerArrayList_Deinit(&arr);

    return 0;
}