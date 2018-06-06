/*
 * boot.S
 *
 *  Created on: Jun 6, 2018
 *      Author: computation
 */

.global _start

_start:
    ldr sp, = STACK_TOP

    bl earlyKernel
    bl mainKernel

1:
    b 1b
.size _start, . - _start

