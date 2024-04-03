#ifndef LOGINUDP_11
#define LOGINUDP_11

#include "../../OLD_endian.hpp"

#define LOGIN_LOGINREQUEST_ID 0x1
#define LOGIN_LOGINREPLY_ID 0x2
#define LOGIN_LOGOUT_ID 0x3
#define LOGIN_FORCEDISCONNECT_ID 0x4
#define LOGIN_CHARACTERCREATEREQUEST_ID 0x5
#define LOGIN_CHARACTERCREATEREPLY_ID 0x6
#define LOGIN_CHARACTERLOGINREQUEST_ID 0x7
#define LOGIN_CHARACTERLOGINREPLY_ID 0x8
#define LOGIN_CHARACTERDELETEREQUEST_ID 0x9
#define LOGIN_TUNNELAPPPACKETCLIENTTOSERVER_ID 0x10
#define LOGIN_TUNNELAPPPACKETSERVERTOCLIENT_ID 0x11
#define LOGIN_CHARACTERTRANSFERREQUEST_ID 0x12
#define LOGIN_CHARACTERTRANSFERREPLY_ID 0x13
#define LOGIN_CHARACTERDELETEREPLY_ID 0xa
#define LOGIN_CHARACTERSELECTINFOREQUEST_ID 0xb
#define LOGIN_CHARACTERSELECTINFOREPLY_ID 0xc
#define LOGIN_SERVERLISTREQUEST_ID 0xd
#define LOGIN_SERVERLISTREPLY_ID 0xe
#define LOGIN_SERVERUPDATE_ID 0xf

#define LOGIN_PACKET_KINDS \
LOGIN_PACKET_KIND(Login_Packet_Kind_Unhandled, "Unhandled"), \
LOGIN_PACKET_KIND(Login_Packet_Kind_LoginRequest, "LoginRequest"), \
LOGIN_PACKET_KIND(Login_Packet_Kind_LoginReply, "LoginReply"), \
LOGIN_PACKET_KIND(Login_Packet_Kind_Logout, "Logout"), \
LOGIN_PACKET_KIND(Login_Packet_Kind_ForceDisconnect, "ForceDisconnect"), \
LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterCreateRequest, "CharacterCreateRequest"), \
LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterCreateReply, "CharacterCreateReply"), \
LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterLoginRequest, "CharacterLoginRequest"), \
LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterLoginReply, "CharacterLoginReply"), \
LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterDeleteRequest, "CharacterDeleteRequest"), \
LOGIN_PACKET_KIND(Login_Packet_Kind_TunnelAppPacketClientToServer, "TunnelAppPacketClientToServer"), \
LOGIN_PACKET_KIND(Login_Packet_Kind_TunnelAppPacketServerToClient, "TunnelAppPacketServerToClient"), \
LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterTransferRequest, "CharacterTransferRequest"), \
LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterTransferReply, "CharacterTransferReply"), \
LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterDeleteReply, "CharacterDeleteReply"), \
LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterSelectInfoRequest, "CharacterSelectInfoRequest"), \
LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterSelectInfoReply, "CharacterSelectInfoReply"), \
LOGIN_PACKET_KIND(Login_Packet_Kind_ServerListRequest, "ServerListRequest"), \
LOGIN_PACKET_KIND(Login_Packet_Kind_ServerListReply, "ServerListReply"), \
LOGIN_PACKET_KIND(Login_Packet_Kind_ServerUpdate, "ServerUpdate"), \
LOGIN_PACKET_KIND(Login_Packet_Kind__End, "")

typedef enum Login_Packet_Kind
{
#define LOGIN_PACKET_KIND(e, s) e
	LOGIN_PACKET_KINDS
#undef LOGIN_PACKET_KIND
} Login_Packet_Kind;

typedef struct Login_Packet_LoginRequest Login_Packet_LoginRequest;
struct Login_Packet_LoginRequest
{
	u32 session_id_length;
	char* session_id;
	u32 system_fingerprint_length;
	char* system_fingerprint;
	u32 locale;
	u32 third_party_auth_ticket;
	u32 third_party_user_id;
	u32 third_party_id;
};


typedef struct Login_Packet_LoginReply Login_Packet_LoginReply;
struct Login_Packet_LoginReply
{
	b8 is_logged_in;
	u32 status;
	u32 result_code;
	b8 is_member;
	b8 is_internal;
	u32 namespace_name_length;
	char* namespace_name;
	u32 account_features_count;
	struct account_features_s
	{
		u32 key;
		u32 id;
		b8 active;
		u32 remaining_count;
		u32 raw_data_length;
		char* raw_data;
	}*account_features;
	u32 application_payload_length;
	uint8_t* application_payload;
	u32 error_details_count;
	struct error_details_s
	{
		u32 unknown_dword1;
		u32 name_length;
		char* name;
		u32 value_length;
		char* value;
	}*error_details;
	u32 ip_country_code_length;
	char* ip_country_code;
};


typedef struct Login_Packet_CharacterCreateRequest Login_Packet_CharacterCreateRequest;
struct Login_Packet_CharacterCreateRequest
{
	u32 server_id;
	u32 unk_u32;
	u32 char_payload_length;
	struct char_payload_s
	{
		u8 empire_id;
		u32 head_type;
		u32 profile_type;
		u32 gender;
		u32 character_name_length;
		char* character_name;
	}*char_payload;
};


typedef struct Login_Packet_CharacterCreateReply Login_Packet_CharacterCreateReply;
struct Login_Packet_CharacterCreateReply
{
	u32 status;
	u64 character_id;
};


