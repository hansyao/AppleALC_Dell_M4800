//
//  kern_file.hpp
//  KernelCommon
//
//  Copyright © 2016 vit9696. All rights reserved.
//

#ifndef kern_file_hpp
#define kern_file_hpp

#include <sys/kernel_types.h>

namespace FileIO {
	/**
	 *  Reads file data at path
	 *
	 *  @param path full file path
	 *  @param size bytes read
	 *
	 *  @return allocated buffer on success or nullptr on error
	 */
	uint8_t *readFileToBuffer(const char *path, size_t &size);
	
	/**
	 *  Read file data from a vnode
	 *
	 *  @param buffer output buffer
	 *  @param off    file offset
	 *  @param sz     bytes to read
	 *  @param vnode  file node
	 *  @param ctxt   filesystem context
	 *
	 *  @return 0 on success
	 */
	int readFileData(void *buffer, off_t off, size_t sz, vnode_t vnode, vfs_context_t ctxt);
	
	/**
	 *  Read file size from a vnode
	 *
	 *  @param vnode file node
	 *  @param ctxt  filesystem context
	 *
	 *  @return file size or 0
	 */
	size_t readFileSize(vnode_t vnode, vfs_context_t ctxt);
}

#endif /* kern_file_hpp */
