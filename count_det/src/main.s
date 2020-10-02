.text

.global count_det

count_det:
    ldr r1, [r0], #4
    ldr r2, [r0], #4
    ldr r3, [r0], #4

    push {r4-r11}

    ldr r4, [r0], #4
    ldr r5, [r0], #4
    ldr r6, [r0], #4
    ldr r7, [r0], #4
    ldr r8, [r0], #4
    ldr r9, [r0], #0

    mul r10, r5, r9
    mul r11, r6, r8
    sub r0, r10, r11
    mul r1, r1, r0

    mul r10, r6, r7
    mul r11, r4, r9
    sub r0, r10, r11
    mul r2, r2, r0

    mul r10, r4, r8
    mul r11, r5, r7
    sub r0, r10, r11
    mul r3, r3, r0

    pop {r4-r11}

    add r0, r1, r2
    add r0, r0, r3

    bx lr
