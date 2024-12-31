#ifndef CENGINE_CORE_ARRAY_LIST_H
#define CENGINE_CORE_ARRAY_LIST_H

#include "core.h"

#define ARRAYLIST(T) T##ArrayList
/**
 * Doesn't need T to be defined anywhere.
 */
#define CENGINE_CORE_ARRAY_LIST_DECLARE(T, PREFIX)                                                      \
    typedef struct {                                                                            \
        struct T *data;                                                                         \
        int len;                                                                                \
        int capacity;                                                                           \
    } PREFIX##ArrayList;                                                                        \
    int PREFIX##ArrayList_Init(PREFIX##ArrayList *arr, int capacity );                          \
    int PREFIX##ArrayList_Reserve(PREFIX##ArrayList *arr, int new_capacity);  \
    void PREFIX##ArrayList_Deinit(PREFIX##ArrayList *arr);                                          \

/**
 * T must be a valid typedef struct {} T;
 */
#define CENGINE_CORE_ARRAY_LIST_DECLARE_NATIVE(T, PREFIX)                                               \
    typedef struct {                                                                            \
        T *data;                                                                                \
        int len;                                                                                \
        int capacity;                                                                           \
    } PREFIX##ArrayList;                                                                        \
    int PREFIX##ArrayList_Init(PREFIX##ArrayList *arr, int capacity );                          \
    int PREFIX##ArrayList_Reserve(PREFIX##ArrayList *arr, int new_capacity);  					\
    void PREFIX##ArrayList_Deinit(PREFIX##ArrayList *arr);                                          					\


// Inline array functions that need the type T to be defined
#define CENGINE_CORE_ARRAY_LIST_INLINE( T, PREFIX )                                                                                        \
	/* Resize */                                                                                                                 \
	static inline void PREFIX##ArrayList_Resize( PREFIX##ArrayList *arr, int len )                                               \
	{                                                                                                                            \
		PREFIX##ArrayList_Reserve( arr, len );                                                                                   \
		arr->len = len;                                                                                                        \
	}                                                                                                                            \
	/* Get */                                                                                                                    \
	static inline T* PREFIX##ArrayList_Get( PREFIX##ArrayList *arr, int index )                                                    \
	{                                                                                                                            \
		CENGINE_CORE_ASSERT( 0 <= index && index < arr->len );                                                                             \
		return arr->data + index;                                                                                                  \
	}                                                                                                                            \
	/* Add */                                                                                                                    \
	static inline T* PREFIX##ArrayList_Add( PREFIX##ArrayList *arr )                                                               \
	{                                                                                                                            \
		if ( arr->len == arr->capacity )                                                                                           \
		{                                                                                                                        \
			int newCapacity = arr->capacity < 2 ? 2 : arr->capacity + ( arr->capacity * 2);                                          \
			PREFIX##ArrayList_Reserve( arr, newCapacity );                                                                         \
		}                                                                                                                        \
		arr->len += 1;                                                                                                           \
		return arr->data + ( arr->len - 1 );                                                                                       \
	}                                                                                                                            \
	/* Push */                                                                                                                   \
	static inline void PREFIX##ArrayList_Push( PREFIX##ArrayList *arr, T value )                                                   \
	{                                                                                                                            \
		if ( arr->len == arr->capacity )                                                                                           \
		{                                                                                                                        \
			int newCapacity = arr->capacity < 2 ? 2 : arr->capacity + ( arr->capacity * 2);                                          \
			PREFIX##ArrayList_Reserve( arr, newCapacity );                                                                         \
		}                                                                                                                        \
		arr->data[arr->len] = value;                                                                                               \
		arr->len += 1;                                                                                                           \
	}                                                                                                                            \
	/* Set */                                                                                                                    \
	static inline void PREFIX##ArrayList_Set( PREFIX##ArrayList *arr, int index, T value )                                         \
	{                                                                                                                            \
		CENGINE_CORE_ASSERT( 0 <= index && index < arr->len );                                                                             \
		arr->data[index] = value;                                                                                                  \
	}                                                                                                                            \
	/* RemoveSwap */                                                                                                             \
	static inline int PREFIX##ArrayList_RemoveSwap( PREFIX##ArrayList *arr, int index )                                            \
	{                                                                                                                            \
		CENGINE_CORE_ASSERT( 0 <= index && index < arr->len );                                                                             \
		int movedIndex = CENGINE_CORE_NULL_INDEX;                                                                                          \
		if ( index != arr->len - 1 )                                                                                             \
		{                                                                                                                        \
			movedIndex = arr->len - 1;                                                                                           \
			arr->data[index] = arr->data[movedIndex];                                                                                \
		}                                                                                                                        \
		arr->len -= 1;                                                                                                           \
		return movedIndex;                                                                                                       \
	}                                                                                                                            \
	/* Pop */                                                                                                                    \
	static inline T PREFIX##ArrayList_Pop( PREFIX##ArrayList *arr )                                                                \
	{                                                                                                                            \
		CENGINE_CORE_ASSERT( arr->len > 0 );                                                                                               \
		T value = arr->data[arr->len - 1];                                                                                         \
		arr->len -= 1;                                                                                                           \
		return value;                                                                                                            \
	}                                                                                                                            \
	/* Clear */                                                                                                                  \
	static inline void PREFIX##ArrayList_Clear( PREFIX##ArrayList *arr )                                                           \
	{                                                                                                                            \
		arr->len = 0;                                                                                                            \
	}                                                                                                                            \
	/* ByteCount */                                                                                                              \
	static inline int PREFIX##ArrayList_ByteSize( PREFIX##ArrayList *arr )                                                        \
	{                                                                                                                            \
		return (int)( arr->capacity * sizeof( T ) );                                                                               \
	}

