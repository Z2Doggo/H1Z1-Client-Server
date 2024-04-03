#include "LoginUdp_11.hpp"

uint32_t
login_packet_pack(enum Login_Packet_Kind packet_kind, void* packet_ptr, uint8_t* buffer)
{
uint32_t offset = 0;

printf("\n");
switch(packet_kind)
{
case Login_Packet_Kind_LoginRequest:
{
printf("[*] Packing LoginRequest...\n");
Login_Packet_LoginRequest* packet = static_cast<Login_Packet_LoginRequest*>(packet_ptr);

endian_write_u8_little(buffer + offset, 0x1);
offset += sizeof(u8);

// string session_id
endian_write_u32_little(buffer + offset, packet->session_id_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->session_id_length, (uint64_t)packet->session_id_length, (double)packet->session_id_length);
for (uint32_t session_id_iter = 0; session_id_iter < packet->session_id_length; session_id_iter++)
{
endian_write_i8_little(buffer + offset, packet->session_id[session_id_iter]);
offset++;
}

// string system_fingerprint
endian_write_u32_little(buffer + offset, packet->system_fingerprint_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->system_fingerprint_length, (uint64_t)packet->system_fingerprint_length, (double)packet->system_fingerprint_length);
for (uint32_t system_fingerprint_iter = 0; system_fingerprint_iter < packet->system_fingerprint_length; system_fingerprint_iter++)
{
endian_write_i8_little(buffer + offset, packet->system_fingerprint[system_fingerprint_iter]);
offset++;
}

// u32 locale
endian_write_u32_little(buffer + offset, packet->locale);
offset += sizeof(u32);
printf("-- locale                  \t%lld\t%llxh\t%f\n", (i64)packet->locale, (uint64_t)packet->locale, (double)packet->locale);

// u32 third_party_auth_ticket
endian_write_u32_little(buffer + offset, packet->third_party_auth_ticket);
offset += sizeof(u32);
printf("-- third_party_auth_ticket \t%lld\t%llxh\t%f\n", (i64)packet->third_party_auth_ticket, (uint64_t)packet->third_party_auth_ticket, (double)packet->third_party_auth_ticket);

// u32 third_party_user_id
endian_write_u32_little(buffer + offset, packet->third_party_user_id);
offset += sizeof(u32);
printf("-- third_party_user_id     \t%lld\t%llxh\t%f\n", (i64)packet->third_party_user_id, (uint64_t)packet->third_party_user_id, (double)packet->third_party_user_id);

// u32 third_party_id
endian_write_u32_little(buffer + offset, packet->third_party_id);
offset += sizeof(u32);
printf("-- third_party_id          \t%lld\t%llxh\t%f\n", (i64)packet->third_party_id, (uint64_t)packet->third_party_id, (double)packet->third_party_id);

} break;

case Login_Packet_Kind_LoginReply:
{
printf("[*] Packing LoginReply...\n");
Login_Packet_LoginReply* packet = static_cast<Login_Packet_LoginReply*>(packet_ptr);

endian_write_u8_little(buffer + offset, 0x2);
offset += sizeof(u8);

// b8 is_logged_in
endian_write_b8_little(buffer + offset, packet->is_logged_in);
offset += sizeof(b8);
printf("-- is_logged_in            \t%lld\t%llxh\t%f\n", (i64)packet->is_logged_in, (uint64_t)packet->is_logged_in, (double)packet->is_logged_in);

// u32 status
endian_write_u32_little(buffer + offset, packet->status);
offset += sizeof(u32);
printf("-- status                  \t%lld\t%llxh\t%f\n", (i64)packet->status, (uint64_t)packet->status, (double)packet->status);

// u32 result_code
endian_write_u32_little(buffer + offset, packet->result_code);
offset += sizeof(u32);
printf("-- result_code             \t%lld\t%llxh\t%f\n", (i64)packet->result_code, (uint64_t)packet->result_code, (double)packet->result_code);

// b8 is_member
endian_write_b8_little(buffer + offset, packet->is_member);
offset += sizeof(b8);
printf("-- is_member               \t%lld\t%llxh\t%f\n", (i64)packet->is_member, (uint64_t)packet->is_member, (double)packet->is_member);

// b8 is_internal
endian_write_b8_little(buffer + offset, packet->is_internal);
offset += sizeof(b8);
printf("-- is_internal             \t%lld\t%llxh\t%f\n", (i64)packet->is_internal, (uint64_t)packet->is_internal, (double)packet->is_internal);

// string namespace_name
endian_write_u32_little(buffer + offset, packet->namespace_name_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->namespace_name_length, (uint64_t)packet->namespace_name_length, (double)packet->namespace_name_length);
for (uint32_t namespace_name_iter = 0; namespace_name_iter < packet->namespace_name_length; namespace_name_iter++)
{
endian_write_i8_little(buffer + offset, packet->namespace_name[namespace_name_iter]);
offset++;
}

// list account_features
endian_write_u32_little(buffer + offset, packet->account_features_count);
offset += sizeof(u32);
printf("-- LIST_COUNT              \t%lld\t%llxh\t%f\n", (i64)packet->account_features_count, (uint64_t)packet->account_features_count, (double)packet->account_features_count);

for (uint32_t account_features_iter = 0; account_features_iter < packet->account_features_count; account_features_iter++)
{
// u32 key
endian_write_u32_little(buffer + offset, packet->account_features[account_features_iter].key);
offset += sizeof(u32);
printf("-- key                     \t%lld\t%llxh\t%f\n", (i64)packet->account_features[account_features_iter].key, (uint64_t)packet->account_features[account_features_iter].key, (double)packet->account_features[account_features_iter].key);

// u32 id
endian_write_u32_little(buffer + offset, packet->account_features[account_features_iter].id);
offset += sizeof(u32);
printf("-- id                      \t%lld\t%llxh\t%f\n", (i64)packet->account_features[account_features_iter].id, (uint64_t)packet->account_features[account_features_iter].id, (double)packet->account_features[account_features_iter].id);

// b8 active
endian_write_b8_little(buffer + offset, packet->account_features[account_features_iter].active);
offset += sizeof(b8);
printf("-- active                  \t%lld\t%llxh\t%f\n", (i64)packet->account_features[account_features_iter].active, (uint64_t)packet->account_features[account_features_iter].active, (double)packet->account_features[account_features_iter].active);

// u32 remaining_count
endian_write_u32_little(buffer + offset, packet->account_features[account_features_iter].remaining_count);
offset += sizeof(u32);
printf("-- remaining_count         \t%lld\t%llxh\t%f\n", (i64)packet->account_features[account_features_iter].remaining_count, (uint64_t)packet->account_features[account_features_iter].remaining_count, (double)packet->account_features[account_features_iter].remaining_count);

// string raw_data
endian_write_u32_little(buffer + offset, packet->account_features[account_features_iter].raw_data_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->account_features[account_features_iter].raw_data_length, (uint64_t)packet->account_features[account_features_iter].raw_data_length, (double)packet->account_features[account_features_iter].raw_data_length);
for (uint32_t raw_data_iter = 0; raw_data_iter < packet->account_features[account_features_iter].raw_data_length; raw_data_iter++)
{
endian_write_i8_little(buffer + offset, packet->account_features[account_features_iter].raw_data[raw_data_iter]);
offset++;
}

} // account_features

// bytes application_payload
endian_write_u32_little(buffer + offset, packet->application_payload_length);
offset += sizeof(u32);
printf("-- BYTES_LENGTH            \t%lld\t%llxh\t%f\n", (i64)packet->application_payload_length, (uint64_t)packet->application_payload_length, (double)packet->application_payload_length);
for (uint32_t application_payload_iter = 0; application_payload_iter < packet->application_payload_length; application_payload_iter++)
{
endian_write_u8_little(buffer + offset, packet->application_payload[application_payload_iter]);
offset++;
}

// list error_details
endian_write_u32_little(buffer + offset, packet->error_details_count);
offset += sizeof(u32);
printf("-- LIST_COUNT              \t%lld\t%llxh\t%f\n", (i64)packet->error_details_count, (uint64_t)packet->error_details_count, (double)packet->error_details_count);

for (uint32_t error_details_iter = 0; error_details_iter < packet->error_details_count; error_details_iter++)
{
// u32 unknown_dword1
endian_write_u32_little(buffer + offset, packet->error_details[error_details_iter].unknown_dword1);
offset += sizeof(u32);
printf("-- unknown_dword1          \t%lld\t%llxh\t%f\n", (i64)packet->error_details[error_details_iter].unknown_dword1, (uint64_t)packet->error_details[error_details_iter].unknown_dword1, (double)packet->error_details[error_details_iter].unknown_dword1);

// string name
endian_write_u32_little(buffer + offset, packet->error_details[error_details_iter].name_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->error_details[error_details_iter].name_length, (uint64_t)packet->error_details[error_details_iter].name_length, (double)packet->error_details[error_details_iter].name_length);
for (uint32_t name_iter = 0; name_iter < packet->error_details[error_details_iter].name_length; name_iter++)
{
endian_write_i8_little(buffer + offset, packet->error_details[error_details_iter].name[name_iter]);
offset++;
}

// string value
endian_write_u32_little(buffer + offset, packet->error_details[error_details_iter].value_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->error_details[error_details_iter].value_length, (uint64_t)packet->error_details[error_details_iter].value_length, (double)packet->error_details[error_details_iter].value_length);
for (uint32_t value_iter = 0; value_iter < packet->error_details[error_details_iter].value_length; value_iter++)
{
endian_write_i8_little(buffer + offset, packet->error_details[error_details_iter].value[value_iter]);
offset++;
}

} // error_details

// string ip_country_code
endian_write_u32_little(buffer + offset, packet->ip_country_code_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->ip_country_code_length, (uint64_t)packet->ip_country_code_length, (double)packet->ip_country_code_length);
for (uint32_t ip_country_code_iter = 0; ip_country_code_iter < packet->ip_country_code_length; ip_country_code_iter++)
{
endian_write_i8_little(buffer + offset, packet->ip_country_code[ip_country_code_iter]);
offset++;
}

} break;

case Login_Packet_Kind_Logout:
{
printf("[*] Packing Logout...\n");
endian_write_u8_little(buffer + offset, 0x3);
offset += sizeof(u8);

} break;

case Login_Packet_Kind_ForceDisconnect:
{
printf("[*] Packing ForceDisconnect...\n");
endian_write_u8_little(buffer + offset, 0x4);
offset += sizeof(u8);

} break;

case Login_Packet_Kind_CharacterCreateRequest:
{
printf("[*] Packing CharacterCreateRequest...\n");
Login_Packet_CharacterCreateRequest* packet = static_cast<Login_Packet_CharacterCreateRequest*>(packet_ptr);

endian_write_u8_little(buffer + offset, 0x5);
offset += sizeof(u8);

// u32 server_id
endian_write_u32_little(buffer + offset, packet->server_id);
offset += sizeof(u32);
printf("-- server_id               \t%lld\t%llxh\t%f\n", (i64)packet->server_id, (uint64_t)packet->server_id, (double)packet->server_id);

// u32 unk_u32
endian_write_u32_little(buffer + offset, packet->unk_u32);
offset += sizeof(u32);
printf("-- unk_u32                 \t%lld\t%llxh\t%f\n", (i64)packet->unk_u32, (uint64_t)packet->unk_u32, (double)packet->unk_u32);

// stream char_payload
void* char_payload_length_ptr = buffer + offset;
offset += sizeof(u32);
printf("-- STREAM_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->char_payload_length, (uint64_t)packet->char_payload_length, (double)packet->char_payload_length);

uint32_t will_pack_char_payload = packet->char_payload_length == ~(u32)0 ? 0 : 1;
for (uint32_t char_payload_iter = 0; char_payload_iter < will_pack_char_payload; char_payload_iter++)
{
// u8 empire_id
endian_write_u8_little(buffer + offset, packet->char_payload[char_payload_iter].empire_id);
offset += sizeof(u8);
printf("-- empire_id               \t%lld\t%llxh\t%f\n", (i64)packet->char_payload[char_payload_iter].empire_id, (uint64_t)packet->char_payload[char_payload_iter].empire_id, (double)packet->char_payload[char_payload_iter].empire_id);

// u32 head_type
endian_write_u32_little(buffer + offset, packet->char_payload[char_payload_iter].head_type);
offset += sizeof(u32);
printf("-- head_type               \t%lld\t%llxh\t%f\n", (i64)packet->char_payload[char_payload_iter].head_type, (uint64_t)packet->char_payload[char_payload_iter].head_type, (double)packet->char_payload[char_payload_iter].head_type);

// u32 profile_type
endian_write_u32_little(buffer + offset, packet->char_payload[char_payload_iter].profile_type);
offset += sizeof(u32);
printf("-- profile_type            \t%lld\t%llxh\t%f\n", (i64)packet->char_payload[char_payload_iter].profile_type, (uint64_t)packet->char_payload[char_payload_iter].profile_type, (double)packet->char_payload[char_payload_iter].profile_type);

// u32 gender
endian_write_u32_little(buffer + offset, packet->char_payload[char_payload_iter].gender);
offset += sizeof(u32);
printf("-- gender                  \t%lld\t%llxh\t%f\n", (i64)packet->char_payload[char_payload_iter].gender, (uint64_t)packet->char_payload[char_payload_iter].gender, (double)packet->char_payload[char_payload_iter].gender);

// string character_name
endian_write_u32_little(buffer + offset, packet->char_payload[char_payload_iter].character_name_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->char_payload[char_payload_iter].character_name_length, (uint64_t)packet->char_payload[char_payload_iter].character_name_length, (double)packet->char_payload[char_payload_iter].character_name_length);
for (uint32_t character_name_iter = 0; character_name_iter < packet->char_payload[char_payload_iter].character_name_length; character_name_iter++)
{
endian_write_i8_little(buffer + offset, packet->char_payload[char_payload_iter].character_name[character_name_iter]);
offset++;
}

} // char_payload
endian_write_u32_little((uint8_t*)char_payload_length_ptr, (u32)((uptr)buffer + (uptr)offset - (uptr)char_payload_length_ptr - sizeof(u32)));

} break;

case Login_Packet_Kind_CharacterCreateReply:
{
printf("[*] Packing CharacterCreateReply...\n");
Login_Packet_CharacterCreateReply* packet = static_cast<Login_Packet_CharacterCreateReply*>(packet_ptr);

endian_write_u8_little(buffer + offset, 0x6);
offset += sizeof(u8);

// u32 status
endian_write_u32_little(buffer + offset, packet->status);
offset += sizeof(u32);
printf("-- status                  \t%lld\t%llxh\t%f\n", (i64)packet->status, (uint64_t)packet->status, (double)packet->status);

// u64 character_id
endian_write_u64_little(buffer + offset, packet->character_id);
offset += sizeof(u64);
printf("-- character_id            \t%lld\t%llxh\t%f\n", (i64)packet->character_id, (uint64_t)packet->character_id, (double)packet->character_id);

} break;

case Login_Packet_Kind_CharacterLoginRequest:
{
printf("[*] Packing CharacterLoginRequest...\n");
Login_Packet_CharacterLoginRequest* packet = static_cast<Login_Packet_CharacterLoginRequest*>(packet_ptr);

endian_write_u8_little(buffer + offset, 0x7);
offset += sizeof(u8);

// u64 character_id
endian_write_u64_little(buffer + offset, packet->character_id);
offset += sizeof(u64);
printf("-- character_id            \t%lld\t%llxh\t%f\n", (i64)packet->character_id, (uint64_t)packet->character_id, (double)packet->character_id);

// u32 server_id
endian_write_u32_little(buffer + offset, packet->server_id);
offset += sizeof(u32);
printf("-- server_id               \t%lld\t%llxh\t%f\n", (i64)packet->server_id, (uint64_t)packet->server_id, (double)packet->server_id);

// u32 status
endian_write_u32_little(buffer + offset, packet->status);
offset += sizeof(u32);
printf("-- status                  \t%lld\t%llxh\t%f\n", (i64)packet->status, (uint64_t)packet->status, (double)packet->status);

// stream payload2
void* payload2_length_ptr = buffer + offset;
offset += sizeof(u32);
printf("-- STREAM_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->payload2_length, (uint64_t)packet->payload2_length, (double)packet->payload2_length);

uint32_t will_pack_payload2 = packet->payload2_length == ~(u32)0 ? 0 : 1;
for (uint32_t payload2_iter = 0; payload2_iter < will_pack_payload2; payload2_iter++)
{
// string locale
endian_write_u32_little(buffer + offset, packet->payload2[payload2_iter].locale_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->payload2[payload2_iter].locale_length, (uint64_t)packet->payload2[payload2_iter].locale_length, (double)packet->payload2[payload2_iter].locale_length);
for (uint32_t locale_iter = 0; locale_iter < packet->payload2[payload2_iter].locale_length; locale_iter++)
{
endian_write_i8_little(buffer + offset, packet->payload2[payload2_iter].locale[locale_iter]);
offset++;
}

// u32 locale_id
endian_write_u32_little(buffer + offset, packet->payload2[payload2_iter].locale_id);
offset += sizeof(u32);
printf("-- locale_id               \t%lld\t%llxh\t%f\n", (i64)packet->payload2[payload2_iter].locale_id, (uint64_t)packet->payload2[payload2_iter].locale_id, (double)packet->payload2[payload2_iter].locale_id);

// u32 preferred_gateway_id
endian_write_u32_little(buffer + offset, packet->payload2[payload2_iter].preferred_gateway_id);
offset += sizeof(u32);
printf("-- preferred_gateway_id    \t%lld\t%llxh\t%f\n", (i64)packet->payload2[payload2_iter].preferred_gateway_id, (uint64_t)packet->payload2[payload2_iter].preferred_gateway_id, (double)packet->payload2[payload2_iter].preferred_gateway_id);

} // payload2
endian_write_u32_little((uint8_t*)payload2_length_ptr, (u32)((uptr)buffer + (uptr)offset - (uptr)payload2_length_ptr - sizeof(u32)));

} break;

case Login_Packet_Kind_CharacterLoginReply:
{
printf("[*] Packing CharacterLoginReply...\n");
Login_Packet_CharacterLoginReply* packet = static_cast<Login_Packet_CharacterLoginReply*>(packet_ptr);

endian_write_u8_little(buffer + offset, 0x8);
offset += sizeof(u8);

// u64 character_id
endian_write_u64_little(buffer + offset, packet->character_id);
offset += sizeof(u64);
printf("-- character_id            \t%lld\t%llxh\t%f\n", (i64)packet->character_id, (uint64_t)packet->character_id, (double)packet->character_id);

// u32 server_id
endian_write_u32_little(buffer + offset, packet->server_id);
offset += sizeof(u32);
printf("-- server_id               \t%lld\t%llxh\t%f\n", (i64)packet->server_id, (uint64_t)packet->server_id, (double)packet->server_id);

// u32 last_login
endian_write_u32_little(buffer + offset, packet->last_login);
offset += sizeof(u32);
printf("-- last_login              \t%lld\t%llxh\t%f\n", (i64)packet->last_login, (uint64_t)packet->last_login, (double)packet->last_login);

// u32 status
endian_write_u32_little(buffer + offset, packet->status);
offset += sizeof(u32);
printf("-- status                  \t%lld\t%llxh\t%f\n", (i64)packet->status, (uint64_t)packet->status, (double)packet->status);

// stream login_payload
void* login_payload_length_ptr = buffer + offset;
offset += sizeof(u32);
printf("-- STREAM_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->login_payload_length, (uint64_t)packet->login_payload_length, (double)packet->login_payload_length);

uint32_t will_pack_login_payload = packet->login_payload_length == ~(u32)0 ? 0 : 1;
for (uint32_t login_payload_iter = 0; login_payload_iter < will_pack_login_payload; login_payload_iter++)
{
// string server_address
endian_write_u32_little(buffer + offset, packet->login_payload[login_payload_iter].server_address_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->login_payload[login_payload_iter].server_address_length, (uint64_t)packet->login_payload[login_payload_iter].server_address_length, (double)packet->login_payload[login_payload_iter].server_address_length);
for (uint32_t server_address_iter = 0; server_address_iter < packet->login_payload[login_payload_iter].server_address_length; server_address_iter++)
{
endian_write_i8_little(buffer + offset, packet->login_payload[login_payload_iter].server_address[server_address_iter]);
offset++;
}

// string server_ticket
endian_write_u32_little(buffer + offset, packet->login_payload[login_payload_iter].server_ticket_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->login_payload[login_payload_iter].server_ticket_length, (uint64_t)packet->login_payload[login_payload_iter].server_ticket_length, (double)packet->login_payload[login_payload_iter].server_ticket_length);
for (uint32_t server_ticket_iter = 0; server_ticket_iter < packet->login_payload[login_payload_iter].server_ticket_length; server_ticket_iter++)
{
endian_write_i8_little(buffer + offset, packet->login_payload[login_payload_iter].server_ticket[server_ticket_iter]);
offset++;
}

// bytes encryption_key
endian_write_u32_little(buffer + offset, packet->login_payload[login_payload_iter].encryption_key_length);
offset += sizeof(u32);
printf("-- BYTES_LENGTH            \t%lld\t%llxh\t%f\n", (i64)packet->login_payload[login_payload_iter].encryption_key_length, (uint64_t)packet->login_payload[login_payload_iter].encryption_key_length, (double)packet->login_payload[login_payload_iter].encryption_key_length);
for (uint32_t encryption_key_iter = 0; encryption_key_iter < packet->login_payload[login_payload_iter].encryption_key_length; encryption_key_iter++)
{
endian_write_u8_little(buffer + offset, packet->login_payload[login_payload_iter].encryption_key[encryption_key_iter]);
offset++;
}

// u32 soe_protocol_version
endian_write_u32_little(buffer + offset, packet->login_payload[login_payload_iter].soe_protocol_version);
offset += sizeof(u32);
printf("-- soe_protocol_version    \t%lld\t%llxh\t%f\n", (i64)packet->login_payload[login_payload_iter].soe_protocol_version, (uint64_t)packet->login_payload[login_payload_iter].soe_protocol_version, (double)packet->login_payload[login_payload_iter].soe_protocol_version);

// u64 character_id
endian_write_u64_little(buffer + offset, packet->login_payload[login_payload_iter].character_id);
offset += sizeof(u64);
printf("-- character_id            \t%lld\t%llxh\t%f\n", (i64)packet->login_payload[login_payload_iter].character_id, (uint64_t)packet->login_payload[login_payload_iter].character_id, (double)packet->login_payload[login_payload_iter].character_id);

// u64 unk_u64
endian_write_u64_little(buffer + offset, packet->login_payload[login_payload_iter].unk_u64);
offset += sizeof(u64);
printf("-- unk_u64                 \t%lld\t%llxh\t%f\n", (i64)packet->login_payload[login_payload_iter].unk_u64, (uint64_t)packet->login_payload[login_payload_iter].unk_u64, (double)packet->login_payload[login_payload_iter].unk_u64);

// string station_name
endian_write_u32_little(buffer + offset, packet->login_payload[login_payload_iter].station_name_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->login_payload[login_payload_iter].station_name_length, (uint64_t)packet->login_payload[login_payload_iter].station_name_length, (double)packet->login_payload[login_payload_iter].station_name_length);
for (uint32_t station_name_iter = 0; station_name_iter < packet->login_payload[login_payload_iter].station_name_length; station_name_iter++)
{
endian_write_i8_little(buffer + offset, packet->login_payload[login_payload_iter].station_name[station_name_iter]);
offset++;
}

// string character_name
endian_write_u32_little(buffer + offset, packet->login_payload[login_payload_iter].character_name_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->login_payload[login_payload_iter].character_name_length, (uint64_t)packet->login_payload[login_payload_iter].character_name_length, (double)packet->login_payload[login_payload_iter].character_name_length);
for (uint32_t character_name_iter = 0; character_name_iter < packet->login_payload[login_payload_iter].character_name_length; character_name_iter++)
{
endian_write_i8_little(buffer + offset, packet->login_payload[login_payload_iter].character_name[character_name_iter]);
offset++;
}

// string unk_str
endian_write_u32_little(buffer + offset, packet->login_payload[login_payload_iter].unk_str_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->login_payload[login_payload_iter].unk_str_length, (uint64_t)packet->login_payload[login_payload_iter].unk_str_length, (double)packet->login_payload[login_payload_iter].unk_str_length);
for (uint32_t unk_str_iter = 0; unk_str_iter < packet->login_payload[login_payload_iter].unk_str_length; unk_str_iter++)
{
endian_write_i8_little(buffer + offset, packet->login_payload[login_payload_iter].unk_str[unk_str_iter]);
offset++;
}

// u64 server_feature_bit
endian_write_u64_little(buffer + offset, packet->login_payload[login_payload_iter].server_feature_bit);
offset += sizeof(u64);
printf("-- server_feature_bit      \t%lld\t%llxh\t%f\n", (i64)packet->login_payload[login_payload_iter].server_feature_bit, (uint64_t)packet->login_payload[login_payload_iter].server_feature_bit, (double)packet->login_payload[login_payload_iter].server_feature_bit);

} // login_payload
endian_write_u32_little((uint8_t*)login_payload_length_ptr, (u32)((uptr)buffer + (uptr)offset - (uptr)login_payload_length_ptr - sizeof(u32)));

} break;

case Login_Packet_Kind_CharacterDeleteRequest:
{
printf("[*] Packing CharacterDeleteRequest...\n");
Login_Packet_CharacterDeleteRequest* packet = static_cast<Login_Packet_CharacterDeleteRequest*>(packet_ptr);

endian_write_u8_little(buffer + offset, 0x9);
offset += sizeof(u8);

// u64 character_id
endian_write_u64_little(buffer + offset, packet->character_id);
offset += sizeof(u64);
printf("-- character_id            \t%lld\t%llxh\t%f\n", (i64)packet->character_id, (uint64_t)packet->character_id, (double)packet->character_id);

} break;

case Login_Packet_Kind_TunnelAppPacketClientToServer:
{
printf("[*] Packing TunnelAppPacketClientToServer...\n");
Login_Packet_TunnelAppPacketClientToServer* packet = static_cast<Login_Packet_TunnelAppPacketClientToServer*>(packet_ptr);

endian_write_u8_little(buffer + offset, 0x10);
offset += sizeof(u8);

// u32 server_id
endian_write_u32_little(buffer + offset, packet->server_id);
offset += sizeof(u32);
printf("-- server_id               \t%lld\t%llxh\t%f\n", (i64)packet->server_id, (uint64_t)packet->server_id, (double)packet->server_id);

// u32 unk1
endian_write_u32_little(buffer + offset, packet->unk1);
offset += sizeof(u32);
printf("-- unk1                    \t%lld\t%llxh\t%f\n", (i64)packet->unk1, (uint64_t)packet->unk1, (double)packet->unk1);

// stream data_client
void* data_client_length_ptr = buffer + offset;
offset += sizeof(u32);
printf("-- STREAM_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->data_client_length, (uint64_t)packet->data_client_length, (double)packet->data_client_length);

uint32_t will_pack_data_client = packet->data_client_length == ~(u32)0 ? 0 : 1;
for (uint32_t data_client_iter = 0; data_client_iter < will_pack_data_client; data_client_iter++)
{
// b8 tunnel_op_code
endian_write_b8_little(buffer + offset, packet->data_client[data_client_iter].tunnel_op_code);
offset += sizeof(b8);
printf("-- tunnel_op_code          \t%lld\t%llxh\t%f\n", (i64)packet->data_client[data_client_iter].tunnel_op_code, (uint64_t)packet->data_client[data_client_iter].tunnel_op_code, (double)packet->data_client[data_client_iter].tunnel_op_code);

// b8 sub_op_code
endian_write_b8_little(buffer + offset, packet->data_client[data_client_iter].sub_op_code);
offset += sizeof(b8);
printf("-- sub_op_code             \t%lld\t%llxh\t%f\n", (i64)packet->data_client[data_client_iter].sub_op_code, (uint64_t)packet->data_client[data_client_iter].sub_op_code, (double)packet->data_client[data_client_iter].sub_op_code);

// string character_name
endian_write_u32_little(buffer + offset, packet->data_client[data_client_iter].character_name_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->data_client[data_client_iter].character_name_length, (uint64_t)packet->data_client[data_client_iter].character_name_length, (double)packet->data_client[data_client_iter].character_name_length);
for (uint32_t character_name_iter = 0; character_name_iter < packet->data_client[data_client_iter].character_name_length; character_name_iter++)
{
endian_write_i8_little(buffer + offset, packet->data_client[data_client_iter].character_name[character_name_iter]);
offset++;
}

} // data_client
endian_write_u32_little((uint8_t*)data_client_length_ptr, (u32)((uptr)buffer + (uptr)offset - (uptr)data_client_length_ptr - sizeof(u32)));

} break;

case Login_Packet_Kind_TunnelAppPacketServerToClient:
{
printf("[*] Packing TunnelAppPacketServerToClient...\n");
Login_Packet_TunnelAppPacketServerToClient* packet = static_cast<Login_Packet_TunnelAppPacketServerToClient*>(packet_ptr);

endian_write_u8_little(buffer + offset, 0x11);
offset += sizeof(u8);

// u32 server_id
endian_write_u32_little(buffer + offset, packet->server_id);
offset += sizeof(u32);
printf("-- server_id               \t%lld\t%llxh\t%f\n", (i64)packet->server_id, (uint64_t)packet->server_id, (double)packet->server_id);

// u32 unk1
endian_write_u32_little(buffer + offset, packet->unk1);
offset += sizeof(u32);
printf("-- unk1                    \t%lld\t%llxh\t%f\n", (i64)packet->unk1, (uint64_t)packet->unk1, (double)packet->unk1);

// stream data_server
void* data_server_length_ptr = buffer + offset;
offset += sizeof(u32);
printf("-- STREAM_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->data_server_length, (uint64_t)packet->data_server_length, (double)packet->data_server_length);

uint32_t will_pack_data_server = packet->data_server_length == ~(u32)0 ? 0 : 1;
for (uint32_t data_server_iter = 0; data_server_iter < will_pack_data_server; data_server_iter++)
{
// u8 tunnel_op_code
endian_write_u8_little(buffer + offset, packet->data_server[data_server_iter].tunnel_op_code);
offset += sizeof(u8);
printf("-- tunnel_op_code          \t%lld\t%llxh\t%f\n", (i64)packet->data_server[data_server_iter].tunnel_op_code, (uint64_t)packet->data_server[data_server_iter].tunnel_op_code, (double)packet->data_server[data_server_iter].tunnel_op_code);

// u8 sub_op_code
endian_write_u8_little(buffer + offset, packet->data_server[data_server_iter].sub_op_code);
offset += sizeof(u8);
printf("-- sub_op_code             \t%lld\t%llxh\t%f\n", (i64)packet->data_server[data_server_iter].sub_op_code, (uint64_t)packet->data_server[data_server_iter].sub_op_code, (double)packet->data_server[data_server_iter].sub_op_code);

// string character_name
endian_write_u32_little(buffer + offset, packet->data_server[data_server_iter].character_name_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->data_server[data_server_iter].character_name_length, (uint64_t)packet->data_server[data_server_iter].character_name_length, (double)packet->data_server[data_server_iter].character_name_length);
for (uint32_t character_name_iter = 0; character_name_iter < packet->data_server[data_server_iter].character_name_length; character_name_iter++)
{
endian_write_i8_little(buffer + offset, packet->data_server[data_server_iter].character_name[character_name_iter]);
offset++;
}

// string character_name2
endian_write_u32_little(buffer + offset, packet->data_server[data_server_iter].character_name2_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->data_server[data_server_iter].character_name2_length, (uint64_t)packet->data_server[data_server_iter].character_name2_length, (double)packet->data_server[data_server_iter].character_name2_length);
for (uint32_t character_name2_iter = 0; character_name2_iter < packet->data_server[data_server_iter].character_name2_length; character_name2_iter++)
{
endian_write_i8_little(buffer + offset, packet->data_server[data_server_iter].character_name2[character_name2_iter]);
offset++;
}

// u32 status
endian_write_u32_little(buffer + offset, packet->data_server[data_server_iter].status);
offset += sizeof(u32);
printf("-- status                  \t%lld\t%llxh\t%f\n", (i64)packet->data_server[data_server_iter].status, (uint64_t)packet->data_server[data_server_iter].status, (double)packet->data_server[data_server_iter].status);

} // data_server
endian_write_u32_little((uint8_t*)data_server_length_ptr, (u32)((uptr)buffer + (uptr)offset - (uptr)data_server_length_ptr - sizeof(u32)));

} break;

case Login_Packet_Kind_CharacterTransferRequest:
{
printf("[*] Packing CharacterTransferRequest...\n");
endian_write_u8_little(buffer + offset, 0x12);
offset += sizeof(u8);

} break;

case Login_Packet_Kind_CharacterTransferReply:
{
printf("[*] Packing CharacterTransferReply...\n");
endian_write_u8_little(buffer + offset, 0x13);
offset += sizeof(u8);

} break;

case Login_Packet_Kind_CharacterDeleteReply:
{
printf("[*] Packing CharacterDeleteReply...\n");
Login_Packet_CharacterDeleteReply* packet = static_cast<Login_Packet_CharacterDeleteReply*>(packet_ptr);

endian_write_u8_little(buffer + offset, 0xa);
offset += sizeof(u8);

// u64 character_id
endian_write_u64_little(buffer + offset, packet->character_id);
offset += sizeof(u64);
printf("-- character_id            \t%lld\t%llxh\t%f\n", (i64)packet->character_id, (uint64_t)packet->character_id, (double)packet->character_id);

// u32 status
endian_write_u32_little(buffer + offset, packet->status);
offset += sizeof(u32);
printf("-- status                  \t%lld\t%llxh\t%f\n", (i64)packet->status, (uint64_t)packet->status, (double)packet->status);

// bytes payload3
endian_write_u32_little(buffer + offset, packet->payload3_length);
offset += sizeof(u32);
printf("-- BYTES_LENGTH            \t%lld\t%llxh\t%f\n", (i64)packet->payload3_length, (uint64_t)packet->payload3_length, (double)packet->payload3_length);
for (uint32_t payload3_iter = 0; payload3_iter < packet->payload3_length; payload3_iter++)
{
endian_write_u8_little(buffer + offset, packet->payload3[payload3_iter]);
offset++;
}

} break;

case Login_Packet_Kind_CharacterSelectInfoRequest:
{
printf("[*] Packing CharacterSelectInfoRequest...\n");
endian_write_u8_little(buffer + offset, 0xb);
offset += sizeof(u8);

} break;

case Login_Packet_Kind_CharacterSelectInfoReply:
{
printf("[*] Packing CharacterSelectInfoReply...\n");
Login_Packet_CharacterSelectInfoReply* packet = static_cast<Login_Packet_CharacterSelectInfoReply*>(packet_ptr);

endian_write_u8_little(buffer + offset, 0xc);
offset += sizeof(u8);

// u32 character_status
endian_write_u32_little(buffer + offset, packet->character_status);
offset += sizeof(u32);
printf("-- character_status        \t%lld\t%llxh\t%f\n", (i64)packet->character_status, (uint64_t)packet->character_status, (double)packet->character_status);

// b8 can_bypass_server_lock
endian_write_b8_little(buffer + offset, packet->can_bypass_server_lock);
offset += sizeof(b8);
printf("-- can_bypass_server_lock  \t%lld\t%llxh\t%f\n", (i64)packet->can_bypass_server_lock, (uint64_t)packet->can_bypass_server_lock, (double)packet->can_bypass_server_lock);

// list stats_item_def_2
endian_write_u32_little(buffer + offset, packet->stats_item_def_2_count);
offset += sizeof(u32);
printf("-- LIST_COUNT              \t%lld\t%llxh\t%f\n", (i64)packet->stats_item_def_2_count, (uint64_t)packet->stats_item_def_2_count, (double)packet->stats_item_def_2_count);

for (uint32_t stats_item_def_2_iter = 0; stats_item_def_2_iter < packet->stats_item_def_2_count; stats_item_def_2_iter++)
{
// u32 unk_dword_9
endian_write_u32_little(buffer + offset, packet->stats_item_def_2[stats_item_def_2_iter].unk_dword_9);
offset += sizeof(u32);
printf("-- unk_dword_9             \t%lld\t%llxh\t%f\n", (i64)packet->stats_item_def_2[stats_item_def_2_iter].unk_dword_9, (uint64_t)packet->stats_item_def_2[stats_item_def_2_iter].unk_dword_9, (double)packet->stats_item_def_2[stats_item_def_2_iter].unk_dword_9);

// u32 stat_id
endian_write_u32_little(buffer + offset, packet->stats_item_def_2[stats_item_def_2_iter].stat_id);
offset += sizeof(u32);
printf("-- stat_id                 \t%lld\t%llxh\t%f\n", (i64)packet->stats_item_def_2[stats_item_def_2_iter].stat_id, (uint64_t)packet->stats_item_def_2[stats_item_def_2_iter].stat_id, (double)packet->stats_item_def_2[stats_item_def_2_iter].stat_id);

endian_write_u8_little(buffer + offset, packet->stats_item_def_2[stats_item_def_2_iter].variabletype8_case);
offset += sizeof(u8);
printf("-- SWITCH_CASE             \t%lld\t%llxh\t%f\n", (i64)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8_case, (uint64_t)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8_case, (double)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8_case);

switch(packet->stats_item_def_2[stats_item_def_2_iter].variabletype8_case)
{
case 0:
{
// u32 base
endian_write_u32_little(buffer + offset, packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue0.base);
offset += sizeof(u32);
printf("-- base                    \t%lld\t%llxh\t%f\n", (i64)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue0.base, (uint64_t)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue0.base, (double)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue0.base);

// u32 modifier
endian_write_u32_little(buffer + offset, packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue0.modifier);
offset += sizeof(u32);
printf("-- modifier                \t%lld\t%llxh\t%f\n", (i64)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue0.modifier, (uint64_t)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue0.modifier, (double)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue0.modifier);

} break;
case 1:
{
// f32 base
endian_write_f32_little(buffer + offset, packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue1.base);
offset += sizeof(f32);
printf("-- base                    \t%lld\t%llxh\t%f\n", (i64)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue1.base, (uint64_t)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue1.base, (double)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue1.base);

// f32 modifier
endian_write_f32_little(buffer + offset, packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue1.modifier);
offset += sizeof(f32);
printf("-- modifier                \t%lld\t%llxh\t%f\n", (i64)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue1.modifier, (uint64_t)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue1.modifier, (double)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue1.modifier);

} break;
};
// u32 unk_dword_10
endian_write_u32_little(buffer + offset, packet->stats_item_def_2[stats_item_def_2_iter].unk_dword_10);
offset += sizeof(u32);
printf("-- unk_dword_10            \t%lld\t%llxh\t%f\n", (i64)packet->stats_item_def_2[stats_item_def_2_iter].unk_dword_10, (uint64_t)packet->stats_item_def_2[stats_item_def_2_iter].unk_dword_10, (double)packet->stats_item_def_2[stats_item_def_2_iter].unk_dword_10);

} // stats_item_def_2

// list itemDefinitions
endian_write_u32_little(buffer + offset, packet->itemDefinitions_count);
offset += sizeof(u32);
printf("-- LIST_COUNT              \t%lld\t%llxh\t%f\n", (i64)packet->itemDefinitions_count, (uint64_t)packet->itemDefinitions_count, (double)packet->itemDefinitions_count);

for (uint32_t itemDefinitions_iter = 0; itemDefinitions_iter < packet->itemDefinitions_count; itemDefinitions_iter++)
{
// u32 ID
endian_write_u32_little(buffer + offset, packet->itemDefinitions[itemDefinitions_iter].ID);
offset += sizeof(u32);
printf("-- ID                      \t%lld\t%llxh\t%f\n", (i64)packet->itemDefinitions[itemDefinitions_iter].ID, (uint64_t)packet->itemDefinitions[itemDefinitions_iter].ID, (double)packet->itemDefinitions[itemDefinitions_iter].ID);

} // itemDefinitions

// list item_defs
endian_write_u32_little(buffer + offset, packet->item_defs_count);
offset += sizeof(u32);
printf("-- LIST_COUNT              \t%lld\t%llxh\t%f\n", (i64)packet->item_defs_count, (uint64_t)packet->item_defs_count, (double)packet->item_defs_count);

for (uint32_t item_defs_iter = 0; item_defs_iter < packet->item_defs_count; item_defs_iter++)
{
// u32 defs_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].defs_id);
offset += sizeof(u32);
printf("-- defs_id                 \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].defs_id, (uint64_t)packet->item_defs[item_defs_iter].defs_id, (double)packet->item_defs[item_defs_iter].defs_id);

// u8 bitflags1
endian_write_u8_little(buffer + offset, packet->item_defs[item_defs_iter].bitflags1);
offset += sizeof(u8);
printf("-- bitflags1               \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].bitflags1, (uint64_t)packet->item_defs[item_defs_iter].bitflags1, (double)packet->item_defs[item_defs_iter].bitflags1);

// u8 bitflags2
endian_write_u8_little(buffer + offset, packet->item_defs[item_defs_iter].bitflags2);
offset += sizeof(u8);
printf("-- bitflags2               \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].bitflags2, (uint64_t)packet->item_defs[item_defs_iter].bitflags2, (double)packet->item_defs[item_defs_iter].bitflags2);

// u32 name_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].name_id);
offset += sizeof(u32);
printf("-- name_id                 \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].name_id, (uint64_t)packet->item_defs[item_defs_iter].name_id, (double)packet->item_defs[item_defs_iter].name_id);

// u32 description_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].description_id);
offset += sizeof(u32);
printf("-- description_id          \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].description_id, (uint64_t)packet->item_defs[item_defs_iter].description_id, (double)packet->item_defs[item_defs_iter].description_id);

// u32 content_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].content_id);
offset += sizeof(u32);
printf("-- content_id              \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].content_id, (uint64_t)packet->item_defs[item_defs_iter].content_id, (double)packet->item_defs[item_defs_iter].content_id);

// u32 image_set_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].image_set_id);
offset += sizeof(u32);
printf("-- image_set_id            \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].image_set_id, (uint64_t)packet->item_defs[item_defs_iter].image_set_id, (double)packet->item_defs[item_defs_iter].image_set_id);

// u32 tint_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].tint_id);
offset += sizeof(u32);
printf("-- tint_id                 \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].tint_id, (uint64_t)packet->item_defs[item_defs_iter].tint_id, (double)packet->item_defs[item_defs_iter].tint_id);

// u32 hud_image_set_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].hud_image_set_id);
offset += sizeof(u32);
printf("-- hud_image_set_id        \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].hud_image_set_id, (uint64_t)packet->item_defs[item_defs_iter].hud_image_set_id, (double)packet->item_defs[item_defs_iter].hud_image_set_id);

// u32 unk_dword_1
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].unk_dword_1);
offset += sizeof(u32);
printf("-- unk_dword_1             \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].unk_dword_1, (uint64_t)packet->item_defs[item_defs_iter].unk_dword_1, (double)packet->item_defs[item_defs_iter].unk_dword_1);

// u32 unk_dword_2
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].unk_dword_2);
offset += sizeof(u32);
printf("-- unk_dword_2             \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].unk_dword_2, (uint64_t)packet->item_defs[item_defs_iter].unk_dword_2, (double)packet->item_defs[item_defs_iter].unk_dword_2);

// u32 cost
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].cost);
offset += sizeof(u32);
printf("-- cost                    \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].cost, (uint64_t)packet->item_defs[item_defs_iter].cost, (double)packet->item_defs[item_defs_iter].cost);

// u32 item_class
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].item_class);
offset += sizeof(u32);
printf("-- item_class              \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].item_class, (uint64_t)packet->item_defs[item_defs_iter].item_class, (double)packet->item_defs[item_defs_iter].item_class);

// u32 profile_override
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].profile_override);
offset += sizeof(u32);
printf("-- profile_override        \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].profile_override, (uint64_t)packet->item_defs[item_defs_iter].profile_override, (double)packet->item_defs[item_defs_iter].profile_override);

// string model_name
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].model_name_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].model_name_length, (uint64_t)packet->item_defs[item_defs_iter].model_name_length, (double)packet->item_defs[item_defs_iter].model_name_length);
for (uint32_t model_name_iter = 0; model_name_iter < packet->item_defs[item_defs_iter].model_name_length; model_name_iter++)
{
endian_write_i8_little(buffer + offset, packet->item_defs[item_defs_iter].model_name[model_name_iter]);
offset++;
}

// string texture_alias
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].texture_alias_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].texture_alias_length, (uint64_t)packet->item_defs[item_defs_iter].texture_alias_length, (double)packet->item_defs[item_defs_iter].texture_alias_length);
for (uint32_t texture_alias_iter = 0; texture_alias_iter < packet->item_defs[item_defs_iter].texture_alias_length; texture_alias_iter++)
{
endian_write_i8_little(buffer + offset, packet->item_defs[item_defs_iter].texture_alias[texture_alias_iter]);
offset++;
}

// u32 gender_usage
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].gender_usage);
offset += sizeof(u32);
printf("-- gender_usage            \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].gender_usage, (uint64_t)packet->item_defs[item_defs_iter].gender_usage, (double)packet->item_defs[item_defs_iter].gender_usage);

// u32 item_type
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].item_type);
offset += sizeof(u32);
printf("-- item_type               \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].item_type, (uint64_t)packet->item_defs[item_defs_iter].item_type, (double)packet->item_defs[item_defs_iter].item_type);

// u32 category_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].category_id);
offset += sizeof(u32);
printf("-- category_id             \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].category_id, (uint64_t)packet->item_defs[item_defs_iter].category_id, (double)packet->item_defs[item_defs_iter].category_id);

// u32 weapon_trail_effect_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].weapon_trail_effect_id);
offset += sizeof(u32);
printf("-- weapon_trail_effect_id  \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].weapon_trail_effect_id, (uint64_t)packet->item_defs[item_defs_iter].weapon_trail_effect_id, (double)packet->item_defs[item_defs_iter].weapon_trail_effect_id);

// u32 composite_effect_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].composite_effect_id);
offset += sizeof(u32);
printf("-- composite_effect_id     \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].composite_effect_id, (uint64_t)packet->item_defs[item_defs_iter].composite_effect_id, (double)packet->item_defs[item_defs_iter].composite_effect_id);

// u32 power_rating
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].power_rating);
offset += sizeof(u32);
printf("-- power_rating            \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].power_rating, (uint64_t)packet->item_defs[item_defs_iter].power_rating, (double)packet->item_defs[item_defs_iter].power_rating);

// u32 min_profile_rank
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].min_profile_rank);
offset += sizeof(u32);
printf("-- min_profile_rank        \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].min_profile_rank, (uint64_t)packet->item_defs[item_defs_iter].min_profile_rank, (double)packet->item_defs[item_defs_iter].min_profile_rank);

// u32 rarity
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].rarity);
offset += sizeof(u32);
printf("-- rarity                  \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].rarity, (uint64_t)packet->item_defs[item_defs_iter].rarity, (double)packet->item_defs[item_defs_iter].rarity);

// u32 activatable_ability_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].activatable_ability_id);
offset += sizeof(u32);
printf("-- activatable_ability_id  \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].activatable_ability_id, (uint64_t)packet->item_defs[item_defs_iter].activatable_ability_id, (double)packet->item_defs[item_defs_iter].activatable_ability_id);

// u32 activatable_ability_set_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].activatable_ability_set_id);
offset += sizeof(u32);
printf("-- activatable_ability_set_id\t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].activatable_ability_set_id, (uint64_t)packet->item_defs[item_defs_iter].activatable_ability_set_id, (double)packet->item_defs[item_defs_iter].activatable_ability_set_id);

// u32 passive_ability_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].passive_ability_id);
offset += sizeof(u32);
printf("-- passive_ability_id      \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].passive_ability_id, (uint64_t)packet->item_defs[item_defs_iter].passive_ability_id, (double)packet->item_defs[item_defs_iter].passive_ability_id);

// u32 passive_ability_set_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].passive_ability_set_id);
offset += sizeof(u32);
printf("-- passive_ability_set_id  \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].passive_ability_set_id, (uint64_t)packet->item_defs[item_defs_iter].passive_ability_set_id, (double)packet->item_defs[item_defs_iter].passive_ability_set_id);

// u32 max_stack_size
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].max_stack_size);
offset += sizeof(u32);
printf("-- max_stack_size          \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].max_stack_size, (uint64_t)packet->item_defs[item_defs_iter].max_stack_size, (double)packet->item_defs[item_defs_iter].max_stack_size);

// u32 min_stack_size
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].min_stack_size);
offset += sizeof(u32);
printf("-- min_stack_size          \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].min_stack_size, (uint64_t)packet->item_defs[item_defs_iter].min_stack_size, (double)packet->item_defs[item_defs_iter].min_stack_size);

// string tint_alias
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].tint_alias_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].tint_alias_length, (uint64_t)packet->item_defs[item_defs_iter].tint_alias_length, (double)packet->item_defs[item_defs_iter].tint_alias_length);
for (uint32_t tint_alias_iter = 0; tint_alias_iter < packet->item_defs[item_defs_iter].tint_alias_length; tint_alias_iter++)
{
endian_write_i8_little(buffer + offset, packet->item_defs[item_defs_iter].tint_alias[tint_alias_iter]);
offset++;
}

// u32 tint_group_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].tint_group_id);
offset += sizeof(u32);
printf("-- tint_group_id           \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].tint_group_id, (uint64_t)packet->item_defs[item_defs_iter].tint_group_id, (double)packet->item_defs[item_defs_iter].tint_group_id);

// u32 member_discount
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].member_discount);
offset += sizeof(u32);
printf("-- member_discount         \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].member_discount, (uint64_t)packet->item_defs[item_defs_iter].member_discount, (double)packet->item_defs[item_defs_iter].member_discount);

// u32 vip_rank_required
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].vip_rank_required);
offset += sizeof(u32);
printf("-- vip_rank_required       \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].vip_rank_required, (uint64_t)packet->item_defs[item_defs_iter].vip_rank_required, (double)packet->item_defs[item_defs_iter].vip_rank_required);

// u32 race_set_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].race_set_id);
offset += sizeof(u32);
printf("-- race_set_id             \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].race_set_id, (uint64_t)packet->item_defs[item_defs_iter].race_set_id, (double)packet->item_defs[item_defs_iter].race_set_id);

// u32 ui_model_camera_id_1
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].ui_model_camera_id_1);
offset += sizeof(u32);
printf("-- ui_model_camera_id_1    \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].ui_model_camera_id_1, (uint64_t)packet->item_defs[item_defs_iter].ui_model_camera_id_1, (double)packet->item_defs[item_defs_iter].ui_model_camera_id_1);

// u32 equip_count_max
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].equip_count_max);
offset += sizeof(u32);
printf("-- equip_count_max         \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].equip_count_max, (uint64_t)packet->item_defs[item_defs_iter].equip_count_max, (double)packet->item_defs[item_defs_iter].equip_count_max);

// i32 curreny_type
endian_write_i32_little(buffer + offset, packet->item_defs[item_defs_iter].curreny_type);
offset += sizeof(i32);
printf("-- curreny_type            \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].curreny_type, (uint64_t)packet->item_defs[item_defs_iter].curreny_type, (double)packet->item_defs[item_defs_iter].curreny_type);

// u32 datasheet_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].datasheet_id);
offset += sizeof(u32);
printf("-- datasheet_id            \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].datasheet_id, (uint64_t)packet->item_defs[item_defs_iter].datasheet_id, (double)packet->item_defs[item_defs_iter].datasheet_id);

// u32 item_type_1
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].item_type_1);
offset += sizeof(u32);
printf("-- item_type_1             \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].item_type_1, (uint64_t)packet->item_defs[item_defs_iter].item_type_1, (double)packet->item_defs[item_defs_iter].item_type_1);

// u32 skill_set_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].skill_set_id);
offset += sizeof(u32);
printf("-- skill_set_id            \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].skill_set_id, (uint64_t)packet->item_defs[item_defs_iter].skill_set_id, (double)packet->item_defs[item_defs_iter].skill_set_id);

// string overlay_texture
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].overlay_texture_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].overlay_texture_length, (uint64_t)packet->item_defs[item_defs_iter].overlay_texture_length, (double)packet->item_defs[item_defs_iter].overlay_texture_length);
for (uint32_t overlay_texture_iter = 0; overlay_texture_iter < packet->item_defs[item_defs_iter].overlay_texture_length; overlay_texture_iter++)
{
endian_write_i8_little(buffer + offset, packet->item_defs[item_defs_iter].overlay_texture[overlay_texture_iter]);
offset++;
}

// string decal_slot
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].decal_slot_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].decal_slot_length, (uint64_t)packet->item_defs[item_defs_iter].decal_slot_length, (double)packet->item_defs[item_defs_iter].decal_slot_length);
for (uint32_t decal_slot_iter = 0; decal_slot_iter < packet->item_defs[item_defs_iter].decal_slot_length; decal_slot_iter++)
{
endian_write_i8_little(buffer + offset, packet->item_defs[item_defs_iter].decal_slot[decal_slot_iter]);
offset++;
}

