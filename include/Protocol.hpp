#include <stddef.h>
typedef struct {
    int msg_type;
    size_t msg_len;
} URI;

#define NORMAL_MSG 1
#define USER_LIST 2