// ArrayList implementations to be instantiated in a source file where the type is known
#define CENGINE_CORE_ARRAY_LIST_SOURCE( T, PREFIX )                                                                                        \
	/* Create */                                                                                                                 \
	int PREFIX##ArrayList_Init( PREFIX##ArrayList *arr, int capacity )                                                           \
	{                                                                                                                            \
		if ( capacity > 0 )                                                                                                      \
		{                                                                                                                        \
			void* mem = malloc( capacity * sizeof( T ) );                                                                        \
            if (mem == NULL) {                                                                                                   \
                return -1;                                                                                                       \
            }                                                                                                                    \
            arr->data = (T*)mem;                                                                                                    \
			arr->capacity = capacity;       																					\
			arr->len = 0;                                                   													\
		}                                                                                                                        \
        return 1;                                                                                                                \
	}                                                                                                                            \
	/* Reserve */                                                                                                                \
	int PREFIX##ArrayList_Reserve( PREFIX##ArrayList *arr, int newCapacity )                                                      \
	{                                                                                                                            \
		if ( newCapacity <= arr->capacity )                                                                                        \
		{                                                                                                                        \
			return -1;                                                                                                              \
		}                                                                                                                        \
		void* mem = realloc( arr->data, newCapacity * sizeof( T ) );                                    							\
        if(mem == NULL) {                                                                                                        \
            return -1;                                                                                                           \
        }                                                                                                                        \
        arr->data = (T*)mem;                                                                                                       \
		arr->capacity = newCapacity;                                                                                               \
        return 1;                                                                                                                \
	}                                                                                                                            \
	/* Destroy */                                                                                                                \
	void PREFIX##ArrayList_Deinit( PREFIX##ArrayList *arr )                                                                       \
	{                                                                                                                            \
		free( arr->data);                                                                              \
		arr->data = NULL;                                                                                                          \
		arr->len = 0;                                                                                                            \
		arr->capacity = 0;                                                                                                         \
	}


CENGINE_CORE_ARRAY_LIST_DECLARE_NATIVE(int, Integer);
CENGINE_CORE_ARRAY_LIST_INLINE(int, Integer);
CENGINE_CORE_ARRAY_LIST_DECLARE_NATIVE(char*, String);
CENGINE_CORE_ARRAY_LIST_INLINE(char*, String);
CENGINE_CORE_ARRAY_LIST_DECLARE_NATIVE(float, Float);
CENGINE_CORE_ARRAY_LIST_INLINE(float, Float);

#endif
