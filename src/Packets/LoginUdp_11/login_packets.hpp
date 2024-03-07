#ifndef LOGIN_PACKETS_HPP
#define LOGIN_PACKETS_HPP

#include "../../app_memory.hpp"

#include "../../Packets/LoginUdp_11/login_packets.hpp"
#include "../../Login_Server/login.hpp"

#define LOGIN_PACKET_ID_SIZE        1
#define LOGIN_PACKET_RESERVED_SIZE  (CORE_DATA_FRAGMENT_EXTRA_SIZE)

#define LOGIN_PACKET_KINDS                                                                                       \
  LOGIN_PACKET_KIND(Login_Packet_Kind_Unhandled, 0x00, "Unhandled"),                                             \
      LOGIN_PACKET_KIND(Login_Packet_Kind_LoginRequest, 0x01, "LoginRequest"),                                   \
      LOGIN_PACKET_KIND(Login_Packet_Kind_LoginReply, 0x02, "LoginReply"),                                       \
      LOGIN_PACKET_KIND(Login_Packet_Kind_Logout, 0x03, "Logout"),                                               \
      LOGIN_PACKET_KIND(Login_Packet_Kind_ForceDisconnect, 0x04, "ForceDisconnect"),                             \
      LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterCreateRequest, 0x05, "CharacterCreateRequest"),               \
      LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterCreateReply, 0x06, "CharacterCreateReply"),                   \
      LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterLoginRequest, 0x07, "CharacterLoginRequest"),                 \
      LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterLoginReply, 0x08, "CharacterLoginReply"),                     \
      LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterDeleteRequest, 0x09, "CharacterDeleteRequest"),               \
      LOGIN_PACKET_KIND(Login_Packet_Kind_TunnelAppPacketClientToServer, 0x10, "TunnelAppPacketClientToServer"), \
      LOGIN_PACKET_KIND(Login_Packet_Kind_TunnelAppPacketServerToClient, 0x11, "TunnelAppPacketServerToClient"), \
      LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterTransferRequest, 0x12, "CharacterTransferRequest"),           \
      LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterTransferReply, 0x13, "CharacterTransferReply"),               \
      LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterDeleteReply, 0x0a, "CharacterDeleteReply"),                   \
      LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterSelectInfoRequest, 0x0b, "CharacterSelectInfoRequest"),       \
      LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterSelectInfoReply, 0x0c, "CharacterSelectInfoReply"),           \
      LOGIN_PACKET_KIND(Login_Packet_Kind_ServerListRequest, 0x0d, "ServerListRequest"),                         \
      LOGIN_PACKET_KIND(Login_Packet_Kind_ServerListReply, 0x0e, "ServerListReply"),                             \
      LOGIN_PACKET_KIND(Login_Packet_Kind_ServerUpdate, 0x0f, "ServerUpdate"),                                   \
      LOGIN_PACKET_KIND(Login_Packet_Kind__End, 0x14, "")

enum Login_Packet_Kind
{
#define LOGIN_PACKET_KIND(kind, id, str) kind
	LOGIN_PACKET_KINDS
#undef LOGIN_PACKET_KIND
};

static Login_Packet_Kind login_packet_kinds[Login_Packet_Kind__End + 1]
{
#define LOGIN_PACKET_KIND(kind, id, str) kind
		LOGIN_PACKET_KINDS
#undef LOGIN_PACKET_KIND
};

static uint16_t login_packet_ids[Login_Packet_Kind__End + 1]
{
#define LOGIN_PACKET_KIND(kind, id, str) id
		LOGIN_PACKET_KINDS
#undef LOGIN_PACKET_KIND
};

static const char* login_packet_names[Login_Packet_Kind__End + 1]
{
#define LOGIN_PACKET_KIND(kind, id, str) str
		LOGIN_PACKET_KINDS
#undef LOGIN_PACKET_KIND
};

struct Login_Packet_LoginRequest
{
	Buffer session_id;
	Buffer system_fingerprint;
	uint32_t locale;
	uint32_t third_party_auth_ticket;
	uint32_t third_party_user_id;
	uint32_t third_party_id;
};

