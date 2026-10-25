
.globl led_on
led_on:

	/* Port M Control Register */
	/* GPM 0-3 -> LED 1-4 */

	ldr r1, =0x7F008820		@ GPMCON - Port M Configuration Register
	ldr r0, =0x1111			@ [3:0] - GPM0, [7:4] - GPM1, [11:8] - GPM2, [15:12] - GPM3, b0001 = Output 
	str r0, [r1]

	ldr r1, =0x7F008824		@ GPMDAT - Port M Data Registe
	mov r0, #0xd			@ [3:0] - GPM[3:0], 0 - led on / 1 - led off, b1101 = LED2 on 
	str r0, [r1]

	mov pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.globl led_blink
led_blink:

	PUSH {LR}

	/* Port M Control Register */
	/* GPM 0-3 -> LED 1-4 */

	ldr r1, =0x7F008820		@ GPMCON - Port M Configuration Register
	ldr	r0, =0x1111			@ [3:0] - GPM0, [7:4] - GPM1, [11:8] - GPM2, [15:12] - GPM3, b0001 = Output 
	str r0, [r1]

	ldr r1, =0x7F008824		@ GPMDAT - Port M Data Registe
	mov r0, #14			@ [3:0] - GPM[3:0], 0 - led on / 1 - led off, 14(b1110) = LED1 on

	mov r3, #8			@ blink count
blink_loop:
	str r0, [r1]
	mvn r0, r0
	orr r0, r0, #14
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

	mov r3, #8			@ flow count
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
	mov r2, #0xF00000
delay_loop:
	sub r2, r2, #1
	cmp r2, #0
	bne delay_loop		
	mov pc, lr


