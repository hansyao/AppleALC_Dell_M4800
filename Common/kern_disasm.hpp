//
//  kern_disasm.hpp
//  KernelCommon
//
//  Copyright © 2016 vit9696. All rights reserved.
//

#ifndef kern_disasm_hpp
#define kern_disasm_hpp

#include <capstone.h>

class Disassembler {
	/**
	 *  Because captsone handle can be 0
	 */
	bool initialised {false};
	
	/**
	 *  Internal capstone handle
	 */
	csh handle;

	/**
	 *  Max instruction size
	 */
	static constexpr size_t MaxInstruction {15};
public:

	/**
	 *  Initialise dissassembling framework
	 *
	 *  @param detailed  debugging output necessity
	 *
	 *  @return true on success
	 */
	bool init(bool detailed=false);
	
	/**
	 *  Deinitialise dissassembling framework, must be called regardless of the init error
	 */
	void deinit();
	
	/**
	 *  Return the real instruction size contained within min bytes
	 *
	 *  @param ptr instruction pointer
	 *  @param min minimal possible size
	 *
	 *  @return instruction size >= min on success or 0
	 */
	size_t instructionSize(mach_vm_address_t ptr, size_t min);
};

#endif /* kern_disasm_hpp */
