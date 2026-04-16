// snippet.S - AArch64
// x0 - history ptr
// x1 - syscall number
// x2 - syscall arg1

.text

.altmacro
.macro do_branch i
	beq .Lnext_\i
	.Lnext_\i:
.endm

.global fill_bhb
.fill_bhb:
	// save stack pointer and return address (link register)
	stp x29, x30, [sp, #-16]

.set i, 0
.rept 256
	ldrb w3, [x0]
	add x0, x0, 1
	cmp w3, 0
	do_branch %i
.set i, i+1
.endr

	mov x8, x1
	mov x0,x2
	svc 0

	ldp x29, x30, [sp], #16
	ret


