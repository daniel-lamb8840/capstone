#ifndef __CS_H__
#define __CS_H__

/* Capstone Disassembler Engine */
/* By Nguyen Anh Quynh <aquynh@gmail.com>, 2013> */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

// Capstone API version
#define CS_API_MAJOR 1
#define CS_API_MINOR 0

// Handle using with all API
typedef size_t csh;

// Architecture type
typedef enum cs_arch {
	CS_ARCH_ARM = 0,	// ARM architecture (including Thumb, Thumb-2)
	CS_ARCH_ARM64,		// ARM-64, also called AArch64
	CS_ARCH_MIPS,		// Mips architecture
	CS_ARCH_X86,		// X86 architecture (including x86 & x86-64)
	CS_ARCH_MAX,
} cs_arch;

// Mode type
typedef enum cs_mode {
	CS_MODE_LITTLE_ENDIAN = 0,	// little endian mode (default mode)
	CS_MODE_ARM = 0,	// 32-bit ARM
	CS_MODE_16 = 1 << 1,	// 16-bit mode
	CS_MODE_32 = 1 << 2,	// 32-bit mode
	CS_MODE_64 = 1 << 3,	// 64-bit mode
	CS_MODE_THUMB = 1 << 4,	// ARM's Thumb mode, including Thumb-2
	CS_MODE_MICRO = 1 << 4, // MicroMips mode (MIPS architecture)
	CS_MODE_N64 = 1 << 5, // Nintendo-64 mode (MIPS architecture)

	CS_MODE_BIG_ENDIAN = 1 << 31	// big endian mode
} cs_mode;

typedef void* (*cs_malloc_t)(size_t size);
typedef void* (*cs_calloc_t)(size_t nmemb, size_t size);
typedef void* (*cs_realloc_t)(void *ptr, size_t size);
typedef void (*cs_free_t)(void *ptr);
typedef int (*cs_vsnprintf_t)(char *str, size_t size, const char *format, va_list ap);


// User-defined dynamic memory related functions: malloc/calloc/realloc/free/vsnprintf()
// By default, Capstone uses system's malloc(), calloc(), realloc(), free() & vsnprintf().
typedef struct cs_opt_mem {
	cs_malloc_t malloc;
	cs_calloc_t calloc;
	cs_realloc_t realloc;
	cs_free_t free;
	cs_vsnprintf_t vsnprintf;
} cs_opt_mem;

// Runtime option for the disassembled engine
typedef enum cs_opt_type {
	CS_OPT_SYNTAX = 1,	// Asssembly output syntax
	CS_OPT_DETAIL,	// Break down instruction structure into details
<<<<<<< HEAD
=======
	CS_OPT_MODE,	// Change engine's mode at run-time
	CS_OPT_MEM,	// User-defined dynamic memory related functions
>>>>>>> upstream/master
} cs_opt_type;

// Runtime option value (associated with option type above)
typedef enum cs_opt_value {
<<<<<<< HEAD
	CS_OPT_OFF = 0,  // Turn OFF an option (CS_OPT_DETAIL)
	CS_OPT_SYNTAX_INTEL = 1, // X86 Intel asm syntax - default syntax on X86 (CS_OPT_SYNTAX).
	CS_OPT_SYNTAX_ATT,   // X86 ATT asm syntax (CS_OPT_SYNTAX)
	CS_OPT_ON, // Turn ON an option - default option for CS_OPT_DETAIL
=======
	CS_OPT_OFF = 0,  // Turn OFF an option - default option for CS_OPT_DETAIL.
	CS_OPT_ON = 3, // Turn ON an option (CS_OPT_DETAIL).
	CS_OPT_SYNTAX_DEFAULT = 0, // Default asm syntax (CS_OPT_SYNTAX).
	CS_OPT_SYNTAX_INTEL, // X86 Intel asm syntax - default on X86 (CS_OPT_SYNTAX).
	CS_OPT_SYNTAX_ATT,   // X86 ATT asm syntax (CS_OPT_SYNTAX).
	CS_OPT_SYNTAX_NOREGNAME, // PPC asm syntax: Prints register name with only number (CS_OPT_SYNTAX)
>>>>>>> upstream/master
} cs_opt_value;


