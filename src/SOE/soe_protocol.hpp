#ifndef SOE_PROTOCOL_HPP
#define SOE_PROTOCOL_HPP

#include "../main_login.hpp"

#define MAX_PACKET_SIZE 512
#define CORE_PACKET_ID_SIZE 2
#define CORE_DATA_SEQUENCE_SIZE 2
#define CORE_DATA_FRAGMENT_EXTRA_SIZE 4
#define MAX_CORE_DATA_FRAGMENT_SIZE (MAX_PACKET_SIZE - CORE_PACKET_ID_SIZE - CORE_DATA_SEQUENCE_SIZE)

enum class SOE_Protocol : uint16_t // just to be sure
{
    Invalid,
    SessionRequest,
    SessionReply,
    MultiPacket,
    Disconnect,
    Ping,
    NetStatusRequest,
    NetStatusReply,
    Data,
    DataFragment,
    OutOfOrder,
    Ack,
    Unknown_1c,
    FatalError,
    FatalErrorReply,
    Unknown_1f,
    End,
};

static std::array<uint16_t, static_cast<uint16_t>(SOE_Protocol::End) + 1>soe_protocol_ids
{ 
    0xffff,
    0x0001, 
    0x0002, 
    0x0003, 
    0x0005, 
    0x0006, 
    0x0007, 
    0x0008, 
    0x0009, 
    0x000d,
    0x0011, 
    0x0015, 
    0x001c, 
    0x001d, 
    0x001e, 
    0x001f, 
    0xffff
};

static std::array<std::string_view, static_cast<uint16_t>(SOE_Protocol::End) + 1>soe_protocol_names
{
    "INVALID", 
    "SessionRequest", 
    "SessionReply", 
    "MultiPacket", 
    "Disconnect",
    "Ping", 
    "NetStatusRequest", 
    "NetStatusReply", 
    "Data", 
    "DataFragment",
    "OutOfOrder", 
    "Ack", 
    "UNKNOWN_1C", 
    "FatalError", 
    "FatalErrorReply",
    "UNKNOWN_1F", ""
};

struct SOE_SessionRequest
{
    uint32_t crc_length;
    uint32_t session_id;
    uint32_t udp_length;
    char protocol_name[32];
};

struct SOE_SessionReply
{
    uint32_t session_id;
    uint32_t crc_seed;
    uint8_t crc_size;
    uint16_t compression;
    uint32_t udp_size;
};

struct SOE_Data
{
    uint16_t sequence;
    uint8_t* data;
    uint32_t data_size;
    uint16_t crc;
};

struct SOE_Ack
{
    uint16_t sequence;
};

class SOE : public Application
{
public:
    // SOE related structs
    SOE_SessionRequest* session_req;
    SOE_SessionReply* session_reply;
    SOE_Data* data;
    SOE_Ack* ack;

    // Game tick related
    Application* app;
    Session* session;
    uint32_t handle_id;
    Stream* stream;
    Buffer buffer;
    Protocol_Params params;

    void soe_packet_callback(Buffer buffer, uint32_t handle_id, Application* app, SOE* soe_protocol);
    void soe_session_callback(uint32_t handle_id, Application* app);
    uint32_t soe_read_data_chunk_size(Stream* stream);
    void soe_input_process_data_chunks(Buffer buffer, Protocol_Params params, uint32_t handle_id, Application* app, SOE* soe_protocol);
    void soe_unpack(Stream* stream, void* ptr, SOE_Protocol soe_protocol, uint32_t is_sub, Protocol_Params params);
    void soe_packet_send(void* ptr, SOE_Protocol soe_protocol, uint32_t handle_id, Application* app);
    void soe_data_send(Buffer buffer, uint32_t ignore_encryption, uint32_t handle_id, Application* app);
    void soe_packet_route(Buffer buffer, uint32_t is_sub, uint32_t handle_id, Application* app, SOE* soe_protocol);
};

class Memory : public SOE
{
public:
    Socket_Api api;
    Arena* arena_total;
    Application* app;
    SOE* soe_protocol;
    float ms_tick;
    float ms_work;
    uint64_t tick_count;
};

#define APP_TICK(name) void name(Memory* memory)
typedef APP_TICK(app_tick_t);

#endif // !SOE_PROTOCOL_HPP