struct Login_Packet_LoginReply
{
	uint8_t is_logged_in;
	uint32_t status;
	uint32_t result_code;
	uint8_t is_member;
	uint8_t is_internal;
	Buffer namespace_name;
	uint32_t account_features_count;
	struct account_features_s
	{
		uint32_t key;
		uint32_t id;
		uint8_t active;
		uint32_t remaining_count;
		Buffer raw_data;
	}*account_features;
	Buffer application_payload;
	uint32_t error_details_count;
	struct error_details_s
	{
		uint32_t unknown_dword1;
		Buffer name;
		Buffer value;
	}*error_details;
	Buffer ip_country_code;
};

struct Login_Packet_CharacterCreateRequest
{
	uint32_t server_id;
	uint32_t unk_u32;
	uint32_t char_payload_size;
	struct char_payload_s
	{
		uint8_t empire_id;
		uint32_t head_type;
		uint32_t profile_type;
		uint32_t gender;
		Buffer character_name;
	} char_payload;
};

struct Login_Packet_CharacterCreateReply
{
	uint32_t status;
	uint64_t character_id;
};

struct Login_Packet_CharacterLoginRequest
{
	uint64_t character_id;
	uint32_t server_id;
	uint32_t status;
	uint32_t payload2_size;
	struct payload2_s
	{
		Buffer locale;
		uint32_t locale_id;
		uint32_t preferred_gateway_id;
	} payload2;
};

struct Login_Packet_CharacterLoginReply
{
	uint64_t character_id;
	uint32_t server_id;
	uint32_t last_login;
	uint32_t status;
	uint32_t login_payload_size;
	struct login_payload_s
	{
		uint8_t unk_byte_1;
		uint8_t unk_byte_2;
		Buffer server_address;
		Buffer server_ticket;
		Buffer encryption_key;
		uint32_t soe_protocol_version;
		uint64_t character_id;
		uint64_t unk_u64;
		Buffer station_name;
		Buffer character_name;
		Buffer unk_str;
		uint64_t server_feature_bit;
	} login_payload;
};

struct Login_Packet_CharacterDeleteRequest
{
	uint64_t character_id;
};

struct Login_Packet_TunnelAppPacketClientToServer
{
	uint32_t server_id;
	uint32_t unk1;
	uint32_t data_client_size;
	struct data_client_s
	{
		uint8_t tunnel_op_code;
		uint8_t sub_op_code;
		Buffer character_name;
	} data_client;
};

struct Login_Packet_TunnelAppPacketServerToClient
{
	uint32_t server_id;
	uint32_t unk1;
	uint32_t data_server_size;
	struct data_server_s
	{
		uint8_t tunnel_op_code;
		uint8_t sub_op_code;
		Buffer character_name;
		Buffer character_name2;
		uint32_t status;
	} data_server;
};

struct Login_Packet_CharacterDeleteReply
{
	uint64_t character_id;
	uint32_t status;
	Buffer payload3;
};