#include "arm.h"
#include "arm64.h"
#include "mips.h"
#include "x86.h"

// Detail information of disassembled instruction
typedef struct cs_insn {
	// Instruction ID
	// Find the instruction id from header file of corresponding architecture,
	// such as arm.h for ARM, x86.h for X86, etc...
	// This information is available even when CS_OPT_DETAIL = CS_OPT_OFF
	unsigned int id;

	// Address (EIP) of this instruction
	// This information is available even when CS_OPT_DETAIL = CS_OPT_OFF
	uint64_t address;

	// Size of this instruction
	// This information is available even when CS_OPT_DETAIL = CS_OPT_OFF
	uint16_t size;
	// Machine bytes of this instruction, with number of bytes indicated by @size above
	// This information is available even when CS_OPT_DETAIL = CS_OPT_OFF
	uint8_t bytes[16];

	// Ascii text of instruction mnemonic
	// This information is available even when CS_OPT_DETAIL = CS_OPT_OFF
	char mnemonic[32];

	// Ascii text of instruction operands
	// This information is available even when CS_OPT_DETAIL = CS_OPT_OFF
	char op_str[160];

	// NOTE: All information below is not available when CS_OPT_DETAIL = CS_OPT_OFF

	unsigned int regs_read[32]; // list of implicit registers read by this insn
	unsigned int regs_read_count; // number of implicit registers read by this insn

	unsigned int regs_write[32]; // list of implicit registers modified by this insn
	unsigned int regs_write_count; // number of implicit registers modified by this insn

	unsigned int groups[8]; // list of group this instruction belong to
	unsigned int groups_count; // number of groups this insn belongs to

	// Architecture-specific instruction info
	union {
		cs_x86 x86;	// X86 architecture, including 16-bit, 32-bit & 64-bit mode
		cs_arm64 arm64;	// ARM64 architecture (aka AArch64)
		cs_arm arm;		// ARM architecture (including Thumb/Thumb2)
		cs_mips mips;	// MIPS architecture
	};
} cs_insn;


// Calculate the offset of a disassembled instruction in its buffer, given its position
// in its array of disassembled insn
// NOTE: this macro works with position (>=1), not index
#define CS_INSN_OFFSET(insns, post) (insns[post - 1].address - insns[0].address)


// All type of errors encountered by Capstone API.
// These are values returned by cs_errno()
typedef enum cs_err {
	CS_ERR_OK = 0,	// No error: everything was fine
	CS_ERR_MEM,		// Out-Of-Memory error: cs_open(), cs_disasm_dyn()
	CS_ERR_ARCH,	// Unsupported architecture: cs_open()
	CS_ERR_HANDLE,	// Invalid handle: cs_op_count(), cs_op_index()
	CS_ERR_CSH,	    // Invalid csh argument: cs_close(), cs_errno(), cs_option()
	CS_ERR_MODE,	// Invalid/unsupported mode: cs_open()
	CS_ERR_OPTION,	// Invalid/unsupported option: cs_option()
<<<<<<< HEAD
=======
	CS_ERR_DETAIL,	// Information is unavailable because detail option is OFF
	CS_ERR_MEMSETUP, // Dynamic memory management uninitialized (see CS_OPT_MEM)
>>>>>>> upstream/master
} cs_err;


/*
 Return API version in major and minor numbers.

 @major: major number of API version (for ex: 1)
 @minor: minor number of API version (for ex: 0)

 For example, first API version would return 1 in @major, and 0 in @minor
*/
<<<<<<< HEAD
void cs_version(int *major, int *minor);
=======
bool cs_support(int arch);
>>>>>>> upstream/master

