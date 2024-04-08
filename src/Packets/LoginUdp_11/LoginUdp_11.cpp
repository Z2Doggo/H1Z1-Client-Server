#include "../../binary.hpp"

typedef f32 f16;
typedef u32 uint2b;

struct Read_Uint2b_Result
{
  uptr size;
  u32 value;
};

Read_Uint2b_Result read_uint2b_little_at(void* memory)
{
  u32 raw = read_u8_at(memory);
  uptr size = raw & 3;
  for (uptr i = 0; i < size; ++i)
  {
    raw |= read_u8_at((void*)((uptr)memory + i + 1)) << ((i + 1) * 8);
  }
  raw >>= 2;

  Read_Uint2b_Result result{};
  result.size = size + 1;
  result.value = raw;

  return result;
}

uptr write_uint2b_little_at(void* memory, u32 value)
{
  value <<= 2;
  uptr size = 0;
  if (value > 0xffffff)
  {
    size = 3;
  }
  else if (value > 0xffff)
  {
    size = 2;
  }
  else if (value > 0xff)
  {
    size = 1;
  }
  value |= size;

  write_u32_little_at(memory, value);
  return size + 1;
}

u32 read_uint2b_little(Stream* stream)
{
  Read_Uint2b_Result result = read_uint2b_little_at(stream->buffer.data + stream->cursor);
  stream->cursor += result.size;
  return result.value;
}

void push_uint2b_little(Arena* arena, u32 value)
{
  void* at = arena_push_size(arena, 4);
  uptr bytes_written = write_uint2b_little_at(at, value);
  arena->cursor -= 4 - bytes_written;
}

#define LOGIN_PACKET_TABLE(LOGIN_X) \
	LOGIN_X(Unhandled), \
  LOGIN_X(LoginRequest), \
  LOGIN_X(LoginReply), \
  LOGIN_X(Logout), \
  LOGIN_X(ForceDisconnect), \
  LOGIN_X(CharacterCreateRequest), \
  LOGIN_X(CharacterCreateReply), \
  LOGIN_X(CharacterLoginRequest), \
  LOGIN_X(CharacterLoginReply), \
  LOGIN_X(CharacterDeleteRequest), \
  LOGIN_X(TunnelAppPacketClientToServer), \
  LOGIN_X(TunnelAppPacketServerToClient), \
  LOGIN_X(CharacterTransferRequest), \
  LOGIN_X(CharacterTransferReply), \
  LOGIN_X(CharacterDeleteReply), \
  LOGIN_X(CharacterSelectInfoRequest), \
  LOGIN_X(CharacterSelectInfoReply), \
  LOGIN_X(ServerListRequest), \
  LOGIN_X(ServerListReply), \
  LOGIN_X(ServerUpdate), \
  LOGIN_X(_End)

#define LOGIN_PACKET_KIND(kind) Login_Packet_Kind_##kind
enum Login_Packet_Kind
{
  LOGIN_PACKET_TABLE(LOGIN_PACKET_KIND)
};

#define LOGIN_PACKET_NAME(str) #str
static std::array<std::string, static_cast<u32>(Login_Packet_Kind__End) + 1>global_login_packet_names
{
  LOGIN_PACKET_TABLE(LOGIN_PACKET_NAME)
};

static std::array<u8, static_cast<u8>(Login_Packet_Kind__End) + 1>global_login_packet_ids
{
  0x1,
  0x2,
  0x3,
  0x4,
  0x5,
  0x6,
  0x7,
  0x8,
  0x9,
  0x10,
  0x11,
  0x12,
  0x13,
  0xa,
  0xb,
  0xc,
  0xd,
  0xe,
  0xf,
};

struct Login_Packet_LoginRequest
{
  Buffer session_id;
  Buffer system_fingerprint;
  u32 locale;
  u32 third_party_auth_ticket;
  u32 third_party_user_id;
  u32 third_party_id;
};


struct Login_Packet_LoginReply
{
  b8 is_logged_in;
  u32 status;
  u32 result_code;
  b8 is_member;
  b8 is_internal;
  Buffer namespace_name;
  u32 account_features_count;
  struct account_features_s
  {
  u32 key;
  u32 id;
  b8 active;
  u32 remaining_count;
  Buffer raw_data;
  }* account_features;
  Buffer application_payload;
  u32 error_details_count;
  struct error_details_s
  {
  u32 unknown_dword1;
  Buffer name;
  Buffer value;
  }* error_details;
  Buffer ip_country_code;
};


struct Login_Packet_CharacterCreateRequest
{
  u32 server_id;
  u32 unk_u32;
  u32 char_payload_size;
  struct char_payload_s
  {
  u8 empire_id;
  u32 head_type;
  u32 profile_type;
  u32 gender;
  Buffer character_name;
  } char_payload;
};


struct Login_Packet_CharacterCreateReply
{
  u32 status;
  u64 character_id;
};


struct Login_Packet_CharacterLoginRequest
{
  u64 character_id;
  u32 server_id;
  u32 status;
  u32 payload2_size;
  struct payload2_s
  {
  Buffer locale;
  u32 locale_id;
  u32 preferred_gateway_id;
  } payload2;
};


struct Login_Packet_CharacterLoginReply
{
  u64 character_id;
  u32 server_id;
  u32 last_login;
  u32 status;
  u32 login_payload_size;
  struct login_payload_s
  {
  u8 unk_byte_1;
  u8 unk_byte_2;
  Buffer server_address;
  Buffer server_ticket;
  Buffer encryption_key;
  u32 soe_protocol_version;
  u64 character_id;
  u64 unk_u64;
  Buffer station_name;
  Buffer character_name;
  Buffer unk_str;
  u64 server_feature_bit;
  } login_payload;
};


struct Login_Packet_CharacterDeleteRequest
{
  u64 character_id;
};


struct Login_Packet_TunnelAppPacketClientToServer
{
  u32 server_id;
  u32 unk1;
  u32 data_client_size;
  struct data_client_s
  {
  b8 tunnel_op_code;
  b8 sub_op_code;
  Buffer character_name;
  } data_client;
};


struct Login_Packet_TunnelAppPacketServerToClient
{
  u32 server_id;
  u32 unk1;
  u32 data_server_size;
  struct data_server_s
  {
  u8 tunnel_op_code;
  u8 sub_op_code;
  Buffer character_name;
  Buffer character_name2;
  u32 status;
  } data_server;
};


struct Login_Packet_CharacterDeleteReply
{
  u64 character_id;
  u32 status;
  Buffer payload3;
};


struct Login_Packet_CharacterSelectInfoReply
{
  u32 character_status;
  b8 can_bypass_server_lock;
  u32 characters_count;
  struct characters_s
  {
  u64 charId;
  u32 serverId;
  u64 lastLoginDate;
  u32 nullField;
  u32 status;
  u32 payload_size;
  struct payload_s
  {
  Buffer name;
  u8 empireId;
  u32 battleRank;
  u32 nextBattleRankPercent;
  u32 headId;
  u32 actorModelId;
  u32 gender;
  u32 profileId;
  u32 unkDword1;
  u32 unkDword2;
  u32 loadoutSlots_count;
  struct loadoutSlots_s
  {
  u32 hotbarSlotId;
  u32 loadoutId;
  u32 slotId;
  u32 itemDefId;
  u64 loadoutItemGuid;
  u8 unkByte1;
  u32 unkDword1;
  }* loadoutSlots;
  u32 itemDefinitions_count;
  struct itemDefinitions_s
  {
  u32 ID;
  u32 item_defs_count;
  struct item_defs_s
  {
  u32 defs_id;
  u8 bitflags1;
  u8 bitflags2;
  u32 name_id;
  u32 description_id;
  u32 content_id;
  u32 image_set_id;
  u32 tint_id;
  u32 hud_image_set_id;
  u32 unk_dword_1;
  u32 unk_dword_2;
  u32 cost;
  u32 item_class;
  u32 profile_override;
  Buffer model_name;
  Buffer texture_alias;
  u32 gender_usage;
  u32 item_type;
  u32 category_id;
  u32 weapon_trail_effect_id;
  u32 composite_effect_id;
  u32 power_rating;
  u32 min_profile_rank;
  u32 rarity;
  u32 activatable_ability_id;
  u32 activatable_ability_set_id;
  u32 passive_ability_id;
  u32 passive_ability_set_id;
  u32 max_stack_size;
  u32 min_stack_size;
  Buffer tint_alias;
  u32 tint_group_id;
  u32 member_discount;
  u32 vip_rank_required;
  u32 race_set_id;
  u32 ui_model_camera_id_1;
  u32 equip_count_max;
  u32 curreny_type;
  u32 datasheet_id;
  u32 item_type_1;
  u32 skill_set_id;
  Buffer overlay_texture;
  Buffer decal_slot;
  u32 overlay_adjustment;
  u32 trial_duration_sec;
  u32 next_trial_delay_sec;
  u32 client_use_requirement;
  Buffer override_appearance;
  u32 override_camera_id;
  u32 unk_dword_3;
  u32 unk_dword_4;
  u32 unk_dword_5;
  u32 bulk;
  u32 active_equip_slot_id;
  u32 passive_equip_slot_id;
  u32 passive_equip_slot_group_id;
  u32 unk_dword_6;
  u32 grinder_reward_set_id;
  u32 build_bar_group_id;
  Buffer unk_string_1;
  b8 unk_bool_1;
  b8 is_armor;
  u32 unk_dword_7;
  u32 param1;
  u32 param2;
  u32 param3;
  Buffer string_param1;
  u32 ui_model_camera_id_2;
  u32 unk_dword_8;
  u32 scrap_value_override;
  u32 stats_item_def_2_count;
  struct stats_item_def_2_s
  {
  u32 unk_dword_9;
  u32 stat_id;
  u8 variabletype8_case;
  union
  {
  struct statValue0_s
  {
  u32 base;
  u32 modifier;
  } statValue0;
  struct statValue1_s
  {
  f32 base;
  f32 modifier;
  } statValue1;
  } variabletype8;
  u32 unk_dword_10;
  }* stats_item_def_2;
  }* item_defs;
  }* itemDefinitions;
  u64 lastUseDate;
  } payload;
  }* characters;
};


