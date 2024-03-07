#include "login_packets.hpp"

Buffer pack_login_packet(Arena* arena, void* packet_ptr, Login_Packet_Kind packet_kind) 
{
	uintptr_t prev_align = arena->alignment;
	arena->alignment = 1;
	uint8_t* data_begin = (uint8_t*)((uintptr_t)arena->memory + arena->cursor);

	switch (packet_kind)
	{
	case Login_Packet_Kind_LoginRequest:
    {
        Login_Packet_LoginRequest* packet = (Login_Packet_LoginRequest*)packet_ptr;

        push_u8_little(arena, 0x1);

        push_uint32_t_little(arena, (uint32_t)packet->session_id.size);
        for (uintptr_t session_id_iter = 0; session_id_iter < (uintptr_t)packet->session_id.size; session_id_iter++)
        {
            push_u8_little(arena, packet->session_id.data[session_id_iter]);
        }

        push_uint32_t_little(arena, (uint32_t)packet->system_fingerprint.size);
        for (uintptr_t system_fingerprint_iter = 0; system_fingerprint_iter < (uintptr_t)packet->system_fingerprint.size; system_fingerprint_iter++)
        {
            push_u8_little(arena, packet->system_fingerprint.data[system_fingerprint_iter]);
        }

        push_uint32_t_little(arena, packet->locale);
        push_uint32_t_little(arena, packet->third_party_auth_ticket);
        push_uint32_t_little(arena, packet->third_party_user_id);
        push_uint32_t_little(arena, packet->third_party_id);
    } break;
    case Login_Packet_Kind_LoginReply:
    {
        Login_Packet_LoginReply* packet = (Login_Packet_LoginReply*)packet_ptr;
        push_u8_little(arena, 0x2);
        // 13:  b8 is_logged_in
        push_b8_little(arena, packet->is_logged_in);
        // 14:  uint32_t status
        push_uint32_t_little(arena, packet->status);
        // 15:  uint32_t result_code
        push_uint32_t_little(arena, packet->result_code);
        // 16:  b8 is_member
        push_b8_little(arena, packet->is_member);
        // 17:  b8 is_internal
        push_b8_little(arena, packet->is_internal);
        // 18:  string:uint32_t namespace_name
        push_uint32_t_little(arena, (uint32_t)packet->namespace_name.size);
        for (uintptr_t namespace_name_iter = 0; namespace_name_iter < (uintptr_t)packet->namespace_name.size; namespace_name_iter++)
        {
            push_u8_little(arena, packet->namespace_name.data[namespace_name_iter]);
        }
        // 19:  list:uint32_t account_features
        push_uint32_t_little(arena, packet->account_features_count);
        for (uintptr_t account_features_iter = 0; account_features_iter < packet->account_features_count; account_features_iter++)
        {
            // 21:  uint32_t key
            push_uint32_t_little(arena, packet->account_features[account_features_iter].key);
            // 22:  uint32_t id
            push_uint32_t_little(arena, packet->account_features[account_features_iter].id);
            // 23:  b8 active
            push_b8_little(arena, packet->account_features[account_features_iter].active);
            // 24:  uint32_t remaining_count
            push_uint32_t_little(arena, packet->account_features[account_features_iter].remaining_count);
            // 25:  string:uint32_t raw_data
            push_uint32_t_little(arena, (uint32_t)packet->account_features[account_features_iter].raw_data.size);
            for (uintptr_t raw_data_iter = 0; raw_data_iter < (uintptr_t)packet->account_features[account_features_iter].raw_data.size; raw_data_iter++)
            {
                push_u8_little(arena, packet->account_features[account_features_iter].raw_data.data[raw_data_iter]);
            }
        } // account_features
          // 27:  string:uint32_t application_payload
        push_uint32_t_little(arena, (uint32_t)packet->application_payload.size);
        for (uintptr_t application_payload_iter = 0; application_payload_iter < (uintptr_t)packet->application_payload.size; application_payload_iter++)
        {
            push_u8_little(arena, packet->application_payload.data[application_payload_iter]);
        }
        // 28:  list:uint32_t error_details
        push_uint32_t_little(arena, packet->error_details_count);
        for (uintptr_t error_details_iter = 0; error_details_iter < packet->error_details_count; error_details_iter++)
        {
            // 30:  uint32_t unknown_dword1
            push_uint32_t_little(arena, packet->error_details[error_details_iter].unknown_dword1);
            // 31:  string:uint32_t name
            push_uint32_t_little(arena, (uint32_t)packet->error_details[error_details_iter].name.size);
            for (uintptr_t name_iter = 0; name_iter < (uintptr_t)packet->error_details[error_details_iter].name.size; name_iter++)
            {
                push_u8_little(arena, packet->error_details[error_details_iter].name.data[name_iter]);
            }
            // 32:  string:uint32_t value
            push_uint32_t_little(arena, (uint32_t)packet->error_details[error_details_iter].value.size);
            for (uintptr_t value_iter = 0; value_iter < (uintptr_t)packet->error_details[error_details_iter].value.size; value_iter++)
            {
                push_u8_little(arena, packet->error_details[error_details_iter].value.data[value_iter]);
            }
        } // error_details
          // 34:  string:uint32_t ip_country_code
        push_uint32_t_little(arena, (uint32_t)packet->ip_country_code.size);
        for (uintptr_t ip_country_code_iter = 0; ip_country_code_iter < (uintptr_t)packet->ip_country_code.size; ip_country_code_iter++)
        {
            push_u8_little(arena, packet->ip_country_code.data[ip_country_code_iter]);
        }
    } break;
    case Login_Packet_Kind_Logout:
    {
        push_u8_little(arena, 0x3);
    } break;

    case Login_Packet_Kind_ForceDisconnect:
    {
        push_u8_little(arena, 0x4);
    } break;
    case Login_Packet_Kind_CharacterCreateRequest:
    {
        Login_Packet_CharacterCreateRequest* packet = (Login_Packet_CharacterCreateRequest*)packet_ptr;
        push_u8_little(arena, 0x5);
        // 41:  uint32_t server_id
        push_uint32_t_little(arena, packet->server_id);
        // 42:  uint32_t unk_uint32_t
        push_uint32_t_little(arena, packet->unk_u32);
        // 44:  stream: char_payload
        void* char_payload_size_ptr = (void*)((uintptr_t)arena->memory + arena->cursor);
        arena->cursor += sizeof(uint32_t);
        uint32_t will_pack_char_payload = packet->char_payload_size == ~0 ? 0 : 1;
        if (!will_pack_char_payload)
        {
            write_uint32_t_little_at(char_payload_size_ptr, 0);
        }
        else
        {
            // 47:  u8 empire_id
            push_u8_little(arena, packet->char_payload.empire_id);
            // 48:  uint32_t head_type
            push_uint32_t_little(arena, packet->char_payload.head_type);
            // 49:  uint32_t profile_type
            push_uint32_t_little(arena, packet->char_payload.profile_type);
            // 50:  uint32_t gender
            push_uint32_t_little(arena, packet->char_payload.gender);
            // 51:  string:uint32_t character_name
            push_uint32_t_little(arena, (uint32_t)packet->char_payload.character_name.size);
            for (uintptr_t character_name_iter = 0; character_name_iter < (uintptr_t)packet->char_payload.character_name.size; character_name_iter++)
            {
                push_u8_little(arena, packet->char_payload.character_name.data[character_name_iter]);
            }
            write_uint32_t_little_at(char_payload_size_ptr, (uint32_t)(((uintptr_t)arena->memory + arena->cursor) - ((uintptr_t)char_payload_size_ptr + sizeof(uint32_t))));
        } // char_payload
    } break;    
    case Login_Packet_Kind_CharacterCreateReply:
    {
        Login_Packet_CharacterCreateReply* packet = (Login_Packet_CharacterCreateReply*)packet_ptr;
        push_u8_little(arena, 0x6);
        // 55:  uint32_t status
        push_uint32_t_little(arena, packet->status);
        // 56:  u64 character_id
        push_uint64_t_little(arena, packet->character_id);
    } break;
    case Login_Packet_Kind_CharacterLoginRequest:
    {
        Login_Packet_CharacterLoginRequest* packet = (Login_Packet_CharacterLoginRequest*)packet_ptr;
        push_u8_little(arena, 0x7);
        // 59:  u64 character_id
        push_uint64_t_little(arena, packet->character_id);
        // 60:  uint32_t server_id
        push_uint32_t_little(arena, packet->server_id);
        // 61:  uint32_t status
        push_uint32_t_little(arena, packet->status);
        // 63:  stream: payload2
        void* payload2_size_ptr = (void*)((uintptr_t)arena->memory + arena->cursor);
        arena->cursor += sizeof(uint32_t);
        uint32_t will_pack_payload2 = packet->payload2_size == ~0 ? 0 : 1;
        if (!will_pack_payload2)
        {
            write_uint32_t_little_at(payload2_size_ptr, 0);
        }
        else
        {
            // 66:  string:uint32_t locale
            push_uint32_t_little(arena, (uint32_t)packet->payload2.locale.size);
            for (uintptr_t locale_iter = 0; locale_iter < (uintptr_t)packet->payload2.locale.size; locale_iter++)
            {
                push_u8_little(arena, packet->payload2.locale.data[locale_iter]);
            }
            // 67:  uint32_t locale_id
            push_uint32_t_little(arena, packet->payload2.locale_id);
            // 68:  uint32_t preferred_gateway_id
            push_uint32_t_little(arena, packet->payload2.preferred_gateway_id);
            write_uint32_t_little_at(payload2_size_ptr, (uint32_t)(((uintptr_t)arena->memory + arena->cursor) - ((uintptr_t)payload2_size_ptr + sizeof(uint32_t))));
        } // payload2
    } break;
    case Login_Packet_Kind_CharacterLoginReply:
    {
        Login_Packet_CharacterLoginReply* packet = (Login_Packet_CharacterLoginReply*)packet_ptr;
        push_u8_little(arena, 0x8);
        // 72:  u64 character_id
        push_uint64_t_little(arena, packet->character_id);
        // 73:  uint32_t server_id
        push_uint32_t_little(arena, packet->server_id);
        // 74:  uint32_t last_login
        push_uint32_t_little(arena, packet->last_login);
        // 75:  uint32_t status
        push_uint32_t_little(arena, packet->status);
        // 77:  stream: login_payload
        void* login_payload_size_ptr = (void*)((uintptr_t)arena->memory + arena->cursor);
        arena->cursor += sizeof(uint32_t);
        uint32_t will_pack_login_payload = packet->login_payload_size == ~0 ? 0 : 1;
        if (!will_pack_login_payload)
        {
            write_uint32_t_little_at(login_payload_size_ptr, 0);
        }
        else
        {
            // 79:  u8 unk_byte_1
            push_u8_little(arena, packet->login_payload.unk_byte_1);
            // 80:  u8 unk_byte_2
            push_u8_little(arena, packet->login_payload.unk_byte_2);
            // 81:  string:uint32_t server_address
            push_uint32_t_little(arena, (uint32_t)packet->login_payload.server_address.size);
            for (uintptr_t server_address_iter = 0; server_address_iter < (uintptr_t)packet->login_payload.server_address.size; server_address_iter++)
            {
                push_u8_little(arena, packet->login_payload.server_address.data[server_address_iter]);
            }
            // 82:  string:uint32_t server_ticket
            push_uint32_t_little(arena, (uint32_t)packet->login_payload.server_ticket.size);
            for (uintptr_t server_ticket_iter = 0; server_ticket_iter < (uintptr_t)packet->login_payload.server_ticket.size; server_ticket_iter++)
            {
                push_u8_little(arena, packet->login_payload.server_ticket.data[server_ticket_iter]);
            }
            // 83:  bytes:uint32_t encryption_key
            push_uint32_t_little(arena, (uint32_t)packet->login_payload.encryption_key.size);
            for (uintptr_t encryption_key_iter = 0; encryption_key_iter < packet->login_payload.encryption_key.size; encryption_key_iter++)
            {
                push_u8_little(arena, packet->login_payload.encryption_key.data[encryption_key_iter]);
            }
            // 84:  uint32_t soe_protocol_version
            push_uint32_t_little(arena, packet->login_payload.soe_protocol_version);
            // 85:  u64 character_id
            push_uint64_t_little(arena, packet->login_payload.character_id);
            // 86:  u64 unk_u64
            push_uint64_t_little(arena, packet->login_payload.unk_u64);
            // 87:  string:uint32_t station_name
            push_uint32_t_little(arena, (uint32_t)packet->login_payload.station_name.size);
            for (uintptr_t station_name_iter = 0; station_name_iter < (uintptr_t)packet->login_payload.station_name.size; station_name_iter++)
            {
                push_u8_little(arena, packet->login_payload.station_name.data[station_name_iter]);
            }
            // 88:  string:uint32_t character_name
            push_uint32_t_little(arena, (uint32_t)packet->login_payload.character_name.size);
            for (uintptr_t character_name_iter = 0; character_name_iter < (uintptr_t)packet->login_payload.character_name.size; character_name_iter++)
            {
                push_u8_little(arena, packet->login_payload.character_name.data[character_name_iter]);
            }
            // 89:  string:uint32_t unk_str
            push_uint32_t_little(arena, (uint32_t)packet->login_payload.unk_str.size);
            for (uintptr_t unk_str_iter = 0; unk_str_iter < (uintptr_t)packet->login_payload.unk_str.size; unk_str_iter++)
            {
                push_u8_little(arena, packet->login_payload.unk_str.data[unk_str_iter]);
            }
            // 90:  u64 server_feature_bit
            push_uint64_t_little(arena, packet->login_payload.server_feature_bit);
            write_uint32_t_little_at(login_payload_size_ptr, (uint32_t)(((uintptr_t)arena->memory + arena->cursor) - ((uintptr_t)login_payload_size_ptr + sizeof(uint32_t))));
        } // login_payload
    } break;
    case Login_Packet_Kind_CharacterDeleteRequest:
    {
        Login_Packet_CharacterDeleteRequest* packet = (Login_Packet_CharacterDeleteRequest*)packet_ptr;
        push_u8_little(arena, 0x9);
        // 94:  u64 character_id
        push_uint64_t_little(arena, packet->character_id);
    } break;
    case Login_Packet_Kind_TunnelAppPacketClientToServer:
    {
        Login_Packet_TunnelAppPacketClientToServer* packet = (Login_Packet_TunnelAppPacketClientToServer*)packet_ptr;
        push_u8_little(arena, 0x10);
        // 97:  uint32_t server_id
        push_uint32_t_little(arena, packet->server_id);
        // 98:  uint32_t unk1
        push_uint32_t_little(arena, packet->unk1);
        // 100:  stream: data_client
        void* data_client_size_ptr = (void*)((uintptr_t)arena->memory + arena->cursor);
        arena->cursor += sizeof(uint32_t);
        uint32_t will_pack_data_client = packet->data_client_size == ~0 ? 0 : 1;
        if (!will_pack_data_client)
        {
            write_uint32_t_little_at(data_client_size_ptr, 0);
        }
        else
        {
            // 102:  b8 tunnel_op_code
            push_b8_little(arena, packet->data_client.tunnel_op_code);
            // 103:  b8 sub_op_code
            push_b8_little(arena, packet->data_client.sub_op_code);
            // 104:  string:uint32_t character_name
            push_uint32_t_little(arena, (uint32_t)packet->data_client.character_name.size);
            for (uintptr_t character_name_iter = 0; character_name_iter < (uintptr_t)packet->data_client.character_name.size; character_name_iter++)
            {
                push_u8_little(arena, packet->data_client.character_name.data[character_name_iter]);
            }
            write_uint32_t_little_at(data_client_size_ptr, (uint32_t)(((uintptr_t)arena->memory + arena->cursor) - ((uintptr_t)data_client_size_ptr + sizeof(uint32_t))));
        } // data_client
    } break;
    case Login_Packet_Kind_TunnelAppPacketServerToClient:
    {
        Login_Packet_TunnelAppPacketServerToClient* packet = (Login_Packet_TunnelAppPacketServerToClient*)packet_ptr;
        push_u8_little(arena, 0x11);
        // 108:  u32 server_id
        push_uint32_t_little(arena, packet->server_id);
        // 109:  u32 unk1
        push_uint32_t_little(arena, packet->unk1);
        // 111:  stream: data_server
        void* data_server_size_ptr = (void*)((uintptr_t)arena->memory + arena->cursor);
        arena->cursor += sizeof(uint32_t);
        uint32_t will_pack_data_server = packet->data_server_size == ~0 ? 0 : 1;
        if (!will_pack_data_server)
        {
            write_uint32_t_little_at(data_server_size_ptr, 0);
        }
        else
        {
            // 113:  b8 tunnel_op_code
            push_b8_little(arena, packet->data_server.tunnel_op_code);
            // 114:  b8 sub_op_code
            push_b8_little(arena, packet->data_server.sub_op_code);
            // 115:  string:u32 character_name
            push_uint32_t_little(arena, (uint32_t)packet->data_server.character_name.size);
            for (uintptr_t character_name_iter = 0; character_name_iter < (uintptr_t)packet->data_server.character_name.size; character_name_iter++)
            {
                push_u8_little(arena, packet->data_server.character_name.data[character_name_iter]);
            }
            // 116:  string:u32 character_name2
            push_uint32_t_little(arena, (uint32_t)packet->data_server.character_name2.size);
            for (uintptr_t character_name2_iter = 0; character_name2_iter < (uintptr_t)packet->data_server.character_name2.size; character_name2_iter++)
            {
                push_u8_little(arena, packet->data_server.character_name2.data[character_name2_iter]);
            }
            // 117:  u32 status
            push_uint32_t_little(arena, packet->data_server.status);
            write_uint32_t_little_at(data_server_size_ptr, (uint32_t)(((uintptr_t)arena->memory + arena->cursor) - ((uintptr_t)data_server_size_ptr + sizeof(uint32_t))));
        } // data_server
    } break;
    case Login_Packet_Kind_CharacterTransferRequest:
    {
        push_u8_little(arena, 0x12);
    } break;
    case Login_Packet_Kind_CharacterTransferReply:
    {
        push_u8_little(arena, 0x13);
    } break;
    case Login_Packet_Kind_CharacterDeleteReply:
    {
        Login_Packet_CharacterDeleteReply* packet = (Login_Packet_CharacterDeleteReply*)packet_ptr;
        push_u8_little(arena, 0xa);
        // 125:  u64 character_id
        push_uint64_t_little(arena, packet->character_id);
        // 126:  u32 status
        push_uint32_t_little(arena, packet->status);
        // 127:  bytes:u32 payload3
        push_uint32_t_little(arena, (uint32_t)packet->payload3.size);
        for (uintptr_t payload3_iter = 0; payload3_iter < packet->payload3.size; payload3_iter++)
        {
            push_u8_little(arena, packet->payload3.data[payload3_iter]);
        }
    } break;
    case Login_Packet_Kind_CharacterSelectInfoRequest:
    {
        push_u8_little(arena, 0xb);
    } break;
    case Login_Packet_Kind_CharacterSelectInfoReply:
    {
        Login_Packet_CharacterSelectInfoReply* packet = (Login_Packet_CharacterSelectInfoReply*)packet_ptr;
        push_u8_little(arena, 0xc);
        // 132:  u32 character_status
        push_uint32_t_little(arena, packet->character_status);
        // 133:  b8 can_bypass_server_lock
        push_b8_little(arena, packet->can_bypass_server_lock);
        // 135:  list:u32 characters
        push_uint32_t_little(arena, packet->characters_count);
        for (uintptr_t characters_iter = 0; characters_iter < packet->characters_count; characters_iter++)
        {
            // 137:  u64 charId
            push_uint64_t_little(arena, packet->characters[characters_iter].charId);
            // 138:  u32 serverId
            push_uint32_t_little(arena, packet->characters[characters_iter].serverId);
            // 139:  u64 lastLoginDate
            push_uint64_t_little(arena, packet->characters[characters_iter].lastLoginDate);
            // 140:  u32 nullField
            push_uint32_t_little(arena, packet->characters[characters_iter].nullField);
            // 141:  u32 status
            push_uint32_t_little(arena, packet->characters[characters_iter].status);
            // 143:  stream: payload
            void* payload_size_ptr = (void*)((uintptr_t)arena->memory + arena->cursor);
            arena->cursor += sizeof(uint32_t);
            uint32_t will_pack_payload = packet->characters[characters_iter].payload_size == ~0 ? 0 : 1;
            if (!will_pack_payload)
            {
                write_uint32_t_little_at(payload_size_ptr, 0);
            }
            else
            {
                // 145:  string:u32 name
                push_uint32_t_little(arena, (uint32_t)packet->characters[characters_iter].payload.name.size);
                for (uintptr_t name_iter = 0; name_iter < (uintptr_t)packet->characters[characters_iter].payload.name.size; name_iter++)
                {
                    push_u8_little(arena, packet->characters[characters_iter].payload.name.data[name_iter]);
                }
                // 146:  u8 empireId
                push_u8_little(arena, packet->characters[characters_iter].payload.empireId);
                // 147:  u32 battleRank
                push_uint32_t_little(arena, packet->characters[characters_iter].payload.battleRank);
                // 148:  u32 nextBattleRankPercent
                push_uint32_t_little(arena, packet->characters[characters_iter].payload.nextBattleRankPercent);
                // 149:  u32 headId
                push_uint32_t_little(arena, packet->characters[characters_iter].payload.headId);
                // 150:  u32 actorModelId
                push_uint32_t_little(arena, packet->characters[characters_iter].payload.actorModelId);
                // 151:  u32 gender
                push_uint32_t_little(arena, packet->characters[characters_iter].payload.gender);
                // 152:  u32 profileId
                push_uint32_t_little(arena, packet->characters[characters_iter].payload.profileId);
                // 153:  u32 unkDword1
                push_uint32_t_little(arena, packet->characters[characters_iter].payload.unkDword1);
                // 154:  u32 unkDword2
                push_uint32_t_little(arena, packet->characters[characters_iter].payload.unkDword2);
                // 156:  list:u32 loadoutSlots
                push_uint32_t_little(arena, packet->characters[characters_iter].payload.loadoutSlots_count);
                for (uintptr_t loadoutSlots_iter = 0; loadoutSlots_iter < packet->characters[characters_iter].payload.loadoutSlots_count; loadoutSlots_iter++)
                {
                    // 158:  u32 hotbarSlotId
                    push_uint32_t_little(arena, packet->characters[characters_iter].payload.loadoutSlots[loadoutSlots_iter].hotbarSlotId);
                    // 159:  u32 loadoutId
                    push_uint32_t_little(arena, packet->characters[characters_iter].payload.loadoutSlots[loadoutSlots_iter].loadoutId);
                    // 160:  u32 slotId
                    push_uint32_t_little(arena, packet->characters[characters_iter].payload.loadoutSlots[loadoutSlots_iter].slotId);
                    // 161:  u32 itemDefId
                    push_uint32_t_little(arena, packet->characters[characters_iter].payload.loadoutSlots[loadoutSlots_iter].itemDefId);
                    // 162:  u64 loadoutItemGuid
                    push_uint64_t_little(arena, packet->characters[characters_iter].payload.loadoutSlots[loadoutSlots_iter].loadoutItemGuid);
                    // 163:  u8 unkByte1
                    push_u8_little(arena, packet->characters[characters_iter].payload.loadoutSlots[loadoutSlots_iter].unkByte1);
                    // 164:  u32 unkDword1
                    push_uint32_t_little(arena, packet->characters[characters_iter].payload.loadoutSlots[loadoutSlots_iter].unkDword1);
                } // loadoutSlots
                  // 167:  list:u32 itemDefinitions
                push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions_count);
                for (uintptr_t itemDefinitions_iter = 0; itemDefinitions_iter < packet->characters[characters_iter].payload.itemDefinitions_count; itemDefinitions_iter++)
                {
                    // 169:  u32 ID
                    push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].ID);
                    // 171:  list:u32 item_defs
                    push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs_count);
                    for (uintptr_t item_defs_iter = 0; item_defs_iter < packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs_count; item_defs_iter++)
                    {
                        // 173:  u32 defs_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].defs_id);
                        // 175:  u8 bitflags1
                        push_u8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].bitflags1);
                        // 185:  u8 bitflags2
                        push_u8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].bitflags2);
                        // 195:  u32 name_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].name_id);
                        // 196:  u32 description_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].description_id);
                        // 197:  u32 content_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].content_id);
                        // 198:  u32 image_set_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].image_set_id);
                        // 199:  u32 tint_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].tint_id);
                        // 200:  u32 hud_image_set_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].hud_image_set_id);
                        // 201:  u32 unk_dword_1
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_1);
                        // 202:  u32 unk_dword_2
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_2);
                        // 203:  u32 cost
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].cost);
                        // 204:  u32 item_class
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].item_class);
                        // 205:  u32 profile_override
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].profile_override);
                        // 206:  string:u32 model_name
                        push_uint32_t_little(arena, (uint32_t)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].model_name.size);
                        for (uintptr_t model_name_iter = 0; model_name_iter < (uintptr_t)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].model_name.size; model_name_iter++)
                        {
                            push_u8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].model_name.data[model_name_iter]);
                        }
                        // 207:  string:u32 texture_alias
                        push_uint32_t_little(arena, (uint32_t)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].texture_alias.size);
                        for (uintptr_t texture_alias_iter = 0; texture_alias_iter < (uintptr_t)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].texture_alias.size; texture_alias_iter++)
                        {
                            push_u8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].texture_alias.data[texture_alias_iter]);
                        }
                        // 208:  u32 gender_usage
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].gender_usage);
                        // 209:  u32 item_type
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].item_type);
                        // 210:  u32 category_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].category_id);
                        // 211:  u32 weapon_trail_effect_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].weapon_trail_effect_id);
                        // 212:  u32 composite_effect_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].composite_effect_id);
                        // 213:  u32 power_rating
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].power_rating);
                        // 214:  u32 min_profile_rank
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].min_profile_rank);
                        // 215:  u32 rarity
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].rarity);
                        // 216:  u32 activatable_ability_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].activatable_ability_id);
                        // 217:  u32 activatable_ability_set_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].activatable_ability_set_id);
                        // 218:  u32 passive_ability_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].passive_ability_id);
                        // 219:  u32 passive_ability_set_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].passive_ability_set_id);
                        // 220:  u32 max_stack_size
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].max_stack_size);
                        // 221:  u32 min_stack_size
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].min_stack_size);
                        // 222:  string:u32 tint_alias
                        push_uint32_t_little(arena, (uint32_t)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].tint_alias.size);
                        for (uintptr_t tint_alias_iter = 0; tint_alias_iter < (uintptr_t)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].tint_alias.size; tint_alias_iter++)
                        {
                            push_u8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].tint_alias.data[tint_alias_iter]);
                        }
                        // 223:  u32 tint_group_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].tint_group_id);
                        // 224:  u32 member_discount
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].member_discount);
                        // 225:  u32 vip_rank_required
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].vip_rank_required);
                        // 226:  u32 race_set_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].race_set_id);
                        // 227:  u32 ui_model_camera_id_1
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].ui_model_camera_id_1);
                        // 228:  u32 equip_count_max
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].equip_count_max);
                        // 229:  u32 curreny_type
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].curreny_type);
                        // 230:  u32 datasheet_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].datasheet_id);
                        // 231:  u32 item_type_1
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].item_type_1);
                        // 232:  u32 skill_set_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].skill_set_id);
                        // 233:  string:u32 overlay_texture
                        push_uint32_t_little(arena, (uint32_t)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].overlay_texture.size);
                        for (uintptr_t overlay_texture_iter = 0; overlay_texture_iter < (uintptr_t)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].overlay_texture.size; overlay_texture_iter++)
                        {
                            push_u8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].overlay_texture.data[overlay_texture_iter]);
                        }
                        // 234:  string:u32 decal_slot
                        push_uint32_t_little(arena, (uint32_t)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].decal_slot.size);
                        for (uintptr_t decal_slot_iter = 0; decal_slot_iter < (uintptr_t)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].decal_slot.size; decal_slot_iter++)
                        {
                            push_u8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].decal_slot.data[decal_slot_iter]);
                        }
                        // 235:  u32 overlay_adjustment
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].overlay_adjustment);
                        // 236:  u32 trial_duration_sec
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].trial_duration_sec);
                        // 237:  u32 next_trial_delay_sec
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].next_trial_delay_sec);
                        // 238:  u32 client_use_requirement
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].client_use_requirement);
                        // 239:  string:u32 override_appearance
                        push_uint32_t_little(arena, (uint32_t)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].override_appearance.size);
                        for (uintptr_t override_appearance_iter = 0; override_appearance_iter < (uintptr_t)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].override_appearance.size; override_appearance_iter++)
                        {
                            push_u8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].override_appearance.data[override_appearance_iter]);
                        }
                        // 240:  u32 override_camera_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].override_camera_id);
                        // 241:  u32 unk_dword_3
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_3);
                        // 242:  u32 unk_dword_4
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_4);
                        // 243:  u32 unk_dword_5
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_5);
                        // 244:  u32 bulk
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].bulk);
                        // 245:  u32 active_equip_slot_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].active_equip_slot_id);
                        // 246:  u32 passive_equip_slot_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].passive_equip_slot_id);
                        // 247:  u32 passive_equip_slot_group_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].passive_equip_slot_group_id);
                        // 248:  u32 unk_dword_6
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_6);
                        // 249:  u32 grinder_reward_set_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].grinder_reward_set_id);
                        // 250:  u32 build_bar_group_id
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].build_bar_group_id);
                        // 251:  string:u32 unk_string_1
                        push_uint32_t_little(arena, (uint32_t)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_string_1.size);
                        for (uintptr_t unk_string_1_iter = 0; unk_string_1_iter < (uintptr_t)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_string_1.size; unk_string_1_iter++)
                        {
                            push_u8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_string_1.data[unk_string_1_iter]);
                        }
                        // 252:  b8 unk_bool_1
                        push_b8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_bool_1);
                        // 253:  b8 is_armor
                        push_b8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].is_armor);
                        // 254:  u32 unk_dword_7
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_7);
                        // 255:  u32 param1
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].param1);
                        // 256:  u32 param2
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].param2);
                        // 257:  u32 param3
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].param3);
                        // 258:  string:u32 string_param1
                        push_uint32_t_little(arena, (uint32_t)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].string_param1.size);
                        for (uintptr_t string_param1_iter = 0; string_param1_iter < (uintptr_t)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].string_param1.size; string_param1_iter++)
                        {
                            push_u8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].string_param1.data[string_param1_iter]);
                        }
                        // 259:  u32 ui_model_camera_id_2
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].ui_model_camera_id_2);
                        // 260:  u32 unk_dword_8
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_8);
                        // 261:  u32 scrap_value_override
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].scrap_value_override);
                        // 263:  list:u32 stats_item_def_2
                        push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2_count);
                        for (uintptr_t stats_item_def_2_iter = 0; stats_item_def_2_iter < packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2_count; stats_item_def_2_iter++)
                        {
                            // 265:  u32 unk_dword_9
                            push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].unk_dword_9);
                            // 266:  u32 stat_id
                            push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].stat_id);
                            // 268:  switch: variabletype8
                            push_u8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].variabletype8_case);
                            switch (packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].variabletype8_case)
                            {
                            case 0:
                            {
                                // 271:  u32 base
                                push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue0.base);
                                // 272:  u32 modifier
                                push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue0.modifier);
                            } break; // statValue0
                            case 1:
                            {
                                // 275:  f32 base
                                push_float_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue1.base);
                                // 276:  f32 modifier
                                push_float_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue1.modifier);
                            } break; // statValue1
                            } // variabletype8
                            // 279:  u32 unk_dword_10
                            push_uint32_t_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].unk_dword_10);
                        } // stats_item_def_2
                    } // item_defs
                } // itemDefinitions
                  // 283:  u64 lastUseDate
                push_uint64_t_little(arena, packet->characters[characters_iter].payload.lastUseDate);
                write_uint32_t_little_at(payload_size_ptr, (uint32_t)(((uintptr_t)arena->memory + arena->cursor) - ((uintptr_t)payload_size_ptr + sizeof(uint32_t))));
            } // payload
        } // characters
    } break;
    case Login_Packet_Kind_ServerListRequest:
    {
        push_u8_little(arena, 0xd);
    } break;
    case Login_Packet_Kind_ServerListReply:
    {
        Login_Packet_ServerListReply* packet = (Login_Packet_ServerListReply*)packet_ptr;
        push_u8_little(arena, 0xe);
        // 290:  list:u32 servers
        push_uint32_t_little(arena, packet->servers_count);
        for (uintptr_t servers_iter = 0; servers_iter < packet->servers_count; servers_iter++)
        {
            // 292:  u32 id
            push_uint32_t_little(arena, packet->servers[servers_iter].id);
            // 293:  u32 state
            push_uint32_t_little(arena, packet->servers[servers_iter].state);
            // 294:  b8 is_locked
            push_b8_little(arena, packet->servers[servers_iter].is_locked);
            // 295:  string:u32 name
            push_uint32_t_little(arena, (uint32_t)packet->servers[servers_iter].name.size);
            for (uintptr_t name_iter = 0; name_iter < (uintptr_t)packet->servers[servers_iter].name.size; name_iter++)
            {
                push_u8_little(arena, packet->servers[servers_iter].name.data[name_iter]);
            }
            // 296:  u32 name_id
            push_uint32_t_little(arena, packet->servers[servers_iter].name_id);
            // 297:  string:u32 description
            push_uint32_t_little(arena, (uint32_t)packet->servers[servers_iter].description.size);
            for (uintptr_t description_iter = 0; description_iter < (uintptr_t)packet->servers[servers_iter].description.size; description_iter++)
            {
                push_u8_little(arena, packet->servers[servers_iter].description.data[description_iter]);
            }
            // 298:  u32 description_id
            push_uint32_t_little(arena, packet->servers[servers_iter].description_id);
            // 299:  u32 req_feature_id
            push_uint32_t_little(arena, packet->servers[servers_iter].req_feature_id);
            // 300:  string:u32 server_info
            push_uint32_t_little(arena, (uint32_t)packet->servers[servers_iter].server_info.size);
            for (uintptr_t server_info_iter = 0; server_info_iter < (uintptr_t)packet->servers[servers_iter].server_info.size; server_info_iter++)
            {
                push_u8_little(arena, packet->servers[servers_iter].server_info.data[server_info_iter]);
            }
            // 301:  u32 population_level
            push_uint32_t_little(arena, packet->servers[servers_iter].population_level);
            // 302:  string:u32 population_data
            push_uint32_t_little(arena, (uint32_t)packet->servers[servers_iter].population_data.size);
            for (uintptr_t population_data_iter = 0; population_data_iter < (uintptr_t)packet->servers[servers_iter].population_data.size; population_data_iter++)
            {
                push_u8_little(arena, packet->servers[servers_iter].population_data.data[population_data_iter]);
            }
            // 303:  string:u32 access_expression
            push_uint32_t_little(arena, (uint32_t)packet->servers[servers_iter].access_expression.size);
            for (uintptr_t access_expression_iter = 0; access_expression_iter < (uintptr_t)packet->servers[servers_iter].access_expression.size; access_expression_iter++)
            {
                push_u8_little(arena, packet->servers[servers_iter].access_expression.data[access_expression_iter]);
            }
            // 304:  b8 is_access_allowed
            push_b8_little(arena, packet->servers[servers_iter].is_access_allowed);
        } // servers
    } break;
    case Login_Packet_Kind_ServerUpdate:
    {
        push_u8_little(arena, 0xf);
    } break;
	default:
		printf("Packing for %s not handled\n", *login_packet_names);
	}

    arena->alignment = prev_align;
	Buffer result{
		.size = ((size_t)arena->memory + arena->cursor) - (size_t)data_begin,
		.data = data_begin,
	};
	return result;
}