// u32 overlay_adjustment
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].overlay_adjustment);
offset += sizeof(u32);
printf("-- overlay_adjustment      \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].overlay_adjustment, (uint64_t)packet->item_defs[item_defs_iter].overlay_adjustment, (double)packet->item_defs[item_defs_iter].overlay_adjustment);

// u32 trial_duration_sec
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].trial_duration_sec);
offset += sizeof(u32);
printf("-- trial_duration_sec      \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].trial_duration_sec, (uint64_t)packet->item_defs[item_defs_iter].trial_duration_sec, (double)packet->item_defs[item_defs_iter].trial_duration_sec);

// u32 next_trial_delay_sec
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].next_trial_delay_sec);
offset += sizeof(u32);
printf("-- next_trial_delay_sec    \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].next_trial_delay_sec, (uint64_t)packet->item_defs[item_defs_iter].next_trial_delay_sec, (double)packet->item_defs[item_defs_iter].next_trial_delay_sec);

// u32 client_use_requirement
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].client_use_requirement);
offset += sizeof(u32);
printf("-- client_use_requirement  \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].client_use_requirement, (uint64_t)packet->item_defs[item_defs_iter].client_use_requirement, (double)packet->item_defs[item_defs_iter].client_use_requirement);

// string override_appearance
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].override_appearance_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].override_appearance_length, (uint64_t)packet->item_defs[item_defs_iter].override_appearance_length, (double)packet->item_defs[item_defs_iter].override_appearance_length);
for (uint32_t override_appearance_iter = 0; override_appearance_iter < packet->item_defs[item_defs_iter].override_appearance_length; override_appearance_iter++)
{
endian_write_i8_little(buffer + offset, packet->item_defs[item_defs_iter].override_appearance[override_appearance_iter]);
offset++;
}