struct Login_Packet_ServerListReply
{
  u32 servers_count;
  struct servers_s
  {
  u32 id;
  u32 state;
  b8 is_locked;
  Buffer name;
  u32 name_id;
  Buffer description;
  u32 description_id;
  u32 req_feature_id;
  Buffer server_info;
  u32 population_level;
  Buffer population_data;
  Buffer access_expression;
  b8 is_access_allowed;
  }* servers;
};


Buffer pack_login_packet(Arena* arena, void* packet_ptr, Login_Packet_Kind packet_kind)
{
  uptr prev_align = arena->alignment;
  arena->alignment = 1;
  void* memory_begin = (static_cast<u8*>(arena->memory + arena->cursor));
  switch (packet_kind)
  {
    case Login_Packet_Kind_LoginRequest:
    {
      Login_Packet_LoginRequest* packet = static_cast<Login_Packet_LoginRequest*>(packet_ptr);
      push_u8_little(arena, 0x1);
      // 5:  string:u32 session_id
      push_u32_little(arena, (u32)packet->session_id.size);
      for (uptr session_id_iter = 0; session_id_iter < (uptr)packet->session_id.size; session_id_iter++)
      {
        push_u8_little(arena, packet->session_id.data[session_id_iter]);
      }
      // 6:  string:u32 system_fingerprint
      push_u32_little(arena, (u32)packet->system_fingerprint.size);
      for (uptr system_fingerprint_iter = 0; system_fingerprint_iter < (uptr)packet->system_fingerprint.size; system_fingerprint_iter++)
      {
        push_u8_little(arena, packet->system_fingerprint.data[system_fingerprint_iter]);
      }
      // 7:  u32 locale
      push_u32_little(arena, packet->locale);
      // 8:  u32 third_party_auth_ticket
      push_u32_little(arena, packet->third_party_auth_ticket);
      // 9:  u32 third_party_user_id
      push_u32_little(arena, packet->third_party_user_id);
      // 10:  u32 third_party_id
      push_u32_little(arena, packet->third_party_id);
    } break;

    case Login_Packet_Kind_LoginReply:
    {
      Login_Packet_LoginReply* packet = static_cast<Login_Packet_LoginReply*>(packet_ptr);
      push_u8_little(arena, 0x2);
      // 13:  b8 is_logged_in
      push_b8_little(arena, packet->is_logged_in);
      // 14:  u32 status
      push_u32_little(arena, packet->status);
      // 15:  u32 result_code
      push_u32_little(arena, packet->result_code);
      // 16:  b8 is_member
      push_b8_little(arena, packet->is_member);
      // 17:  b8 is_internal
      push_b8_little(arena, packet->is_internal);
      // 18:  string:u32 namespace_name
      push_u32_little(arena, (u32)packet->namespace_name.size);
      for (uptr namespace_name_iter = 0; namespace_name_iter < (uptr)packet->namespace_name.size; namespace_name_iter++)
      {
        push_u8_little(arena, packet->namespace_name.data[namespace_name_iter]);
      }
      // 19:  list:u32 account_features
      push_u32_little(arena, packet->account_features_count);
      for (uptr account_features_iter = 0; account_features_iter < packet->account_features_count; account_features_iter++)
      {
      // 21:  u32 key
      push_u32_little(arena, packet->account_features[account_features_iter].key);
      // 22:  u32 id
      push_u32_little(arena, packet->account_features[account_features_iter].id);
      // 23:  b8 active
      push_b8_little(arena, packet->account_features[account_features_iter].active);
      // 24:  u32 remaining_count
      push_u32_little(arena, packet->account_features[account_features_iter].remaining_count);
      // 25:  string:u32 raw_data
      push_u32_little(arena, (u32)packet->account_features[account_features_iter].raw_data.size);
      for (uptr raw_data_iter = 0; raw_data_iter < (uptr)packet->account_features[account_features_iter].raw_data.size; raw_data_iter++)
      {
        push_u8_little(arena, packet->account_features[account_features_iter].raw_data.data[raw_data_iter]);
      }
    } // account_features
      // 27:  bytes:u32 application_payload
      push_u32_little(arena, (u32)packet->application_payload.size);
      for (uptr application_payload_iter = 0; application_payload_iter < packet->application_payload.size; application_payload_iter++)
      {
        push_u8_little(arena, packet->application_payload.data[application_payload_iter]);
      }
      // 28:  list:u32 error_details
      push_u32_little(arena, packet->error_details_count);
      for (uptr error_details_iter = 0; error_details_iter < packet->error_details_count; error_details_iter++)
      {
      // 30:  u32 unknown_dword1
      push_u32_little(arena, packet->error_details[error_details_iter].unknown_dword1);
      // 31:  string:u32 name
      push_u32_little(arena, (u32)packet->error_details[error_details_iter].name.size);
      for (uptr name_iter = 0; name_iter < (uptr)packet->error_details[error_details_iter].name.size; name_iter++)
      {
        push_u8_little(arena, packet->error_details[error_details_iter].name.data[name_iter]);
      }
      // 32:  string:u32 value
      push_u32_little(arena, (u32)packet->error_details[error_details_iter].value.size);
      for (uptr value_iter = 0; value_iter < (uptr)packet->error_details[error_details_iter].value.size; value_iter++)
      {
        push_u8_little(arena, packet->error_details[error_details_iter].value.data[value_iter]);
      }
    } // error_details
      // 34:  string:u32 ip_country_code
      push_u32_little(arena, (u32)packet->ip_country_code.size);
      for (uptr ip_country_code_iter = 0; ip_country_code_iter < (uptr)packet->ip_country_code.size; ip_country_code_iter++)
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
      Login_Packet_CharacterCreateRequest* packet = static_cast<Login_Packet_CharacterCreateRequest*>(packet_ptr);
      push_u8_little(arena, 0x5);
      // 41:  u32 server_id
      push_u32_little(arena, packet->server_id);
      // 42:  u32 unk_u32
      push_u32_little(arena, packet->unk_u32);
      // 44:  stream: char_payload
      void* char_payload_size_ptr = (void*)((uptr)arena->memory + arena->cursor);
      arena->cursor += sizeof(u32);
      u32 will_pack_char_payload = packet->char_payload_size == ~0 ? FALSE : TRUE;
      if (!will_pack_char_payload)
      {
        write_u32_little_at(char_payload_size_ptr, 0);
      }
      else
      {
      // 47:  u8 empire_id
      push_u8_little(arena, packet->char_payload.empire_id);
      // 48:  u32 head_type
      push_u32_little(arena, packet->char_payload.head_type);
      // 49:  u32 profile_type
      push_u32_little(arena, packet->char_payload.profile_type);
      // 50:  u32 gender
      push_u32_little(arena, packet->char_payload.gender);
      // 51:  string:u32 character_name
      push_u32_little(arena, (u32)packet->char_payload.character_name.size);
      for (uptr character_name_iter = 0; character_name_iter < (uptr)packet->char_payload.character_name.size; character_name_iter++)
      {
        push_u8_little(arena, packet->char_payload.character_name.data[character_name_iter]);
      }
      write_u32_little_at(char_payload_size_ptr, (u32)(((uptr)arena->memory + arena->cursor) - ((uptr)char_payload_size_ptr + sizeof(u32))));
      } // char_payload
    } break;

    case Login_Packet_Kind_CharacterCreateReply:
    {
      Login_Packet_CharacterCreateReply* packet = static_cast<Login_Packet_CharacterCreateReply*>(packet_ptr);
      push_u8_little(arena, 0x6);
      // 55:  u32 status
      push_u32_little(arena, packet->status);
      // 56:  u64 character_id
      push_u64_little(arena, packet->character_id);
    } break;

    case Login_Packet_Kind_CharacterLoginRequest:
    {
      Login_Packet_CharacterLoginRequest* packet = static_cast<Login_Packet_CharacterLoginRequest*>(packet_ptr);
      push_u8_little(arena, 0x7);
      // 59:  u64 character_id
      push_u64_little(arena, packet->character_id);
      // 60:  u32 server_id
      push_u32_little(arena, packet->server_id);
      // 61:  u32 status
      push_u32_little(arena, packet->status);
      // 63:  stream: payload2
      void* payload2_size_ptr = (void*)((uptr)arena->memory + arena->cursor);
      arena->cursor += sizeof(u32);
      u32 will_pack_payload2 = packet->payload2_size == ~0 ? FALSE : TRUE;
      if (!will_pack_payload2)
      {
        write_u32_little_at(payload2_size_ptr, 0);
      }
      else
      {
      // 66:  string:u32 locale
      push_u32_little(arena, (u32)packet->payload2.locale.size);
      for (uptr locale_iter = 0; locale_iter < (uptr)packet->payload2.locale.size; locale_iter++)
      {
        push_u8_little(arena, packet->payload2.locale.data[locale_iter]);
      }
      // 67:  u32 locale_id
      push_u32_little(arena, packet->payload2.locale_id);
      // 68:  u32 preferred_gateway_id
      push_u32_little(arena, packet->payload2.preferred_gateway_id);
      write_u32_little_at(payload2_size_ptr, (u32)(((uptr)arena->memory + arena->cursor) - ((uptr)payload2_size_ptr + sizeof(u32))));
      } // payload2
    } break;

    case Login_Packet_Kind_CharacterLoginReply:
    {
      Login_Packet_CharacterLoginReply* packet = static_cast<Login_Packet_CharacterLoginReply*>(packet_ptr);
      push_u8_little(arena, 0x8);
      // 72:  u64 character_id
      push_u64_little(arena, packet->character_id);
      // 73:  u32 server_id
      push_u32_little(arena, packet->server_id);
      // 74:  u32 last_login
      push_u32_little(arena, packet->last_login);
      // 75:  u32 status
      push_u32_little(arena, packet->status);
      // 77:  stream: login_payload
      void* login_payload_size_ptr = (void*)((uptr)arena->memory + arena->cursor);
      arena->cursor += sizeof(u32);
      u32 will_pack_login_payload = packet->login_payload_size == ~0 ? FALSE : TRUE;
      if (!will_pack_login_payload)
      {
        write_u32_little_at(login_payload_size_ptr, 0);
      }
      else
      {
      // 79:  u8 unk_byte_1
      push_u8_little(arena, packet->login_payload.unk_byte_1);
      // 80:  u8 unk_byte_2
      push_u8_little(arena, packet->login_payload.unk_byte_2);
      // 81:  string:u32 server_address
      push_u32_little(arena, (u32)packet->login_payload.server_address.size);
      for (uptr server_address_iter = 0; server_address_iter < (uptr)packet->login_payload.server_address.size; server_address_iter++)
      {
        push_u8_little(arena, packet->login_payload.server_address.data[server_address_iter]);
      }
      // 82:  string:u32 server_ticket
      push_u32_little(arena, (u32)packet->login_payload.server_ticket.size);
      for (uptr server_ticket_iter = 0; server_ticket_iter < (uptr)packet->login_payload.server_ticket.size; server_ticket_iter++)
      {
        push_u8_little(arena, packet->login_payload.server_ticket.data[server_ticket_iter]);
      }
      // 83:  bytes:u32 encryption_key
      push_u32_little(arena, (u32)packet->login_payload.encryption_key.size);
      for (uptr encryption_key_iter = 0; encryption_key_iter < packet->login_payload.encryption_key.size; encryption_key_iter++)
      {
        push_u8_little(arena, packet->login_payload.encryption_key.data[encryption_key_iter]);
      }
      // 84:  u32 soe_protocol_version
      push_u32_little(arena, packet->login_payload.soe_protocol_version);
      // 85:  u64 character_id
      push_u64_little(arena, packet->login_payload.character_id);
      // 86:  u64 unk_u64
      push_u64_little(arena, packet->login_payload.unk_u64);
      // 87:  string:u32 station_name
      push_u32_little(arena, (u32)packet->login_payload.station_name.size);
      for (uptr station_name_iter = 0; station_name_iter < (uptr)packet->login_payload.station_name.size; station_name_iter++)
      {
        push_u8_little(arena, packet->login_payload.station_name.data[station_name_iter]);
      }
      // 88:  string:u32 character_name
      push_u32_little(arena, (u32)packet->login_payload.character_name.size);
      for (uptr character_name_iter = 0; character_name_iter < (uptr)packet->login_payload.character_name.size; character_name_iter++)
      {
        push_u8_little(arena, packet->login_payload.character_name.data[character_name_iter]);
      }
      // 89:  string:u32 unk_str
      push_u32_little(arena, (u32)packet->login_payload.unk_str.size);
      for (uptr unk_str_iter = 0; unk_str_iter < (uptr)packet->login_payload.unk_str.size; unk_str_iter++)
      {
        push_u8_little(arena, packet->login_payload.unk_str.data[unk_str_iter]);
      }
      // 90:  u64 server_feature_bit
      push_u64_little(arena, packet->login_payload.server_feature_bit);
      write_u32_little_at(login_payload_size_ptr, (u32)(((uptr)arena->memory + arena->cursor) - ((uptr)login_payload_size_ptr + sizeof(u32))));
      } // login_payload
    } break;

    case Login_Packet_Kind_CharacterDeleteRequest:
    {
      Login_Packet_CharacterDeleteRequest* packet = static_cast<Login_Packet_CharacterDeleteRequest*>(packet_ptr);
      push_u8_little(arena, 0x9);
      // 94:  u64 character_id
      push_u64_little(arena, packet->character_id);
    } break;

    case Login_Packet_Kind_TunnelAppPacketClientToServer:
    {
      Login_Packet_TunnelAppPacketClientToServer* packet = static_cast<Login_Packet_TunnelAppPacketClientToServer*>(packet_ptr);
      push_u8_little(arena, 0x10);
      // 97:  u32 server_id
      push_u32_little(arena, packet->server_id);
      // 98:  u32 unk1
      push_u32_little(arena, packet->unk1);
      // 100:  stream: data_client
      void* data_client_size_ptr = (void*)((uptr)arena->memory + arena->cursor);
      arena->cursor += sizeof(u32);
      u32 will_pack_data_client = packet->data_client_size == ~0 ? FALSE : TRUE;
      if (!will_pack_data_client)
      {
        write_u32_little_at(data_client_size_ptr, 0);
      }
      else
      {
      // 102:  b8 tunnel_op_code
      push_b8_little(arena, packet->data_client.tunnel_op_code);
      // 103:  b8 sub_op_code
      push_b8_little(arena, packet->data_client.sub_op_code);
      // 104:  string:u32 character_name
      push_u32_little(arena, (u32)packet->data_client.character_name.size);
      for (uptr character_name_iter = 0; character_name_iter < (uptr)packet->data_client.character_name.size; character_name_iter++)
      {
        push_u8_little(arena, packet->data_client.character_name.data[character_name_iter]);
      }
      write_u32_little_at(data_client_size_ptr, (u32)(((uptr)arena->memory + arena->cursor) - ((uptr)data_client_size_ptr + sizeof(u32))));
      } // data_client
    } break;

    case Login_Packet_Kind_TunnelAppPacketServerToClient:
    {
      Login_Packet_TunnelAppPacketServerToClient* packet = static_cast<Login_Packet_TunnelAppPacketServerToClient*>(packet_ptr);
      push_u8_little(arena, 0x11);
      // 108:  u32 server_id
      push_u32_little(arena, packet->server_id);
      // 109:  u32 unk1
      push_u32_little(arena, packet->unk1);
      // 111:  stream: data_server
      void* data_server_size_ptr = (void*)((uptr)arena->memory + arena->cursor);
      arena->cursor += sizeof(u32);
      u32 will_pack_data_server = packet->data_server_size == ~0 ? FALSE : TRUE;
      if (!will_pack_data_server)
      {
        write_u32_little_at(data_server_size_ptr, 0);
      }
      else
      {
      // 113:  u8 tunnel_op_code
      push_u8_little(arena, packet->data_server.tunnel_op_code);
      // 114:  u8 sub_op_code
      push_u8_little(arena, packet->data_server.sub_op_code);
      // 115:  string:u32 character_name
      push_u32_little(arena, (u32)packet->data_server.character_name.size);
      for (uptr character_name_iter = 0; character_name_iter < (uptr)packet->data_server.character_name.size; character_name_iter++)
      {
        push_u8_little(arena, packet->data_server.character_name.data[character_name_iter]);
      }
      // 116:  string:u32 character_name2
      push_u32_little(arena, (u32)packet->data_server.character_name2.size);
      for (uptr character_name2_iter = 0; character_name2_iter < (uptr)packet->data_server.character_name2.size; character_name2_iter++)
      {
        push_u8_little(arena, packet->data_server.character_name2.data[character_name2_iter]);
      }
      // 117:  u32 status
      push_u32_little(arena, packet->data_server.status);
      write_u32_little_at(data_server_size_ptr, (u32)(((uptr)arena->memory + arena->cursor) - ((uptr)data_server_size_ptr + sizeof(u32))));
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
      Login_Packet_CharacterDeleteReply* packet = static_cast<Login_Packet_CharacterDeleteReply*>(packet_ptr);
      push_u8_little(arena, 0xa);
      // 125:  u64 character_id
      push_u64_little(arena, packet->character_id);
      // 126:  u32 status
      push_u32_little(arena, packet->status);
      // 127:  bytes:u32 payload3
      push_u32_little(arena, (u32)packet->payload3.size);
      for (uptr payload3_iter = 0; payload3_iter < packet->payload3.size; payload3_iter++)
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
      Login_Packet_CharacterSelectInfoReply* packet = static_cast<Login_Packet_CharacterSelectInfoReply*>(packet_ptr);
      push_u8_little(arena, 0xc);
      // 132:  u32 character_status
      push_u32_little(arena, packet->character_status);
      // 133:  b8 can_bypass_server_lock
      push_b8_little(arena, packet->can_bypass_server_lock);
      // 135:  list:u32 characters
      push_u32_little(arena, packet->characters_count);
      for (uptr characters_iter = 0; characters_iter < packet->characters_count; characters_iter++)
      {
      // 137:  u64 charId
      push_u64_little(arena, packet->characters[characters_iter].charId);
      // 138:  u32 serverId
      push_u32_little(arena, packet->characters[characters_iter].serverId);
      // 139:  u64 lastLoginDate
      push_u64_little(arena, packet->characters[characters_iter].lastLoginDate);
      // 140:  u32 nullField
      push_u32_little(arena, packet->characters[characters_iter].nullField);
      // 141:  u32 status
      push_u32_little(arena, packet->characters[characters_iter].status);
      // 143:  stream: payload
      void* payload_size_ptr = (void*)((uptr)arena->memory + arena->cursor);
      arena->cursor += sizeof(u32);
      u32 will_pack_payload = packet->characters[characters_iter].payload_size == ~0 ? FALSE : TRUE;
      if (!will_pack_payload)
      {
        write_u32_little_at(payload_size_ptr, 0);
      }
      else
      {
      // 145:  string:u32 name
      push_u32_little(arena, (u32)packet->characters[characters_iter].payload.name.size);
      for (uptr name_iter = 0; name_iter < (uptr)packet->characters[characters_iter].payload.name.size; name_iter++)
      {
        push_u8_little(arena, packet->characters[characters_iter].payload.name.data[name_iter]);
      }
      // 146:  u8 empireId
      push_u8_little(arena, packet->characters[characters_iter].payload.empireId);
      // 147:  u32 battleRank
      push_u32_little(arena, packet->characters[characters_iter].payload.battleRank);
      // 148:  u32 nextBattleRankPercent
      push_u32_little(arena, packet->characters[characters_iter].payload.nextBattleRankPercent);
      // 149:  u32 headId
      push_u32_little(arena, packet->characters[characters_iter].payload.headId);
      // 150:  u32 actorModelId
      push_u32_little(arena, packet->characters[characters_iter].payload.actorModelId);
      // 151:  u32 gender
      push_u32_little(arena, packet->characters[characters_iter].payload.gender);
      // 152:  u32 profileId
      push_u32_little(arena, packet->characters[characters_iter].payload.profileId);
      // 153:  u32 unkDword1
      push_u32_little(arena, packet->characters[characters_iter].payload.unkDword1);
      // 154:  u32 unkDword2
      push_u32_little(arena, packet->characters[characters_iter].payload.unkDword2);
      // 156:  list:u32 loadoutSlots
      push_u32_little(arena, packet->characters[characters_iter].payload.loadoutSlots_count);
      for (uptr loadoutSlots_iter = 0; loadoutSlots_iter < packet->characters[characters_iter].payload.loadoutSlots_count; loadoutSlots_iter++)
      {
      // 158:  u32 hotbarSlotId
      push_u32_little(arena, packet->characters[characters_iter].payload.loadoutSlots[loadoutSlots_iter].hotbarSlotId);
      // 159:  u32 loadoutId
      push_u32_little(arena, packet->characters[characters_iter].payload.loadoutSlots[loadoutSlots_iter].loadoutId);
      // 160:  u32 slotId
      push_u32_little(arena, packet->characters[characters_iter].payload.loadoutSlots[loadoutSlots_iter].slotId);
      // 161:  u32 itemDefId
      push_u32_little(arena, packet->characters[characters_iter].payload.loadoutSlots[loadoutSlots_iter].itemDefId);
      // 162:  u64 loadoutItemGuid
      push_u64_little(arena, packet->characters[characters_iter].payload.loadoutSlots[loadoutSlots_iter].loadoutItemGuid);
      // 163:  u8 unkByte1
      push_u8_little(arena, packet->characters[characters_iter].payload.loadoutSlots[loadoutSlots_iter].unkByte1);
      // 164:  u32 unkDword1
      push_u32_little(arena, packet->characters[characters_iter].payload.loadoutSlots[loadoutSlots_iter].unkDword1);
    } // loadoutSlots
      // 167:  list:u32 itemDefinitions
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions_count);
      for (uptr itemDefinitions_iter = 0; itemDefinitions_iter < packet->characters[characters_iter].payload.itemDefinitions_count; itemDefinitions_iter++)
      {
      // 169:  u32 ID
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].ID);
      // 171:  list:u32 item_defs
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs_count);
      for (uptr item_defs_iter = 0; item_defs_iter < packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs_count; item_defs_iter++)
      {
      // 173:  u32 defs_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].defs_id);
      // 175:  u8 bitflags1
      push_u8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].bitflags1);
      // 185:  u8 bitflags2
      push_u8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].bitflags2);
      // 195:  u32 name_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].name_id);
      // 196:  u32 description_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].description_id);
      // 197:  u32 content_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].content_id);
      // 198:  u32 image_set_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].image_set_id);
      // 199:  u32 tint_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].tint_id);
      // 200:  u32 hud_image_set_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].hud_image_set_id);
      // 201:  u32 unk_dword_1
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_1);
      // 202:  u32 unk_dword_2
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_2);
      // 203:  u32 cost
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].cost);
      // 204:  u32 item_class
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].item_class);
      // 205:  u32 profile_override
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].profile_override);
      // 206:  string:u32 model_name
      push_u32_little(arena, (u32)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].model_name.size);
      for (uptr model_name_iter = 0; model_name_iter < (uptr)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].model_name.size; model_name_iter++)
      {
        push_u8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].model_name.data[model_name_iter]);
      }
      // 207:  string:u32 texture_alias
      push_u32_little(arena, (u32)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].texture_alias.size);
      for (uptr texture_alias_iter = 0; texture_alias_iter < (uptr)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].texture_alias.size; texture_alias_iter++)
      {
        push_u8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].texture_alias.data[texture_alias_iter]);
      }
      // 208:  u32 gender_usage
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].gender_usage);
      // 209:  u32 item_type
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].item_type);
      // 210:  u32 category_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].category_id);
      // 211:  u32 weapon_trail_effect_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].weapon_trail_effect_id);
      // 212:  u32 composite_effect_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].composite_effect_id);
      // 213:  u32 power_rating
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].power_rating);
      // 214:  u32 min_profile_rank
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].min_profile_rank);
      // 215:  u32 rarity
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].rarity);
      // 216:  u32 activatable_ability_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].activatable_ability_id);
      // 217:  u32 activatable_ability_set_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].activatable_ability_set_id);
      // 218:  u32 passive_ability_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].passive_ability_id);
      // 219:  u32 passive_ability_set_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].passive_ability_set_id);
      // 220:  u32 max_stack_size
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].max_stack_size);
      // 221:  u32 min_stack_size
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].min_stack_size);
      // 222:  string:u32 tint_alias
      push_u32_little(arena, (u32)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].tint_alias.size);
      for (uptr tint_alias_iter = 0; tint_alias_iter < (uptr)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].tint_alias.size; tint_alias_iter++)
      {
        push_u8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].tint_alias.data[tint_alias_iter]);
      }
      // 223:  u32 tint_group_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].tint_group_id);
      // 224:  u32 member_discount
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].member_discount);
      // 225:  u32 vip_rank_required
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].vip_rank_required);
      // 226:  u32 race_set_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].race_set_id);
      // 227:  u32 ui_model_camera_id_1
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].ui_model_camera_id_1);
      // 228:  u32 equip_count_max
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].equip_count_max);
      // 229:  u32 curreny_type
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].curreny_type);
      // 230:  u32 datasheet_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].datasheet_id);
      // 231:  u32 item_type_1
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].item_type_1);
      // 232:  u32 skill_set_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].skill_set_id);
      // 233:  string:u32 overlay_texture
      push_u32_little(arena, (u32)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].overlay_texture.size);
      for (uptr overlay_texture_iter = 0; overlay_texture_iter < (uptr)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].overlay_texture.size; overlay_texture_iter++)
      {
        push_u8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].overlay_texture.data[overlay_texture_iter]);
      }
      // 234:  string:u32 decal_slot
      push_u32_little(arena, (u32)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].decal_slot.size);
      for (uptr decal_slot_iter = 0; decal_slot_iter < (uptr)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].decal_slot.size; decal_slot_iter++)
      {
        push_u8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].decal_slot.data[decal_slot_iter]);
      }
      // 235:  u32 overlay_adjustment
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].overlay_adjustment);
      // 236:  u32 trial_duration_sec
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].trial_duration_sec);
      // 237:  u32 next_trial_delay_sec
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].next_trial_delay_sec);
      // 238:  u32 client_use_requirement
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].client_use_requirement);
      // 239:  string:u32 override_appearance
      push_u32_little(arena, (u32)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].override_appearance.size);
      for (uptr override_appearance_iter = 0; override_appearance_iter < (uptr)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].override_appearance.size; override_appearance_iter++)
      {
        push_u8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].override_appearance.data[override_appearance_iter]);
      }
      // 240:  u32 override_camera_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].override_camera_id);
      // 241:  u32 unk_dword_3
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_3);
      // 242:  u32 unk_dword_4
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_4);
      // 243:  u32 unk_dword_5
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_5);
      // 244:  u32 bulk
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].bulk);
      // 245:  u32 active_equip_slot_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].active_equip_slot_id);
      // 246:  u32 passive_equip_slot_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].passive_equip_slot_id);
      // 247:  u32 passive_equip_slot_group_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].passive_equip_slot_group_id);
      // 248:  u32 unk_dword_6
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_6);
      // 249:  u32 grinder_reward_set_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].grinder_reward_set_id);
      // 250:  u32 build_bar_group_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].build_bar_group_id);
      // 251:  string:u32 unk_string_1
      push_u32_little(arena, (u32)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_string_1.size);
      for (uptr unk_string_1_iter = 0; unk_string_1_iter < (uptr)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_string_1.size; unk_string_1_iter++)
      {
        push_u8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_string_1.data[unk_string_1_iter]);
      }
      // 252:  b8 unk_bool_1
      push_b8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_bool_1);
      // 253:  b8 is_armor
      push_b8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].is_armor);
      // 254:  u32 unk_dword_7
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_7);
      // 255:  u32 param1
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].param1);
      // 256:  u32 param2
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].param2);
      // 257:  u32 param3
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].param3);
      // 258:  string:u32 string_param1
      push_u32_little(arena, (u32)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].string_param1.size);
      for (uptr string_param1_iter = 0; string_param1_iter < (uptr)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].string_param1.size; string_param1_iter++)
      {
        push_u8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].string_param1.data[string_param1_iter]);
      }
      // 259:  u32 ui_model_camera_id_2
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].ui_model_camera_id_2);
      // 260:  u32 unk_dword_8
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_8);
      // 261:  u32 scrap_value_override
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].scrap_value_override);
      // 263:  list:u32 stats_item_def_2
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2_count);
      for (uptr stats_item_def_2_iter = 0; stats_item_def_2_iter < packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2_count; stats_item_def_2_iter++)
      {
      // 265:  u32 unk_dword_9
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].unk_dword_9);
      // 266:  u32 stat_id
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].stat_id);
      // 268:  switch: variabletype8
      push_u8_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].variabletype8_case);
      switch(packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].variabletype8_case)
      {
      case 0:
      {
      // 271:  u32 base
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue0.base);
      // 272:  u32 modifier
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue0.modifier);
      } break; // statValue0
      case 1:
      {
      // 275:  f32 base
      push_f32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue1.base);
      // 276:  f32 modifier
      push_f32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue1.modifier);
      } break; // statValue1
      } // variabletype8
      // 279:  u32 unk_dword_10
      push_u32_little(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].unk_dword_10);
    } // stats_item_def_2
    } // item_defs
    } // itemDefinitions
      // 283:  u64 lastUseDate
      push_u64_little(arena, packet->characters[characters_iter].payload.lastUseDate);
      write_u32_little_at(payload_size_ptr, (u32)(((uptr)arena->memory + arena->cursor) - ((uptr)payload_size_ptr + sizeof(u32))));
      } // payload
    } // characters
    } break;

    case Login_Packet_Kind_ServerListRequest:
    {
      push_u8_little(arena, 0xd);
    } break;

    case Login_Packet_Kind_ServerListReply:
    {
      Login_Packet_ServerListReply* packet = static_cast<Login_Packet_ServerListReply*>(packet_ptr);
      push_u8_little(arena, 0xe);
      // 290:  list:u32 servers
      push_u32_little(arena, packet->servers_count);
      for (uptr servers_iter = 0; servers_iter < packet->servers_count; servers_iter++)
      {
      // 292:  u32 id
      push_u32_little(arena, packet->servers[servers_iter].id);
      // 293:  u32 state
      push_u32_little(arena, packet->servers[servers_iter].state);
      // 294:  b8 is_locked
      push_b8_little(arena, packet->servers[servers_iter].is_locked);
      // 295:  string:u32 name
      push_u32_little(arena, (u32)packet->servers[servers_iter].name.size);
      for (uptr name_iter = 0; name_iter < (uptr)packet->servers[servers_iter].name.size; name_iter++)
      {
        push_u8_little(arena, packet->servers[servers_iter].name.data[name_iter]);
      }
      // 296:  u32 name_id
      push_u32_little(arena, packet->servers[servers_iter].name_id);
      // 297:  string:u32 description
      push_u32_little(arena, (u32)packet->servers[servers_iter].description.size);
      for (uptr description_iter = 0; description_iter < (uptr)packet->servers[servers_iter].description.size; description_iter++)
      {
        push_u8_little(arena, packet->servers[servers_iter].description.data[description_iter]);
      }
      // 298:  u32 description_id
      push_u32_little(arena, packet->servers[servers_iter].description_id);
      // 299:  u32 req_feature_id
      push_u32_little(arena, packet->servers[servers_iter].req_feature_id);
      // 300:  string:u32 server_info
      push_u32_little(arena, (u32)packet->servers[servers_iter].server_info.size);
      for (uptr server_info_iter = 0; server_info_iter < (uptr)packet->servers[servers_iter].server_info.size; server_info_iter++)
      {
        push_u8_little(arena, packet->servers[servers_iter].server_info.data[server_info_iter]);
      }
      // 301:  u32 population_level
      push_u32_little(arena, packet->servers[servers_iter].population_level);
      // 302:  string:u32 population_data
      push_u32_little(arena, (u32)packet->servers[servers_iter].population_data.size);
      for (uptr population_data_iter = 0; population_data_iter < (uptr)packet->servers[servers_iter].population_data.size; population_data_iter++)
      {
        push_u8_little(arena, packet->servers[servers_iter].population_data.data[population_data_iter]);
      }
      // 303:  string:u32 access_expression
      push_u32_little(arena, (u32)packet->servers[servers_iter].access_expression.size);
      for (uptr access_expression_iter = 0; access_expression_iter < (uptr)packet->servers[servers_iter].access_expression.size; access_expression_iter++)
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
    {
      std::cout << "packing not handled";
    }
  }

  arena->alignment = prev_align;
  Buffer result{};
  result.data = static_cast<char*>(memory_begin);
  result.size = reinterpret_cast<uptr>((static_cast<u8*>(arena->memory + arena->cursor) - *static_cast<uptr*>(memory_begin)));
  return result;
}

