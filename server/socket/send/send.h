#ifndef SEND_ONUS_H
#define SEND_ONUS_H
#include "../../def.h"
#ifdef __cplusplus
extern "C" {
#endif

    void sk_send_onus (int);
    int sk_send_get_onus (char*);
    int sk_send_data (const char* data);
    int sk_send_shutdown();
#ifdef __cplusplus
}
#endif

#endif //SEND_ONUS_H
