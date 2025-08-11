#ifndef __PWM_H__
#define __PWM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void PWM_Init(void);
void PWM_Update(void);

#ifdef __cplusplus
}
#endif
#endif /* __PWM_H__ */
