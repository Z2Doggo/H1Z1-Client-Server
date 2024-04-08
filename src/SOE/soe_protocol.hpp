#ifndef SOE_PROTOCOL_HPP
#define SOE_PROTOCOL_HPP

#include "../main_login.hpp"

#define MAX_PACKET_SIZE 512
#define CORE_PACKET_ID_SIZE 2
#define CORE_DATA_SEQUENCE_SIZE 2
#define CORE_DATA_FRAGMENT_EXTRA_SIZE 4
#define MAX_CORE_DATA_FRAGMENT_SIZE (MAX_PACKET_SIZE - CORE_PACKET_ID_SIZE - CORE_DATA_SEQUENCE_SIZE)

enum class SOE_Protocol : u16 // just to be sure
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

static std::array<u16, static_cast<u16>(SOE_Protocol::End) + 1>soe_protocol_ids
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

static std::array<std::string_view, static_cast<u16>(SOE_Protocol::End) + 1>soe_protocol_names
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
    u32 crc_length;
    u32 session_id;
    u32 udp_length;
    char protocol_name[32];
};

struct SOE_SessionReply
{
    u32 session_id;
    u32 crc_seed;
    u8 crc_size;
    u16 compression;
    u32 udp_size;
};

struct SOE_Data
{
    u16 sequence;
    char* data;
    u32 data_size;
    u16 crc;
};

struct SOE_Ack
{
    u16 sequence;
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
    u32 handle_id;
    Stream* stream;
    Buffer buffer;
    Protocol_Params params;

    void soe_packet_callback(Buffer buffer, u32 handle_id, Application* app, SOE* soe_protocol);
    void soe_session_callback(u32 handle_id, Application* app);
    u32 soe_read_data_chunk_size(Stream* stream);
    void soe_input_process_data_chunks(Buffer buffer, Protocol_Params params, u32 handle_id, Application* app, SOE* soe_protocol);
    void soe_unpack(Stream* stream, void* ptr, SOE_Protocol soe_protocol, u32 is_sub, Protocol_Params params);
    void soe_packet_send(void* ptr, SOE_Protocol soe_protocol, u32 handle_id, Application* app);
    void soe_data_send(Buffer buffer, u32 ignore_encryption, u32 handle_id, Application* app);
    void soe_packet_route(Buffer buffer, u32 is_sub, u32 handle_id, Application* app, SOE* soe_protocol);
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