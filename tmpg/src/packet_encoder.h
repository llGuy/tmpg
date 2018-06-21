#ifndef _PACKET_ENCODER_H_
#define _PACKET_ENCODER_H_

#include <iostream>
#include <vector>

namespace net {

    enum delimiter_t
    {
	CHAR_DELIMITER = '#'
    };
    
    using Byte = uint8_t;

    class PacketEncoder
    {
    public:
	PacketEncoder(void) = default;
	
	uint32_t Size(void)
	{
	    return m_bytes.size();
	}    
	
	Byte* Data(void)
	{
	    return &m_bytes[0];
	}
	
	template<typename _Ty>
	void PushBytes(_Ty&& data)
	{
	    uint32_t size = sizeof(data);
	    Byte* valptrCast = (Byte*)&data;
	    for(uint32_t ptr = 0; ptr < size; ++ptr)
	    {
		m_bytes.push_back(valptrCast[ptr]);
	    }
	    m_bytes.push_back(CHAR_DELIMITER);
	}
	
	template<typename _First, typename... _Rest>
	void PushBytes(_First&& first, _Rest&&... rest)
	{
	    PushBytes(std::forward<_First>(first));
	    PushBytes(std::forward<_Rest>(rest)...);
	}

	void PushString(const std::string& str)
	{
	    for(uint32_t i = 0; i < str.length(); ++i)
	    {
		m_bytes.push_back(str[i]);
	    }
	    m_bytes.push_back(CHAR_DELIMITER);
	}
    private:
	std::vector<Byte> m_bytes;
    };
    
}

#endif /* _PACKET_ENCODER_H_ */