// u32 override_camera_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].override_camera_id);
offset += sizeof(u32);
printf("-- override_camera_id      \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].override_camera_id, (uint64_t)packet->item_defs[item_defs_iter].override_camera_id, (double)packet->item_defs[item_defs_iter].override_camera_id);

// u32 unk_dword_3
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].unk_dword_3);
offset += sizeof(u32);
printf("-- unk_dword_3             \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].unk_dword_3, (uint64_t)packet->item_defs[item_defs_iter].unk_dword_3, (double)packet->item_defs[item_defs_iter].unk_dword_3);

// u32 unk_dword_4
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].unk_dword_4);
offset += sizeof(u32);
printf("-- unk_dword_4             \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].unk_dword_4, (uint64_t)packet->item_defs[item_defs_iter].unk_dword_4, (double)packet->item_defs[item_defs_iter].unk_dword_4);

// u32 unk_dword_5
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].unk_dword_5);
offset += sizeof(u32);
printf("-- unk_dword_5             \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].unk_dword_5, (uint64_t)packet->item_defs[item_defs_iter].unk_dword_5, (double)packet->item_defs[item_defs_iter].unk_dword_5);

// u32 bulk
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].bulk);
offset += sizeof(u32);
printf("-- bulk                    \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].bulk, (uint64_t)packet->item_defs[item_defs_iter].bulk, (double)packet->item_defs[item_defs_iter].bulk);

// u32 active_equip_slot_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].active_equip_slot_id);
offset += sizeof(u32);
printf("-- active_equip_slot_id    \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].active_equip_slot_id, (uint64_t)packet->item_defs[item_defs_iter].active_equip_slot_id, (double)packet->item_defs[item_defs_iter].active_equip_slot_id);

// u32 passive_equip_slot_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].passive_equip_slot_id);
offset += sizeof(u32);
printf("-- passive_equip_slot_id   \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].passive_equip_slot_id, (uint64_t)packet->item_defs[item_defs_iter].passive_equip_slot_id, (double)packet->item_defs[item_defs_iter].passive_equip_slot_id);

// u32 passive_equip_slot_group_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].passive_equip_slot_group_id);
offset += sizeof(u32);
printf("-- passive_equip_slot_group_id\t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].passive_equip_slot_group_id, (uint64_t)packet->item_defs[item_defs_iter].passive_equip_slot_group_id, (double)packet->item_defs[item_defs_iter].passive_equip_slot_group_id);

// u32 unk_dword_6
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].unk_dword_6);
offset += sizeof(u32);
printf("-- unk_dword_6             \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].unk_dword_6, (uint64_t)packet->item_defs[item_defs_iter].unk_dword_6, (double)packet->item_defs[item_defs_iter].unk_dword_6);

// u32 grinder_reward_set_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].grinder_reward_set_id);
offset += sizeof(u32);
printf("-- grinder_reward_set_id   \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].grinder_reward_set_id, (uint64_t)packet->item_defs[item_defs_iter].grinder_reward_set_id, (double)packet->item_defs[item_defs_iter].grinder_reward_set_id);

// u32 build_bar_group_id
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].build_bar_group_id);
offset += sizeof(u32);
printf("-- build_bar_group_id      \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].build_bar_group_id, (uint64_t)packet->item_defs[item_defs_iter].build_bar_group_id, (double)packet->item_defs[item_defs_iter].build_bar_group_id);

// string unk_string_1
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].unk_string_1_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].unk_string_1_length, (uint64_t)packet->item_defs[item_defs_iter].unk_string_1_length, (double)packet->item_defs[item_defs_iter].unk_string_1_length);
for (uint32_t unk_string_1_iter = 0; unk_string_1_iter < packet->item_defs[item_defs_iter].unk_string_1_length; unk_string_1_iter++)
{
endian_write_i8_little(buffer + offset, packet->item_defs[item_defs_iter].unk_string_1[unk_string_1_iter]);
offset++;
}

