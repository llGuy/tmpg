#ifndef _PACKET_HISTORY_H_
#define _PACKET_HISTORY_H_

#include <glm/glm.hpp>
#include <array>

namespace net {

	struct Packet
	{
		uint64_t id;
		glm::vec3 position;
		glm::vec3 direction;
	};

	class PacketHistory
	{
	public:
		PacketHistory(void)
			: m_ptr(0), canInterpolate(false)
		{
		}

		bool CanInterpolate(void)
		{
			return canInterpolate;
		}

		Packet & At(uint64_t id)
		{
			return m_history[id % 32];
		}

		void Push(Packet const & packet)
		{
			m_history[m_ptr++] = packet;
			if (m_ptr == 32) m_ptr = 0;
		}
	private:
		std::array<Packet, 32> m_history;
		uint32_t m_ptr;
		bool canInterpolate;
	};


}

#endif