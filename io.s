	.file	"io.c"
	.intel_syntax noprefix
	.text
.Ltext0:
	.file 0 "/home/user/Desktop/c_stuff/Turing-compiler" "src/io.c"
	.section	.rodata
	.align 8
.LC0:
	.string	"Memory allocation failed [allocate_all_tape]\n"
.LC1:
	.string	"size=%ld\n"
	.text
	.globl	allocate_all_tape
	.type	allocate_all_tape, @function
allocate_all_tape:
.LFB7:
	.file 1 "src/io.c"
	.loc 1 17 64
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	.loc 1 24 21
	mov	rax, QWORD PTR -24[rbp]
	mov	r9d, 0
	mov	r8d, -1
	mov	ecx, 34
	mov	edx, 3
	mov	rsi, rax
	mov	edi, 0
	call	mmap@PLT
	mov	QWORD PTR -8[rbp], rax
	.loc 1 25 9
	cmp	QWORD PTR -8[rbp], -1
	jne	.L2
	.loc 1 28 9
	lea	rax, .LC0[rip]
	mov	rdi, rax
	call	perror@PLT
	.loc 1 29 9
	mov	rax, QWORD PTR -24[rbp]
	mov	rsi, rax
	lea	rax, .LC1[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	.loc 1 30 9
	mov	edi, 1
	call	exit@PLT
.L2:
	.loc 1 32 12
	mov	rax, QWORD PTR -8[rbp]
	.loc 1 33 1
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	allocate_all_tape, .-allocate_all_tape
	.section	.rodata
	.align 8
.LC2:
	.string	"Memory deallocation failed [free_all_tape]\n"
	.text
	.globl	free_all_tape
	.type	free_all_tape, @function
free_all_tape:
.LFB8:
	.loc 1 37 73
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	QWORD PTR -8[rbp], rdi
	mov	QWORD PTR -16[rbp], rsi
	.loc 1 46 13
	mov	rdx, QWORD PTR -16[rbp]
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	munmap@PLT
	.loc 1 46 12
	cmp	eax, -1
	jne	.L6
	.loc 1 47 13
	lea	rax, .LC2[rip]
	mov	rdi, rax
	call	perror@PLT
	.loc 1 48 13
	mov	edi, 1
	call	exit@PLT
.L6:
	.loc 1 51 1
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	free_all_tape, .-free_all_tape
	.section	.rodata
.LC3:
	.string	"haulted"
.LC4:
	.string	"didnt hault"
.LC5:
	.string	"  ERROR: out of tape"
.LC6:
	.string	" unknowen error code: %d!!!"
	.text
	.globl	exit_turing
	.type	exit_turing, @function
exit_turing:
.LFB9:
	.loc 1 54 60
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	DWORD PTR -4[rbp], edi
	.loc 1 55 4
	cmp	DWORD PTR -4[rbp], 0
	jne	.L8
	.loc 1 56 3
	lea	rax, .LC3[rip]
	mov	rdi, rax
	call	puts@PLT
	.loc 1 57 3
	jmp	.L13
.L8:
	.loc 1 60 2
	lea	rax, .LC4[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	.loc 1 61 2
	cmp	DWORD PTR -4[rbp], 1
	je	.L10
	cmp	DWORD PTR -4[rbp], 2
	jne	.L11
	.loc 1 63 4
	lea	rax, .LC5[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	.loc 1 64 4
	jmp	.L12
.L10:
	.loc 1 66 4
	lea	rax, .LC5[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	.loc 1 67 4
	jmp	.L12
.L11:
	.loc 1 70 4
	mov	eax, DWORD PTR -4[rbp]
	mov	esi, eax
	lea	rax, .LC6[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	.loc 1 71 4
	nop
.L12:
	.loc 1 74 2
	mov	eax, DWORD PTR -4[rbp]
	mov	edi, eax
	call	exit@PLT
.L13:
	.loc 1 75 1
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	exit_turing, .-exit_turing
	.type	pack_bits, @function
pack_bits:
.LFB10:
	.loc 1 90 68
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	QWORD PTR -24[rbp], rdi
	mov	QWORD PTR -32[rbp], rsi
	mov	DWORD PTR -36[rbp], edx
.LBB2:
	.loc 1 91 14
	mov	DWORD PTR -4[rbp], 0
	.loc 1 91 5
	jmp	.L15
.L17:
	.loc 1 92 19
	mov	eax, DWORD PTR -4[rbp]
	cdqe
	lea	rdx, 0[0+rax*4]
	mov	rax, QWORD PTR -32[rbp]
	add	rax, rdx
	mov	eax, DWORD PTR [rax]
	.loc 1 92 12
	cmp	eax, 1
	jne	.L16
	.loc 1 93 20
	mov	eax, DWORD PTR -4[rbp]
	lea	edx, 7[rax]
	test	eax, eax
	cmovs	eax, edx
	sar	eax, 3
	.loc 1 93 25
	movsx	rcx, eax
	mov	rdx, QWORD PTR -24[rbp]
	add	rdx, rcx
	movzx	edx, BYTE PTR [rdx]
	mov	edi, edx
	.loc 1 93 31
	mov	edx, DWORD PTR -4[rbp]
	and	edx, 7
	mov	esi, 1
	mov	ecx, edx
	sal	esi, cl
	mov	edx, esi
	.loc 1 93 25
	or	edi, edx
	mov	ecx, edi
	movsx	rdx, eax
	mov	rax, QWORD PTR -24[rbp]
	add	rax, rdx
	mov	edx, ecx
	mov	BYTE PTR [rax], dl
.L16:
	.loc 1 91 32 discriminator 2
	add	DWORD PTR -4[rbp], 1
.L15:
	.loc 1 91 23 discriminator 1
	mov	eax, DWORD PTR -4[rbp]
	cmp	eax, DWORD PTR -36[rbp]
	jl	.L17
.LBE2:
	.loc 1 96 1
	nop
	nop
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	pack_bits, .-pack_bits
	.type	unpack_bits, @function
unpack_bits:
.LFB11:
	.loc 1 99 70
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	QWORD PTR -24[rbp], rdi
	mov	QWORD PTR -32[rbp], rsi
	mov	DWORD PTR -36[rbp], edx
.LBB3:
	.loc 1 100 14
	mov	DWORD PTR -4[rbp], 0
	.loc 1 100 5
	jmp	.L19
.L20:
	.loc 1 101 29 discriminator 3
	mov	eax, DWORD PTR -4[rbp]
	lea	edx, 7[rax]
	test	eax, eax
	cmovs	eax, edx
	sar	eax, 3
	movsx	rdx, eax
	.loc 1 101 26 discriminator 3
	mov	rax, QWORD PTR -24[rbp]
	add	rax, rdx
	movzx	eax, BYTE PTR [rax]
	movzx	edx, al
	.loc 1 101 39 discriminator 3
	mov	eax, DWORD PTR -4[rbp]
	and	eax, 7
	.loc 1 101 52 discriminator 3
	mov	ecx, eax
	sar	edx, cl
	mov	eax, edx
	.loc 1 101 60 discriminator 3
	and	eax, 1
	mov	edx, eax
	.loc 1 101 13 discriminator 3
	mov	eax, DWORD PTR -4[rbp]
	cdqe
	lea	rcx, 0[0+rax*4]
	mov	rax, QWORD PTR -32[rbp]
	add	rax, rcx
	.loc 1 101 17 discriminator 3
	mov	DWORD PTR [rax], edx
	.loc 1 100 32 discriminator 3
	add	DWORD PTR -4[rbp], 1
.L19:
	.loc 1 100 23 discriminator 1
	mov	eax, DWORD PTR -4[rbp]
	cmp	eax, DWORD PTR -36[rbp]
	jl	.L20
.LBE3:
	.loc 1 103 1
	nop
	nop
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	unpack_bits, .-unpack_bits
	.section	.rodata
.LC7:
	.string	"Invalid tape or base pointer\n"
.LC8:
	.string	"Allocation failed [DumpTape]"
.LC9:
	.string	"wb"
.LC10:
	.string	"Failed to open output file"
.LC11:
	.string	"Failed to write to file"
	.text
	.globl	DumpTape
	.type	DumpTape, @function
DumpTape:
.LFB12:
	.loc 1 120 79
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 80
	mov	QWORD PTR -72[rbp], rdi
	mov	QWORD PTR -80[rbp], rsi
	.loc 1 120 79
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR -8[rbp], rax
	xor	eax, eax
	.loc 1 121 8
	cmp	QWORD PTR -72[rbp], 0
	je	.L22
	.loc 1 121 23 discriminator 1
	mov	rax, QWORD PTR -72[rbp]
	mov	rax, QWORD PTR 8[rax]
	.loc 1 121 15 discriminator 1
	test	rax, rax
	jne	.L23
.L22:
	.loc 1 122 9
	mov	rax, QWORD PTR stderr[rip]
	mov	rcx, rax
	mov	edx, 29
	mov	esi, 1
	lea	rax, .LC7[rip]
	mov	rdi, rax
	call	fwrite@PLT
	.loc 1 123 9
	mov	edi, 1
	call	exit@PLT
.L23:
	.loc 1 126 21
	mov	rax, QWORD PTR -72[rbp]
	mov	rdx, QWORD PTR [rax]
	.loc 1 126 33
	mov	rax, QWORD PTR -72[rbp]
	mov	rcx, QWORD PTR 8[rax]
	.loc 1 126 27
	mov	rax, rdx
	sub	rax, rcx
	sar	rax, 2
	.loc 1 126 9
	mov	DWORD PTR -56[rbp], eax
	.loc 1 127 22
	mov	rax, QWORD PTR -72[rbp]
	mov	edx, DWORD PTR 28[rax]
	.loc 1 127 41
	mov	rax, QWORD PTR -72[rbp]
	mov	ecx, DWORD PTR 24[rax]
	.loc 1 127 35
	mov	eax, edx
	sub	eax, ecx
	.loc 1 127 9
	add	eax, 1
	mov	DWORD PTR -52[rbp], eax
	.loc 1 128 14
	mov	eax, DWORD PTR -56[rbp]
	mov	DWORD PTR -32[rbp], eax
	.loc 1 128 37
	mov	rax, QWORD PTR -72[rbp]
	mov	eax, DWORD PTR 24[rax]
	.loc 1 128 14
	mov	DWORD PTR -28[rbp], eax
	.loc 1 128 54
	mov	rax, QWORD PTR -72[rbp]
	mov	eax, DWORD PTR 28[rax]
	.loc 1 128 14
	mov	DWORD PTR -24[rbp], eax
	.loc 1 129 9
	mov	rax, QWORD PTR -72[rbp]
	mov	eax, DWORD PTR 16[rax]
	.loc 1 128 14
	mov	DWORD PTR -20[rbp], eax
	.loc 1 129 26
	mov	rax, QWORD PTR -72[rbp]
	mov	eax, DWORD PTR 20[rax]
	.loc 1 128 14
	mov	DWORD PTR -16[rbp], eax
	.loc 1 134 38
	mov	eax, DWORD PTR -52[rbp]
	add	eax, 7
	.loc 1 134 43
	lea	edx, 7[rax]
	test	eax, eax
	cmovs	eax, edx
	sar	eax, 3
	.loc 1 134 23
	cdqe
	mov	esi, 1
	mov	rdi, rax
	call	calloc@PLT
	mov	QWORD PTR -48[rbp], rax
	.loc 1 135 8
	cmp	QWORD PTR -48[rbp], 0
	jne	.L24
	.loc 1 136 9
	lea	rax, .LC8[rip]
	mov	rdi, rax
	call	perror@PLT
	.loc 1 137 9
	mov	edi, 1
	call	exit@PLT
.L24:
	.loc 1 139 28
	mov	rax, QWORD PTR -72[rbp]
	mov	rdx, QWORD PTR 8[rax]
	.loc 1 139 39
	mov	rax, QWORD PTR -72[rbp]
	mov	eax, DWORD PTR 24[rax]
	cdqe
	.loc 1 139 34
	sal	rax, 2
	.loc 1 139 23
	lea	rcx, [rdx+rax]
	.loc 1 139 5
	mov	edx, DWORD PTR -52[rbp]
	mov	rax, QWORD PTR -48[rbp]
	mov	rsi, rcx
	mov	rdi, rax
	call	pack_bits
	.loc 1 141 22
	mov	rax, QWORD PTR -80[rbp]
	lea	rdx, .LC9[rip]
	mov	rsi, rdx
	mov	rdi, rax
	call	fopen@PLT
	mov	QWORD PTR -40[rbp], rax
	.loc 1 142 8
	cmp	QWORD PTR -40[rbp], 0
	jne	.L25
	.loc 1 143 9
	lea	rax, .LC10[rip]
	mov	rdi, rax
	call	perror@PLT
	.loc 1 144 9
	mov	rax, QWORD PTR -48[rbp]
	mov	rdi, rax
	call	free@PLT
	.loc 1 145 9
	mov	edi, 1
	call	exit@PLT
.L25:
	.loc 1 148 9
	mov	rdx, QWORD PTR -40[rbp]
	lea	rax, -32[rbp]
	mov	rcx, rdx
	mov	edx, 1
	mov	esi, 20
	mov	rdi, rax
	call	fwrite@PLT
	.loc 1 148 8
	cmp	rax, 1
	jne	.L26
	.loc 1 149 32 discriminator 1
	mov	eax, DWORD PTR -52[rbp]
	add	eax, 7
	.loc 1 149 37 discriminator 1
	lea	edx, 7[rax]
	test	eax, eax
	cmovs	eax, edx
	sar	eax, 3
	.loc 1 149 9 discriminator 1
	movsx	rsi, eax
	mov	rdx, QWORD PTR -40[rbp]
	mov	rax, QWORD PTR -48[rbp]
	mov	rcx, rdx
	mov	edx, 1
	mov	rdi, rax
	call	fwrite@PLT
	.loc 1 148 63 discriminator 1
	cmp	rax, 1
	je	.L27
.L26:
	.loc 1 150 9
	lea	rax, .LC11[rip]
	mov	rdi, rax
	call	perror@PLT
	.loc 1 151 9
	mov	rax, QWORD PTR -40[rbp]
	mov	rdi, rax
	call	fclose@PLT
	.loc 1 152 9
	mov	rax, QWORD PTR -48[rbp]
	mov	rdi, rax
	call	free@PLT
	.loc 1 153 9
	mov	edi, 1
	call	exit@PLT
.L27:
	.loc 1 156 5
	mov	rax, QWORD PTR -40[rbp]
	mov	rdi, rax
	call	fclose@PLT
	.loc 1 157 5
	mov	rax, QWORD PTR -48[rbp]
	mov	rdi, rax
	call	free@PLT
	.loc 1 158 1
	nop
	mov	rax, QWORD PTR -8[rbp]
	sub	rax, QWORD PTR fs:40
	je	.L28
	call	__stack_chk_fail@PLT
.L28:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	DumpTape, .-DumpTape
	.section	.rodata
.LC12:
	.string	"rb"
.LC13:
	.string	"Failed to open input file\n"
.LC14:
	.string	"Failed to read full metadata\n"
	.align 8
.LC15:
	.string	"Failed to allocate memory for tape data"
	.align 8
.LC16:
	.string	"Failed to read full tape data\n"
.LC17:
	.string	"src/io.c"
	.align 8
.LC18:
	.string	"0 && \"Unreachable code reached\""
	.text
	.globl	ReadTape
	.type	ReadTape, @function
ReadTape:
.LFB13:
	.loc 1 160 66
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 144
	mov	QWORD PTR -136[rbp], rdi
	mov	QWORD PTR -144[rbp], rsi
	.loc 1 160 66
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR -8[rbp], rax
	xor	eax, eax
	.loc 1 162 5
	lea	rax, -48[rbp]
	mov	edx, 32
	mov	esi, 0
	mov	rdi, rax
	call	memset@PLT
	.loc 1 165 21
	mov	rax, QWORD PTR -144[rbp]
	lea	rdx, .LC12[rip]
	mov	rsi, rdx
	mov	rdi, rax
	call	fopen@PLT
	mov	QWORD PTR -120[rbp], rax
	.loc 1 166 8
	cmp	QWORD PTR -120[rbp], 0
	jne	.L30
	.loc 1 167 9
	mov	rax, QWORD PTR stderr[rip]
	mov	rcx, rax
	mov	edx, 26
	mov	esi, 1
	lea	rax, .LC13[rip]
	mov	rdi, rax
	call	fwrite@PLT
	.loc 1 168 9
	mov	edi, 1
	call	exit@PLT
.L30:
	.loc 1 171 9
	mov	rdx, QWORD PTR -120[rbp]
	lea	rax, -80[rbp]
	mov	rcx, rdx
	mov	edx, 1
	mov	esi, 20
	mov	rdi, rax
	call	fread@PLT
	.loc 1 171 8
	cmp	rax, 1
	je	.L31
	.loc 1 172 9
	mov	rax, QWORD PTR stderr[rip]
	mov	rcx, rax
	mov	edx, 29
	mov	esi, 1
	lea	rax, .LC14[rip]
	mov	rdi, rax
	call	fwrite@PLT
	.loc 1 173 9
	mov	rax, QWORD PTR -120[rbp]
	mov	rdi, rax
	call	fclose@PLT
	.loc 1 174 9
	mov	edi, 1
	call	exit@PLT
.L31:
	.loc 1 178 24
	mov	eax, DWORD PTR -72[rbp]
	.loc 1 178 45
	mov	edx, DWORD PTR -76[rbp]
	.loc 1 178 36
	sub	eax, edx
	.loc 1 178 9
	add	eax, 1
	mov	DWORD PTR -128[rbp], eax
	.loc 1 181 38
	mov	eax, DWORD PTR -128[rbp]
	add	eax, 7
	.loc 1 181 43
	lea	edx, 7[rax]
	test	eax, eax
	cmovs	eax, edx
	sar	eax, 3
	.loc 1 181 23
	cdqe
	mov	esi, 1
	mov	rdi, rax
	call	calloc@PLT
	mov	QWORD PTR -112[rbp], rax
	.loc 1 182 8
	cmp	QWORD PTR -112[rbp], 0
	jne	.L32
	.loc 1 183 9
	lea	rax, .LC15[rip]
	mov	rdi, rax
	call	perror@PLT
	.loc 1 184 9
	mov	rax, QWORD PTR -120[rbp]
	mov	rdi, rax
	call	fclose@PLT
	.loc 1 185 9
	mov	edi, 1
	call	exit@PLT
.L32:
	.loc 1 188 31
	mov	eax, DWORD PTR -128[rbp]
	add	eax, 7
	.loc 1 188 36
	lea	edx, 7[rax]
	test	eax, eax
	cmovs	eax, edx
	sar	eax, 3
	.loc 1 188 9
	movsx	rsi, eax
	mov	rdx, QWORD PTR -120[rbp]
	mov	rax, QWORD PTR -112[rbp]
	mov	rcx, rdx
	mov	edx, 1
	mov	rdi, rax
	call	fread@PLT
	.loc 1 188 8
	cmp	rax, 1
	je	.L33
	.loc 1 189 9
	mov	rax, QWORD PTR stderr[rip]
	mov	rcx, rax
	mov	edx, 30
	mov	esi, 1
	lea	rax, .LC16[rip]
	mov	rdi, rax
	call	fwrite@PLT
	.loc 1 190 9
	mov	rax, QWORD PTR -112[rbp]
	mov	rdi, rax
	call	free@PLT
	.loc 1 191 9
	mov	rax, QWORD PTR -120[rbp]
	mov	rdi, rax
	call	fclose@PLT
	.loc 1 192 9
	mov	edi, 1
	call	exit@PLT
.L33:
	.loc 1 195 5
	mov	rax, QWORD PTR -120[rbp]
	mov	rdi, rax
	call	fclose@PLT
	.loc 1 197 35
	mov	eax, DWORD PTR -64[rbp]
	.loc 1 197 56
	mov	edx, DWORD PTR -68[rbp]
	.loc 1 197 47
	sub	eax, edx
	.loc 1 197 9
	add	eax, 1
	mov	DWORD PTR -124[rbp], eax
	.loc 1 199 16
	mov	eax, DWORD PTR -124[rbp]
	cdqe
	sal	rax, 2
	mov	rdi, rax
	call	allocate_all_tape
	mov	QWORD PTR -104[rbp], rax
	.loc 1 200 29
	mov	eax, DWORD PTR -68[rbp]
	cdqe
	.loc 1 200 20
	sal	rax, 2
	neg	rax
	mov	rdx, rax
	.loc 1 200 10
	mov	rax, QWORD PTR -104[rbp]
	add	rax, rdx
	mov	QWORD PTR -96[rbp], rax
	.loc 1 201 34
	mov	eax, DWORD PTR -76[rbp]
	cdqe
	.loc 1 201 25
	lea	rdx, 0[0+rax*4]
	.loc 1 201 10
	mov	rax, QWORD PTR -96[rbp]
	add	rax, rdx
	mov	QWORD PTR -88[rbp], rax
	.loc 1 203 22
	mov	eax, DWORD PTR -72[rbp]
	cdqe
	.loc 1 203 13
	sal	rax, 2
	.loc 1 203 35
	mov	rdx, rax
	.loc 1 203 50
	mov	eax, DWORD PTR -76[rbp]
	cdqe
	.loc 1 203 41
	sal	rax, 2
	.loc 1 203 35
	mov	rcx, rax
	mov	rax, rdx
	sub	rax, rcx
	sar	rax, 2
	.loc 1 203 62
	lea	rdx, 1[rax]
	.loc 1 203 64
	mov	eax, DWORD PTR -128[rbp]
	cdqe
	.loc 1 203 7
	cmp	rdx, rax
	je	.L34
	.loc 1 204 6 discriminator 1
	lea	rax, __PRETTY_FUNCTION__.0[rip]
	mov	rcx, rax
	mov	edx, 204
	lea	rax, .LC17[rip]
	mov	rsi, rax
	lea	rax, .LC18[rip]
	mov	rdi, rax
	call	__assert_fail@PLT
.L34:
	.loc 1 207 5
	mov	eax, DWORD PTR -128[rbp]
	movsx	rdx, eax
	mov	rax, QWORD PTR -88[rbp]
	mov	esi, 0
	mov	rdi, rax
	call	memset@PLT
	.loc 1 208 2
	mov	edx, DWORD PTR -128[rbp]
	mov	rcx, QWORD PTR -88[rbp]
	mov	rax, QWORD PTR -112[rbp]
	mov	rsi, rcx
	mov	rdi, rax
	call	unpack_bits
	.loc 1 220 5
	mov	rax, QWORD PTR -112[rbp]
	mov	rdi, rax
	call	free@PLT
	.loc 1 222 15
	mov	rax, QWORD PTR -96[rbp]
	mov	QWORD PTR -40[rbp], rax
	.loc 1 223 20
	mov	rdx, QWORD PTR -40[rbp]
	.loc 1 223 36
	mov	eax, DWORD PTR -80[rbp]
	cdqe
	.loc 1 223 26
	sal	rax, 2
	add	rax, rdx
	.loc 1 223 14
	mov	QWORD PTR -48[rbp], rax
	.loc 1 224 30
	mov	eax, DWORD PTR -76[rbp]
	.loc 1 224 20
	mov	DWORD PTR -24[rbp], eax
	.loc 1 225 31
	mov	eax, DWORD PTR -72[rbp]
	.loc 1 225 21
	mov	DWORD PTR -20[rbp], eax
	.loc 1 229 31
	mov	eax, DWORD PTR -68[rbp]
	.loc 1 229 21
	mov	DWORD PTR -32[rbp], eax
	.loc 1 230 32
	mov	eax, DWORD PTR -64[rbp]
	.loc 1 230 22
	mov	DWORD PTR -28[rbp], eax
	.loc 1 232 12
	mov	rcx, QWORD PTR -136[rbp]
	mov	rax, QWORD PTR -48[rbp]
	mov	rdx, QWORD PTR -40[rbp]
	mov	QWORD PTR [rcx], rax
	mov	QWORD PTR 8[rcx], rdx
	mov	rax, QWORD PTR -32[rbp]
	mov	rdx, QWORD PTR -24[rbp]
	mov	QWORD PTR 16[rcx], rax
	mov	QWORD PTR 24[rcx], rdx
	.loc 1 233 1
	mov	rax, QWORD PTR -8[rbp]
	sub	rax, QWORD PTR fs:40
	je	.L36
	call	__stack_chk_fail@PLT
.L36:
	mov	rax, QWORD PTR -136[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	ReadTape, .-ReadTape
	.section	.rodata
	.align 8
	.type	__PRETTY_FUNCTION__.0, @object
	.size	__PRETTY_FUNCTION__.0, 9
__PRETTY_FUNCTION__.0:
	.string	"ReadTape"
	.text
.Letext0:
	.file 2 "/usr/lib/gcc/x86_64-linux-gnu/11/include/stddef.h"
	.file 3 "/usr/include/x86_64-linux-gnu/bits/types.h"
	.file 4 "/usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h"
	.file 5 "/usr/include/x86_64-linux-gnu/bits/types/FILE.h"
	.file 6 "include/turing.h"
	.file 7 "include/io.h"
	.file 8 "/usr/include/x86_64-linux-gnu/bits/stdint-uintn.h"
	.file 9 "/usr/include/assert.h"
	.file 10 "/usr/include/stdio.h"
	.file 11 "/usr/include/string.h"
	.file 12 "/usr/include/stdlib.h"
	.file 13 "/usr/include/x86_64-linux-gnu/sys/mman.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x856
	.value	0x5
	.byte	0x1
	.byte	0x8
	.long	.Ldebug_abbrev0
	.uleb128 0x16
	.long	.LASF105
	.byte	0x1d
	.long	.LASF0
	.long	.LASF1
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0x7
	.long	.LASF7
	.byte	0x2
	.byte	0xd1
	.byte	0x17
	.long	0x3a
	.uleb128 0x6
	.byte	0x8
	.byte	0x7
	.long	.LASF2
	.uleb128 0x6
	.byte	0x4
	.byte	0x7
	.long	.LASF3
	.uleb128 0x17
	.byte	0x8
	.uleb128 0xa
	.long	0x48
	.uleb128 0x6
	.byte	0x1
	.byte	0x8
	.long	.LASF4
	.uleb128 0x6
	.byte	0x2
	.byte	0x7
	.long	.LASF5
	.uleb128 0x6
	.byte	0x1
	.byte	0x6
	.long	.LASF6
	.uleb128 0x7
	.long	.LASF8
	.byte	0x3
	.byte	0x26
	.byte	0x17
	.long	0x4f
	.uleb128 0x6
	.byte	0x2
	.byte	0x5
	.long	.LASF9
	.uleb128 0x18
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x6
	.byte	0x8
	.byte	0x5
	.long	.LASF10
	.uleb128 0x7
	.long	.LASF11
	.byte	0x3
	.byte	0x98
	.byte	0x19
	.long	0x7e
	.uleb128 0x7
	.long	.LASF12
	.byte	0x3
	.byte	0x99
	.byte	0x1b
	.long	0x7e
	.uleb128 0x4
	.long	0xa2
	.uleb128 0x6
	.byte	0x1
	.byte	0x6
	.long	.LASF13
	.uleb128 0xb
	.long	0xa2
	.uleb128 0x11
	.long	.LASF53
	.byte	0xd8
	.byte	0x4
	.byte	0x31
	.byte	0x8
	.long	0x235
	.uleb128 0x1
	.long	.LASF14
	.byte	0x4
	.byte	0x33
	.byte	0x7
	.long	0x77
	.byte	0
	.uleb128 0x1
	.long	.LASF15
	.byte	0x4
	.byte	0x36
	.byte	0x9
	.long	0x9d
	.byte	0x8
	.uleb128 0x1
	.long	.LASF16
	.byte	0x4
	.byte	0x37
	.byte	0x9
	.long	0x9d
	.byte	0x10
	.uleb128 0x1
	.long	.LASF17
	.byte	0x4
	.byte	0x38
	.byte	0x9
	.long	0x9d
	.byte	0x18
	.uleb128 0x1
	.long	.LASF18
	.byte	0x4
	.byte	0x39
	.byte	0x9
	.long	0x9d
	.byte	0x20
	.uleb128 0x1
	.long	.LASF19
	.byte	0x4
	.byte	0x3a
	.byte	0x9
	.long	0x9d
	.byte	0x28
	.uleb128 0x1
	.long	.LASF20
	.byte	0x4
	.byte	0x3b
	.byte	0x9
	.long	0x9d
	.byte	0x30
	.uleb128 0x1
	.long	.LASF21
	.byte	0x4
	.byte	0x3c
	.byte	0x9
	.long	0x9d
	.byte	0x38
	.uleb128 0x1
	.long	.LASF22
	.byte	0x4
	.byte	0x3d
	.byte	0x9
	.long	0x9d
	.byte	0x40
	.uleb128 0x1
	.long	.LASF23
	.byte	0x4
	.byte	0x40
	.byte	0x9
	.long	0x9d
	.byte	0x48
	.uleb128 0x1
	.long	.LASF24
	.byte	0x4
	.byte	0x41
	.byte	0x9
	.long	0x9d
	.byte	0x50
	.uleb128 0x1
	.long	.LASF25
	.byte	0x4
	.byte	0x42
	.byte	0x9
	.long	0x9d
	.byte	0x58
	.uleb128 0x1
	.long	.LASF26
	.byte	0x4
	.byte	0x44
	.byte	0x16
	.long	0x24e
	.byte	0x60
	.uleb128 0x1
	.long	.LASF27
	.byte	0x4
	.byte	0x46
	.byte	0x14
	.long	0x253
	.byte	0x68
	.uleb128 0x1
	.long	.LASF28
	.byte	0x4
	.byte	0x48
	.byte	0x7
	.long	0x77
	.byte	0x70
	.uleb128 0x1
	.long	.LASF29
	.byte	0x4
	.byte	0x49
	.byte	0x7
	.long	0x77
	.byte	0x74
	.uleb128 0x1
	.long	.LASF30
	.byte	0x4
	.byte	0x4a
	.byte	0xb
	.long	0x85
	.byte	0x78
	.uleb128 0x1
	.long	.LASF31
	.byte	0x4
	.byte	0x4d
	.byte	0x12
	.long	0x56
	.byte	0x80
	.uleb128 0x1
	.long	.LASF32
	.byte	0x4
	.byte	0x4e
	.byte	0xf
	.long	0x5d
	.byte	0x82
	.uleb128 0x1
	.long	.LASF33
	.byte	0x4
	.byte	0x4f
	.byte	0x8
	.long	0x258
	.byte	0x83
	.uleb128 0x1
	.long	.LASF34
	.byte	0x4
	.byte	0x51
	.byte	0xf
	.long	0x268
	.byte	0x88
	.uleb128 0x1
	.long	.LASF35
	.byte	0x4
	.byte	0x59
	.byte	0xd
	.long	0x91
	.byte	0x90
	.uleb128 0x1
	.long	.LASF36
	.byte	0x4
	.byte	0x5b
	.byte	0x17
	.long	0x272
	.byte	0x98
	.uleb128 0x1
	.long	.LASF37
	.byte	0x4
	.byte	0x5c
	.byte	0x19
	.long	0x27c
	.byte	0xa0
	.uleb128 0x1
	.long	.LASF38
	.byte	0x4
	.byte	0x5d
	.byte	0x14
	.long	0x253
	.byte	0xa8
	.uleb128 0x1
	.long	.LASF39
	.byte	0x4
	.byte	0x5e
	.byte	0x9
	.long	0x48
	.byte	0xb0
	.uleb128 0x1
	.long	.LASF40
	.byte	0x4
	.byte	0x5f
	.byte	0xa
	.long	0x2e
	.byte	0xb8
	.uleb128 0x1
	.long	.LASF41
	.byte	0x4
	.byte	0x60
	.byte	0x7
	.long	0x77
	.byte	0xc0
	.uleb128 0x1
	.long	.LASF42
	.byte	0x4
	.byte	0x62
	.byte	0x8
	.long	0x281
	.byte	0xc4
	.byte	0
	.uleb128 0x7
	.long	.LASF43
	.byte	0x5
	.byte	0x7
	.byte	0x19
	.long	0xae
	.uleb128 0x19
	.long	.LASF106
	.byte	0x4
	.byte	0x2b
	.byte	0xe
	.uleb128 0xd
	.long	.LASF44
	.uleb128 0x4
	.long	0x249
	.uleb128 0x4
	.long	0xae
	.uleb128 0xe
	.long	0xa2
	.long	0x268
	.uleb128 0xf
	.long	0x3a
	.byte	0
	.byte	0
	.uleb128 0x4
	.long	0x241
	.uleb128 0xd
	.long	.LASF45
	.uleb128 0x4
	.long	0x26d
	.uleb128 0xd
	.long	.LASF46
	.uleb128 0x4
	.long	0x277
	.uleb128 0xe
	.long	0xa2
	.long	0x291
	.uleb128 0xf
	.long	0x3a
	.byte	0x13
	.byte	0
	.uleb128 0x4
	.long	0x235
	.uleb128 0xa
	.long	0x291
	.uleb128 0x1a
	.long	.LASF107
	.byte	0xa
	.byte	0x91
	.byte	0xe
	.long	0x291
	.uleb128 0x1b
	.long	.LASF50
	.byte	0x7
	.byte	0x4
	.long	0x41
	.byte	0x6
	.byte	0x6
	.byte	0xe
	.long	0x2cc
	.uleb128 0x8
	.long	.LASF47
	.byte	0
	.uleb128 0x8
	.long	.LASF48
	.byte	0x1
	.uleb128 0x8
	.long	.LASF49
	.byte	0x2
	.byte	0
	.uleb128 0x7
	.long	.LASF50
	.byte	0x6
	.byte	0xa
	.byte	0x2
	.long	0x2a7
	.uleb128 0x1c
	.string	"Bit"
	.byte	0x7
	.byte	0x4
	.long	0x41
	.byte	0x6
	.byte	0xc
	.byte	0xf
	.long	0x2f7
	.uleb128 0x8
	.long	.LASF51
	.byte	0
	.uleb128 0x8
	.long	.LASF52
	.byte	0x1
	.byte	0
	.uleb128 0x1d
	.string	"Bit"
	.byte	0x6
	.byte	0xf
	.byte	0x3
	.long	0x2d8
	.uleb128 0xb
	.long	0x2f7
	.uleb128 0x11
	.long	.LASF54
	.byte	0x20
	.byte	0x6
	.byte	0x29
	.byte	0x10
	.long	0x364
	.uleb128 0x1e
	.string	"cur"
	.byte	0x6
	.byte	0x2a
	.byte	0x7
	.long	0x364
	.byte	0
	.uleb128 0x1
	.long	.LASF55
	.byte	0x6
	.byte	0x2b
	.byte	0x7
	.long	0x364
	.byte	0x8
	.uleb128 0x1
	.long	.LASF56
	.byte	0x6
	.byte	0x2e
	.byte	0x6
	.long	0x77
	.byte	0x10
	.uleb128 0x1
	.long	.LASF57
	.byte	0x6
	.byte	0x2f
	.byte	0x6
	.long	0x77
	.byte	0x14
	.uleb128 0x1
	.long	.LASF58
	.byte	0x6
	.byte	0x31
	.byte	0x6
	.long	0x77
	.byte	0x18
	.uleb128 0x1
	.long	.LASF59
	.byte	0x6
	.byte	0x32
	.byte	0x6
	.long	0x77
	.byte	0x1c
	.byte	0
	.uleb128 0x4
	.long	0x2f7
	.uleb128 0x7
	.long	.LASF54
	.byte	0x6
	.byte	0x33
	.byte	0x3
	.long	0x308
	.uleb128 0x6
	.byte	0x8
	.byte	0x5
	.long	.LASF60
	.uleb128 0x6
	.byte	0x10
	.byte	0x4
	.long	.LASF61
	.uleb128 0x1f
	.byte	0x14
	.byte	0x7
	.byte	0xc
	.byte	0x9
	.long	0x3ce
	.uleb128 0x1
	.long	.LASF62
	.byte	0x7
	.byte	0xd
	.byte	0x9
	.long	0x77
	.byte	0
	.uleb128 0x1
	.long	.LASF63
	.byte	0x7
	.byte	0xe
	.byte	0x9
	.long	0x77
	.byte	0x4
	.uleb128 0x1
	.long	.LASF64
	.byte	0x7
	.byte	0xf
	.byte	0x9
	.long	0x77
	.byte	0x8
	.uleb128 0x1
	.long	.LASF56
	.byte	0x7
	.byte	0x12
	.byte	0x9
	.long	0x77
	.byte	0xc
	.uleb128 0x1
	.long	.LASF57
	.byte	0x7
	.byte	0x13
	.byte	0x9
	.long	0x77
	.byte	0x10
	.byte	0
	.uleb128 0x7
	.long	.LASF65
	.byte	0x7
	.byte	0x14
	.byte	0x3
	.long	0x383
	.uleb128 0x6
	.byte	0x8
	.byte	0x7
	.long	.LASF66
	.uleb128 0x4
	.long	0x3eb
	.uleb128 0xa
	.long	0x3e1
	.uleb128 0x20
	.uleb128 0x7
	.long	.LASF67
	.byte	0x8
	.byte	0x18
	.byte	0x13
	.long	0x64
	.uleb128 0xb
	.long	0x3ec
	.uleb128 0x4
	.long	0xa9
	.uleb128 0xa
	.long	0x3fd
	.uleb128 0x21
	.long	.LASF68
	.byte	0x9
	.byte	0x45
	.byte	0xd
	.long	0x428
	.uleb128 0x2
	.long	0x3fd
	.uleb128 0x2
	.long	0x3fd
	.uleb128 0x2
	.long	0x41
	.uleb128 0x2
	.long	0x3fd
	.byte	0
	.uleb128 0x9
	.long	.LASF69
	.byte	0xa
	.value	0x2a3
	.byte	0xf
	.long	0x2e
	.long	0x44e
	.uleb128 0x2
	.long	0x4a
	.uleb128 0x2
	.long	0x2e
	.uleb128 0x2
	.long	0x2e
	.uleb128 0x2
	.long	0x296
	.byte	0
	.uleb128 0xc
	.long	.LASF70
	.byte	0xb
	.byte	0x3d
	.byte	0xe
	.long	0x48
	.long	0x46e
	.uleb128 0x2
	.long	0x48
	.uleb128 0x2
	.long	0x77
	.uleb128 0x2
	.long	0x2e
	.byte	0
	.uleb128 0xc
	.long	.LASF71
	.byte	0xa
	.byte	0xb2
	.byte	0xc
	.long	0x77
	.long	0x484
	.uleb128 0x2
	.long	0x291
	.byte	0
	.uleb128 0x9
	.long	.LASF72
	.byte	0xa
	.value	0x2a9
	.byte	0xf
	.long	0x2e
	.long	0x4aa
	.uleb128 0x2
	.long	0x3e6
	.uleb128 0x2
	.long	0x2e
	.uleb128 0x2
	.long	0x2e
	.uleb128 0x2
	.long	0x296
	.byte	0
	.uleb128 0x12
	.long	.LASF78
	.byte	0xc
	.value	0x22b
	.long	0x4bc
	.uleb128 0x2
	.long	0x48
	.byte	0
	.uleb128 0x9
	.long	.LASF73
	.byte	0xa
	.value	0x102
	.byte	0xe
	.long	0x291
	.long	0x4d8
	.uleb128 0x2
	.long	0x402
	.uleb128 0x2
	.long	0x402
	.byte	0
	.uleb128 0x9
	.long	.LASF74
	.byte	0xc
	.value	0x21f
	.byte	0xe
	.long	0x48
	.long	0x4f4
	.uleb128 0x2
	.long	0x2e
	.uleb128 0x2
	.long	0x2e
	.byte	0
	.uleb128 0xc
	.long	.LASF75
	.byte	0xd
	.byte	0x4c
	.byte	0xc
	.long	0x77
	.long	0x50f
	.uleb128 0x2
	.long	0x48
	.uleb128 0x2
	.long	0x2e
	.byte	0
	.uleb128 0x22
	.long	.LASF76
	.byte	0xc
	.value	0x270
	.byte	0xd
	.long	0x522
	.uleb128 0x2
	.long	0x77
	.byte	0
	.uleb128 0x9
	.long	.LASF77
	.byte	0xa
	.value	0x164
	.byte	0xc
	.long	0x77
	.long	0x53a
	.uleb128 0x2
	.long	0x3fd
	.uleb128 0x23
	.byte	0
	.uleb128 0x12
	.long	.LASF79
	.byte	0xa
	.value	0x324
	.long	0x54c
	.uleb128 0x2
	.long	0x3fd
	.byte	0
	.uleb128 0xc
	.long	.LASF80
	.byte	0xd
	.byte	0x39
	.byte	0xe
	.long	0x48
	.long	0x57b
	.uleb128 0x2
	.long	0x48
	.uleb128 0x2
	.long	0x2e
	.uleb128 0x2
	.long	0x77
	.uleb128 0x2
	.long	0x77
	.uleb128 0x2
	.long	0x77
	.uleb128 0x2
	.long	0x85
	.byte	0
	.uleb128 0x24
	.long	.LASF108
	.byte	0x1
	.byte	0xa0
	.byte	0x20
	.long	0x369
	.quad	.LFB13
	.quad	.LFE13-.LFB13
	.uleb128 0x1
	.byte	0x9c
	.long	0x646
	.uleb128 0x5
	.long	.LASF89
	.byte	0xa0
	.byte	0x35
	.long	0x3fd
	.uleb128 0x3
	.byte	0x91
	.sleb128 -160
	.uleb128 0x3
	.long	.LASF81
	.byte	0xa1
	.byte	0xa
	.long	0x369
	.uleb128 0x2
	.byte	0x91
	.sleb128 -64
	.uleb128 0x3
	.long	.LASF82
	.byte	0xa3
	.byte	0xe
	.long	0x3ce
	.uleb128 0x3
	.byte	0x91
	.sleb128 -96
	.uleb128 0x3
	.long	.LASF83
	.byte	0xa5
	.byte	0xb
	.long	0x291
	.uleb128 0x3
	.byte	0x91
	.sleb128 -136
	.uleb128 0x3
	.long	.LASF84
	.byte	0xb2
	.byte	0x9
	.long	0x77
	.uleb128 0x3
	.byte	0x91
	.sleb128 -144
	.uleb128 0x3
	.long	.LASF85
	.byte	0xb5
	.byte	0xe
	.long	0x646
	.uleb128 0x3
	.byte	0x91
	.sleb128 -128
	.uleb128 0x3
	.long	.LASF86
	.byte	0xc5
	.byte	0x9
	.long	0x77
	.uleb128 0x3
	.byte	0x91
	.sleb128 -140
	.uleb128 0x3
	.long	.LASF87
	.byte	0xc7
	.byte	0xa
	.long	0x364
	.uleb128 0x3
	.byte	0x91
	.sleb128 -120
	.uleb128 0x3
	.long	.LASF55
	.byte	0xc8
	.byte	0xa
	.long	0x364
	.uleb128 0x3
	.byte	0x91
	.sleb128 -112
	.uleb128 0x3
	.long	.LASF88
	.byte	0xc9
	.byte	0xa
	.long	0x364
	.uleb128 0x3
	.byte	0x91
	.sleb128 -104
	.uleb128 0x25
	.long	.LASF109
	.long	0x65b
	.uleb128 0x9
	.byte	0x3
	.quad	__PRETTY_FUNCTION__.0
	.byte	0
	.uleb128 0x4
	.long	0x3ec
	.uleb128 0xe
	.long	0xa9
	.long	0x65b
	.uleb128 0xf
	.long	0x3a
	.byte	0x8
	.byte	0
	.uleb128 0xb
	.long	0x64b
	.uleb128 0x10
	.long	.LASF98
	.byte	0x78
	.quad	.LFB12
	.quad	.LFE12-.LFB12
	.uleb128 0x1
	.byte	0x9c
	.long	0x6e3
	.uleb128 0x5
	.long	.LASF81
	.byte	0x78
	.byte	0x2f
	.long	0x6e3
	.uleb128 0x3
	.byte	0x91
	.sleb128 -88
	.uleb128 0x5
	.long	.LASF90
	.byte	0x78
	.byte	0x41
	.long	0x3fd
	.uleb128 0x3
	.byte	0x91
	.sleb128 -96
	.uleb128 0x3
	.long	.LASF91
	.byte	0x7e
	.byte	0x9
	.long	0x77
	.uleb128 0x3
	.byte	0x91
	.sleb128 -72
	.uleb128 0x3
	.long	.LASF84
	.byte	0x7f
	.byte	0x9
	.long	0x77
	.uleb128 0x3
	.byte	0x91
	.sleb128 -68
	.uleb128 0x3
	.long	.LASF82
	.byte	0x80
	.byte	0xe
	.long	0x3ce
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x3
	.long	.LASF85
	.byte	0x86
	.byte	0xe
	.long	0x646
	.uleb128 0x2
	.byte	0x91
	.sleb128 -64
	.uleb128 0x3
	.long	.LASF92
	.byte	0x8d
	.byte	0xb
	.long	0x291
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.byte	0
	.uleb128 0x4
	.long	0x369
	.uleb128 0x13
	.long	.LASF96
	.byte	0x63
	.quad	.LFB11
	.quad	.LFE11-.LFB11
	.uleb128 0x1
	.byte	0x9c
	.long	0x74c
	.uleb128 0x5
	.long	.LASF93
	.byte	0x63
	.byte	0x28
	.long	0x74c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x5
	.long	.LASF94
	.byte	0x63
	.byte	0x35
	.long	0x364
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x5
	.long	.LASF95
	.byte	0x63
	.byte	0x3f
	.long	0x77
	.uleb128 0x2
	.byte	0x91
	.sleb128 -52
	.uleb128 0x14
	.quad	.LBB3
	.quad	.LBE3-.LBB3
	.uleb128 0x15
	.string	"i"
	.byte	0x64
	.long	0x77
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.byte	0
	.uleb128 0x4
	.long	0x3f8
	.uleb128 0x13
	.long	.LASF97
	.byte	0x5a
	.quad	.LFB10
	.quad	.LFE10-.LFB10
	.uleb128 0x1
	.byte	0x9c
	.long	0x7b5
	.uleb128 0x5
	.long	.LASF94
	.byte	0x5a
	.byte	0x20
	.long	0x646
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x5
	.long	.LASF93
	.byte	0x5a
	.byte	0x31
	.long	0x7b5
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x5
	.long	.LASF95
	.byte	0x5a
	.byte	0x3d
	.long	0x77
	.uleb128 0x2
	.byte	0x91
	.sleb128 -52
	.uleb128 0x14
	.quad	.LBB2
	.quad	.LBE2-.LBB2
	.uleb128 0x15
	.string	"i"
	.byte	0x5b
	.long	0x77
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.byte	0
	.uleb128 0x4
	.long	0x303
	.uleb128 0x10
	.long	.LASF99
	.byte	0x36
	.quad	.LFB9
	.quad	.LFE9-.LFB9
	.uleb128 0x1
	.byte	0x9c
	.long	0x7e5
	.uleb128 0x5
	.long	.LASF100
	.byte	0x36
	.byte	0x37
	.long	0x2cc
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0x10
	.long	.LASF101
	.byte	0x25
	.quad	.LFB8
	.quad	.LFE8-.LFB8
	.uleb128 0x1
	.byte	0x9c
	.long	0x81e
	.uleb128 0x5
	.long	.LASF102
	.byte	0x25
	.byte	0x34
	.long	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x5
	.long	.LASF103
	.byte	0x25
	.byte	0x43
	.long	0x2e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0x26
	.long	.LASF104
	.byte	0x1
	.byte	0x11
	.byte	0x21
	.long	0x48
	.quad	.LFB7
	.quad	.LFE7-.LFB7
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x5
	.long	.LASF103
	.byte	0x11
	.byte	0x3a
	.long	0x2e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x3
	.long	.LASF102
	.byte	0x18
	.byte	0xc
	.long	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0x21
	.sleb128 8
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x28
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x37
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x13
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 32
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 13
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 13
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7a
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 14
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x1f
	.uleb128 0x1b
	.uleb128 0x1f
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0x4
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0x4
	.byte	0x1
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1d
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1e
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x1f
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x20
	.uleb128 0x26
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x21
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x87
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x22
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x87
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x23
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x24
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x25
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0x19
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x26
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7c
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0
	.value	0
	.value	0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	0
	.quad	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF29:
	.string	"_flags2"
.LASF73:
	.string	"fopen"
.LASF37:
	.string	"_wide_data"
.LASF58:
	.string	"left_init"
.LASF59:
	.string	"right_init"
.LASF53:
	.string	"_IO_FILE"
.LASF88:
	.string	"tape_data"
.LASF89:
	.string	"in_filename"
.LASF25:
	.string	"_IO_save_end"
.LASF9:
	.string	"short int"
.LASF7:
	.string	"size_t"
.LASF35:
	.string	"_offset"
.LASF109:
	.string	"__PRETTY_FUNCTION__"
.LASF19:
	.string	"_IO_write_ptr"
.LASF14:
	.string	"_flags"
.LASF34:
	.string	"_lock"
.LASF26:
	.string	"_markers"
.LASF16:
	.string	"_IO_read_end"
.LASF39:
	.string	"_freeres_buf"
.LASF91:
	.string	"index"
.LASF62:
	.string	"cur_index"
.LASF105:
	.string	"GNU C17 11.4.0 -masm=intel -mtune=generic -march=x86-64 -g2 -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection -fcf-protection"
.LASF84:
	.string	"length"
.LASF102:
	.string	"memory"
.LASF81:
	.string	"tape"
.LASF107:
	.string	"stderr"
.LASF60:
	.string	"long long int"
.LASF92:
	.string	"out_file"
.LASF83:
	.string	"in_file"
.LASF70:
	.string	"memset"
.LASF10:
	.string	"long int"
.LASF77:
	.string	"printf"
.LASF86:
	.string	"total_tape_length"
.LASF31:
	.string	"_cur_column"
.LASF65:
	.string	"MetaData"
.LASF8:
	.string	"__uint8_t"
.LASF79:
	.string	"perror"
.LASF93:
	.string	"source"
.LASF94:
	.string	"dest"
.LASF76:
	.string	"exit"
.LASF30:
	.string	"_old_offset"
.LASF61:
	.string	"long double"
.LASF4:
	.string	"unsigned char"
.LASF57:
	.string	"right_limit"
.LASF54:
	.string	"Tape"
.LASF6:
	.string	"signed char"
.LASF36:
	.string	"_codecvt"
.LASF66:
	.string	"long long unsigned int"
.LASF52:
	.string	"Bit_1"
.LASF56:
	.string	"left_limit"
.LASF3:
	.string	"unsigned int"
.LASF44:
	.string	"_IO_marker"
.LASF33:
	.string	"_shortbuf"
.LASF100:
	.string	"code"
.LASF18:
	.string	"_IO_write_base"
.LASF42:
	.string	"_unused2"
.LASF15:
	.string	"_IO_read_ptr"
.LASF87:
	.string	"start"
.LASF22:
	.string	"_IO_buf_end"
.LASF101:
	.string	"free_all_tape"
.LASF13:
	.string	"char"
.LASF80:
	.string	"mmap"
.LASF104:
	.string	"allocate_all_tape"
.LASF99:
	.string	"exit_turing"
.LASF38:
	.string	"_freeres_list"
.LASF74:
	.string	"calloc"
.LASF71:
	.string	"fclose"
.LASF40:
	.string	"__pad5"
.LASF82:
	.string	"metadata"
.LASF85:
	.string	"buffer"
.LASF64:
	.string	"right_index"
.LASF50:
	.string	"TuringDone"
.LASF72:
	.string	"fwrite"
.LASF2:
	.string	"long unsigned int"
.LASF90:
	.string	"out_filename"
.LASF20:
	.string	"_IO_write_end"
.LASF63:
	.string	"left_index"
.LASF12:
	.string	"__off64_t"
.LASF28:
	.string	"_fileno"
.LASF5:
	.string	"short unsigned int"
.LASF27:
	.string	"_chain"
.LASF103:
	.string	"size"
.LASF46:
	.string	"_IO_wide_data"
.LASF95:
	.string	"count"
.LASF41:
	.string	"_mode"
.LASF48:
	.string	"TIME_OUT"
.LASF11:
	.string	"__off_t"
.LASF24:
	.string	"_IO_backup_base"
.LASF75:
	.string	"munmap"
.LASF51:
	.string	"Bit_0"
.LASF21:
	.string	"_IO_buf_base"
.LASF67:
	.string	"uint8_t"
.LASF96:
	.string	"unpack_bits"
.LASF45:
	.string	"_IO_codecvt"
.LASF17:
	.string	"_IO_read_base"
.LASF32:
	.string	"_vtable_offset"
.LASF98:
	.string	"DumpTape"
.LASF108:
	.string	"ReadTape"
.LASF49:
	.string	"OUT_OF_TAPE"
.LASF23:
	.string	"_IO_save_base"
.LASF43:
	.string	"FILE"
.LASF47:
	.string	"HAULT"
.LASF68:
	.string	"__assert_fail"
.LASF55:
	.string	"base"
.LASF97:
	.string	"pack_bits"
.LASF78:
	.string	"free"
.LASF69:
	.string	"fread"
.LASF106:
	.string	"_IO_lock_t"
	.section	.debug_line_str,"MS",@progbits,1
.LASF0:
	.string	"src/io.c"
.LASF1:
	.string	"/home/user/Desktop/c_stuff/Turing-compiler"
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