typedef struct Login_Packet_CharacterLoginRequest Login_Packet_CharacterLoginRequest;
struct Login_Packet_CharacterLoginRequest
{
	u64 character_id;
	u32 server_id;
	u32 status;
	u32 payload2_length;
	struct payload2_s
	{
		u32 locale_length;
		char* locale;
		u32 locale_id;
		u32 preferred_gateway_id;
	}*payload2;
};


typedef struct Login_Packet_CharacterLoginReply Login_Packet_CharacterLoginReply;
struct Login_Packet_CharacterLoginReply
{
	u64 character_id;
	u32 server_id;
	u32 last_login;
	u32 status;
	u32 login_payload_length;
	struct login_payload_s
	{
		u32 server_address_length;
		char* server_address;
		u32 server_ticket_length;
		char* server_ticket;
		u32 encryption_key_length;
		uint8_t* encryption_key;
		u32 soe_protocol_version;
		u64 character_id;
		u64 unk_u64;
		u32 station_name_length;
		char* station_name;
		u32 character_name_length;
		char* character_name;
		u32 unk_str_length;
		char* unk_str;
		u64 server_feature_bit;
	}*login_payload;
};


typedef struct Login_Packet_CharacterDeleteRequest Login_Packet_CharacterDeleteRequest;
struct Login_Packet_CharacterDeleteRequest
{
	u64 character_id;
};


typedef struct Login_Packet_TunnelAppPacketClientToServer Login_Packet_TunnelAppPacketClientToServer;
struct Login_Packet_TunnelAppPacketClientToServer
{
	u32 server_id;
	u32 unk1;
	u32 data_client_length;
	struct data_client_s
	{
		b8 tunnel_op_code;
		b8 sub_op_code;
		u32 character_name_length;
		char* character_name;
	}*data_client;
};


typedef struct Login_Packet_TunnelAppPacketServerToClient Login_Packet_TunnelAppPacketServerToClient;
struct Login_Packet_TunnelAppPacketServerToClient
{
	u32 server_id;
	u32 unk1;
	u32 data_server_length;
	struct data_server_s
	{
		u8 tunnel_op_code;
		u8 sub_op_code;
		u32 character_name_length;
		char* character_name;
		u32 character_name2_length;
		char* character_name2;
		u32 status;
	}*data_server;
};


typedef struct Login_Packet_CharacterDeleteReply Login_Packet_CharacterDeleteReply;
struct Login_Packet_CharacterDeleteReply
{
	u64 character_id;
	u32 status;
	u32 payload3_length;
	uint8_t* payload3;
};


typedef struct Login_Packet_CharacterSelectInfoReply Login_Packet_CharacterSelectInfoReply;
struct Login_Packet_CharacterSelectInfoReply
{
	u32 character_status;
	b8 can_bypass_server_lock;
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
	}*stats_item_def_2;
	u32 itemDefinitions_count;
	struct itemDefinitions_s
	{
		u32 ID;
	}*itemDefinitions;
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
		u32 model_name_length;
		char* model_name;
		u32 texture_alias_length;
		char* texture_alias;
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
		u32 tint_alias_length;
		char* tint_alias;
		u32 tint_group_id;
		u32 member_discount;
		u32 vip_rank_required;
		u32 race_set_id;
		u32 ui_model_camera_id_1;
		u32 equip_count_max;
		i32 curreny_type;
		u32 datasheet_id;
		u32 item_type_1;
		u32 skill_set_id;
		u32 overlay_texture_length;
		char* overlay_texture;
		u32 decal_slot_length;
		char* decal_slot;
		u32 overlay_adjustment;
		u32 trial_duration_sec;
		u32 next_trial_delay_sec;
		u32 client_use_requirement;
		u32 override_appearance_length;
		char* override_appearance;
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
		u32 unk_string_1_length;
		char* unk_string_1;
		b8 unk_bool_1;
		b8 is_armor;
		u32 unk_dword_7;
		u32 param1;
		u32 param2;
		u32 param3;
		u32 string_param1_length;
		char* string_param1;
		u32 ui_model_camera_id_2;
		u32 unk_dword_8;
		i32 scrap_value_override;
	}*item_defs;
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
	}*loadoutSlots;
	u32 characters_count;
	struct characters_s
	{
		u64 charId;
		u32 serverId;
		u64 lastLoginDate;
		u32 nullField;
		u32 status;
		u32 payload_length;
		struct payload_s
		{
			u32 name_length;
			char* name;
			u8 empireId;
			u32 battleRank;
			u32 nextBattleRankPercent;
			u32 headId;
			u32 actorModelId;
			u32 gender;
			u32 profileId;
			u32 unkDword1;
			u32 unkDword2;
			u64 lastUseDate;
		}*payload;
	}*characters;
};


typedef struct Login_Packet_ServerListReply Login_Packet_ServerListReply;
struct Login_Packet_ServerListReply
{
	u32 servers_count;
	struct servers_s
	{
		u32 id;
		u32 state;
		b8 is_locked;
		u32 name_length;
		char* name;
		u32 name_id;
		u32 description_length;
		char* description;
		u32 description_id;
		u32 req_feature_id;
		u32 server_info_length;
		char* server_info;
		u32 population_level;
		u32 population_data_length;
		char* population_data;
		u32 access_expression_length;
		char* access_expression;
		b8 is_access_allowed;
	}*servers;
};

uint32_t login_packet_pack(enum Login_Packet_Kind packet_kind, void* packet_ptr, uint8_t* buffer);
void login_packet_unpack(uint8_t* data, uint32_t data_length, enum Login_Packet_Kind packet_kind, void* packet_ptr, Arena* arena);

#endif // !LOGINUDP_11