// b8 unk_bool_1
endian_write_b8_little(buffer + offset, packet->item_defs[item_defs_iter].unk_bool_1);
offset += sizeof(b8);
printf("-- unk_bool_1              \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].unk_bool_1, (uint64_t)packet->item_defs[item_defs_iter].unk_bool_1, (double)packet->item_defs[item_defs_iter].unk_bool_1);

// b8 is_armor
endian_write_b8_little(buffer + offset, packet->item_defs[item_defs_iter].is_armor);
offset += sizeof(b8);
printf("-- is_armor                \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].is_armor, (uint64_t)packet->item_defs[item_defs_iter].is_armor, (double)packet->item_defs[item_defs_iter].is_armor);

// u32 unk_dword_7
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].unk_dword_7);
offset += sizeof(u32);
printf("-- unk_dword_7             \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].unk_dword_7, (uint64_t)packet->item_defs[item_defs_iter].unk_dword_7, (double)packet->item_defs[item_defs_iter].unk_dword_7);

// u32 param1
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].param1);
offset += sizeof(u32);
printf("-- param1                  \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].param1, (uint64_t)packet->item_defs[item_defs_iter].param1, (double)packet->item_defs[item_defs_iter].param1);

// u32 param2
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].param2);
offset += sizeof(u32);
printf("-- param2                  \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].param2, (uint64_t)packet->item_defs[item_defs_iter].param2, (double)packet->item_defs[item_defs_iter].param2);

// u32 param3
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].param3);
offset += sizeof(u32);
printf("-- param3                  \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].param3, (uint64_t)packet->item_defs[item_defs_iter].param3, (double)packet->item_defs[item_defs_iter].param3);

// string string_param1
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].string_param1_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].string_param1_length, (uint64_t)packet->item_defs[item_defs_iter].string_param1_length, (double)packet->item_defs[item_defs_iter].string_param1_length);
for (uint32_t string_param1_iter = 0; string_param1_iter < packet->item_defs[item_defs_iter].string_param1_length; string_param1_iter++)
{
endian_write_i8_little(buffer + offset, packet->item_defs[item_defs_iter].string_param1[string_param1_iter]);
offset++;
}

