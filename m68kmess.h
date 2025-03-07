/*mess needed for musashi to work. don't really intend to do something better for now.*/

INLINE unsigned int  m68k_read_memory_8(unsigned int address);

INLINE unsigned int  m68k_read_memory_16(unsigned int address){
	unsigned int ret = 0;
	ret =  m68k_read_memory_8(address+1) | (m68k_read_memory_8(address)<<8);
#ifdef DEBUG_I
	printf("read  2 @%06x = %04x\n",address,ret);
#endif
	return ret;
}

INLINE unsigned int  m68k_read_memory_32(unsigned int address){
	unsigned int ret = 0;
	ret = m68k_read_memory_8(address+3) | (m68k_read_memory_8(address+2)<<8) \
	 | (m68k_read_memory_8(address+1)<<16) | (m68k_read_memory_8(address)<<24);
#ifdef DEBUG_I
	printf("read  4 @%06x = %08x\n",address,ret);
#endif
	return ret;
}

INLINE unsigned int m68k_read_disassembler_16(unsigned int address){
//	return m68k_read_memory_16(address);
	unsigned int ret = 0;
	ret =  m68k_read_memory_8(address+1) | (m68k_read_memory_8(address)<<8);
//	printf("r2:%06x:%04x\n",address,ret);
	return ret;
}
INLINE unsigned int m68k_read_disassembler_32(unsigned int address){
//	return m68k_read_memory_32(address);
	unsigned int ret = 0;
	ret = m68k_read_memory_8(address+3) | (m68k_read_memory_8(address+2)<<8) \
	 | (m68k_read_memory_8(address+1)<<16) | (m68k_read_memory_8(address)<<24);
//	printf("r4:%06x:%08x\n",address,ret);
	return ret;
}

INLINE void m68k_write_memory_8(unsigned int address, unsigned int value);

INLINE void m68k_write_memory_16(unsigned int address, unsigned int value){
#ifdef DEBUG_I
	printf("write 2 @%06x = %04x\n",address,value);
#endif
	//printf("w8(2):\n");
	m68k_write_memory_8(address, value >> 8);
	m68k_write_memory_8(address+1, value);
}

INLINE void m68k_write_memory_32(unsigned int address, unsigned int value){
#ifdef DEBUG_I
	printf("write 4 @%06x = %08x\n",address,value);
#endif
	//printf("w8(4):\n");
	m68k_write_memory_8(address, value >> 24);
	m68k_write_memory_8(address+1, value >> 16);
	m68k_write_memory_8(address+2, value >> 8);
	m68k_write_memory_8(address+3, value);
}