uintptr_t unpack_login_packet(Arena* arena, Stream* stream, void* result_ptr, Login_Packet_Kind packet_kind)
{
    uintptr_t cursor_begin = stream->cursor;
    switch (packet_kind)
    {
    case Login_Packet_Kind_LoginRequest:
    {
        Login_Packet_LoginRequest* packet = (Login_Packet_LoginRequest*)result_ptr;
        // 5:  string:u32 session_id
        packet->session_id.size = read_uint32_t_little(stream);
        packet->session_id.data = (uint8_t*)arena_push_size(arena, packet->session_id.size);
        for (uintptr_t session_id_iter = 0; session_id_iter < (uintptr_t)packet->session_id.size; session_id_iter++)
        {
            packet->session_id.data[session_id_iter] = read_u8_little(stream);
        }
        // 6:  string:u32 system_fingerprint
        packet->system_fingerprint.size = read_uint32_t_little(stream);
        packet->system_fingerprint.data = (uint8_t*)arena_push_size(arena, (uint32_t)packet->system_fingerprint.size);
        for (uintptr_t system_fingerprint_iter = 0; system_fingerprint_iter < (uintptr_t)packet->system_fingerprint.size; system_fingerprint_iter++)
        {
            packet->system_fingerprint.data[system_fingerprint_iter] = read_u8_little(stream);
        }
        // 7:  u32 locale
        packet->locale = read_uint32_t_little(stream);
        // 8:  u32 third_party_auth_ticket
        packet->third_party_auth_ticket = read_uint32_t_little(stream);
        // 9:  u32 third_party_user_id
        packet->third_party_user_id = read_uint32_t_little(stream);
        // 10:  u32 third_party_id
        packet->third_party_id = read_uint32_t_little(stream);
    } break;
    default:
    {
        printf("unpacking not handled");
    }
    }

    return stream->cursor - cursor_begin;
}