// u32 ui_model_camera_id_2
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].ui_model_camera_id_2);
offset += sizeof(u32);
printf("-- ui_model_camera_id_2    \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].ui_model_camera_id_2, (uint64_t)packet->item_defs[item_defs_iter].ui_model_camera_id_2, (double)packet->item_defs[item_defs_iter].ui_model_camera_id_2);

// u32 unk_dword_8
endian_write_u32_little(buffer + offset, packet->item_defs[item_defs_iter].unk_dword_8);
offset += sizeof(u32);
printf("-- unk_dword_8             \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].unk_dword_8, (uint64_t)packet->item_defs[item_defs_iter].unk_dword_8, (double)packet->item_defs[item_defs_iter].unk_dword_8);

// i32 scrap_value_override
endian_write_i32_little(buffer + offset, packet->item_defs[item_defs_iter].scrap_value_override);
offset += sizeof(i32);
printf("-- scrap_value_override    \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].scrap_value_override, (uint64_t)packet->item_defs[item_defs_iter].scrap_value_override, (double)packet->item_defs[item_defs_iter].scrap_value_override);

} // item_defs

// list loadoutSlots
endian_write_u32_little(buffer + offset, packet->loadoutSlots_count);
offset += sizeof(u32);
printf("-- LIST_COUNT              \t%lld\t%llxh\t%f\n", (i64)packet->loadoutSlots_count, (uint64_t)packet->loadoutSlots_count, (double)packet->loadoutSlots_count);

for (uint32_t loadoutSlots_iter = 0; loadoutSlots_iter < packet->loadoutSlots_count; loadoutSlots_iter++)
{
// u32 hotbarSlotId
endian_write_u32_little(buffer + offset, packet->loadoutSlots[loadoutSlots_iter].hotbarSlotId);
offset += sizeof(u32);
printf("-- hotbarSlotId            \t%lld\t%llxh\t%f\n", (i64)packet->loadoutSlots[loadoutSlots_iter].hotbarSlotId, (uint64_t)packet->loadoutSlots[loadoutSlots_iter].hotbarSlotId, (double)packet->loadoutSlots[loadoutSlots_iter].hotbarSlotId);

// u32 loadoutId
endian_write_u32_little(buffer + offset, packet->loadoutSlots[loadoutSlots_iter].loadoutId);
offset += sizeof(u32);
printf("-- loadoutId               \t%lld\t%llxh\t%f\n", (i64)packet->loadoutSlots[loadoutSlots_iter].loadoutId, (uint64_t)packet->loadoutSlots[loadoutSlots_iter].loadoutId, (double)packet->loadoutSlots[loadoutSlots_iter].loadoutId);

// u32 slotId
endian_write_u32_little(buffer + offset, packet->loadoutSlots[loadoutSlots_iter].slotId);
offset += sizeof(u32);
printf("-- slotId                  \t%lld\t%llxh\t%f\n", (i64)packet->loadoutSlots[loadoutSlots_iter].slotId, (uint64_t)packet->loadoutSlots[loadoutSlots_iter].slotId, (double)packet->loadoutSlots[loadoutSlots_iter].slotId);

// u32 itemDefId
endian_write_u32_little(buffer + offset, packet->loadoutSlots[loadoutSlots_iter].itemDefId);
offset += sizeof(u32);
printf("-- itemDefId               \t%lld\t%llxh\t%f\n", (i64)packet->loadoutSlots[loadoutSlots_iter].itemDefId, (uint64_t)packet->loadoutSlots[loadoutSlots_iter].itemDefId, (double)packet->loadoutSlots[loadoutSlots_iter].itemDefId);

// u64 loadoutItemGuid
endian_write_u64_little(buffer + offset, packet->loadoutSlots[loadoutSlots_iter].loadoutItemGuid);
offset += sizeof(u64);
printf("-- loadoutItemGuid         \t%lld\t%llxh\t%f\n", (i64)packet->loadoutSlots[loadoutSlots_iter].loadoutItemGuid, (uint64_t)packet->loadoutSlots[loadoutSlots_iter].loadoutItemGuid, (double)packet->loadoutSlots[loadoutSlots_iter].loadoutItemGuid);

// u8 unkByte1
endian_write_u8_little(buffer + offset, packet->loadoutSlots[loadoutSlots_iter].unkByte1);
offset += sizeof(u8);
printf("-- unkByte1                \t%lld\t%llxh\t%f\n", (i64)packet->loadoutSlots[loadoutSlots_iter].unkByte1, (uint64_t)packet->loadoutSlots[loadoutSlots_iter].unkByte1, (double)packet->loadoutSlots[loadoutSlots_iter].unkByte1);

// u32 unkDword1
endian_write_u32_little(buffer + offset, packet->loadoutSlots[loadoutSlots_iter].unkDword1);
offset += sizeof(u32);
printf("-- unkDword1               \t%lld\t%llxh\t%f\n", (i64)packet->loadoutSlots[loadoutSlots_iter].unkDword1, (uint64_t)packet->loadoutSlots[loadoutSlots_iter].unkDword1, (double)packet->loadoutSlots[loadoutSlots_iter].unkDword1);

} // loadoutSlots

// list characters
endian_write_u32_little(buffer + offset, packet->characters_count);
offset += sizeof(u32);
printf("-- LIST_COUNT              \t%lld\t%llxh\t%f\n", (i64)packet->characters_count, (uint64_t)packet->characters_count, (double)packet->characters_count);

for (uint32_t characters_iter = 0; characters_iter < packet->characters_count; characters_iter++)
{
// u64 charId
endian_write_u64_little(buffer + offset, packet->characters[characters_iter].charId);
offset += sizeof(u64);
printf("-- charId                  \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].charId, (uint64_t)packet->characters[characters_iter].charId, (double)packet->characters[characters_iter].charId);

// u32 serverId
endian_write_u32_little(buffer + offset, packet->characters[characters_iter].serverId);
offset += sizeof(u32);
printf("-- serverId                \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].serverId, (uint64_t)packet->characters[characters_iter].serverId, (double)packet->characters[characters_iter].serverId);

// u64 lastLoginDate
endian_write_u64_little(buffer + offset, packet->characters[characters_iter].lastLoginDate);
offset += sizeof(u64);
printf("-- lastLoginDate           \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].lastLoginDate, (uint64_t)packet->characters[characters_iter].lastLoginDate, (double)packet->characters[characters_iter].lastLoginDate);

// u32 nullField
endian_write_u32_little(buffer + offset, packet->characters[characters_iter].nullField);
offset += sizeof(u32);
printf("-- nullField               \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].nullField, (uint64_t)packet->characters[characters_iter].nullField, (double)packet->characters[characters_iter].nullField);

// u32 status
endian_write_u32_little(buffer + offset, packet->characters[characters_iter].status);
offset += sizeof(u32);
printf("-- status                  \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].status, (uint64_t)packet->characters[characters_iter].status, (double)packet->characters[characters_iter].status);

// stream payload
void* payload_length_ptr = buffer + offset;
offset += sizeof(u32);
printf("-- STREAM_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].payload_length, (uint64_t)packet->characters[characters_iter].payload_length, (double)packet->characters[characters_iter].payload_length);

uint32_t will_pack_payload = packet->characters[characters_iter].payload_length == ~(u32)0 ? 0 : 1;
for (uint32_t payload_iter = 0; payload_iter < will_pack_payload; payload_iter++)
{
// string name
endian_write_u32_little(buffer + offset, packet->characters[characters_iter].payload[payload_iter].name_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].payload[payload_iter].name_length, (uint64_t)packet->characters[characters_iter].payload[payload_iter].name_length, (double)packet->characters[characters_iter].payload[payload_iter].name_length);
for (uint32_t name_iter = 0; name_iter < packet->characters[characters_iter].payload[payload_iter].name_length; name_iter++)
{
endian_write_i8_little(buffer + offset, packet->characters[characters_iter].payload[payload_iter].name[name_iter]);
offset++;
}

// u8 empireId
endian_write_u8_little(buffer + offset, packet->characters[characters_iter].payload[payload_iter].empireId);
offset += sizeof(u8);
printf("-- empireId                \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].payload[payload_iter].empireId, (uint64_t)packet->characters[characters_iter].payload[payload_iter].empireId, (double)packet->characters[characters_iter].payload[payload_iter].empireId);

// u32 battleRank
endian_write_u32_little(buffer + offset, packet->characters[characters_iter].payload[payload_iter].battleRank);
offset += sizeof(u32);
printf("-- battleRank              \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].payload[payload_iter].battleRank, (uint64_t)packet->characters[characters_iter].payload[payload_iter].battleRank, (double)packet->characters[characters_iter].payload[payload_iter].battleRank);

// u32 nextBattleRankPercent
endian_write_u32_little(buffer + offset, packet->characters[characters_iter].payload[payload_iter].nextBattleRankPercent);
offset += sizeof(u32);
printf("-- nextBattleRankPercent   \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].payload[payload_iter].nextBattleRankPercent, (uint64_t)packet->characters[characters_iter].payload[payload_iter].nextBattleRankPercent, (double)packet->characters[characters_iter].payload[payload_iter].nextBattleRankPercent);

// u32 headId
endian_write_u32_little(buffer + offset, packet->characters[characters_iter].payload[payload_iter].headId);
offset += sizeof(u32);
printf("-- headId                  \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].payload[payload_iter].headId, (uint64_t)packet->characters[characters_iter].payload[payload_iter].headId, (double)packet->characters[characters_iter].payload[payload_iter].headId);

// u32 actorModelId
endian_write_u32_little(buffer + offset, packet->characters[characters_iter].payload[payload_iter].actorModelId);
offset += sizeof(u32);
printf("-- actorModelId            \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].payload[payload_iter].actorModelId, (uint64_t)packet->characters[characters_iter].payload[payload_iter].actorModelId, (double)packet->characters[characters_iter].payload[payload_iter].actorModelId);

// u32 gender
endian_write_u32_little(buffer + offset, packet->characters[characters_iter].payload[payload_iter].gender);
offset += sizeof(u32);
printf("-- gender                  \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].payload[payload_iter].gender, (uint64_t)packet->characters[characters_iter].payload[payload_iter].gender, (double)packet->characters[characters_iter].payload[payload_iter].gender);

// u32 profileId
endian_write_u32_little(buffer + offset, packet->characters[characters_iter].payload[payload_iter].profileId);
offset += sizeof(u32);
printf("-- profileId               \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].payload[payload_iter].profileId, (uint64_t)packet->characters[characters_iter].payload[payload_iter].profileId, (double)packet->characters[characters_iter].payload[payload_iter].profileId);

// u32 unkDword1
endian_write_u32_little(buffer + offset, packet->characters[characters_iter].payload[payload_iter].unkDword1);
offset += sizeof(u32);
printf("-- unkDword1               \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].payload[payload_iter].unkDword1, (uint64_t)packet->characters[characters_iter].payload[payload_iter].unkDword1, (double)packet->characters[characters_iter].payload[payload_iter].unkDword1);

// u32 unkDword2
endian_write_u32_little(buffer + offset, packet->characters[characters_iter].payload[payload_iter].unkDword2);
offset += sizeof(u32);
printf("-- unkDword2               \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].payload[payload_iter].unkDword2, (uint64_t)packet->characters[characters_iter].payload[payload_iter].unkDword2, (double)packet->characters[characters_iter].payload[payload_iter].unkDword2);

// u64 lastUseDate
endian_write_u64_little(buffer + offset, packet->characters[characters_iter].payload[payload_iter].lastUseDate);
offset += sizeof(u64);
printf("-- lastUseDate             \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].payload[payload_iter].lastUseDate, (uint64_t)packet->characters[characters_iter].payload[payload_iter].lastUseDate, (double)packet->characters[characters_iter].payload[payload_iter].lastUseDate);

} // payload
endian_write_u32_little((uint8_t*)payload_length_ptr, (u32)((uptr)buffer + (uptr)offset - (uptr)payload_length_ptr - sizeof(u32)));

} // characters

} break;

case Login_Packet_Kind_ServerListRequest:
{
printf("[*] Packing ServerListRequest...\n");
endian_write_u8_little(buffer + offset, 0xd);
offset += sizeof(u8);

} break;

case Login_Packet_Kind_ServerListReply:
{
printf("[*] Packing ServerListReply...\n");
Login_Packet_ServerListReply* packet = static_cast<Login_Packet_ServerListReply*>(packet_ptr);

endian_write_u8_little(buffer + offset, 0xe);
offset += sizeof(u8);

// list servers
endian_write_u32_little(buffer + offset, packet->servers_count);
offset += sizeof(u32);
printf("-- LIST_COUNT              \t%lld\t%llxh\t%f\n", (i64)packet->servers_count, (uint64_t)packet->servers_count, (double)packet->servers_count);

for (uint32_t servers_iter = 0; servers_iter < packet->servers_count; servers_iter++)
{
// u32 id
endian_write_u32_little(buffer + offset, packet->servers[servers_iter].id);
offset += sizeof(u32);
printf("-- id                      \t%lld\t%llxh\t%f\n", (i64)packet->servers[servers_iter].id, (uint64_t)packet->servers[servers_iter].id, (double)packet->servers[servers_iter].id);

// u32 state
endian_write_u32_little(buffer + offset, packet->servers[servers_iter].state);
offset += sizeof(u32);
printf("-- state                   \t%lld\t%llxh\t%f\n", (i64)packet->servers[servers_iter].state, (uint64_t)packet->servers[servers_iter].state, (double)packet->servers[servers_iter].state);

// b8 is_locked
endian_write_b8_little(buffer + offset, packet->servers[servers_iter].is_locked);
offset += sizeof(b8);
printf("-- is_locked               \t%lld\t%llxh\t%f\n", (i64)packet->servers[servers_iter].is_locked, (uint64_t)packet->servers[servers_iter].is_locked, (double)packet->servers[servers_iter].is_locked);

// string name
endian_write_u32_little(buffer + offset, packet->servers[servers_iter].name_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->servers[servers_iter].name_length, (uint64_t)packet->servers[servers_iter].name_length, (double)packet->servers[servers_iter].name_length);
for (uint32_t name_iter = 0; name_iter < packet->servers[servers_iter].name_length; name_iter++)
{
endian_write_i8_little(buffer + offset, packet->servers[servers_iter].name[name_iter]);
offset++;
}

// u32 name_id
endian_write_u32_little(buffer + offset, packet->servers[servers_iter].name_id);
offset += sizeof(u32);
printf("-- name_id                 \t%lld\t%llxh\t%f\n", (i64)packet->servers[servers_iter].name_id, (uint64_t)packet->servers[servers_iter].name_id, (double)packet->servers[servers_iter].name_id);

// string description
endian_write_u32_little(buffer + offset, packet->servers[servers_iter].description_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->servers[servers_iter].description_length, (uint64_t)packet->servers[servers_iter].description_length, (double)packet->servers[servers_iter].description_length);
for (uint32_t description_iter = 0; description_iter < packet->servers[servers_iter].description_length; description_iter++)
{
endian_write_i8_little(buffer + offset, packet->servers[servers_iter].description[description_iter]);
offset++;
}

// u32 description_id
endian_write_u32_little(buffer + offset, packet->servers[servers_iter].description_id);
offset += sizeof(u32);
printf("-- description_id          \t%lld\t%llxh\t%f\n", (i64)packet->servers[servers_iter].description_id, (uint64_t)packet->servers[servers_iter].description_id, (double)packet->servers[servers_iter].description_id);

// u32 req_feature_id
endian_write_u32_little(buffer + offset, packet->servers[servers_iter].req_feature_id);
offset += sizeof(u32);
printf("-- req_feature_id          \t%lld\t%llxh\t%f\n", (i64)packet->servers[servers_iter].req_feature_id, (uint64_t)packet->servers[servers_iter].req_feature_id, (double)packet->servers[servers_iter].req_feature_id);

// string server_info
endian_write_u32_little(buffer + offset, packet->servers[servers_iter].server_info_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->servers[servers_iter].server_info_length, (uint64_t)packet->servers[servers_iter].server_info_length, (double)packet->servers[servers_iter].server_info_length);
for (uint32_t server_info_iter = 0; server_info_iter < packet->servers[servers_iter].server_info_length; server_info_iter++)
{
endian_write_i8_little(buffer + offset, packet->servers[servers_iter].server_info[server_info_iter]);
offset++;
}

// u32 population_level
endian_write_u32_little(buffer + offset, packet->servers[servers_iter].population_level);
offset += sizeof(u32);
printf("-- population_level        \t%lld\t%llxh\t%f\n", (i64)packet->servers[servers_iter].population_level, (uint64_t)packet->servers[servers_iter].population_level, (double)packet->servers[servers_iter].population_level);

// string population_data
endian_write_u32_little(buffer + offset, packet->servers[servers_iter].population_data_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->servers[servers_iter].population_data_length, (uint64_t)packet->servers[servers_iter].population_data_length, (double)packet->servers[servers_iter].population_data_length);
for (uint32_t population_data_iter = 0; population_data_iter < packet->servers[servers_iter].population_data_length; population_data_iter++)
{
endian_write_i8_little(buffer + offset, packet->servers[servers_iter].population_data[population_data_iter]);
offset++;
}

// string access_expression
endian_write_u32_little(buffer + offset, packet->servers[servers_iter].access_expression_length);
offset += sizeof(u32);
printf("-- STRING_LENGTH           \t%lld\t%llxh\t%f\n", (i64)packet->servers[servers_iter].access_expression_length, (uint64_t)packet->servers[servers_iter].access_expression_length, (double)packet->servers[servers_iter].access_expression_length);
for (uint32_t access_expression_iter = 0; access_expression_iter < packet->servers[servers_iter].access_expression_length; access_expression_iter++)
{
endian_write_i8_little(buffer + offset, packet->servers[servers_iter].access_expression[access_expression_iter]);
offset++;
}

// b8 is_access_allowed
endian_write_b8_little(buffer + offset, packet->servers[servers_iter].is_access_allowed);
offset += sizeof(b8);
printf("-- is_access_allowed       \t%lld\t%llxh\t%f\n", (i64)packet->servers[servers_iter].is_access_allowed, (uint64_t)packet->servers[servers_iter].is_access_allowed, (double)packet->servers[servers_iter].is_access_allowed);

} // servers

} break;

case Login_Packet_Kind_ServerUpdate:
{
printf("[*] Packing ServerUpdate...\n");
endian_write_u8_little(buffer + offset, 0xf);
offset += sizeof(u8);

} break;

default:
{
printf("Packing not implemented\n");
}
}
return offset;
}

void
login_packet_unpack(uint8_t* data, uint32_t data_length, enum Login_Packet_Kind packet_kind, void* packet_ptr, Arena* arena)
{
UNUSED(data_length);
uint32_t offset = 0;

printf("\n");
switch (packet_kind)
{
case Login_Packet_Kind_LoginRequest:
{
printf("[*] Unpacking LoginRequest...\n");
Login_Packet_LoginRequest* packet = static_cast<Login_Packet_LoginRequest*>(packet_ptr);

// string session_id
packet->session_id_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->session_id = reinterpret_cast<char*>(arena_push_size(arena, packet->session_id_length));
printf("-- STRING_LENGTH           \t%d\n", packet->session_id_length);
for (u32 session_id_iter = 0; session_id_iter < packet->session_id_length; session_id_iter++)
{
packet->session_id[session_id_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// string system_fingerprint
packet->system_fingerprint_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->system_fingerprint = reinterpret_cast<char*>(arena_push_size(arena, packet->system_fingerprint_length));
printf("-- STRING_LENGTH           \t%d\n", packet->system_fingerprint_length);
for (u32 system_fingerprint_iter = 0; system_fingerprint_iter < packet->system_fingerprint_length; system_fingerprint_iter++)
{
packet->system_fingerprint[system_fingerprint_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// u32 locale
packet->locale = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- locale                  \t%lld\t%llxh\t%f\n", (i64)packet->locale, (uint64_t)packet->locale, (double)packet->locale);

// u32 third_party_auth_ticket
packet->third_party_auth_ticket = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- third_party_auth_ticket \t%lld\t%llxh\t%f\n", (i64)packet->third_party_auth_ticket, (uint64_t)packet->third_party_auth_ticket, (double)packet->third_party_auth_ticket);

// u32 third_party_user_id
packet->third_party_user_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- third_party_user_id     \t%lld\t%llxh\t%f\n", (i64)packet->third_party_user_id, (uint64_t)packet->third_party_user_id, (double)packet->third_party_user_id);

// u32 third_party_id
packet->third_party_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- third_party_id          \t%lld\t%llxh\t%f\n", (i64)packet->third_party_id, (uint64_t)packet->third_party_id, (double)packet->third_party_id);

} break;

case Login_Packet_Kind_LoginReply:
{
printf("[*] Unpacking LoginReply...\n");
Login_Packet_LoginReply* packet = static_cast<Login_Packet_LoginReply*>(packet_ptr);

// b8 is_logged_in
packet->is_logged_in = endian_read_b8_little(data + offset);
offset += sizeof(b8);
printf("-- is_logged_in            \t%lld\t%llxh\t%f\n", (i64)packet->is_logged_in, (uint64_t)packet->is_logged_in, (double)packet->is_logged_in);

// u32 status
packet->status = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- status                  \t%lld\t%llxh\t%f\n", (i64)packet->status, (uint64_t)packet->status, (double)packet->status);

// u32 result_code
packet->result_code = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- result_code             \t%lld\t%llxh\t%f\n", (i64)packet->result_code, (uint64_t)packet->result_code, (double)packet->result_code);

// b8 is_member
packet->is_member = endian_read_b8_little(data + offset);
offset += sizeof(b8);
printf("-- is_member               \t%lld\t%llxh\t%f\n", (i64)packet->is_member, (uint64_t)packet->is_member, (double)packet->is_member);

// b8 is_internal
packet->is_internal = endian_read_b8_little(data + offset);
offset += sizeof(b8);
printf("-- is_internal             \t%lld\t%llxh\t%f\n", (i64)packet->is_internal, (uint64_t)packet->is_internal, (double)packet->is_internal);

// string namespace_name
packet->namespace_name_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->namespace_name = reinterpret_cast<char*>(arena_push_size(arena, packet->namespace_name_length));
printf("-- STRING_LENGTH           \t%d\n", packet->namespace_name_length);
for (u32 namespace_name_iter = 0; namespace_name_iter < packet->namespace_name_length; namespace_name_iter++)
{
packet->namespace_name[namespace_name_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// list account_features
packet->account_features_count = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->account_features = static_cast<Login_Packet_LoginReply::account_features_s*>(arena_push_size(arena, packet->account_features_count * sizeof(packet->account_features[0])));
printf("-- LIST_COUNT              \t%d\n", packet->account_features_count);
for (u32 account_features_iter = 0; account_features_iter < packet->account_features_count; account_features_iter++)
{
// u32 key
packet->account_features[account_features_iter].key = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- key                     \t%lld\t%llxh\t%f\n", (i64)packet->account_features[account_features_iter].key, (uint64_t)packet->account_features[account_features_iter].key, (double)packet->account_features[account_features_iter].key);

// u32 id
packet->account_features[account_features_iter].id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- id                      \t%lld\t%llxh\t%f\n", (i64)packet->account_features[account_features_iter].id, (uint64_t)packet->account_features[account_features_iter].id, (double)packet->account_features[account_features_iter].id);

// b8 active
packet->account_features[account_features_iter].active = endian_read_b8_little(data + offset);
offset += sizeof(b8);
printf("-- active                  \t%lld\t%llxh\t%f\n", (i64)packet->account_features[account_features_iter].active, (uint64_t)packet->account_features[account_features_iter].active, (double)packet->account_features[account_features_iter].active);

// u32 remaining_count
packet->account_features[account_features_iter].remaining_count = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- remaining_count         \t%lld\t%llxh\t%f\n", (i64)packet->account_features[account_features_iter].remaining_count, (uint64_t)packet->account_features[account_features_iter].remaining_count, (double)packet->account_features[account_features_iter].remaining_count);

// string raw_data
packet->account_features[account_features_iter].raw_data_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->account_features[account_features_iter].raw_data = reinterpret_cast<char*>(arena_push_size(arena, packet->account_features[account_features_iter].raw_data_length));
printf("-- STRING_LENGTH           \t%d\n", packet->account_features[account_features_iter].raw_data_length);
for (u32 raw_data_iter = 0; raw_data_iter < packet->account_features[account_features_iter].raw_data_length; raw_data_iter++)
{
packet->account_features[account_features_iter].raw_data[raw_data_iter] = *(i8*)((uptr)data + offset);
offset++;
}

} // account_features

// bytes application_payload
packet->application_payload_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->application_payload = reinterpret_cast<uint8_t*>(arena_push_size(arena, packet->application_payload_length));
printf("-- BYTES_LENGTH            \t%d\n", packet->application_payload_length);
for (u32 application_payload_iter = 0; application_payload_iter < packet->application_payload_length; application_payload_iter++)
{
packet->application_payload[application_payload_iter] = *(uint8_t*)((uptr)data + offset);
offset++;
}

// list error_details
packet->error_details_count = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->error_details = static_cast<Login_Packet_LoginReply::error_details_s*>(arena_push_size(arena, packet->error_details_count * sizeof(packet->error_details[0])));
printf("-- LIST_COUNT              \t%d\n", packet->error_details_count);
for (u32 error_details_iter = 0; error_details_iter < packet->error_details_count; error_details_iter++)
{
// u32 unknown_dword1
packet->error_details[error_details_iter].unknown_dword1 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- unknown_dword1          \t%lld\t%llxh\t%f\n", (i64)packet->error_details[error_details_iter].unknown_dword1, (uint64_t)packet->error_details[error_details_iter].unknown_dword1, (double)packet->error_details[error_details_iter].unknown_dword1);

// string name
packet->error_details[error_details_iter].name_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->error_details[error_details_iter].name = reinterpret_cast<char*>(arena_push_size(arena, packet->error_details[error_details_iter].name_length));
printf("-- STRING_LENGTH           \t%d\n", packet->error_details[error_details_iter].name_length);
for (u32 name_iter = 0; name_iter < packet->error_details[error_details_iter].name_length; name_iter++)
{
packet->error_details[error_details_iter].name[name_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// string value
packet->error_details[error_details_iter].value_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->error_details[error_details_iter].value = reinterpret_cast<char*>(arena_push_size(arena, packet->error_details[error_details_iter].value_length));
printf("-- STRING_LENGTH           \t%d\n", packet->error_details[error_details_iter].value_length);
for (u32 value_iter = 0; value_iter < packet->error_details[error_details_iter].value_length; value_iter++)
{
packet->error_details[error_details_iter].value[value_iter] = *(i8*)((uptr)data + offset);
offset++;
}

} // error_details

// string ip_country_code
packet->ip_country_code_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->ip_country_code = reinterpret_cast<char*>(arena_push_size(arena, packet->ip_country_code_length));
printf("-- STRING_LENGTH           \t%d\n", packet->ip_country_code_length);
for (u32 ip_country_code_iter = 0; ip_country_code_iter < packet->ip_country_code_length; ip_country_code_iter++)
{
packet->ip_country_code[ip_country_code_iter] = *(i8*)((uptr)data + offset);
offset++;
}

} break;

case Login_Packet_Kind_CharacterCreateRequest:
{
printf("[*] Unpacking CharacterCreateRequest...\n");
Login_Packet_CharacterCreateRequest* packet = static_cast<Login_Packet_CharacterCreateRequest*>(packet_ptr);

// u32 server_id
packet->server_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- server_id               \t%lld\t%llxh\t%f\n", (i64)packet->server_id, (uint64_t)packet->server_id, (double)packet->server_id);

// u32 unk_u32
packet->unk_u32 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- unk_u32                 \t%lld\t%llxh\t%f\n", (i64)packet->unk_u32, (uint64_t)packet->unk_u32, (double)packet->unk_u32);

// stream char_payload
packet->char_payload_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->char_payload = static_cast<Login_Packet_CharacterCreateRequest::char_payload_s*>(arena_push_size(arena, packet->char_payload_length * sizeof(packet->char_payload[0])));
printf("-- STREAM_LENGTH           \t%d\n", packet->char_payload_length);
for (u32 char_payload_iter = 0; char_payload_iter < (packet->char_payload_length > (u32)0 ? (u32)1 : (u32)0); char_payload_iter++)
{
// u8 empire_id
packet->char_payload[char_payload_iter].empire_id = endian_read_u8_little(data + offset);
offset += sizeof(u8);
printf("-- empire_id               \t%lld\t%llxh\t%f\n", (i64)packet->char_payload[char_payload_iter].empire_id, (uint64_t)packet->char_payload[char_payload_iter].empire_id, (double)packet->char_payload[char_payload_iter].empire_id);

// u32 head_type
packet->char_payload[char_payload_iter].head_type = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- head_type               \t%lld\t%llxh\t%f\n", (i64)packet->char_payload[char_payload_iter].head_type, (uint64_t)packet->char_payload[char_payload_iter].head_type, (double)packet->char_payload[char_payload_iter].head_type);

// u32 profile_type
packet->char_payload[char_payload_iter].profile_type = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- profile_type            \t%lld\t%llxh\t%f\n", (i64)packet->char_payload[char_payload_iter].profile_type, (uint64_t)packet->char_payload[char_payload_iter].profile_type, (double)packet->char_payload[char_payload_iter].profile_type);

// u32 gender
packet->char_payload[char_payload_iter].gender = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- gender                  \t%lld\t%llxh\t%f\n", (i64)packet->char_payload[char_payload_iter].gender, (uint64_t)packet->char_payload[char_payload_iter].gender, (double)packet->char_payload[char_payload_iter].gender);

// string character_name
packet->char_payload[char_payload_iter].character_name_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->char_payload[char_payload_iter].character_name = reinterpret_cast<char*>(arena_push_size(arena, packet->char_payload[char_payload_iter].character_name_length));
printf("-- STRING_LENGTH           \t%d\n", packet->char_payload[char_payload_iter].character_name_length);
for (u32 character_name_iter = 0; character_name_iter < packet->char_payload[char_payload_iter].character_name_length; character_name_iter++)
{
packet->char_payload[char_payload_iter].character_name[character_name_iter] = *(i8*)((uptr)data + offset);
offset++;
}

} // char_payload

} break;

case Login_Packet_Kind_CharacterCreateReply:
{
printf("[*] Unpacking CharacterCreateReply...\n");
Login_Packet_CharacterCreateReply* packet = static_cast<Login_Packet_CharacterCreateReply*>(packet_ptr);

// u32 status
packet->status = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- status                  \t%lld\t%llxh\t%f\n", (i64)packet->status, (uint64_t)packet->status, (double)packet->status);

// u64 character_id
packet->character_id = endian_read_u64_little(data + offset);
offset += sizeof(u64);
printf("-- character_id            \t%lld\t%llxh\t%f\n", (i64)packet->character_id, (uint64_t)packet->character_id, (double)packet->character_id);

} break;

case Login_Packet_Kind_CharacterLoginRequest:
{
printf("[*] Unpacking CharacterLoginRequest...\n");
Login_Packet_CharacterLoginRequest* packet = static_cast<Login_Packet_CharacterLoginRequest*>(packet_ptr);

// u64 character_id
packet->character_id = endian_read_u64_little(data + offset);
offset += sizeof(u64);
printf("-- character_id            \t%lld\t%llxh\t%f\n", (i64)packet->character_id, (uint64_t)packet->character_id, (double)packet->character_id);

// u32 server_id
packet->server_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- server_id               \t%lld\t%llxh\t%f\n", (i64)packet->server_id, (uint64_t)packet->server_id, (double)packet->server_id);

// u32 status
packet->status = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- status                  \t%lld\t%llxh\t%f\n", (i64)packet->status, (uint64_t)packet->status, (double)packet->status);

// stream payload2
packet->payload2_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->payload2 = static_cast<Login_Packet_CharacterLoginRequest::payload2_s*>(arena_push_size(arena, packet->payload2_length * sizeof(packet->payload2[0])));
printf("-- STREAM_LENGTH           \t%d\n", packet->payload2_length);
for (u32 payload2_iter = 0; payload2_iter < (packet->payload2_length > (u32)0 ? (u32)1 : (u32)0); payload2_iter++)
{
// string locale
packet->payload2[payload2_iter].locale_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->payload2[payload2_iter].locale = reinterpret_cast<char*>(arena_push_size(arena, packet->payload2[payload2_iter].locale_length));
printf("-- STRING_LENGTH           \t%d\n", packet->payload2[payload2_iter].locale_length);
for (u32 locale_iter = 0; locale_iter < packet->payload2[payload2_iter].locale_length; locale_iter++)
{
packet->payload2[payload2_iter].locale[locale_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// u32 locale_id
packet->payload2[payload2_iter].locale_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- locale_id               \t%lld\t%llxh\t%f\n", (i64)packet->payload2[payload2_iter].locale_id, (uint64_t)packet->payload2[payload2_iter].locale_id, (double)packet->payload2[payload2_iter].locale_id);

// u32 preferred_gateway_id
packet->payload2[payload2_iter].preferred_gateway_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- preferred_gateway_id    \t%lld\t%llxh\t%f\n", (i64)packet->payload2[payload2_iter].preferred_gateway_id, (uint64_t)packet->payload2[payload2_iter].preferred_gateway_id, (double)packet->payload2[payload2_iter].preferred_gateway_id);

} // payload2

} break;

case Login_Packet_Kind_CharacterLoginReply:
{
printf("[*] Unpacking CharacterLoginReply...\n");
Login_Packet_CharacterLoginReply* packet = static_cast<Login_Packet_CharacterLoginReply*>(packet_ptr);

// u64 character_id
packet->character_id = endian_read_u64_little(data + offset);
offset += sizeof(u64);
printf("-- character_id            \t%lld\t%llxh\t%f\n", (i64)packet->character_id, (uint64_t)packet->character_id, (double)packet->character_id);

// u32 server_id
packet->server_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- server_id               \t%lld\t%llxh\t%f\n", (i64)packet->server_id, (uint64_t)packet->server_id, (double)packet->server_id);

// u32 last_login
packet->last_login = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- last_login              \t%lld\t%llxh\t%f\n", (i64)packet->last_login, (uint64_t)packet->last_login, (double)packet->last_login);

// u32 status
packet->status = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- status                  \t%lld\t%llxh\t%f\n", (i64)packet->status, (uint64_t)packet->status, (double)packet->status);

// stream login_payload
packet->login_payload_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->login_payload = static_cast<Login_Packet_CharacterLoginReply::login_payload_s*>(arena_push_size(arena, packet->login_payload_length * sizeof(packet->login_payload[0])));
printf("-- STREAM_LENGTH           \t%d\n", packet->login_payload_length);
for (u32 login_payload_iter = 0; login_payload_iter < (packet->login_payload_length > (u32)0 ? (u32)1 : (u32)0); login_payload_iter++)
{
// string server_address
packet->login_payload[login_payload_iter].server_address_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->login_payload[login_payload_iter].server_address = reinterpret_cast<char*>(arena_push_size(arena, packet->login_payload[login_payload_iter].server_address_length));
printf("-- STRING_LENGTH           \t%d\n", packet->login_payload[login_payload_iter].server_address_length);
for (u32 server_address_iter = 0; server_address_iter < packet->login_payload[login_payload_iter].server_address_length; server_address_iter++)
{
packet->login_payload[login_payload_iter].server_address[server_address_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// string server_ticket
packet->login_payload[login_payload_iter].server_ticket_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->login_payload[login_payload_iter].server_ticket = reinterpret_cast<char*>(arena_push_size(arena, packet->login_payload[login_payload_iter].server_ticket_length));
printf("-- STRING_LENGTH           \t%d\n", packet->login_payload[login_payload_iter].server_ticket_length);
for (u32 server_ticket_iter = 0; server_ticket_iter < packet->login_payload[login_payload_iter].server_ticket_length; server_ticket_iter++)
{
packet->login_payload[login_payload_iter].server_ticket[server_ticket_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// bytes encryption_key
packet->login_payload[login_payload_iter].encryption_key_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->login_payload[login_payload_iter].encryption_key = reinterpret_cast<uint8_t*>(arena_push_size(arena, packet->login_payload[login_payload_iter].encryption_key_length));
printf("-- BYTES_LENGTH            \t%d\n", packet->login_payload[login_payload_iter].encryption_key_length);
for (u32 encryption_key_iter = 0; encryption_key_iter < packet->login_payload[login_payload_iter].encryption_key_length; encryption_key_iter++)
{
packet->login_payload[login_payload_iter].encryption_key[encryption_key_iter] = *(uint8_t*)((uptr)data + offset);
offset++;
}

// u32 soe_protocol_version
packet->login_payload[login_payload_iter].soe_protocol_version = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- soe_protocol_version    \t%lld\t%llxh\t%f\n", (i64)packet->login_payload[login_payload_iter].soe_protocol_version, (uint64_t)packet->login_payload[login_payload_iter].soe_protocol_version, (double)packet->login_payload[login_payload_iter].soe_protocol_version);

// u64 character_id
packet->login_payload[login_payload_iter].character_id = endian_read_u64_little(data + offset);
offset += sizeof(u64);
printf("-- character_id            \t%lld\t%llxh\t%f\n", (i64)packet->login_payload[login_payload_iter].character_id, (uint64_t)packet->login_payload[login_payload_iter].character_id, (double)packet->login_payload[login_payload_iter].character_id);

// u64 unk_u64
packet->login_payload[login_payload_iter].unk_u64 = endian_read_u64_little(data + offset);
offset += sizeof(u64);
printf("-- unk_u64                 \t%lld\t%llxh\t%f\n", (i64)packet->login_payload[login_payload_iter].unk_u64, (uint64_t)packet->login_payload[login_payload_iter].unk_u64, (double)packet->login_payload[login_payload_iter].unk_u64);

// string station_name
packet->login_payload[login_payload_iter].station_name_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->login_payload[login_payload_iter].station_name = reinterpret_cast<char*>(arena_push_size(arena, packet->login_payload[login_payload_iter].station_name_length));
printf("-- STRING_LENGTH           \t%d\n", packet->login_payload[login_payload_iter].station_name_length);
for (u32 station_name_iter = 0; station_name_iter < packet->login_payload[login_payload_iter].station_name_length; station_name_iter++)
{
packet->login_payload[login_payload_iter].station_name[station_name_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// string character_name
packet->login_payload[login_payload_iter].character_name_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->login_payload[login_payload_iter].character_name = reinterpret_cast<char*>(arena_push_size(arena, packet->login_payload[login_payload_iter].character_name_length));
printf("-- STRING_LENGTH           \t%d\n", packet->login_payload[login_payload_iter].character_name_length);
for (u32 character_name_iter = 0; character_name_iter < packet->login_payload[login_payload_iter].character_name_length; character_name_iter++)
{
packet->login_payload[login_payload_iter].character_name[character_name_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// string unk_str
packet->login_payload[login_payload_iter].unk_str_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->login_payload[login_payload_iter].unk_str = reinterpret_cast<char*>(arena_push_size(arena, packet->login_payload[login_payload_iter].unk_str_length));
printf("-- STRING_LENGTH           \t%d\n", packet->login_payload[login_payload_iter].unk_str_length);
for (u32 unk_str_iter = 0; unk_str_iter < packet->login_payload[login_payload_iter].unk_str_length; unk_str_iter++)
{
packet->login_payload[login_payload_iter].unk_str[unk_str_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// u64 server_feature_bit
packet->login_payload[login_payload_iter].server_feature_bit = endian_read_u64_little(data + offset);
offset += sizeof(u64);
printf("-- server_feature_bit      \t%lld\t%llxh\t%f\n", (i64)packet->login_payload[login_payload_iter].server_feature_bit, (uint64_t)packet->login_payload[login_payload_iter].server_feature_bit, (double)packet->login_payload[login_payload_iter].server_feature_bit);

} // login_payload

} break;

case Login_Packet_Kind_CharacterDeleteRequest:
{
printf("[*] Unpacking CharacterDeleteRequest...\n");
Login_Packet_CharacterDeleteRequest* packet = static_cast<Login_Packet_CharacterDeleteRequest*>(packet_ptr);

// u64 character_id
packet->character_id = endian_read_u64_little(data + offset);
offset += sizeof(u64);
printf("-- character_id            \t%lld\t%llxh\t%f\n", (i64)packet->character_id, (uint64_t)packet->character_id, (double)packet->character_id);

} break;

case Login_Packet_Kind_TunnelAppPacketClientToServer:
{
printf("[*] Unpacking TunnelAppPacketClientToServer...\n");
Login_Packet_TunnelAppPacketClientToServer* packet = static_cast<Login_Packet_TunnelAppPacketClientToServer*>(packet_ptr);

// u32 server_id
packet->server_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- server_id               \t%lld\t%llxh\t%f\n", (i64)packet->server_id, (uint64_t)packet->server_id, (double)packet->server_id);

// u32 unk1
packet->unk1 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- unk1                    \t%lld\t%llxh\t%f\n", (i64)packet->unk1, (uint64_t)packet->unk1, (double)packet->unk1);

// stream data_client
packet->data_client_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->data_client = static_cast<Login_Packet_TunnelAppPacketClientToServer::data_client_s*>(arena_push_size(arena, packet->data_client_length * sizeof(packet->data_client[0])));
printf("-- STREAM_LENGTH           \t%d\n", packet->data_client_length);
for (u32 data_client_iter = 0; data_client_iter < (packet->data_client_length > (u32)0 ? (u32)1 : (u32)0); data_client_iter++)
{
// b8 tunnel_op_code
packet->data_client[data_client_iter].tunnel_op_code = endian_read_b8_little(data + offset);
offset += sizeof(b8);
printf("-- tunnel_op_code          \t%lld\t%llxh\t%f\n", (i64)packet->data_client[data_client_iter].tunnel_op_code, (uint64_t)packet->data_client[data_client_iter].tunnel_op_code, (double)packet->data_client[data_client_iter].tunnel_op_code);

// b8 sub_op_code
packet->data_client[data_client_iter].sub_op_code = endian_read_b8_little(data + offset);
offset += sizeof(b8);
printf("-- sub_op_code             \t%lld\t%llxh\t%f\n", (i64)packet->data_client[data_client_iter].sub_op_code, (uint64_t)packet->data_client[data_client_iter].sub_op_code, (double)packet->data_client[data_client_iter].sub_op_code);

// string character_name
packet->data_client[data_client_iter].character_name_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->data_client[data_client_iter].character_name = reinterpret_cast<char*>(arena_push_size(arena, packet->data_client[data_client_iter].character_name_length));
printf("-- STRING_LENGTH           \t%d\n", packet->data_client[data_client_iter].character_name_length);
for (u32 character_name_iter = 0; character_name_iter < packet->data_client[data_client_iter].character_name_length; character_name_iter++)
{
packet->data_client[data_client_iter].character_name[character_name_iter] = *(i8*)((uptr)data + offset);
offset++;
}

} // data_client

} break;

case Login_Packet_Kind_TunnelAppPacketServerToClient:
{
printf("[*] Unpacking TunnelAppPacketServerToClient...\n");
Login_Packet_TunnelAppPacketServerToClient* packet = static_cast<Login_Packet_TunnelAppPacketServerToClient*>(packet_ptr);

// u32 server_id
packet->server_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- server_id               \t%lld\t%llxh\t%f\n", (i64)packet->server_id, (uint64_t)packet->server_id, (double)packet->server_id);

// u32 unk1
packet->unk1 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- unk1                    \t%lld\t%llxh\t%f\n", (i64)packet->unk1, (uint64_t)packet->unk1, (double)packet->unk1);

// stream data_server
packet->data_server_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->data_server = static_cast<Login_Packet_TunnelAppPacketServerToClient::data_server_s*>(arena_push_size(arena, packet->data_server_length * sizeof(packet->data_server[0])));
printf("-- STREAM_LENGTH           \t%d\n", packet->data_server_length);
for (u32 data_server_iter = 0; data_server_iter < (packet->data_server_length > (u32)0 ? (u32)1 : (u32)0); data_server_iter++)
{
// u8 tunnel_op_code
packet->data_server[data_server_iter].tunnel_op_code = endian_read_u8_little(data + offset);
offset += sizeof(u8);
printf("-- tunnel_op_code          \t%lld\t%llxh\t%f\n", (i64)packet->data_server[data_server_iter].tunnel_op_code, (uint64_t)packet->data_server[data_server_iter].tunnel_op_code, (double)packet->data_server[data_server_iter].tunnel_op_code);

// u8 sub_op_code
packet->data_server[data_server_iter].sub_op_code = endian_read_u8_little(data + offset);
offset += sizeof(u8);
printf("-- sub_op_code             \t%lld\t%llxh\t%f\n", (i64)packet->data_server[data_server_iter].sub_op_code, (uint64_t)packet->data_server[data_server_iter].sub_op_code, (double)packet->data_server[data_server_iter].sub_op_code);

// string character_name
packet->data_server[data_server_iter].character_name_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->data_server[data_server_iter].character_name = reinterpret_cast<char*>(arena_push_size(arena, packet->data_server[data_server_iter].character_name_length));
printf("-- STRING_LENGTH           \t%d\n", packet->data_server[data_server_iter].character_name_length);
for (u32 character_name_iter = 0; character_name_iter < packet->data_server[data_server_iter].character_name_length; character_name_iter++)
{
packet->data_server[data_server_iter].character_name[character_name_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// string character_name2
packet->data_server[data_server_iter].character_name2_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->data_server[data_server_iter].character_name2 = reinterpret_cast<char*>(arena_push_size(arena, packet->data_server[data_server_iter].character_name2_length));
printf("-- STRING_LENGTH           \t%d\n", packet->data_server[data_server_iter].character_name2_length);
for (u32 character_name2_iter = 0; character_name2_iter < packet->data_server[data_server_iter].character_name2_length; character_name2_iter++)
{
packet->data_server[data_server_iter].character_name2[character_name2_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// u32 status
packet->data_server[data_server_iter].status = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- status                  \t%lld\t%llxh\t%f\n", (i64)packet->data_server[data_server_iter].status, (uint64_t)packet->data_server[data_server_iter].status, (double)packet->data_server[data_server_iter].status);

} // data_server

} break;

case Login_Packet_Kind_CharacterDeleteReply:
{
printf("[*] Unpacking CharacterDeleteReply...\n");
Login_Packet_CharacterDeleteReply* packet = static_cast<Login_Packet_CharacterDeleteReply*>(packet_ptr);

// u64 character_id
packet->character_id = endian_read_u64_little(data + offset);
offset += sizeof(u64);
printf("-- character_id            \t%lld\t%llxh\t%f\n", (i64)packet->character_id, (uint64_t)packet->character_id, (double)packet->character_id);

// u32 status
packet->status = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- status                  \t%lld\t%llxh\t%f\n", (i64)packet->status, (uint64_t)packet->status, (double)packet->status);

// bytes payload3
packet->payload3_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->payload3 = reinterpret_cast<uint8_t*>(arena_push_size(arena, packet->payload3_length));
printf("-- BYTES_LENGTH            \t%d\n", packet->payload3_length);
for (u32 payload3_iter = 0; payload3_iter < packet->payload3_length; payload3_iter++)
{
packet->payload3[payload3_iter] = *(uint8_t*)((uptr)data + offset);
offset++;
}

} break;

case Login_Packet_Kind_CharacterSelectInfoReply:
{
printf("[*] Unpacking CharacterSelectInfoReply...\n");
Login_Packet_CharacterSelectInfoReply* packet = static_cast<Login_Packet_CharacterSelectInfoReply*>(packet_ptr);

// u32 character_status
packet->character_status = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- character_status        \t%lld\t%llxh\t%f\n", (i64)packet->character_status, (uint64_t)packet->character_status, (double)packet->character_status);

// b8 can_bypass_server_lock
packet->can_bypass_server_lock = endian_read_b8_little(data + offset);
offset += sizeof(b8);
printf("-- can_bypass_server_lock  \t%lld\t%llxh\t%f\n", (i64)packet->can_bypass_server_lock, (uint64_t)packet->can_bypass_server_lock, (double)packet->can_bypass_server_lock);

// list stats_item_def_2
packet->stats_item_def_2_count = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->stats_item_def_2 = static_cast<Login_Packet_CharacterSelectInfoReply::stats_item_def_2_s*>(arena_push_size(arena, packet->stats_item_def_2_count * sizeof(packet->stats_item_def_2[0])));
printf("-- LIST_COUNT              \t%d\n", packet->stats_item_def_2_count);
for (u32 stats_item_def_2_iter = 0; stats_item_def_2_iter < packet->stats_item_def_2_count; stats_item_def_2_iter++)
{
// u32 unk_dword_9
packet->stats_item_def_2[stats_item_def_2_iter].unk_dword_9 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- unk_dword_9             \t%lld\t%llxh\t%f\n", (i64)packet->stats_item_def_2[stats_item_def_2_iter].unk_dword_9, (uint64_t)packet->stats_item_def_2[stats_item_def_2_iter].unk_dword_9, (double)packet->stats_item_def_2[stats_item_def_2_iter].unk_dword_9);

// u32 stat_id
packet->stats_item_def_2[stats_item_def_2_iter].stat_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- stat_id                 \t%lld\t%llxh\t%f\n", (i64)packet->stats_item_def_2[stats_item_def_2_iter].stat_id, (uint64_t)packet->stats_item_def_2[stats_item_def_2_iter].stat_id, (double)packet->stats_item_def_2[stats_item_def_2_iter].stat_id);

packet->stats_item_def_2[stats_item_def_2_iter].variabletype8_case = endian_read_u8_little(data + offset);
offset += sizeof(u8);
switch(packet->stats_item_def_2[stats_item_def_2_iter].variabletype8_case)
{
case 0:
{
// u32 base
packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue0.base = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- base                    \t%lld\t%llxh\t%f\n", (i64)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue0.base, (uint64_t)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue0.base, (double)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue0.base);

// u32 modifier
packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue0.modifier = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- modifier                \t%lld\t%llxh\t%f\n", (i64)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue0.modifier, (uint64_t)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue0.modifier, (double)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue0.modifier);

} break;
case 1:
{
// f32 base
packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue1.base = endian_read_f32_little(data + offset);
offset += sizeof(f32);
printf("-- base                    \t%lld\t%llxh\t%f\n", (i64)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue1.base, (uint64_t)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue1.base, (double)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue1.base);

// f32 modifier
packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue1.modifier = endian_read_f32_little(data + offset);
offset += sizeof(f32);
printf("-- modifier                \t%lld\t%llxh\t%f\n", (i64)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue1.modifier, (uint64_t)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue1.modifier, (double)packet->stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue1.modifier);

} break;
};
// u32 unk_dword_10
packet->stats_item_def_2[stats_item_def_2_iter].unk_dword_10 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- unk_dword_10            \t%lld\t%llxh\t%f\n", (i64)packet->stats_item_def_2[stats_item_def_2_iter].unk_dword_10, (uint64_t)packet->stats_item_def_2[stats_item_def_2_iter].unk_dword_10, (double)packet->stats_item_def_2[stats_item_def_2_iter].unk_dword_10);

} // stats_item_def_2

// list itemDefinitions
packet->itemDefinitions_count = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->itemDefinitions = static_cast<Login_Packet_CharacterSelectInfoReply::itemDefinitions_s*>(arena_push_size(arena, packet->itemDefinitions_count * sizeof(packet->itemDefinitions[0])));
printf("-- LIST_COUNT              \t%d\n", packet->itemDefinitions_count);
for (u32 itemDefinitions_iter = 0; itemDefinitions_iter < packet->itemDefinitions_count; itemDefinitions_iter++)
{
// u32 ID
packet->itemDefinitions[itemDefinitions_iter].ID = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- ID                      \t%lld\t%llxh\t%f\n", (i64)packet->itemDefinitions[itemDefinitions_iter].ID, (uint64_t)packet->itemDefinitions[itemDefinitions_iter].ID, (double)packet->itemDefinitions[itemDefinitions_iter].ID);

} // itemDefinitions

// list item_defs
packet->item_defs_count = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->item_defs = static_cast<Login_Packet_CharacterSelectInfoReply::item_defs_s*>(arena_push_size(arena, packet->item_defs_count * sizeof(packet->item_defs[0])));
printf("-- LIST_COUNT              \t%d\n", packet->item_defs_count);
for (u32 item_defs_iter = 0; item_defs_iter < packet->item_defs_count; item_defs_iter++)
{
// u32 defs_id
packet->item_defs[item_defs_iter].defs_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- defs_id                 \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].defs_id, (uint64_t)packet->item_defs[item_defs_iter].defs_id, (double)packet->item_defs[item_defs_iter].defs_id);

// u8 bitflags1
packet->item_defs[item_defs_iter].bitflags1 = endian_read_u8_little(data + offset);
offset += sizeof(u8);
printf("-- bitflags1               \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].bitflags1, (uint64_t)packet->item_defs[item_defs_iter].bitflags1, (double)packet->item_defs[item_defs_iter].bitflags1);

// u8 bitflags2
packet->item_defs[item_defs_iter].bitflags2 = endian_read_u8_little(data + offset);
offset += sizeof(u8);
printf("-- bitflags2               \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].bitflags2, (uint64_t)packet->item_defs[item_defs_iter].bitflags2, (double)packet->item_defs[item_defs_iter].bitflags2);

// u32 name_id
packet->item_defs[item_defs_iter].name_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- name_id                 \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].name_id, (uint64_t)packet->item_defs[item_defs_iter].name_id, (double)packet->item_defs[item_defs_iter].name_id);

// u32 description_id
packet->item_defs[item_defs_iter].description_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- description_id          \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].description_id, (uint64_t)packet->item_defs[item_defs_iter].description_id, (double)packet->item_defs[item_defs_iter].description_id);

// u32 content_id
packet->item_defs[item_defs_iter].content_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- content_id              \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].content_id, (uint64_t)packet->item_defs[item_defs_iter].content_id, (double)packet->item_defs[item_defs_iter].content_id);

// u32 image_set_id
packet->item_defs[item_defs_iter].image_set_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- image_set_id            \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].image_set_id, (uint64_t)packet->item_defs[item_defs_iter].image_set_id, (double)packet->item_defs[item_defs_iter].image_set_id);

// u32 tint_id
packet->item_defs[item_defs_iter].tint_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- tint_id                 \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].tint_id, (uint64_t)packet->item_defs[item_defs_iter].tint_id, (double)packet->item_defs[item_defs_iter].tint_id);

// u32 hud_image_set_id
packet->item_defs[item_defs_iter].hud_image_set_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- hud_image_set_id        \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].hud_image_set_id, (uint64_t)packet->item_defs[item_defs_iter].hud_image_set_id, (double)packet->item_defs[item_defs_iter].hud_image_set_id);

// u32 unk_dword_1
packet->item_defs[item_defs_iter].unk_dword_1 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- unk_dword_1             \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].unk_dword_1, (uint64_t)packet->item_defs[item_defs_iter].unk_dword_1, (double)packet->item_defs[item_defs_iter].unk_dword_1);

// u32 unk_dword_2
packet->item_defs[item_defs_iter].unk_dword_2 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- unk_dword_2             \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].unk_dword_2, (uint64_t)packet->item_defs[item_defs_iter].unk_dword_2, (double)packet->item_defs[item_defs_iter].unk_dword_2);

// u32 cost
packet->item_defs[item_defs_iter].cost = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- cost                    \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].cost, (uint64_t)packet->item_defs[item_defs_iter].cost, (double)packet->item_defs[item_defs_iter].cost);

// u32 item_class
packet->item_defs[item_defs_iter].item_class = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- item_class              \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].item_class, (uint64_t)packet->item_defs[item_defs_iter].item_class, (double)packet->item_defs[item_defs_iter].item_class);

// u32 profile_override
packet->item_defs[item_defs_iter].profile_override = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- profile_override        \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].profile_override, (uint64_t)packet->item_defs[item_defs_iter].profile_override, (double)packet->item_defs[item_defs_iter].profile_override);

// string model_name
packet->item_defs[item_defs_iter].model_name_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->item_defs[item_defs_iter].model_name = reinterpret_cast<char*>(arena_push_size(arena, packet->item_defs[item_defs_iter].model_name_length));
printf("-- STRING_LENGTH           \t%d\n", packet->item_defs[item_defs_iter].model_name_length);
for (u32 model_name_iter = 0; model_name_iter < packet->item_defs[item_defs_iter].model_name_length; model_name_iter++)
{
packet->item_defs[item_defs_iter].model_name[model_name_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// string texture_alias
packet->item_defs[item_defs_iter].texture_alias_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->item_defs[item_defs_iter].texture_alias = reinterpret_cast<char*>(arena_push_size(arena, packet->item_defs[item_defs_iter].texture_alias_length));
printf("-- STRING_LENGTH           \t%d\n", packet->item_defs[item_defs_iter].texture_alias_length);
for (u32 texture_alias_iter = 0; texture_alias_iter < packet->item_defs[item_defs_iter].texture_alias_length; texture_alias_iter++)
{
packet->item_defs[item_defs_iter].texture_alias[texture_alias_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// u32 gender_usage
packet->item_defs[item_defs_iter].gender_usage = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- gender_usage            \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].gender_usage, (uint64_t)packet->item_defs[item_defs_iter].gender_usage, (double)packet->item_defs[item_defs_iter].gender_usage);

// u32 item_type
packet->item_defs[item_defs_iter].item_type = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- item_type               \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].item_type, (uint64_t)packet->item_defs[item_defs_iter].item_type, (double)packet->item_defs[item_defs_iter].item_type);

// u32 category_id
packet->item_defs[item_defs_iter].category_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- category_id             \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].category_id, (uint64_t)packet->item_defs[item_defs_iter].category_id, (double)packet->item_defs[item_defs_iter].category_id);

// u32 weapon_trail_effect_id
packet->item_defs[item_defs_iter].weapon_trail_effect_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- weapon_trail_effect_id  \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].weapon_trail_effect_id, (uint64_t)packet->item_defs[item_defs_iter].weapon_trail_effect_id, (double)packet->item_defs[item_defs_iter].weapon_trail_effect_id);

// u32 composite_effect_id
packet->item_defs[item_defs_iter].composite_effect_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- composite_effect_id     \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].composite_effect_id, (uint64_t)packet->item_defs[item_defs_iter].composite_effect_id, (double)packet->item_defs[item_defs_iter].composite_effect_id);

// u32 power_rating
packet->item_defs[item_defs_iter].power_rating = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- power_rating            \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].power_rating, (uint64_t)packet->item_defs[item_defs_iter].power_rating, (double)packet->item_defs[item_defs_iter].power_rating);

// u32 min_profile_rank
packet->item_defs[item_defs_iter].min_profile_rank = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- min_profile_rank        \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].min_profile_rank, (uint64_t)packet->item_defs[item_defs_iter].min_profile_rank, (double)packet->item_defs[item_defs_iter].min_profile_rank);

// u32 rarity
packet->item_defs[item_defs_iter].rarity = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- rarity                  \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].rarity, (uint64_t)packet->item_defs[item_defs_iter].rarity, (double)packet->item_defs[item_defs_iter].rarity);

// u32 activatable_ability_id
packet->item_defs[item_defs_iter].activatable_ability_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- activatable_ability_id  \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].activatable_ability_id, (uint64_t)packet->item_defs[item_defs_iter].activatable_ability_id, (double)packet->item_defs[item_defs_iter].activatable_ability_id);

// u32 activatable_ability_set_id
packet->item_defs[item_defs_iter].activatable_ability_set_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- activatable_ability_set_id\t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].activatable_ability_set_id, (uint64_t)packet->item_defs[item_defs_iter].activatable_ability_set_id, (double)packet->item_defs[item_defs_iter].activatable_ability_set_id);

// u32 passive_ability_id
packet->item_defs[item_defs_iter].passive_ability_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- passive_ability_id      \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].passive_ability_id, (uint64_t)packet->item_defs[item_defs_iter].passive_ability_id, (double)packet->item_defs[item_defs_iter].passive_ability_id);

// u32 passive_ability_set_id
packet->item_defs[item_defs_iter].passive_ability_set_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- passive_ability_set_id  \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].passive_ability_set_id, (uint64_t)packet->item_defs[item_defs_iter].passive_ability_set_id, (double)packet->item_defs[item_defs_iter].passive_ability_set_id);

