#ifndef _PACKET_PARSER_H_
#define _PACKET_PARSER_H_

#include "packet_delimiter.h"

namespace net {

    using Byte = uint8_t;
    
    class PacketParser
    {
    public:
	PacketParser(Byte* bytes, int32_t size)
	    : m_bytes(bytes), m_index(0), m_size(size)
	{
	}

	bool Max(void)
	{
	    return m_index >= m_size - 1;
	}
	
	template<typename _Ty>
	_Ty ReadNext(char delimiter)
	{
	    _Ty* ptr = (_Ty*)&m_bytes[m_index];
	    m_index += sizeof(_Ty) + 1 /* delimiter of section */;
	    return *ptr;
	}
	
	void ReadNext(char delimiter, std::string& s)
	{
	    for(; m_bytes[m_index] != delimiter; ++m_index)
	    {
		s.push_back(m_bytes[m_index]);
	    }
	    ++m_index;
	}
    private:
	Byte* m_bytes;
	int32_t m_size;
	uint32_t m_index;
    };
    
}

#endif /* _PACKET_PARSER_H_ */
