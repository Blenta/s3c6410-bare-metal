@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.globl led_on
led_on:

	/* Port M Control Register */
	/* GPM 0-3 -> LED 1-4 */

	ldr r1, =0x7F008820		@ GPMCON - Port M Configuration Register
	mov r0, #0x10			@ [3:0] - GPM0, [7:4] - GPM1, [11:8] - GPM2, [15:12] - GPM3, b0001 = Output 
	str r0, [r1]

	ldr r1, =0x7F008824		@ GPMDAT - Port M Data Registe
	mov r0, #0			@ [3:0] - GPM[3:0], 0 - led on / 1 - led off, b0000 = led on
	str r0, [r1]

	mov pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.globl led_blink
led_blink:

	PUSH {LR}

	/* Port M Control Register */
	/* GPM 0-3 -> LED 1-4 */

	ldr r1, =0x7F008820		@ GPMCON - Port M Configuration Register
	mov r0, #0x1000			@ [3:0] - GPM0, [7:4] - GPM1, [11:8] - GPM2, [15:12] - GPM3, b0001 = Output 
	str r0, [r1]

	ldr r1, =0x7F008824		@ GPMDAT - Port M Data Registe
	mov r0, #7			@ [3:0] - GPM[3:0], 0 - led on / 1 - led off, 7(b0111) = LED4 on

	mov r3, #12			@ blink count
blink_loop:
	str r0, [r1]
	cmp r0, #7 
	moveq r0, #15
	movne r0, #7
	bl delay

	subs r3, r3, #1
	bne blink_loop
	
	POP {LR}

	mov pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.globl led_flow
led_flow:

	PUSH {LR}

	/* Port M Control Register */
	/* GPM 0-3 -> LED 1-4 */

	ldr r1, =0x7F008820		@ GPMCON - Port M Configuration Register
	ldr r0, =0x1111			@ [3:0] - GPM0, [7:4] - GPM1, [11:8] - GPM2, [15:12] - GPM3, b0001 = Output 
	str r0, [r1]

	ldr r1, =0x7F008824		@ GPMDAT - Port M Data Registe
	mov r0, #7			@ [3:0] - GPM[3:0], 0 - led on / 1 - led off, 7(b0111) = LED4 on

	mov r3, #12			@ flow count
flow_loop:
	str r0, [r1]
	ror r0, r0, #1
	ldr r2, =0x70000000	
	cmp r0, r2 
	moveq r0, #0x70
	bl delay

	subs r3, r3, #1
	bne flow_loop

	POP {LR}

	mov pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

delay:
	mov r2, #0x200000
delay_loop:
	sub r2, r2, #1
	cmp r2, #0
	bne delay_loop		
	mov pc, lr