/*
 Initialize CS handle: this must be done before any usage of CS.

 @arch: architecture type (CS_ARCH_*)
 @mode: hardware mode. This is combined of CS_MODE_*
 @handle: pointer to handle, which will be updated at return time

 @return CS_ERR_OK on success, or other value on failure (refer to cs_err enum
 for detailed error).
*/
cs_err cs_open(cs_arch arch, cs_mode mode, csh *handle);

/*
 Close CS handle: MUST do to release the handle when it is not used anymore.

 @handle: handle returned by cs_open()

 @return CS_ERR_OK on success, or other value on failure (refer to cs_err enum
 for detailed error).
*/
cs_err cs_close(csh handle);

/*
 Set option for disassembling engine at runtime

 @handle: handle returned by cs_open()
 @type: type of option to be set
 @value: option value corresponding with @type

 @return CS_ERR_OK on success, or other value on failure.
 Refer to cs_err enum for detailed error.

 NOTE: in the case of CS_OPT_MEM, handle's value can be anything,
 so that cs_option(handle, CS_OPT_MEM, value) can (i.e must) be called
 even before cs_open()
*/
cs_err cs_option(csh handle, cs_opt_type type, size_t value);

/*
 Report the last error number when some API function fail.
 Like glibc's errno, cs_errno might not retain its old value once accessed.

 @handle: handle returned by cs_open()

 @return: error code of cs_err enum type (CS_ERR_*, see above)
*/
cs_err cs_errno(csh handle);

/*
 Disasm the binary code in @buffer.
 Disassembled instructions will be put into @insn
 NOTE: this API requires the pre-allocated buffer in @insn

 @handle: handle returned by cs_open()
 @code: buffer containing raw binary code to be disassembled
 @code_size: size of above code
 @address: address of the first insn in given raw code buffer
 @insn: array of insn filled in by this function
       NOTE: @insn size must be at least @count to avoid buffer overflow
 @count: number of instrutions to be disassembled, or 0 to get all of them
 @return: the number of succesfully disassembled instructions,
 or 0 if this function failed to disassemble the given code

 On failure, call cs_errno() for error code.
*/
size_t cs_disasm(csh handle,
		const uint8_t *code, size_t code_size,
		uint64_t address,
		size_t count,
		cs_insn *insn);

/*
 Dynamicly allocate memory to contain disasm insn
 Disassembled instructions will be put into @*insn

 NOTE 1: this API will automatically determine memory needed to contain
 output disassembled instructions in @insn.
 NOTE 2: caller must free() the allocated memory itself to avoid memory leaking

 @handle: handle returned by cs_open()
 @code: buffer containing raw binary code to be disassembled
 @code_size: size of above code
 @address: address of the first insn in given raw code buffer
 @insn: array of insn filled in by this function
       NOTE: @insn will be allocated by this function, and should be freed
	   with cs_free() API.
 @count: number of instrutions to be disassembled, or 0 to get all of them
 @return: the number of succesfully disassembled instructions,
 or 0 if this function failed to disassemble the given code

 On failure, call cs_errno() for error code.
*/
size_t cs_disasm_dyn(csh handle,
		const uint8_t *code, size_t code_size,
		uint64_t address,
		size_t count,
		cs_insn **insn);

/*
 Free memory allocated in @insn by cs_disasm_dyn()

 @mem: pointer returned by @insn argument in cs_disasm_dyn()
*/
void cs_free(void *mem);

/*
 Return friendly name of regiser in a string
 Find the instruction id from header file of corresponding architecture (arm.h for ARM, x86.h for X86, ...)

 @handle: handle returned by cs_open()
 @reg: register id
 @return: string name of the register, or NULL if @reg_id is invalid.
*/
const char *cs_reg_name(csh handle, unsigned int reg_id);