uptr unpack_login_packet(Arena* arena, Stream* stream, void* result_ptr, Login_Packet_Kind packet_kind)
{
  uptr cursor_begin = stream->cursor;
  switch (packet_kind)
  {
    case Login_Packet_Kind_LoginRequest:
    {
      Login_Packet_LoginRequest* packet = static_cast<Login_Packet_LoginRequest*>(result_ptr);
      // 5:  string:u32 session_id
      packet->session_id.size = read_u32_little(stream);
      packet->session_id.data = static_cast<char*>(arena_push_size(arena, packet->session_id.size));
      for (uptr session_id_iter = 0; session_id_iter < (uptr)packet->session_id.size; session_id_iter++)
      {
        packet->session_id.data[session_id_iter] = read_u8_little(stream);
      }
      // 6:  string:u32 system_fingerprint
      packet->system_fingerprint.size = read_u32_little(stream);
      packet->system_fingerprint.data = static_cast<char*>(arena_push_size(arena, packet->system_fingerprint.size));
      for (uptr system_fingerprint_iter = 0; system_fingerprint_iter < (uptr)packet->system_fingerprint.size; system_fingerprint_iter++)
      {
        packet->system_fingerprint.data[system_fingerprint_iter] = read_u8_little(stream);
      }
      // 7:  u32 locale
      packet->locale = read_u32_little(stream);
      // 8:  u32 third_party_auth_ticket
      packet->third_party_auth_ticket = read_u32_little(stream);
      // 9:  u32 third_party_user_id
      packet->third_party_user_id = read_u32_little(stream);
      // 10:  u32 third_party_id
      packet->third_party_id = read_u32_little(stream);
    } break;

    case Login_Packet_Kind_LoginReply:
    {
      Login_Packet_LoginReply* packet = static_cast<Login_Packet_LoginReply*>(result_ptr);
      // 13:  b8 is_logged_in
      packet->is_logged_in = read_b8_little(stream);
      // 14:  u32 status
      packet->status = read_u32_little(stream);
      // 15:  u32 result_code
      packet->result_code = read_u32_little(stream);
      // 16:  b8 is_member
      packet->is_member = read_b8_little(stream);
      // 17:  b8 is_internal
      packet->is_internal = read_b8_little(stream);
      // 18:  string:u32 namespace_name
      packet->namespace_name.size = read_u32_little(stream);
      packet->namespace_name.data = static_cast<char*>(arena_push_size(arena, packet->namespace_name.size));
      for (uptr namespace_name_iter = 0; namespace_name_iter < (uptr)packet->namespace_name.size; namespace_name_iter++)
      {
        packet->namespace_name.data[namespace_name_iter] = read_u8_little(stream);
      }
      // 19:  list:u32 account_features
      packet->account_features_count = read_u32_little(stream);
      packet->account_features = static_cast<account_features_s>(arena_push_size(arena, sizeof(account_features_s) * packet->account_features_count));
      for (uptr account_features_iter = 0; account_features_iter < packet->account_features_count; account_features_iter++)
      {
      // 21:  u32 key
      packet->account_features[account_features_iter].key = read_u32_little(stream);
      // 22:  u32 id
      packet->account_features[account_features_iter].id = read_u32_little(stream);
      // 23:  b8 active
      packet->account_features[account_features_iter].active = read_b8_little(stream);
      // 24:  u32 remaining_count
      packet->account_features[account_features_iter].remaining_count = read_u32_little(stream);
      // 25:  string:u32 raw_data
      packet->account_features[account_features_iter].raw_data.size = read_u32_little(stream);
      packet->account_features[account_features_iter].raw_data.data = static_cast<char*>(arena_push_size(arena, packet->account_features[account_features_iter].raw_data.size));
      for (uptr raw_data_iter = 0; raw_data_iter < (uptr)packet->account_features[account_features_iter].raw_data.size; raw_data_iter++)
      {
        packet->account_features[account_features_iter].raw_data.data[raw_data_iter] = read_u8_little(stream);
      }
    } // account_features
      // 27:  bytes:u32 application_payload
      packet->application_payload.size = read_u32_little(stream);
			 packet->application_payload.data = static_cast<char*>(arena_push_size(arena, packet->application_payload.size));
      for (uptr application_payload_iter = 0; application_payload_iter < packet->application_payload.size; application_payload_iter++)
      {
        packet->application_payload.data[application_payload_iter] = read_u8_little(stream);
      }
      // 28:  list:u32 error_details
      packet->error_details_count = read_u32_little(stream);
      packet->error_details = static_cast<error_details_s>(arena_push_size(arena, sizeof(error_details_s) * packet->error_details_count));
      for (uptr error_details_iter = 0; error_details_iter < packet->error_details_count; error_details_iter++)
      {
      // 30:  u32 unknown_dword1
      packet->error_details[error_details_iter].unknown_dword1 = read_u32_little(stream);
      // 31:  string:u32 name
      packet->error_details[error_details_iter].name.size = read_u32_little(stream);
      packet->error_details[error_details_iter].name.data = static_cast<char*>(arena_push_size(arena, packet->error_details[error_details_iter].name.size));
      for (uptr name_iter = 0; name_iter < (uptr)packet->error_details[error_details_iter].name.size; name_iter++)
      {
        packet->error_details[error_details_iter].name.data[name_iter] = read_u8_little(stream);
      }
      // 32:  string:u32 value
      packet->error_details[error_details_iter].value.size = read_u32_little(stream);
      packet->error_details[error_details_iter].value.data = static_cast<char*>(arena_push_size(arena, packet->error_details[error_details_iter].value.size));
      for (uptr value_iter = 0; value_iter < (uptr)packet->error_details[error_details_iter].value.size; value_iter++)
      {
        packet->error_details[error_details_iter].value.data[value_iter] = read_u8_little(stream);
      }
    } // error_details
      // 34:  string:u32 ip_country_code
      packet->ip_country_code.size = read_u32_little(stream);
      packet->ip_country_code.data = static_cast<char*>(arena_push_size(arena, packet->ip_country_code.size));
      for (uptr ip_country_code_iter = 0; ip_country_code_iter < (uptr)packet->ip_country_code.size; ip_country_code_iter++)
      {
        packet->ip_country_code.data[ip_country_code_iter] = read_u8_little(stream);
      }
    } break;

    case Login_Packet_Kind_CharacterCreateRequest:
    {
      Login_Packet_CharacterCreateRequest* packet = static_cast<Login_Packet_CharacterCreateRequest*>(result_ptr);
      // 41:  u32 server_id
      packet->server_id = read_u32_little(stream);
      // 42:  u32 unk_u32
      packet->unk_u32 = read_u32_little(stream);
      // 44:  stream: char_payload
      packet->char_payload_size = read_u32_little(stream);
      if ((packet->char_payload_size = packet->char_payload_size ? packet->char_payload_size : ~((u32)0)) != ~((u32)0))
      {
      // 47:  u8 empire_id
      packet->char_payload.empire_id = read_u8_little(stream);
      // 48:  u32 head_type
      packet->char_payload.head_type = read_u32_little(stream);
      // 49:  u32 profile_type
      packet->char_payload.profile_type = read_u32_little(stream);
      // 50:  u32 gender
      packet->char_payload.gender = read_u32_little(stream);
      // 51:  string:u32 character_name
      packet->char_payload.character_name.size = read_u32_little(stream);
      packet->char_payload.character_name.data = static_cast<char*>(arena_push_size(arena, packet->char_payload.character_name.size));
      for (uptr character_name_iter = 0; character_name_iter < (uptr)packet->char_payload.character_name.size; character_name_iter++)
      {
        packet->char_payload.character_name.data[character_name_iter] = read_u8_little(stream);
      }
      }
    } break;

    case Login_Packet_Kind_CharacterCreateReply:
    {
      Login_Packet_CharacterCreateReply* packet = static_cast<Login_Packet_CharacterCreateReply*>(result_ptr);
      // 55:  u32 status
      packet->status = read_u32_little(stream);
      // 56:  u64 character_id
      packet->character_id = read_u64_little(stream);
    } break;

    case Login_Packet_Kind_CharacterLoginRequest:
    {
      Login_Packet_CharacterLoginRequest* packet = static_cast<Login_Packet_CharacterLoginRequest*>(result_ptr);
      // 59:  u64 character_id
      packet->character_id = read_u64_little(stream);
      // 60:  u32 server_id
      packet->server_id = read_u32_little(stream);
      // 61:  u32 status
      packet->status = read_u32_little(stream);
      // 63:  stream: payload2
      packet->payload2_size = read_u32_little(stream);
      if ((packet->payload2_size = packet->payload2_size ? packet->payload2_size : ~((u32)0)) != ~((u32)0))
      {
      // 66:  string:u32 locale
      packet->payload2.locale.size = read_u32_little(stream);
      packet->payload2.locale.data = static_cast<char*>(arena_push_size(arena, packet->payload2.locale.size));
      for (uptr locale_iter = 0; locale_iter < (uptr)packet->payload2.locale.size; locale_iter++)
      {
        packet->payload2.locale.data[locale_iter] = read_u8_little(stream);
      }
      // 67:  u32 locale_id
      packet->payload2.locale_id = read_u32_little(stream);
      // 68:  u32 preferred_gateway_id
      packet->payload2.preferred_gateway_id = read_u32_little(stream);
      }
    } break;

    case Login_Packet_Kind_CharacterLoginReply:
    {
      Login_Packet_CharacterLoginReply* packet = static_cast<Login_Packet_CharacterLoginReply*>(result_ptr);
      // 72:  u64 character_id
      packet->character_id = read_u64_little(stream);
      // 73:  u32 server_id
      packet->server_id = read_u32_little(stream);
      // 74:  u32 last_login
      packet->last_login = read_u32_little(stream);
      // 75:  u32 status
      packet->status = read_u32_little(stream);
      // 77:  stream: login_payload
      packet->login_payload_size = read_u32_little(stream);
      if ((packet->login_payload_size = packet->login_payload_size ? packet->login_payload_size : ~((u32)0)) != ~((u32)0))
      {
      // 79:  u8 unk_byte_1
      packet->login_payload.unk_byte_1 = read_u8_little(stream);
      // 80:  u8 unk_byte_2
      packet->login_payload.unk_byte_2 = read_u8_little(stream);
      // 81:  string:u32 server_address
      packet->login_payload.server_address.size = read_u32_little(stream);
      packet->login_payload.server_address.data = static_cast<char*>(arena_push_size(arena, packet->login_payload.server_address.size));
      for (uptr server_address_iter = 0; server_address_iter < (uptr)packet->login_payload.server_address.size; server_address_iter++)
      {
        packet->login_payload.server_address.data[server_address_iter] = read_u8_little(stream);
      }
      // 82:  string:u32 server_ticket
      packet->login_payload.server_ticket.size = read_u32_little(stream);
      packet->login_payload.server_ticket.data = static_cast<char*>(arena_push_size(arena, packet->login_payload.server_ticket.size));
      for (uptr server_ticket_iter = 0; server_ticket_iter < (uptr)packet->login_payload.server_ticket.size; server_ticket_iter++)
      {
        packet->login_payload.server_ticket.data[server_ticket_iter] = read_u8_little(stream);
      }
      // 83:  bytes:u32 encryption_key
      packet->login_payload.encryption_key.size = read_u32_little(stream);
			 packet->login_payload.encryption_key.data = static_cast<char*>(arena_push_size(arena, packet->login_payload.encryption_key.size));
      for (uptr encryption_key_iter = 0; encryption_key_iter < packet->login_payload.encryption_key.size; encryption_key_iter++)
      {
        packet->login_payload.encryption_key.data[encryption_key_iter] = read_u8_little(stream);
      }
      // 84:  u32 soe_protocol_version
      packet->login_payload.soe_protocol_version = read_u32_little(stream);
      // 85:  u64 character_id
      packet->login_payload.character_id = read_u64_little(stream);
      // 86:  u64 unk_u64
      packet->login_payload.unk_u64 = read_u64_little(stream);
      // 87:  string:u32 station_name
      packet->login_payload.station_name.size = read_u32_little(stream);
      packet->login_payload.station_name.data = static_cast<char*>(arena_push_size(arena, packet->login_payload.station_name.size));
      for (uptr station_name_iter = 0; station_name_iter < (uptr)packet->login_payload.station_name.size; station_name_iter++)
      {
        packet->login_payload.station_name.data[station_name_iter] = read_u8_little(stream);
      }
      // 88:  string:u32 character_name
      packet->login_payload.character_name.size = read_u32_little(stream);
      packet->login_payload.character_name.data = static_cast<char*>(arena_push_size(arena, packet->login_payload.character_name.size));
      for (uptr character_name_iter = 0; character_name_iter < (uptr)packet->login_payload.character_name.size; character_name_iter++)
      {
        packet->login_payload.character_name.data[character_name_iter] = read_u8_little(stream);
      }
      // 89:  string:u32 unk_str
      packet->login_payload.unk_str.size = read_u32_little(stream);
      packet->login_payload.unk_str.data = static_cast<char*>(arena_push_size(arena, packet->login_payload.unk_str.size));
      for (uptr unk_str_iter = 0; unk_str_iter < (uptr)packet->login_payload.unk_str.size; unk_str_iter++)
      {
        packet->login_payload.unk_str.data[unk_str_iter] = read_u8_little(stream);
      }
      // 90:  u64 server_feature_bit
      packet->login_payload.server_feature_bit = read_u64_little(stream);
      }
    } break;

    case Login_Packet_Kind_CharacterDeleteRequest:
    {
      Login_Packet_CharacterDeleteRequest* packet = static_cast<Login_Packet_CharacterDeleteRequest*>(result_ptr);
      // 94:  u64 character_id
      packet->character_id = read_u64_little(stream);
    } break;

    case Login_Packet_Kind_TunnelAppPacketClientToServer:
    {
      Login_Packet_TunnelAppPacketClientToServer* packet = static_cast<Login_Packet_TunnelAppPacketClientToServer*>(result_ptr);
      // 97:  u32 server_id
      packet->server_id = read_u32_little(stream);
      // 98:  u32 unk1
      packet->unk1 = read_u32_little(stream);
      // 100:  stream: data_client
      packet->data_client_size = read_u32_little(stream);
      if ((packet->data_client_size = packet->data_client_size ? packet->data_client_size : ~((u32)0)) != ~((u32)0))
      {
      // 102:  b8 tunnel_op_code
      packet->data_client.tunnel_op_code = read_b8_little(stream);
      // 103:  b8 sub_op_code
      packet->data_client.sub_op_code = read_b8_little(stream);
      // 104:  string:u32 character_name
      packet->data_client.character_name.size = read_u32_little(stream);
      packet->data_client.character_name.data = static_cast<char*>(arena_push_size(arena, packet->data_client.character_name.size));
      for (uptr character_name_iter = 0; character_name_iter < (uptr)packet->data_client.character_name.size; character_name_iter++)
      {
        packet->data_client.character_name.data[character_name_iter] = read_u8_little(stream);
      }
      }
    } break;

    case Login_Packet_Kind_TunnelAppPacketServerToClient:
    {
      Login_Packet_TunnelAppPacketServerToClient* packet = static_cast<Login_Packet_TunnelAppPacketServerToClient*>(result_ptr);
      // 108:  u32 server_id
      packet->server_id = read_u32_little(stream);
      // 109:  u32 unk1
      packet->unk1 = read_u32_little(stream);
      // 111:  stream: data_server
      packet->data_server_size = read_u32_little(stream);
      if ((packet->data_server_size = packet->data_server_size ? packet->data_server_size : ~((u32)0)) != ~((u32)0))
      {
      // 113:  u8 tunnel_op_code
      packet->data_server.tunnel_op_code = read_u8_little(stream);
      // 114:  u8 sub_op_code
      packet->data_server.sub_op_code = read_u8_little(stream);
      // 115:  string:u32 character_name
      packet->data_server.character_name.size = read_u32_little(stream);
      packet->data_server.character_name.data = static_cast<char*>(arena_push_size(arena, packet->data_server.character_name.size));
      for (uptr character_name_iter = 0; character_name_iter < (uptr)packet->data_server.character_name.size; character_name_iter++)
      {
        packet->data_server.character_name.data[character_name_iter] = read_u8_little(stream);
      }
      // 116:  string:u32 character_name2
      packet->data_server.character_name2.size = read_u32_little(stream);
      packet->data_server.character_name2.data = static_cast<char*>(arena_push_size(arena, packet->data_server.character_name2.size));
      for (uptr character_name2_iter = 0; character_name2_iter < (uptr)packet->data_server.character_name2.size; character_name2_iter++)
      {
        packet->data_server.character_name2.data[character_name2_iter] = read_u8_little(stream);
      }
      // 117:  u32 status
      packet->data_server.status = read_u32_little(stream);
      }
    } break;

    case Login_Packet_Kind_CharacterDeleteReply:
    {
      Login_Packet_CharacterDeleteReply* packet = static_cast<Login_Packet_CharacterDeleteReply*>(result_ptr);
      // 125:  u64 character_id
      packet->character_id = read_u64_little(stream);
      // 126:  u32 status
      packet->status = read_u32_little(stream);
      // 127:  bytes:u32 payload3
      packet->payload3.size = read_u32_little(stream);
			 packet->payload3.data = static_cast<char*>(arena_push_size(arena, packet->payload3.size));
      for (uptr payload3_iter = 0; payload3_iter < packet->payload3.size; payload3_iter++)
      {
        packet->payload3.data[payload3_iter] = read_u8_little(stream);
      }
    } break;

    case Login_Packet_Kind_CharacterSelectInfoReply:
    {
      Login_Packet_CharacterSelectInfoReply* packet = static_cast<Login_Packet_CharacterSelectInfoReply*>(result_ptr);
      // 132:  u32 character_status
      packet->character_status = read_u32_little(stream);
      // 133:  b8 can_bypass_server_lock
      packet->can_bypass_server_lock = read_b8_little(stream);
      // 135:  list:u32 characters
      packet->characters_count = read_u32_little(stream);
      packet->characters = static_cast<characters_s>(arena_push_size(arena, sizeof(characters_s) * packet->characters_count));
      for (uptr characters_iter = 0; characters_iter < packet->characters_count; characters_iter++)
      {
      // 137:  u64 charId
      packet->characters[characters_iter].charId = read_u64_little(stream);
      // 138:  u32 serverId
      packet->characters[characters_iter].serverId = read_u32_little(stream);
      // 139:  u64 lastLoginDate
      packet->characters[characters_iter].lastLoginDate = read_u64_little(stream);
      // 140:  u32 nullField
      packet->characters[characters_iter].nullField = read_u32_little(stream);
      // 141:  u32 status
      packet->characters[characters_iter].status = read_u32_little(stream);
      // 143:  stream: payload
      packet->characters[characters_iter].payload_size = read_u32_little(stream);
      if ((packet->characters[characters_iter].payload_size = packet->characters[characters_iter].payload_size ? packet->characters[characters_iter].payload_size : ~((u32)0)) != ~((u32)0))
      {
      // 145:  string:u32 name
      packet->characters[characters_iter].payload.name.size = read_u32_little(stream);
      packet->characters[characters_iter].payload.name.data = static_cast<char*>(arena_push_size(arena, packet->characters[characters_iter].payload.name.size));
      for (uptr name_iter = 0; name_iter < (uptr)packet->characters[characters_iter].payload.name.size; name_iter++)
      {
        packet->characters[characters_iter].payload.name.data[name_iter] = read_u8_little(stream);
      }
      // 146:  u8 empireId
      packet->characters[characters_iter].payload.empireId = read_u8_little(stream);
      // 147:  u32 battleRank
      packet->characters[characters_iter].payload.battleRank = read_u32_little(stream);
      // 148:  u32 nextBattleRankPercent
      packet->characters[characters_iter].payload.nextBattleRankPercent = read_u32_little(stream);
      // 149:  u32 headId
      packet->characters[characters_iter].payload.headId = read_u32_little(stream);
      // 150:  u32 actorModelId
      packet->characters[characters_iter].payload.actorModelId = read_u32_little(stream);
      // 151:  u32 gender
      packet->characters[characters_iter].payload.gender = read_u32_little(stream);
      // 152:  u32 profileId
      packet->characters[characters_iter].payload.profileId = read_u32_little(stream);
      // 153:  u32 unkDword1
      packet->characters[characters_iter].payload.unkDword1 = read_u32_little(stream);
      // 154:  u32 unkDword2
      packet->characters[characters_iter].payload.unkDword2 = read_u32_little(stream);
      // 156:  list:u32 loadoutSlots
      packet->characters[characters_iter].payload.loadoutSlots_count = read_u32_little(stream);
      packet->characters[characters_iter].payload.loadoutSlots = static_cast<loadoutSlots_s>(arena_push_size(arena, sizeof(loadoutSlots_s) * packet->characters[characters_iter].payload.loadoutSlots_count));
      for (uptr loadoutSlots_iter = 0; loadoutSlots_iter < packet->characters[characters_iter].payload.loadoutSlots_count; loadoutSlots_iter++)
      {
      // 158:  u32 hotbarSlotId
      packet->characters[characters_iter].payload.loadoutSlots[loadoutSlots_iter].hotbarSlotId = read_u32_little(stream);
      // 159:  u32 loadoutId
      packet->characters[characters_iter].payload.loadoutSlots[loadoutSlots_iter].loadoutId = read_u32_little(stream);
      // 160:  u32 slotId
      packet->characters[characters_iter].payload.loadoutSlots[loadoutSlots_iter].slotId = read_u32_little(stream);
      // 161:  u32 itemDefId
      packet->characters[characters_iter].payload.loadoutSlots[loadoutSlots_iter].itemDefId = read_u32_little(stream);
      // 162:  u64 loadoutItemGuid
      packet->characters[characters_iter].payload.loadoutSlots[loadoutSlots_iter].loadoutItemGuid = read_u64_little(stream);
      // 163:  u8 unkByte1
      packet->characters[characters_iter].payload.loadoutSlots[loadoutSlots_iter].unkByte1 = read_u8_little(stream);
      // 164:  u32 unkDword1
      packet->characters[characters_iter].payload.loadoutSlots[loadoutSlots_iter].unkDword1 = read_u32_little(stream);
    } // loadoutSlots
      // 167:  list:u32 itemDefinitions
      packet->characters[characters_iter].payload.itemDefinitions_count = read_u32_little(stream);
      packet->characters[characters_iter].payload.itemDefinitions = static_cast<itemDefinitions_s>(arena_push_size(arena, sizeof(itemDefinitions_s) * packet->characters[characters_iter].payload.itemDefinitions_count));
      for (uptr itemDefinitions_iter = 0; itemDefinitions_iter < packet->characters[characters_iter].payload.itemDefinitions_count; itemDefinitions_iter++)
      {
      // 169:  u32 ID
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].ID = read_u32_little(stream);
      // 171:  list:u32 item_defs
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs_count = read_u32_little(stream);
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs = static_cast<item_defs_s>(arena_push_size(arena, sizeof(item_defs_s) * packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs_count));
      for (uptr item_defs_iter = 0; item_defs_iter < packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs_count; item_defs_iter++)
      {
      // 173:  u32 defs_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].defs_id = read_u32_little(stream);
      // 175:  u8 bitflags1
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].bitflags1 = read_u8_little(stream);
      // 185:  u8 bitflags2
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].bitflags2 = read_u8_little(stream);
      // 195:  u32 name_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].name_id = read_u32_little(stream);
      // 196:  u32 description_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].description_id = read_u32_little(stream);
      // 197:  u32 content_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].content_id = read_u32_little(stream);
      // 198:  u32 image_set_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].image_set_id = read_u32_little(stream);
      // 199:  u32 tint_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].tint_id = read_u32_little(stream);
      // 200:  u32 hud_image_set_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].hud_image_set_id = read_u32_little(stream);
      // 201:  u32 unk_dword_1
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_1 = read_u32_little(stream);
      // 202:  u32 unk_dword_2
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_2 = read_u32_little(stream);
      // 203:  u32 cost
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].cost = read_u32_little(stream);
      // 204:  u32 item_class
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].item_class = read_u32_little(stream);
      // 205:  u32 profile_override
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].profile_override = read_u32_little(stream);
      // 206:  string:u32 model_name
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].model_name.size = read_u32_little(stream);
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].model_name.data = static_cast<char*>(arena_push_size(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].model_name.size));
      for (uptr model_name_iter = 0; model_name_iter < (uptr)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].model_name.size; model_name_iter++)
      {
        packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].model_name.data[model_name_iter] = read_u8_little(stream);
      }
      // 207:  string:u32 texture_alias
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].texture_alias.size = read_u32_little(stream);
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].texture_alias.data = static_cast<char*>(arena_push_size(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].texture_alias.size));
      for (uptr texture_alias_iter = 0; texture_alias_iter < (uptr)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].texture_alias.size; texture_alias_iter++)
      {
        packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].texture_alias.data[texture_alias_iter] = read_u8_little(stream);
      }
      // 208:  u32 gender_usage
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].gender_usage = read_u32_little(stream);
      // 209:  u32 item_type
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].item_type = read_u32_little(stream);
      // 210:  u32 category_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].category_id = read_u32_little(stream);
      // 211:  u32 weapon_trail_effect_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].weapon_trail_effect_id = read_u32_little(stream);
      // 212:  u32 composite_effect_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].composite_effect_id = read_u32_little(stream);
      // 213:  u32 power_rating
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].power_rating = read_u32_little(stream);
      // 214:  u32 min_profile_rank
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].min_profile_rank = read_u32_little(stream);
      // 215:  u32 rarity
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].rarity = read_u32_little(stream);
      // 216:  u32 activatable_ability_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].activatable_ability_id = read_u32_little(stream);
      // 217:  u32 activatable_ability_set_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].activatable_ability_set_id = read_u32_little(stream);
      // 218:  u32 passive_ability_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].passive_ability_id = read_u32_little(stream);
      // 219:  u32 passive_ability_set_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].passive_ability_set_id = read_u32_little(stream);
      // 220:  u32 max_stack_size
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].max_stack_size = read_u32_little(stream);
      // 221:  u32 min_stack_size
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].min_stack_size = read_u32_little(stream);
      // 222:  string:u32 tint_alias
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].tint_alias.size = read_u32_little(stream);
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].tint_alias.data = static_cast<char*>(arena_push_size(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].tint_alias.size));
      for (uptr tint_alias_iter = 0; tint_alias_iter < (uptr)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].tint_alias.size; tint_alias_iter++)
      {
        packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].tint_alias.data[tint_alias_iter] = read_u8_little(stream);
      }
      // 223:  u32 tint_group_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].tint_group_id = read_u32_little(stream);
      // 224:  u32 member_discount
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].member_discount = read_u32_little(stream);
      // 225:  u32 vip_rank_required
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].vip_rank_required = read_u32_little(stream);
      // 226:  u32 race_set_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].race_set_id = read_u32_little(stream);
      // 227:  u32 ui_model_camera_id_1
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].ui_model_camera_id_1 = read_u32_little(stream);
      // 228:  u32 equip_count_max
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].equip_count_max = read_u32_little(stream);
      // 229:  u32 curreny_type
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].curreny_type = read_u32_little(stream);
      // 230:  u32 datasheet_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].datasheet_id = read_u32_little(stream);
      // 231:  u32 item_type_1
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].item_type_1 = read_u32_little(stream);
      // 232:  u32 skill_set_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].skill_set_id = read_u32_little(stream);
      // 233:  string:u32 overlay_texture
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].overlay_texture.size = read_u32_little(stream);
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].overlay_texture.data = static_cast<char*>(arena_push_size(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].overlay_texture.size));
      for (uptr overlay_texture_iter = 0; overlay_texture_iter < (uptr)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].overlay_texture.size; overlay_texture_iter++)
      {
        packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].overlay_texture.data[overlay_texture_iter] = read_u8_little(stream);
      }
      // 234:  string:u32 decal_slot
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].decal_slot.size = read_u32_little(stream);
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].decal_slot.data = static_cast<char*>(arena_push_size(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].decal_slot.size));
      for (uptr decal_slot_iter = 0; decal_slot_iter < (uptr)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].decal_slot.size; decal_slot_iter++)
      {
        packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].decal_slot.data[decal_slot_iter] = read_u8_little(stream);
      }
      // 235:  u32 overlay_adjustment
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].overlay_adjustment = read_u32_little(stream);
      // 236:  u32 trial_duration_sec
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].trial_duration_sec = read_u32_little(stream);
      // 237:  u32 next_trial_delay_sec
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].next_trial_delay_sec = read_u32_little(stream);
      // 238:  u32 client_use_requirement
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].client_use_requirement = read_u32_little(stream);
      // 239:  string:u32 override_appearance
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].override_appearance.size = read_u32_little(stream);
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].override_appearance.data = static_cast<char*>(arena_push_size(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].override_appearance.size));
      for (uptr override_appearance_iter = 0; override_appearance_iter < (uptr)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].override_appearance.size; override_appearance_iter++)
      {
        packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].override_appearance.data[override_appearance_iter] = read_u8_little(stream);
      }
      // 240:  u32 override_camera_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].override_camera_id = read_u32_little(stream);
      // 241:  u32 unk_dword_3
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_3 = read_u32_little(stream);
      // 242:  u32 unk_dword_4
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_4 = read_u32_little(stream);
      // 243:  u32 unk_dword_5
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_5 = read_u32_little(stream);
      // 244:  u32 bulk
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].bulk = read_u32_little(stream);
      // 245:  u32 active_equip_slot_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].active_equip_slot_id = read_u32_little(stream);
      // 246:  u32 passive_equip_slot_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].passive_equip_slot_id = read_u32_little(stream);
      // 247:  u32 passive_equip_slot_group_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].passive_equip_slot_group_id = read_u32_little(stream);
      // 248:  u32 unk_dword_6
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_6 = read_u32_little(stream);
      // 249:  u32 grinder_reward_set_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].grinder_reward_set_id = read_u32_little(stream);
      // 250:  u32 build_bar_group_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].build_bar_group_id = read_u32_little(stream);
      // 251:  string:u32 unk_string_1
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_string_1.size = read_u32_little(stream);
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_string_1.data = static_cast<char*>(arena_push_size(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_string_1.size));
      for (uptr unk_string_1_iter = 0; unk_string_1_iter < (uptr)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_string_1.size; unk_string_1_iter++)
      {
        packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_string_1.data[unk_string_1_iter] = read_u8_little(stream);
      }
      // 252:  b8 unk_bool_1
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_bool_1 = read_b8_little(stream);
      // 253:  b8 is_armor
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].is_armor = read_b8_little(stream);
      // 254:  u32 unk_dword_7
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_7 = read_u32_little(stream);
      // 255:  u32 param1
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].param1 = read_u32_little(stream);
      // 256:  u32 param2
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].param2 = read_u32_little(stream);
      // 257:  u32 param3
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].param3 = read_u32_little(stream);
      // 258:  string:u32 string_param1
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].string_param1.size = read_u32_little(stream);
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].string_param1.data = static_cast<char*>(arena_push_size(arena, packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].string_param1.size));
      for (uptr string_param1_iter = 0; string_param1_iter < (uptr)packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].string_param1.size; string_param1_iter++)
      {
        packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].string_param1.data[string_param1_iter] = read_u8_little(stream);
      }
      // 259:  u32 ui_model_camera_id_2
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].ui_model_camera_id_2 = read_u32_little(stream);
      // 260:  u32 unk_dword_8
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].unk_dword_8 = read_u32_little(stream);
      // 261:  u32 scrap_value_override
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].scrap_value_override = read_u32_little(stream);
      // 263:  list:u32 stats_item_def_2
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2_count = read_u32_little(stream);
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2 = static_cast<stats_item_def_2_s>(arena_push_size(arena, sizeof(stats_item_def_2_s) * packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2_count));
      for (uptr stats_item_def_2_iter = 0; stats_item_def_2_iter < packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2_count; stats_item_def_2_iter++)
      {
      // 265:  u32 unk_dword_9
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].unk_dword_9 = read_u32_little(stream);
      // 266:  u32 stat_id
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].stat_id = read_u32_little(stream);
      // 268:  switch: variabletype8
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].variabletype8_case = read_u8_little(stream);
      switch(packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].variabletype8_case)
      {
      case 0:
      {
      // 271:  u32 base
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue0.base = read_u32_little(stream);
      // 272:  u32 modifier
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue0.modifier = read_u32_little(stream);
      } break; // statValue0
      case 1:
      {
      // 275:  f32 base
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue1.base = read_f32_little(stream);
      // 276:  f32 modifier
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].variabletype8.statValue1.modifier = read_f32_little(stream);
      } break; // statValue1
      } // variabletype8
      // 279:  u32 unk_dword_10
      packet->characters[characters_iter].payload.itemDefinitions[itemDefinitions_iter].item_defs[item_defs_iter].stats_item_def_2[stats_item_def_2_iter].unk_dword_10 = read_u32_little(stream);
    } // stats_item_def_2
    } // item_defs
    } // itemDefinitions
      // 283:  u64 lastUseDate
      packet->characters[characters_iter].payload.lastUseDate = read_u64_little(stream);
      }
    } // characters
    } break;

    case Login_Packet_Kind_ServerListReply:
    {
      Login_Packet_ServerListReply* packet = static_cast<Login_Packet_ServerListReply*>(result_ptr);
      // 290:  list:u32 servers
      packet->servers_count = read_u32_little(stream);
      packet->servers = static_cast<servers_s>(arena_push_size(arena, sizeof(servers_s) * packet->servers_count));
      for (uptr servers_iter = 0; servers_iter < packet->servers_count; servers_iter++)
      {
      // 292:  u32 id
      packet->servers[servers_iter].id = read_u32_little(stream);
      // 293:  u32 state
      packet->servers[servers_iter].state = read_u32_little(stream);
      // 294:  b8 is_locked
      packet->servers[servers_iter].is_locked = read_b8_little(stream);
      // 295:  string:u32 name
      packet->servers[servers_iter].name.size = read_u32_little(stream);
      packet->servers[servers_iter].name.data = static_cast<char*>(arena_push_size(arena, packet->servers[servers_iter].name.size));
      for (uptr name_iter = 0; name_iter < (uptr)packet->servers[servers_iter].name.size; name_iter++)
      {
        packet->servers[servers_iter].name.data[name_iter] = read_u8_little(stream);
      }
      // 296:  u32 name_id
      packet->servers[servers_iter].name_id = read_u32_little(stream);
      // 297:  string:u32 description
      packet->servers[servers_iter].description.size = read_u32_little(stream);
      packet->servers[servers_iter].description.data = static_cast<char*>(arena_push_size(arena, packet->servers[servers_iter].description.size));
      for (uptr description_iter = 0; description_iter < (uptr)packet->servers[servers_iter].description.size; description_iter++)
      {
        packet->servers[servers_iter].description.data[description_iter] = read_u8_little(stream);
      }
      // 298:  u32 description_id
      packet->servers[servers_iter].description_id = read_u32_little(stream);
      // 299:  u32 req_feature_id
      packet->servers[servers_iter].req_feature_id = read_u32_little(stream);
      // 300:  string:u32 server_info
      packet->servers[servers_iter].server_info.size = read_u32_little(stream);
      packet->servers[servers_iter].server_info.data = static_cast<char*>(arena_push_size(arena, packet->servers[servers_iter].server_info.size));
      for (uptr server_info_iter = 0; server_info_iter < (uptr)packet->servers[servers_iter].server_info.size; server_info_iter++)
      {
        packet->servers[servers_iter].server_info.data[server_info_iter] = read_u8_little(stream);
      }
      // 301:  u32 population_level
      packet->servers[servers_iter].population_level = read_u32_little(stream);
      // 302:  string:u32 population_data
      packet->servers[servers_iter].population_data.size = read_u32_little(stream);
      packet->servers[servers_iter].population_data.data = static_cast<char*>(arena_push_size(arena, packet->servers[servers_iter].population_data.size));
      for (uptr population_data_iter = 0; population_data_iter < (uptr)packet->servers[servers_iter].population_data.size; population_data_iter++)
      {
        packet->servers[servers_iter].population_data.data[population_data_iter] = read_u8_little(stream);
      }
      // 303:  string:u32 access_expression
      packet->servers[servers_iter].access_expression.size = read_u32_little(stream);
      packet->servers[servers_iter].access_expression.data = static_cast<char*>(arena_push_size(arena, packet->servers[servers_iter].access_expression.size));
      for (uptr access_expression_iter = 0; access_expression_iter < (uptr)packet->servers[servers_iter].access_expression.size; access_expression_iter++)
      {
        packet->servers[servers_iter].access_expression.data[access_expression_iter] = read_u8_little(stream);
      }
      // 304:  b8 is_access_allowed
      packet->servers[servers_iter].is_access_allowed = read_b8_little(stream);
    } // servers
    } break;

    default:
    {
      std::cout << "unpacking not handled";
    }
  }

  return stream->cursor - cursor_begin;
}