struct Login_Packet_CharacterSelectInfoReply
{
	uint32_t character_status;
	uint8_t can_bypass_server_lock;
	uint32_t characters_count;
	struct characters_s
	{
		uint64_t charId;
		uint32_t serverId;
		uint64_t lastLoginDate;
		uint32_t nullField;
		uint32_t status;
		uint32_t payload_size;
		struct payload_s
		{
			Buffer name;
			uint8_t empireId;
			uint32_t battleRank;
			uint32_t nextBattleRankPercent;
			uint32_t headId;
			uint32_t actorModelId;
			uint32_t gender;
			uint32_t profileId;
			uint32_t unkDword1;
			uint32_t unkDword2;
			uint32_t loadoutSlots_count;
			struct loadoutSlots_s
			{
				uint32_t hotbarSlotId;
				uint32_t loadoutId;
				uint32_t slotId;
				uint32_t itemDefId;
				uint64_t loadoutItemGuid;
				uint8_t unkByte1;
				uint32_t unkDword1;
			}*loadoutSlots;
			uint32_t itemDefinitions_count;
			struct itemDefinitions_s
			{
				uint32_t ID;
				uint32_t item_defs_count;
				struct item_defs_s
				{
					uint32_t defs_id;
					uint8_t bitflags1;
					uint8_t bitflags2;
					uint32_t name_id;
					uint32_t description_id;
					uint32_t content_id;
					uint32_t image_set_id;
					uint32_t tint_id;
					uint32_t hud_image_set_id;
					uint32_t unk_dword_1;
					uint32_t unk_dword_2;
					uint32_t cost;
					uint32_t item_class;
					uint32_t profile_override;
					Buffer model_name;
					Buffer texture_alias;
					uint32_t gender_usage;
					uint32_t item_type;
					uint32_t category_id;
					uint32_t weapon_trail_effect_id;
					uint32_t composite_effect_id;
					uint32_t power_rating;
					uint32_t min_profile_rank;
					uint32_t rarity;
					uint32_t activatable_ability_id;
					uint32_t activatable_ability_set_id;
					uint32_t passive_ability_id;
					uint32_t passive_ability_set_id;
					uint32_t max_stack_size;
					uint32_t min_stack_size;
					Buffer tint_alias;
					uint32_t tint_group_id;
					uint32_t member_discount;
					uint32_t vip_rank_required;
					uint32_t race_set_id;
					uint32_t ui_model_camera_id_1;
					uint32_t equip_count_max;
					uint32_t curreny_type;
					uint32_t datasheet_id;
					uint32_t item_type_1;
					uint32_t skill_set_id;
					Buffer overlay_texture;
					Buffer decal_slot;
					uint32_t overlay_adjustment;
					uint32_t trial_duration_sec;
					uint32_t next_trial_delay_sec;
					uint32_t client_use_requirement;
					Buffer override_appearance;
					uint32_t override_camera_id;
					uint32_t unk_dword_3;
					uint32_t unk_dword_4;
					uint32_t unk_dword_5;
					uint32_t bulk;
					uint32_t active_equip_slot_id;
					uint32_t passive_equip_slot_id;
					uint32_t passive_equip_slot_group_id;
					uint32_t unk_dword_6;
					uint32_t grinder_reward_set_id;
					uint32_t build_bar_group_id;
					Buffer unk_string_1;
					uint8_t unk_bool_1;
					uint8_t is_armor;
					uint32_t unk_dword_7;
					uint32_t param1;
					uint32_t param2;
					uint32_t param3;
					Buffer string_param1;
					uint32_t ui_model_camera_id_2;
					uint32_t unk_dword_8;
					uint32_t scrap_value_override;
					uint32_t stats_item_def_2_count;
					struct stats_item_def_2_s
					{
						uint32_t unk_dword_9;
						uint32_t stat_id;
						uint8_t variabletype8_case;
						union
						{
							struct statValue0_s
							{
								uint32_t base;
								uint32_t modifier;
							} statValue0;
							struct statValue1_s
							{
								float base;
								float modifier;
							} statValue1;
						} variabletype8;
						uint32_t unk_dword_10;
					}*stats_item_def_2;
				}*item_defs;
			}*itemDefinitions;
			uint64_t lastUseDate;
		} payload;
	}*characters;
};

struct Login_Packet_ServerListReply
{
	uint32_t servers_count;
	struct servers_s
	{
		uint32_t id;
		uint32_t state;
		uint8_t is_locked;
		Buffer name;
		uint32_t name_id;
		Buffer description;
		uint32_t description_id;
		uint32_t req_feature_id;
		Buffer server_info;
		uint32_t population_level;
		Buffer population_data;
		Buffer access_expression;
		uint8_t is_access_allowed;
	}*servers;
};

Buffer pack_login_packet(Arena* arena, void* packet_ptr, Login_Packet_Kind packet_kind);
uintptr_t unpack_login_packet(Arena* arena, Stream* stream, void* result_ptr, Login_Packet_Kind packet_kind);

#endif // !LOGIN_PACKETS_HPP