/*
 Return friendly name of an instruction in a string
 Find the instruction id from header file of corresponding architecture (arm.h for ARM, x86.h for X86, ...)

 @handle: handle returned by cs_open()
 @insn: instruction id

 @return: string name of the instruction, or NULL if @insn_id is invalid.
*/
const char *cs_insn_name(csh handle, unsigned int insn_id);

/*
 Check if a disassembled instruction belong to a particular group.
 Find the group id from header file of corresponding architecture (arm.h for ARM, x86.h for X86, ...)
 Internally, this simply verifies if @group_id matches any member of insn->groups array.

 NOTE: this API is only valid when detail option is ON (which is OFF by default)

 @handle: handle returned by cs_open()
 @insn: disassembled instruction structure received from cs_disasm() or cs_disasm_dyn()
 @group_id: group that you want to check if this instruction belong to.

 @return: true if this instruction indeed belongs to aboved group, or false otherwise.
*/
bool cs_insn_group(csh handle, cs_insn *insn, unsigned int group_id);

/*
 Check if a disassembled instruction IMPLICITLY used a particular register.
 Find the register id from header file of corresponding architecture (arm.h for ARM, x86.h for X86, ...)
 Internally, this simply verifies if @reg_id matches any member of insn->regs_read array.

<<<<<<< HEAD
 @insn: disassembled instruction structure received from cs_disasm() or cs_disasm_dyn()
=======
 NOTE: this API is only valid when detail option is ON (which is OFF by default)

 @insn: disassembled instruction structure received from cs_disasm() or cs_disasm_ex()
>>>>>>> upstream/master
 @reg_id: register that you want to check if this instruction used it.

 @return: true if this instruction indeed implicitly used aboved register, or false otherwise.
*/
bool cs_reg_read(csh handle, cs_insn *insn, unsigned int reg_id);

/*
 Check if a disassembled instruction IMPLICITLY modified a particular register.
 Find the register id from header file of corresponding architecture (arm.h for ARM, x86.h for X86, ...)
 Internally, this simply verifies if @reg_id matches any member of insn->regs_write array.

<<<<<<< HEAD
 @insn: disassembled instruction structure received from cs_disasm() or cs_disasm_dyn()
=======
 NOTE: this API is only valid when detail option is ON (which is OFF by default)

 @insn: disassembled instruction structure received from cs_disasm() or cs_disasm_ex()
>>>>>>> upstream/master
 @reg_id: register that you want to check if this instruction modified it.

 @return: true if this instruction indeed implicitly modified aboved register, or false otherwise.
*/
bool cs_reg_write(csh handle, cs_insn *insn, unsigned int reg_id);

/*
 Count the number of operands of a given type.
 Find the operand type in header file of corresponding architecture (arm.h for ARM, x86.h for X86, ...)

 NOTE: this API is only valid when detail option is ON (which is OFF by default)

 @handle: handle returned by cs_open()
 @insn: disassembled instruction structure received from cs_disasm() or cs_disasm_dyn()
 @op_type: Operand type to be found.

 @return: number of operands of given type @op_type in instruction @insn,
 or -1 on failure.
*/
int cs_op_count(csh handle, cs_insn *insn, unsigned int op_type);

/*
 Retrieve the position of operand of given type in arch.op_info[] array.
 Later, the operand can be accessed using the returned position.
 Find the operand type in header file of corresponding architecture (arm.h for ARM, x86.h for X86, ...)

 NOTE: this API is only valid when detail option is ON (which is OFF by default)

 @handle: handle returned by cs_open()
 @insn: disassembled instruction structure received from cs_disasm() or cs_disasm_dyn()
 @op_type: Operand type to be found.
 @position: position of the operand to be found. This must be in the range
			[1, cs_op_count(handle, insn, op_type)]

 @return: index of operand of given type @op_type in arch.op_info[] array
 in instruction @insn, or -1 on failure.
*/
int cs_op_index(csh handle, cs_insn *insn, unsigned int op_type,
		unsigned int position);

#ifdef __cplusplus
}
#endif

#endif