// u32 max_stack_size
packet->item_defs[item_defs_iter].max_stack_size = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- max_stack_size          \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].max_stack_size, (uint64_t)packet->item_defs[item_defs_iter].max_stack_size, (double)packet->item_defs[item_defs_iter].max_stack_size);

// u32 min_stack_size
packet->item_defs[item_defs_iter].min_stack_size = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- min_stack_size          \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].min_stack_size, (uint64_t)packet->item_defs[item_defs_iter].min_stack_size, (double)packet->item_defs[item_defs_iter].min_stack_size);

// string tint_alias
packet->item_defs[item_defs_iter].tint_alias_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->item_defs[item_defs_iter].tint_alias = reinterpret_cast<char*>(arena_push_size(arena, packet->item_defs[item_defs_iter].tint_alias_length));
printf("-- STRING_LENGTH           \t%d\n", packet->item_defs[item_defs_iter].tint_alias_length);
for (u32 tint_alias_iter = 0; tint_alias_iter < packet->item_defs[item_defs_iter].tint_alias_length; tint_alias_iter++)
{
packet->item_defs[item_defs_iter].tint_alias[tint_alias_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// u32 tint_group_id
packet->item_defs[item_defs_iter].tint_group_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- tint_group_id           \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].tint_group_id, (uint64_t)packet->item_defs[item_defs_iter].tint_group_id, (double)packet->item_defs[item_defs_iter].tint_group_id);

