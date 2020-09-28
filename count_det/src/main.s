.text

.global count_det

count_det:
    ldr r1, [r0], #4
    ldrb r2, [r0], #4
    ldrb r3, [r0], #4
    ldrb r4, [r0], #4
    ldrb r5, [r0], #4
    ldrb r6, [r0], #4
    ldrb r7, [r0], #4
    ldrb r8, [r0], #4
    ldrb r9, [r0], #0

    mul r10, r5, r9
    mul r0, r6, r8
    sub r0, r10, r0
    mul r1, r1, r0

    mul r10, r6, r7
    mul r0, r4, r9
    sub r0, r10, r0
    mul r2, r2, r0

    mul r10, r4, r8
    mul r0, r5, r7
    sub r0, r10, r0
    mul r3, r3, r0

    add r0, r1, r2
    add r0, r0, r3

    bx lr
