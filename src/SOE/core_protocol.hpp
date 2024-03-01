#ifndef CORE_PROTOCOL_HPP
#define CORE_PROTOCOL_HPP

#define MAX_PACKET_SIZE 512
#define CORE_PACKET_ID_SIZE 2
#define CORE_DATA_SEQUENCE_SIZE 2
#define CORE_DATA_FRAGMENT_EXTRA_SIZE 4
#define MAX_CORE_DATA_FRAGMENT_SIZE (MAX_PACKET_SIZE - CORE_PACKET_ID_SIZE - CORE_DATA_SEQUENCE_SIZE)

#endif // !CORE_PROTOCOL_HPP