// u32 member_discount
packet->item_defs[item_defs_iter].member_discount = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- member_discount         \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].member_discount, (uint64_t)packet->item_defs[item_defs_iter].member_discount, (double)packet->item_defs[item_defs_iter].member_discount);

// u32 vip_rank_required
packet->item_defs[item_defs_iter].vip_rank_required = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- vip_rank_required       \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].vip_rank_required, (uint64_t)packet->item_defs[item_defs_iter].vip_rank_required, (double)packet->item_defs[item_defs_iter].vip_rank_required);

// u32 race_set_id
packet->item_defs[item_defs_iter].race_set_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- race_set_id             \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].race_set_id, (uint64_t)packet->item_defs[item_defs_iter].race_set_id, (double)packet->item_defs[item_defs_iter].race_set_id);

// u32 ui_model_camera_id_1
packet->item_defs[item_defs_iter].ui_model_camera_id_1 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- ui_model_camera_id_1    \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].ui_model_camera_id_1, (uint64_t)packet->item_defs[item_defs_iter].ui_model_camera_id_1, (double)packet->item_defs[item_defs_iter].ui_model_camera_id_1);

// u32 equip_count_max
packet->item_defs[item_defs_iter].equip_count_max = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- equip_count_max         \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].equip_count_max, (uint64_t)packet->item_defs[item_defs_iter].equip_count_max, (double)packet->item_defs[item_defs_iter].equip_count_max);

// i32 curreny_type
packet->item_defs[item_defs_iter].curreny_type = endian_read_i32_little(data + offset);
offset += sizeof(i32);
printf("-- curreny_type            \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].curreny_type, (uint64_t)packet->item_defs[item_defs_iter].curreny_type, (double)packet->item_defs[item_defs_iter].curreny_type);

// u32 datasheet_id
packet->item_defs[item_defs_iter].datasheet_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- datasheet_id            \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].datasheet_id, (uint64_t)packet->item_defs[item_defs_iter].datasheet_id, (double)packet->item_defs[item_defs_iter].datasheet_id);

// u32 item_type_1
packet->item_defs[item_defs_iter].item_type_1 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- item_type_1             \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].item_type_1, (uint64_t)packet->item_defs[item_defs_iter].item_type_1, (double)packet->item_defs[item_defs_iter].item_type_1);

// u32 skill_set_id
packet->item_defs[item_defs_iter].skill_set_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- skill_set_id            \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].skill_set_id, (uint64_t)packet->item_defs[item_defs_iter].skill_set_id, (double)packet->item_defs[item_defs_iter].skill_set_id);

// string overlay_texture
packet->item_defs[item_defs_iter].overlay_texture_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->item_defs[item_defs_iter].overlay_texture = reinterpret_cast<char*>(arena_push_size(arena, packet->item_defs[item_defs_iter].overlay_texture_length));
printf("-- STRING_LENGTH           \t%d\n", packet->item_defs[item_defs_iter].overlay_texture_length);
for (u32 overlay_texture_iter = 0; overlay_texture_iter < packet->item_defs[item_defs_iter].overlay_texture_length; overlay_texture_iter++)
{
packet->item_defs[item_defs_iter].overlay_texture[overlay_texture_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// string decal_slot
packet->item_defs[item_defs_iter].decal_slot_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->item_defs[item_defs_iter].decal_slot = reinterpret_cast<char*>(arena_push_size(arena, packet->item_defs[item_defs_iter].decal_slot_length));
printf("-- STRING_LENGTH           \t%d\n", packet->item_defs[item_defs_iter].decal_slot_length);
for (u32 decal_slot_iter = 0; decal_slot_iter < packet->item_defs[item_defs_iter].decal_slot_length; decal_slot_iter++)
{
packet->item_defs[item_defs_iter].decal_slot[decal_slot_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// u32 overlay_adjustment
packet->item_defs[item_defs_iter].overlay_adjustment = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- overlay_adjustment      \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].overlay_adjustment, (uint64_t)packet->item_defs[item_defs_iter].overlay_adjustment, (double)packet->item_defs[item_defs_iter].overlay_adjustment);

// u32 trial_duration_sec
packet->item_defs[item_defs_iter].trial_duration_sec = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- trial_duration_sec      \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].trial_duration_sec, (uint64_t)packet->item_defs[item_defs_iter].trial_duration_sec, (double)packet->item_defs[item_defs_iter].trial_duration_sec);

// u32 next_trial_delay_sec
packet->item_defs[item_defs_iter].next_trial_delay_sec = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- next_trial_delay_sec    \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].next_trial_delay_sec, (uint64_t)packet->item_defs[item_defs_iter].next_trial_delay_sec, (double)packet->item_defs[item_defs_iter].next_trial_delay_sec);

// u32 client_use_requirement
packet->item_defs[item_defs_iter].client_use_requirement = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- client_use_requirement  \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].client_use_requirement, (uint64_t)packet->item_defs[item_defs_iter].client_use_requirement, (double)packet->item_defs[item_defs_iter].client_use_requirement);

// string override_appearance
packet->item_defs[item_defs_iter].override_appearance_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->item_defs[item_defs_iter].override_appearance = reinterpret_cast<char*>(arena_push_size(arena, packet->item_defs[item_defs_iter].override_appearance_length));
printf("-- STRING_LENGTH           \t%d\n", packet->item_defs[item_defs_iter].override_appearance_length);
for (u32 override_appearance_iter = 0; override_appearance_iter < packet->item_defs[item_defs_iter].override_appearance_length; override_appearance_iter++)
{
packet->item_defs[item_defs_iter].override_appearance[override_appearance_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// u32 override_camera_id
packet->item_defs[item_defs_iter].override_camera_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- override_camera_id      \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].override_camera_id, (uint64_t)packet->item_defs[item_defs_iter].override_camera_id, (double)packet->item_defs[item_defs_iter].override_camera_id);

// u32 unk_dword_3
packet->item_defs[item_defs_iter].unk_dword_3 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- unk_dword_3             \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].unk_dword_3, (uint64_t)packet->item_defs[item_defs_iter].unk_dword_3, (double)packet->item_defs[item_defs_iter].unk_dword_3);

// u32 unk_dword_4
packet->item_defs[item_defs_iter].unk_dword_4 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- unk_dword_4             \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].unk_dword_4, (uint64_t)packet->item_defs[item_defs_iter].unk_dword_4, (double)packet->item_defs[item_defs_iter].unk_dword_4);

// u32 unk_dword_5
packet->item_defs[item_defs_iter].unk_dword_5 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- unk_dword_5             \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].unk_dword_5, (uint64_t)packet->item_defs[item_defs_iter].unk_dword_5, (double)packet->item_defs[item_defs_iter].unk_dword_5);

// u32 bulk
packet->item_defs[item_defs_iter].bulk = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- bulk                    \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].bulk, (uint64_t)packet->item_defs[item_defs_iter].bulk, (double)packet->item_defs[item_defs_iter].bulk);

// u32 active_equip_slot_id
packet->item_defs[item_defs_iter].active_equip_slot_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- active_equip_slot_id    \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].active_equip_slot_id, (uint64_t)packet->item_defs[item_defs_iter].active_equip_slot_id, (double)packet->item_defs[item_defs_iter].active_equip_slot_id);

// u32 passive_equip_slot_id
packet->item_defs[item_defs_iter].passive_equip_slot_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- passive_equip_slot_id   \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].passive_equip_slot_id, (uint64_t)packet->item_defs[item_defs_iter].passive_equip_slot_id, (double)packet->item_defs[item_defs_iter].passive_equip_slot_id);

// u32 passive_equip_slot_group_id
packet->item_defs[item_defs_iter].passive_equip_slot_group_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- passive_equip_slot_group_id\t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].passive_equip_slot_group_id, (uint64_t)packet->item_defs[item_defs_iter].passive_equip_slot_group_id, (double)packet->item_defs[item_defs_iter].passive_equip_slot_group_id);

// u32 unk_dword_6
packet->item_defs[item_defs_iter].unk_dword_6 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- unk_dword_6             \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].unk_dword_6, (uint64_t)packet->item_defs[item_defs_iter].unk_dword_6, (double)packet->item_defs[item_defs_iter].unk_dword_6);

// u32 grinder_reward_set_id
packet->item_defs[item_defs_iter].grinder_reward_set_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- grinder_reward_set_id   \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].grinder_reward_set_id, (uint64_t)packet->item_defs[item_defs_iter].grinder_reward_set_id, (double)packet->item_defs[item_defs_iter].grinder_reward_set_id);

// u32 build_bar_group_id
packet->item_defs[item_defs_iter].build_bar_group_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- build_bar_group_id      \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].build_bar_group_id, (uint64_t)packet->item_defs[item_defs_iter].build_bar_group_id, (double)packet->item_defs[item_defs_iter].build_bar_group_id);

// string unk_string_1
packet->item_defs[item_defs_iter].unk_string_1_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->item_defs[item_defs_iter].unk_string_1 = reinterpret_cast<char*>(arena_push_size(arena, packet->item_defs[item_defs_iter].unk_string_1_length));
printf("-- STRING_LENGTH           \t%d\n", packet->item_defs[item_defs_iter].unk_string_1_length);
for (u32 unk_string_1_iter = 0; unk_string_1_iter < packet->item_defs[item_defs_iter].unk_string_1_length; unk_string_1_iter++)
{
packet->item_defs[item_defs_iter].unk_string_1[unk_string_1_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// b8 unk_bool_1
packet->item_defs[item_defs_iter].unk_bool_1 = endian_read_b8_little(data + offset);
offset += sizeof(b8);
printf("-- unk_bool_1              \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].unk_bool_1, (uint64_t)packet->item_defs[item_defs_iter].unk_bool_1, (double)packet->item_defs[item_defs_iter].unk_bool_1);

// b8 is_armor
packet->item_defs[item_defs_iter].is_armor = endian_read_b8_little(data + offset);
offset += sizeof(b8);
printf("-- is_armor                \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].is_armor, (uint64_t)packet->item_defs[item_defs_iter].is_armor, (double)packet->item_defs[item_defs_iter].is_armor);

// u32 unk_dword_7
packet->item_defs[item_defs_iter].unk_dword_7 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- unk_dword_7             \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].unk_dword_7, (uint64_t)packet->item_defs[item_defs_iter].unk_dword_7, (double)packet->item_defs[item_defs_iter].unk_dword_7);

// u32 param1
packet->item_defs[item_defs_iter].param1 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- param1                  \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].param1, (uint64_t)packet->item_defs[item_defs_iter].param1, (double)packet->item_defs[item_defs_iter].param1);

// u32 param2
packet->item_defs[item_defs_iter].param2 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- param2                  \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].param2, (uint64_t)packet->item_defs[item_defs_iter].param2, (double)packet->item_defs[item_defs_iter].param2);

// u32 param3
packet->item_defs[item_defs_iter].param3 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- param3                  \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].param3, (uint64_t)packet->item_defs[item_defs_iter].param3, (double)packet->item_defs[item_defs_iter].param3);

// string string_param1
packet->item_defs[item_defs_iter].string_param1_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->item_defs[item_defs_iter].string_param1 = reinterpret_cast<char*>(arena_push_size(arena, packet->item_defs[item_defs_iter].string_param1_length));
printf("-- STRING_LENGTH           \t%d\n", packet->item_defs[item_defs_iter].string_param1_length);
for (u32 string_param1_iter = 0; string_param1_iter < packet->item_defs[item_defs_iter].string_param1_length; string_param1_iter++)
{
packet->item_defs[item_defs_iter].string_param1[string_param1_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// u32 ui_model_camera_id_2
packet->item_defs[item_defs_iter].ui_model_camera_id_2 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- ui_model_camera_id_2    \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].ui_model_camera_id_2, (uint64_t)packet->item_defs[item_defs_iter].ui_model_camera_id_2, (double)packet->item_defs[item_defs_iter].ui_model_camera_id_2);

// u32 unk_dword_8
packet->item_defs[item_defs_iter].unk_dword_8 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- unk_dword_8             \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].unk_dword_8, (uint64_t)packet->item_defs[item_defs_iter].unk_dword_8, (double)packet->item_defs[item_defs_iter].unk_dword_8);

// i32 scrap_value_override
packet->item_defs[item_defs_iter].scrap_value_override = endian_read_i32_little(data + offset);
offset += sizeof(i32);
printf("-- scrap_value_override    \t%lld\t%llxh\t%f\n", (i64)packet->item_defs[item_defs_iter].scrap_value_override, (uint64_t)packet->item_defs[item_defs_iter].scrap_value_override, (double)packet->item_defs[item_defs_iter].scrap_value_override);

} // item_defs

// list loadoutSlots
packet->loadoutSlots_count = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->loadoutSlots = static_cast<Login_Packet_CharacterSelectInfoReply::loadoutSlots_s*>(arena_push_size(arena, packet->loadoutSlots_count * sizeof(packet->loadoutSlots[0])));
printf("-- LIST_COUNT              \t%d\n", packet->loadoutSlots_count);
for (u32 loadoutSlots_iter = 0; loadoutSlots_iter < packet->loadoutSlots_count; loadoutSlots_iter++)
{
// u32 hotbarSlotId
packet->loadoutSlots[loadoutSlots_iter].hotbarSlotId = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- hotbarSlotId            \t%lld\t%llxh\t%f\n", (i64)packet->loadoutSlots[loadoutSlots_iter].hotbarSlotId, (uint64_t)packet->loadoutSlots[loadoutSlots_iter].hotbarSlotId, (double)packet->loadoutSlots[loadoutSlots_iter].hotbarSlotId);

// u32 loadoutId
packet->loadoutSlots[loadoutSlots_iter].loadoutId = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- loadoutId               \t%lld\t%llxh\t%f\n", (i64)packet->loadoutSlots[loadoutSlots_iter].loadoutId, (uint64_t)packet->loadoutSlots[loadoutSlots_iter].loadoutId, (double)packet->loadoutSlots[loadoutSlots_iter].loadoutId);

// u32 slotId
packet->loadoutSlots[loadoutSlots_iter].slotId = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- slotId                  \t%lld\t%llxh\t%f\n", (i64)packet->loadoutSlots[loadoutSlots_iter].slotId, (uint64_t)packet->loadoutSlots[loadoutSlots_iter].slotId, (double)packet->loadoutSlots[loadoutSlots_iter].slotId);

// u32 itemDefId
packet->loadoutSlots[loadoutSlots_iter].itemDefId = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- itemDefId               \t%lld\t%llxh\t%f\n", (i64)packet->loadoutSlots[loadoutSlots_iter].itemDefId, (uint64_t)packet->loadoutSlots[loadoutSlots_iter].itemDefId, (double)packet->loadoutSlots[loadoutSlots_iter].itemDefId);

// u64 loadoutItemGuid
packet->loadoutSlots[loadoutSlots_iter].loadoutItemGuid = endian_read_u64_little(data + offset);
offset += sizeof(u64);
printf("-- loadoutItemGuid         \t%lld\t%llxh\t%f\n", (i64)packet->loadoutSlots[loadoutSlots_iter].loadoutItemGuid, (uint64_t)packet->loadoutSlots[loadoutSlots_iter].loadoutItemGuid, (double)packet->loadoutSlots[loadoutSlots_iter].loadoutItemGuid);

// u8 unkByte1
packet->loadoutSlots[loadoutSlots_iter].unkByte1 = endian_read_u8_little(data + offset);
offset += sizeof(u8);
printf("-- unkByte1                \t%lld\t%llxh\t%f\n", (i64)packet->loadoutSlots[loadoutSlots_iter].unkByte1, (uint64_t)packet->loadoutSlots[loadoutSlots_iter].unkByte1, (double)packet->loadoutSlots[loadoutSlots_iter].unkByte1);

// u32 unkDword1
packet->loadoutSlots[loadoutSlots_iter].unkDword1 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- unkDword1               \t%lld\t%llxh\t%f\n", (i64)packet->loadoutSlots[loadoutSlots_iter].unkDword1, (uint64_t)packet->loadoutSlots[loadoutSlots_iter].unkDword1, (double)packet->loadoutSlots[loadoutSlots_iter].unkDword1);

} // loadoutSlots

// list characters
packet->characters_count = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->characters = static_cast<Login_Packet_CharacterSelectInfoReply::characters_s*>(arena_push_size(arena, packet->characters_count * sizeof(packet->characters[0])));
printf("-- LIST_COUNT              \t%d\n", packet->characters_count);
for (u32 characters_iter = 0; characters_iter < packet->characters_count; characters_iter++)
{
// u64 charId
packet->characters[characters_iter].charId = endian_read_u64_little(data + offset);
offset += sizeof(u64);
printf("-- charId                  \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].charId, (uint64_t)packet->characters[characters_iter].charId, (double)packet->characters[characters_iter].charId);

// u32 serverId
packet->characters[characters_iter].serverId = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- serverId                \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].serverId, (uint64_t)packet->characters[characters_iter].serverId, (double)packet->characters[characters_iter].serverId);

// u64 lastLoginDate
packet->characters[characters_iter].lastLoginDate = endian_read_u64_little(data + offset);
offset += sizeof(u64);
printf("-- lastLoginDate           \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].lastLoginDate, (uint64_t)packet->characters[characters_iter].lastLoginDate, (double)packet->characters[characters_iter].lastLoginDate);

// u32 nullField
packet->characters[characters_iter].nullField = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- nullField               \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].nullField, (uint64_t)packet->characters[characters_iter].nullField, (double)packet->characters[characters_iter].nullField);

// u32 status
packet->characters[characters_iter].status = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- status                  \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].status, (uint64_t)packet->characters[characters_iter].status, (double)packet->characters[characters_iter].status);

// stream payload
packet->characters[characters_iter].payload_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->characters[characters_iter].payload = static_cast<Login_Packet_CharacterSelectInfoReply::characters_s::payload_s*>(arena_push_size(arena, packet->characters[characters_iter].payload_length * sizeof(packet->characters[characters_iter].payload[0])));
printf("-- STREAM_LENGTH           \t%d\n", packet->characters[characters_iter].payload_length);
for (u32 payload_iter = 0; payload_iter < (packet->characters[characters_iter].payload_length > (u32)0 ? (u32)1 : (u32)0); payload_iter++)
{
// string name
packet->characters[characters_iter].payload[payload_iter].name_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->characters[characters_iter].payload[payload_iter].name = reinterpret_cast<char*>(arena_push_size(arena, packet->characters[characters_iter].payload[payload_iter].name_length));
printf("-- STRING_LENGTH           \t%d\n", packet->characters[characters_iter].payload[payload_iter].name_length);
for (u32 name_iter = 0; name_iter < packet->characters[characters_iter].payload[payload_iter].name_length; name_iter++)
{
packet->characters[characters_iter].payload[payload_iter].name[name_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// u8 empireId
packet->characters[characters_iter].payload[payload_iter].empireId = endian_read_u8_little(data + offset);
offset += sizeof(u8);
printf("-- empireId                \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].payload[payload_iter].empireId, (uint64_t)packet->characters[characters_iter].payload[payload_iter].empireId, (double)packet->characters[characters_iter].payload[payload_iter].empireId);

// u32 battleRank
packet->characters[characters_iter].payload[payload_iter].battleRank = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- battleRank              \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].payload[payload_iter].battleRank, (uint64_t)packet->characters[characters_iter].payload[payload_iter].battleRank, (double)packet->characters[characters_iter].payload[payload_iter].battleRank);

// u32 nextBattleRankPercent
packet->characters[characters_iter].payload[payload_iter].nextBattleRankPercent = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- nextBattleRankPercent   \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].payload[payload_iter].nextBattleRankPercent, (uint64_t)packet->characters[characters_iter].payload[payload_iter].nextBattleRankPercent, (double)packet->characters[characters_iter].payload[payload_iter].nextBattleRankPercent);

// u32 headId
packet->characters[characters_iter].payload[payload_iter].headId = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- headId                  \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].payload[payload_iter].headId, (uint64_t)packet->characters[characters_iter].payload[payload_iter].headId, (double)packet->characters[characters_iter].payload[payload_iter].headId);

// u32 actorModelId
packet->characters[characters_iter].payload[payload_iter].actorModelId = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- actorModelId            \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].payload[payload_iter].actorModelId, (uint64_t)packet->characters[characters_iter].payload[payload_iter].actorModelId, (double)packet->characters[characters_iter].payload[payload_iter].actorModelId);

// u32 gender
packet->characters[characters_iter].payload[payload_iter].gender = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- gender                  \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].payload[payload_iter].gender, (uint64_t)packet->characters[characters_iter].payload[payload_iter].gender, (double)packet->characters[characters_iter].payload[payload_iter].gender);

// u32 profileId
packet->characters[characters_iter].payload[payload_iter].profileId = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- profileId               \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].payload[payload_iter].profileId, (uint64_t)packet->characters[characters_iter].payload[payload_iter].profileId, (double)packet->characters[characters_iter].payload[payload_iter].profileId);

// u32 unkDword1
packet->characters[characters_iter].payload[payload_iter].unkDword1 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- unkDword1               \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].payload[payload_iter].unkDword1, (uint64_t)packet->characters[characters_iter].payload[payload_iter].unkDword1, (double)packet->characters[characters_iter].payload[payload_iter].unkDword1);

// u32 unkDword2
packet->characters[characters_iter].payload[payload_iter].unkDword2 = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- unkDword2               \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].payload[payload_iter].unkDword2, (uint64_t)packet->characters[characters_iter].payload[payload_iter].unkDword2, (double)packet->characters[characters_iter].payload[payload_iter].unkDword2);

// u64 lastUseDate
packet->characters[characters_iter].payload[payload_iter].lastUseDate = endian_read_u64_little(data + offset);
offset += sizeof(u64);
printf("-- lastUseDate             \t%lld\t%llxh\t%f\n", (i64)packet->characters[characters_iter].payload[payload_iter].lastUseDate, (uint64_t)packet->characters[characters_iter].payload[payload_iter].lastUseDate, (double)packet->characters[characters_iter].payload[payload_iter].lastUseDate);

} // payload

} // characters

} break;

case Login_Packet_Kind_ServerListReply:
{
printf("[*] Unpacking ServerListReply...\n");
Login_Packet_ServerListReply* packet = static_cast<Login_Packet_ServerListReply*>(packet_ptr);

// list servers
packet->servers_count = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->servers = static_cast<Login_Packet_ServerListReply::servers_s*>(arena_push_size(arena, packet->servers_count * sizeof(packet->servers[0])));
printf("-- LIST_COUNT              \t%d\n", packet->servers_count);
for (u32 servers_iter = 0; servers_iter < packet->servers_count; servers_iter++)
{
// u32 id
packet->servers[servers_iter].id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- id                      \t%lld\t%llxh\t%f\n", (i64)packet->servers[servers_iter].id, (uint64_t)packet->servers[servers_iter].id, (double)packet->servers[servers_iter].id);

// u32 state
packet->servers[servers_iter].state = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- state                   \t%lld\t%llxh\t%f\n", (i64)packet->servers[servers_iter].state, (uint64_t)packet->servers[servers_iter].state, (double)packet->servers[servers_iter].state);

// b8 is_locked
packet->servers[servers_iter].is_locked = endian_read_b8_little(data + offset);
offset += sizeof(b8);
printf("-- is_locked               \t%lld\t%llxh\t%f\n", (i64)packet->servers[servers_iter].is_locked, (uint64_t)packet->servers[servers_iter].is_locked, (double)packet->servers[servers_iter].is_locked);

// string name
packet->servers[servers_iter].name_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->servers[servers_iter].name = reinterpret_cast<char*>(arena_push_size(arena, packet->servers[servers_iter].name_length));
printf("-- STRING_LENGTH           \t%d\n", packet->servers[servers_iter].name_length);
for (u32 name_iter = 0; name_iter < packet->servers[servers_iter].name_length; name_iter++)
{
packet->servers[servers_iter].name[name_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// u32 name_id
packet->servers[servers_iter].name_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- name_id                 \t%lld\t%llxh\t%f\n", (i64)packet->servers[servers_iter].name_id, (uint64_t)packet->servers[servers_iter].name_id, (double)packet->servers[servers_iter].name_id);

// string description
packet->servers[servers_iter].description_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->servers[servers_iter].description = reinterpret_cast<char*>(arena_push_size(arena, packet->servers[servers_iter].description_length));
printf("-- STRING_LENGTH           \t%d\n", packet->servers[servers_iter].description_length);
for (u32 description_iter = 0; description_iter < packet->servers[servers_iter].description_length; description_iter++)
{
packet->servers[servers_iter].description[description_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// u32 description_id
packet->servers[servers_iter].description_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- description_id          \t%lld\t%llxh\t%f\n", (i64)packet->servers[servers_iter].description_id, (uint64_t)packet->servers[servers_iter].description_id, (double)packet->servers[servers_iter].description_id);

// u32 req_feature_id
packet->servers[servers_iter].req_feature_id = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- req_feature_id          \t%lld\t%llxh\t%f\n", (i64)packet->servers[servers_iter].req_feature_id, (uint64_t)packet->servers[servers_iter].req_feature_id, (double)packet->servers[servers_iter].req_feature_id);

// string server_info
packet->servers[servers_iter].server_info_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->servers[servers_iter].server_info = reinterpret_cast<char*>(arena_push_size(arena, packet->servers[servers_iter].server_info_length));
printf("-- STRING_LENGTH           \t%d\n", packet->servers[servers_iter].server_info_length);
for (u32 server_info_iter = 0; server_info_iter < packet->servers[servers_iter].server_info_length; server_info_iter++)
{
packet->servers[servers_iter].server_info[server_info_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// u32 population_level
packet->servers[servers_iter].population_level = endian_read_u32_little(data + offset);
offset += sizeof(u32);
printf("-- population_level        \t%lld\t%llxh\t%f\n", (i64)packet->servers[servers_iter].population_level, (uint64_t)packet->servers[servers_iter].population_level, (double)packet->servers[servers_iter].population_level);

// string population_data
packet->servers[servers_iter].population_data_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->servers[servers_iter].population_data = reinterpret_cast<char*>(arena_push_size(arena, packet->servers[servers_iter].population_data_length));
printf("-- STRING_LENGTH           \t%d\n", packet->servers[servers_iter].population_data_length);
for (u32 population_data_iter = 0; population_data_iter < packet->servers[servers_iter].population_data_length; population_data_iter++)
{
packet->servers[servers_iter].population_data[population_data_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// string access_expression
packet->servers[servers_iter].access_expression_length = endian_read_u32_little(data + offset);
offset += sizeof(u32);
packet->servers[servers_iter].access_expression = reinterpret_cast<char*>(arena_push_size(arena, packet->servers[servers_iter].access_expression_length));
printf("-- STRING_LENGTH           \t%d\n", packet->servers[servers_iter].access_expression_length);
for (u32 access_expression_iter = 0; access_expression_iter < packet->servers[servers_iter].access_expression_length; access_expression_iter++)
{
packet->servers[servers_iter].access_expression[access_expression_iter] = *(i8*)((uptr)data + offset);
offset++;
}

// b8 is_access_allowed
packet->servers[servers_iter].is_access_allowed = endian_read_b8_little(data + offset);
offset += sizeof(b8);
printf("-- is_access_allowed       \t%lld\t%llxh\t%f\n", (i64)packet->servers[servers_iter].is_access_allowed, (uint64_t)packet->servers[servers_iter].is_access_allowed, (double)packet->servers[servers_iter].is_access_allowed);

} // servers

} break;

default:
{
printf("Unpacking not implemented\n");
}
}
}
