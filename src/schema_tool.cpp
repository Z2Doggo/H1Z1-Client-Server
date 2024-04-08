// FUCK IT, WE BALLIN' - Z2Doggo, April 7th, 2024 @ 5:21PM Central US Time 

#include "misc.hpp"

//------------------------------------------------------------------------------------------------------------------------
uptr load_file_to_buffer(const char* path, Buffer buffer)
{
	uptr result = 0;
	FILE* file = fopen(path, "rb");
	result = fread(buffer.data, 1, buffer.size, file);
	fclose(file);

	return result;
}

u32 is_char_hex_digit(char c)
{
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

u32 is_char_digit(char c)
{
	return (c >= '0' && c <= '9');
}

u32 is_char_alpha(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

char char_to_upper(char c)
{
	return ('a' <= c && c <= 'z') ? c - ('a' - 'A') : c;
}

char char_to_lower(char c)
{
	return ('A' <= c && c <= 'Z') ? c + ('a' - 'A') : c;
}

void string_to_upper(Buffer buffer)
{
	for (uptr i = 0; i < buffer.size; i++)
	{
		buffer.data[i] = char_to_upper(buffer.data[i]);
	}
}

void string_to_lower(Buffer buffer)
{
	for (uptr i = 0; i < buffer.size; i++)
	{
		buffer.data[i] = char_to_lower(buffer.data[i]);
	}
}

char* string_copy_as_upper_zt(Buffer buffer, Arena* arena)
{
	Buffer result{};
	result.size = buffer.size;
	result.data = static_cast<char*>(arena_push_copy_zt(arena, buffer.data, buffer.size));
	string_to_upper(result);

	return result.data;
}

char* string_copy_as_lower_zt(Buffer buffer, Arena* arena)
{
	Buffer result{};
	result.size = buffer.size;
	result.data = (char*)arena_push_copy_zt(arena, buffer.data, buffer.size);
	string_to_lower(result);

	return result.data;
}

struct String_Triplet
{
	uptr size;
	char* original;
	char* upper;
	char* lower;
};

String_Triplet string_triplet_build(Buffer original, Arena* arena)
{
	String_Triplet result{};
	result.size = original.size;
	result.original = original.data;
	result.upper = string_copy_as_upper_zt(original, arena);
	result.lower = string_copy_as_lower_zt(original, arena);

	return result;
}

Stream stream_allocate(uptr size)
{
	Stream result{};
	result.buffer.size = size;
	result.buffer.data = static_cast<char*>(calloc(size, 1));

	return result;
}

#define STREAM_REMAINING_DATA_P(stream)  (stream->buffer.data + stream->cursor)
#define STREAM_REMAINING_SIZE_P(stream)  (stream->buffer.size - stream->cursor)

void stream_write_string(Stream* stream, Buffer string_buffer, ...)
{
	va_list args;
	va_start(args, string_buffer);

	stream->cursor += vsnprintf((char*)STREAM_REMAINING_DATA_P(stream),
		STREAM_REMAINING_SIZE_P(stream),
		(char*)string_buffer.data,
		args);

	va_end(args);
}

struct Parent
{
	Buffer parent;
	u32 use_arrow;
};

struct Parent_Stack
{
	Parent* parents;
	uptr capacity;
	Parent* tail;
};

void parent_stack_push(Parent_Stack* stack, Buffer s, u32 use_arrow)
{
	Parent temp{};
	temp.parent = s;
	temp.use_arrow = use_arrow;
	*stack->tail++ = temp;
}

void parent_stack_pop(Parent_Stack* stack)
{
	--stack->tail;
}

Parent_Stack parent_stack_allocate(i32 count, Arena* arena)
{
	Parent_Stack result{};
	result.parents = result.tail = static_cast<Parent*>(arena_push_size(arena, sizeof(Parent) * count));
	result.capacity = reinterpret_cast<uptr>(result.parents + count);

	return result;
}

Buffer parent_stack_assemble(Parent_Stack stack, Arena* arena)
{
	Stream stream{};
	stream.buffer.size = KB(10);
	stream.buffer.data = static_cast<char*>(arena_push_size(arena, stream.buffer.size));

	while (stack.parents < stack.tail)
	{
		stream_write_string(&stream, BUNDLE_STRING("%.*s"), stack.parents->parent.size, stack.parents->parent.data);
		stream_write_string(&stream, stack.parents->use_arrow ? BUNDLE_STRING("->") : BUNDLE_STRING("."));
		stack.parents++;
	}

	return buffer_from_array(stream.cursor, stream.buffer.data);
}

static Stream kind_stream;
static Stream registered_ids_stream;
static Stream packer_stream;
static Stream unpacker_stream;
static Stream main_struct_stream;

#define TOKEN_TABLE(X) \
	X(Error), \
	X(End_Of_Stream), \
	X(Open_Brace), \
	X(Closed_Brace), \
	X(Colon), \
	X(Identifier), \
	X(Number), \
	X(Hex_Number), \
	X(Group), \
	X(Packet), \
	X(Endian), \
	X(Opcode), \
	X(List), \
	X(String), \
	X(Bytes), \
	X(Stream), \
	X(Uint2b), \
	X(Vec3), \
	X(Vec4), \
	X(Switch), \
	X(Case), \
	X(B8), \
	X(B16), \
	X(B32), \
	X(B64), \
	X(I8), \
	X(I16), \
	X(I32), \
	X(I64), \
	X(U8), \
	X(U16), \
	X(U32), \
	X(U64), \
	X(F16), \
	X(F32), \
	X(F64), \
	X(Little), \
	X(Big)

#define TOKEN_KIND(kind) Token_Kind_##kind
enum Token_Kind
{
	TOKEN_TABLE(TOKEN_KIND)
};

#define TOKEN_NAME(name) BUFFER_STRING(#name)
static Buffer global_token_names[] =
{
	TOKEN_TABLE(TOKEN_NAME)
};

struct Tokenizer
{
	char* path;
	u8* cursor_begin;
	u8* cursor;
	uptr line;
	uptr column_begin;
	uptr column;
};

struct Token
{
	char* path;
	Token_Kind kind;
	Buffer lexeme;
	uptr line;
	uptr column_begin;
};

#define LIST_TABLE(LIST_X) \
	LIST_X(Invalid), \
	LIST_X(Variable), \
	LIST_X(Literal)

#define LIST_KIND(kind) List_Kind_##kind
enum List_Kind
{
	LIST_TABLE(LIST_KIND)
};

#define LIST_NAME(kind)  BUFFER_STRING(#kind)
static Buffer global_list_names[] =
{
	LIST_TABLE(LIST_NAME)
};

void emit_group_enter(String_Triplet name)
{
	stream_write_string(&kind_stream,
		BUNDLE_STRING(
			"#define %.*s_PACKET_TABLE(%.*s_X) \\\n"
			"\t%.*s_X(Unhandled), \\\n"
		),
		name.size, name.upper,
		name.size, name.upper,
		name.size, name.upper
	);

	stream_write_string(&registered_ids_stream,
		BUNDLE_STRING(
			"static std::array<u8, static_cast<u8>(%.*s_Packet_Kind__End) + 1>global_%.*s_packet_ids\n"
			"{\n"
		),
		name.size, name.original,
		name.size, name.lower
	);

	stream_write_string(&packer_stream,
		BUNDLE_STRING(
			"\n"
			"Buffer pack_%.*s_packet(Arena* arena, void* packet_ptr, %.*s_Packet_Kind packet_kind)\n"
			"{\n"
			"  uptr prev_align = arena->alignment;\n"
			"  arena->alignment = 1;\n"
			"  void* memory_begin = (static_cast<u8*>(arena->memory + arena->cursor));\n"
			"  switch (packet_kind)\n"
			"  {\n"
		),
		name.size, name.lower,
		name.size, name.original
	);

	stream_write_string(&unpacker_stream,
		BUNDLE_STRING(
			"\n"
			"uptr unpack_%.*s_packet(Arena* arena, Stream* stream, void* result_ptr, %.*s_Packet_Kind packet_kind)\n"
			"{\n"
			"  uptr cursor_begin = stream->cursor;\n"
			"  switch (packet_kind)\n"
			"  {\n"
		),
		name.size, name.lower,
		name.size, name.original
	);
}

void emit_group_exit(String_Triplet name)
{
	stream_write_string(&kind_stream,
		BUNDLE_STRING(
			"  %.*s_X(_End)\n"
			"\n"
			"#define %.*s_PACKET_KIND(kind) %.*s_Packet_Kind_##kind\n"
			"enum %.*s_Packet_Kind\n"
			"{\n"
			"  %.*s_PACKET_TABLE(%.*s_PACKET_KIND)\n"
			"};\n"
			"\n"
			"#define %.*s_PACKET_NAME(str) #str\n"
			"static std::array<std::string, static_cast<u32>(%.*s_Packet_Kind__End) + 1>global_%.*s_packet_names\n"
			"{\n"
			"  %.*s_PACKET_TABLE(%.*s_PACKET_NAME)\n"
			"};\n\n"
		),
		name.size, name.upper,

		name.size, name.upper,
		name.size, name.original,
		name.size, name.original,

		name.size, name.upper,
		name.size, name.upper,

		name.size, name.upper,

		name.size, name.original,
		name.size, name.lower,

		name.size, name.upper,
		name.size, name.upper
	);

	stream_write_string(&registered_ids_stream,
		BUNDLE_STRING(
			"};\n"
		));

	stream_write_string(&packer_stream,
		BUNDLE_STRING(
			"    default:\n"
			"    {\n"
			"      std::cout << \"packing not handled\";\n"
			"    }\n"
			"  }\n"
			"\n"
			"  arena->alignment = prev_align;\n"
			"  Buffer result{};\n"
			"  result.data = static_cast<char*>(memory_begin);\n"
			"  result.size = reinterpret_cast<uptr>((static_cast<u8*>(arena->memory + arena->cursor) - *static_cast<uptr*>(memory_begin)));\n"
			"  return result;\n"
			"}\n"
		));

	stream_write_string(&unpacker_stream,
		BUNDLE_STRING(
			"    default:\n"
			"    {\n"
			"      std::cout << \"unpacking not handled\";\n"
			"    }\n"
			"  }\n"
			"\n"
			"  return stream->cursor - cursor_begin;\n"
			"}\n"
		));
}

void emit_packet_enter(String_Triplet group_name,
	String_Triplet packet_name,
	u64 opcode_value,
	Token_Kind opcode_type,
	u32 has_subcode,
	u64 subcode_value,
	Token_Kind subcode_type,
	Token_Kind endian_type,
	u32 will_generate_struct,
	Arena* arena)
{
	Arena_Snapshot snapshot = arena_take_snapshot(arena);
	DEFER_SCOPE(0, arena_apply_snapshot(snapshot))
	{
		//String_Triplet name = string_triplet_build(packet_name.lexeme, arena);

		char* opcode_lexeme = string_copy_as_lower_zt(global_token_names[opcode_type], arena);
		char* subcode_lexeme = 0;
		if (has_subcode)
		{
			subcode_lexeme = string_copy_as_lower_zt(global_token_names[subcode_type], arena);
		}
		char* endian_lexeme = string_copy_as_lower_zt(global_token_names[endian_type], arena);

		u64 opcode_sum = opcode_value;
		if (has_subcode)
		{
			uptr shift_size = 0;
			switch (subcode_type)
			{
			case Token_Kind_U8: // fallthrough
			case Token_Kind_I8:
			{
				shift_size = 8;
			} break;

			case Token_Kind_U16: // fallthrough
			case Token_Kind_I16:
			{
				shift_size = 16;
			} break;

			case Token_Kind_U32: // fallthrough
			case Token_Kind_I32:
			{
				shift_size = 32;
			} break;

			default:
			{
				printf("Invalid subcode size type");
			}
			}
			opcode_sum = (opcode_sum << shift_size) | subcode_value;
		}

		UNUSED(opcode_lexeme);
		UNUSED(subcode_lexeme);


		stream_write_string(&kind_stream,
			BUNDLE_STRING(
				"  %.*s_X(%.*s), \\\n"
			),
			group_name.size, group_name.upper,
			packet_name.size, packet_name.original
		);

		stream_write_string(&registered_ids_stream,
			BUNDLE_STRING(
				"  %#llx,\n"
			),
			opcode_sum
		);

		stream_write_string(&packer_stream,
			BUNDLE_STRING(
				"    case %.*s_Packet_Kind_%.*s:\n"
				"    {\n"
			),
			group_name.size, group_name.original,
			packet_name.size, packet_name.original
		);

		if (will_generate_struct)
		{
			stream_write_string(&main_struct_stream,
				BUNDLE_STRING(
					"\n"
					"struct %.*s_Packet_%.*s\n"
					"{\n"
				),
				group_name.size, group_name.original,
				packet_name.size, packet_name.original
			);

			stream_write_string(&packer_stream,
				BUNDLE_STRING(
					"      %.*s_Packet_%.*s* packet = static_cast<%.*s_Packet_%.*s*>(packet_ptr);\n"
				),
				group_name.size, group_name.original,
				packet_name.size, packet_name.original,
				group_name.size, group_name.original,
				packet_name.size, packet_name.original
			);

			stream_write_string(&unpacker_stream,
				BUNDLE_STRING(
					"    case %.*s_Packet_Kind_%.*s:\n"
					"    {\n"
					"      %.*s_Packet_%.*s* packet = static_cast<%.*s_Packet_%.*s*>(result_ptr);\n"
				),
				group_name.size, group_name.original,
				packet_name.size, packet_name.original,
				group_name.size, group_name.original,
				packet_name.size, packet_name.original,
				group_name.size, group_name.original,
				packet_name.size, packet_name.original
			);
		}

		stream_write_string(&packer_stream,
			BUNDLE_STRING(
				"      push_%s_%s(arena, %#llx);\n"
			),
			opcode_lexeme,
			endian_lexeme,
			opcode_value
		);

		if (has_subcode)
		{
			stream_write_string(&packer_stream,
				BUNDLE_STRING(
					"      push_%s_%s(arena, %#llx);\n"
				),
				subcode_lexeme,
				endian_lexeme,
				subcode_value
			);
		}
	}
}

void emit_packet_exit(u32 will_generate_struct)
{
	stream_write_string(&packer_stream, BUNDLE_STRING(
		"    } break;\n"
		"\n"
	));

	if (will_generate_struct)
	{
		stream_write_string(&main_struct_stream, BUNDLE_STRING(
			"};\n"
			"\n"
		));

		stream_write_string(&unpacker_stream, BUNDLE_STRING(
			"    } break;\n"
			"\n"
		));
	}
}

void emit_field_simple(uptr line, Parent_Stack parent_stack, char* type_lexeme, char* endian_lexeme, String_Triplet field_name, Arena* arena)
{
	Arena_Snapshot snapshot = arena_take_snapshot(arena);
	DEFER_SCOPE(0, arena_apply_snapshot(snapshot))
	{
		stream_write_string(&main_struct_stream,
			BUNDLE_STRING(
				"  %s %.*s;\n"
			),
			type_lexeme,
			field_name.size, field_name.original
		);

		Buffer parents = parent_stack_assemble(parent_stack, arena);
		stream_write_string(&packer_stream,
			BUNDLE_STRING(
				"      // %lld:  %s %.*s\n"
				"      push_%s_%s(arena, %.*s%.*s);\n"
			),
			line,
			type_lexeme,
			field_name.size, field_name.original,

			type_lexeme,
			endian_lexeme,
			parents.size, parents.data,
			field_name.size, field_name.original
		);

		stream_write_string(&unpacker_stream,
			BUNDLE_STRING(
				"      // %lld:  %s %.*s\n"
				"      %.*s%.*s = read_%s_%s(stream);\n"
			),
			line,
			type_lexeme,
			field_name.size, field_name.original,

			parents.size, parents.data,
			field_name.size, field_name.original,
			type_lexeme,
			endian_lexeme
		);
	}
}

void emit_field_f16(uptr line, Parent_Stack parent_stack, char* type_lexeme, char* endian_lexeme, String_Triplet field_name, Arena* arena)
{
	Arena_Snapshot snapshot = arena_take_snapshot(arena);
	DEFER_SCOPE(0, arena_apply_snapshot(snapshot))
	{
		stream_write_string(&main_struct_stream,
			BUNDLE_STRING(
				"  %s %.*s;\n"
			),
			type_lexeme,
			field_name.size, field_name.original
		);

		Buffer parents = parent_stack_assemble(parent_stack, arena);
		stream_write_string(&packer_stream,
			BUNDLE_STRING(
				"      // %lld:  %s %.*s\n"
				"      push_%s_%s(arena, %.*s%.*s);\n"
			),
			line,
			type_lexeme,
			field_name.size, field_name.original,

			type_lexeme,
			endian_lexeme,
			parents.size, parents.data,
			field_name.size, field_name.original
		);

		stream_write_string(&unpacker_stream,
			BUNDLE_STRING(
				"      // %lld:  %s %.*s\n"
				"      %.*s%.*s = read_%s_%s(stream);\n"
			),
			line,
			type_lexeme,
			field_name.size, field_name.original,

			parents.size, parents.data,
			field_name.size, field_name.original,
			type_lexeme,
			endian_lexeme
		);
	}
}

void emit_field_uint2b(uptr line, Parent_Stack parent_stack, char* endian_lexeme, String_Triplet field_name, Arena* arena)
{
	Arena_Snapshot snapshot = arena_take_snapshot(arena);
	DEFER_SCOPE(0, arena_apply_snapshot(snapshot))
	{
		stream_write_string(&main_struct_stream,
			BUNDLE_STRING(
				"  uint2b %.*s;\n"
			),
			field_name.size, field_name.original
		);

		Buffer parents = parent_stack_assemble(parent_stack, arena);
		stream_write_string(&packer_stream,
			BUNDLE_STRING(
				"      // %lld:  uint2b %.*s\n"
				"      push_uint2b_%s(arena, %.*s%.*s);\n"
			),
			line,
			field_name.size, field_name.original,

			endian_lexeme,
			parents.size, parents.data,
			field_name.size, field_name.original
		);

		stream_write_string(&unpacker_stream,
			BUNDLE_STRING(
				"      // %lld:  uint2b %.*s\n"
				"      %.*s%.*s = read_uint2b_%s(stream);\n"
			),
			line,
			field_name.size, field_name.original,

			parents.size, parents.data,
			field_name.size, field_name.original,
			endian_lexeme
		);
	}
}

void emit_field_string(uptr line, Parent_Stack parent_stack, char* type_lexeme, char* endian_lexeme, String_Triplet field_name, Arena* arena)
{
	Arena_Snapshot snapshot = arena_take_snapshot(arena);
	DEFER_SCOPE(0, arena_apply_snapshot(snapshot))
	{
		// TODO(rhett): we can validate length in packer/unpacker
		stream_write_string(&main_struct_stream,
			BUNDLE_STRING(
				"  Buffer %.*s;\n"
			),
			field_name.size, field_name.original
		);

		// worry about this later - doggo
		Buffer parents = parent_stack_assemble(parent_stack, arena);
		stream_write_string(&packer_stream,
			BUNDLE_STRING(
				"      // %lld:  string:%s %.*s\n"
				"      push_%s_%s(arena, (%s)%.*s%.*s.size);\n"
				"      for (uptr %.*s_iter = 0; %.*s_iter < (uptr)%.*s%.*s.size; %.*s_iter++)\n"
				"      {\n"
				"        push_u8_%s(arena, %.*s%.*s.data[%.*s_iter]);\n"
				"      }\n"
			),
			line,
			type_lexeme,
			field_name.size, field_name.original,

			type_lexeme, endian_lexeme,
			type_lexeme,
			parents.size, parents.data,
			field_name.size, field_name.original,

			field_name.size, field_name.original,
			field_name.size, field_name.original,
			parents.size, parents.data,
			field_name.size, field_name.original,
			field_name.size, field_name.original,

			//type_lexeme, endian_lexeme,
			endian_lexeme,
			parents.size, parents.data,
			field_name.size, field_name.original,
			field_name.size, field_name.original
		);

		stream_write_string(&unpacker_stream,
			BUNDLE_STRING(
				"      // %lld:  string:%s %.*s\n"
				"      %.*s%.*s.size = read_%s_%s(stream);\n"
				"      %.*s%.*s.data = static_cast<char*>(arena_push_size(arena, %.*s%.*s.size));\n"
				"      for (uptr %.*s_iter = 0; %.*s_iter < (uptr)%.*s%.*s.size; %.*s_iter++)\n"
				"      {\n"
				"        %.*s%.*s.data[%.*s_iter] = read_u8_%s(stream);\n"
				"      }\n"
			),
			line,
			type_lexeme,
			field_name.size, field_name.original,

			parents.size, parents.data,
			field_name.size, field_name.original,
			type_lexeme, endian_lexeme,

			parents.size, parents.data,
			field_name.size, field_name.original,
			parents.size, parents.data,
			field_name.size, field_name.original,

			field_name.size, field_name.original,
			field_name.size, field_name.original,
			parents.size, parents.data,
			field_name.size, field_name.original,
			field_name.size, field_name.original,

			parents.size, parents.data,
			field_name.size, field_name.original,
			field_name.size, field_name.original,
			endian_lexeme
			//type_lexeme, endian_lexeme
		);
	}
}

void emit_field_vec3(uptr line, Parent_Stack parent_stack, char* endian_lexeme, String_Triplet field_name, Arena* arena)
{
	Arena_Snapshot snapshot = arena_take_snapshot(arena);
	DEFER_SCOPE(0, arena_apply_snapshot(snapshot))
	{
		// TODO(rhett): we can validate length in packer/unpacker
		stream_write_string(&main_struct_stream,
			BUNDLE_STRING(
				"  vec3 %.*s;\n"
			),
			field_name.size, field_name.original
		);

		Buffer parents = parent_stack_assemble(parent_stack, arena);
		stream_write_string(&packer_stream,
			BUNDLE_STRING(
				"      // %lld:  vec3 %.*s\n"
				"      push_vec3_%s(arena, %.*s%.*s);\n"
			),
			line,
			field_name.size, field_name.original,

			endian_lexeme,
			parents.size, parents.data,
			field_name.size, field_name.original
		);

		stream_write_string(&unpacker_stream,
			BUNDLE_STRING(
				"      // %lld:  vec3 %.*s\n"
				"      %.*s%.*s = read_vec3_%s(stream);\n"
			),
			line,
			field_name.size, field_name.original,

			parents.size, parents.data,
			field_name.size, field_name.original,
			endian_lexeme
		);
	}
}

void emit_field_vec4(uptr line, Parent_Stack parent_stack, char* endian_lexeme, String_Triplet field_name, Arena* arena)
{
	Arena_Snapshot snapshot = arena_take_snapshot(arena);
	DEFER_SCOPE(0, arena_apply_snapshot(snapshot))
	{
		// TODO(rhett): we can validate length in packer/unpacker
		stream_write_string(&main_struct_stream,
			BUNDLE_STRING(
				"  vec4 %.*s;\n"
			),
			field_name.size, field_name.original
		);

		Buffer parents = parent_stack_assemble(parent_stack, arena);
		stream_write_string(&packer_stream,
			BUNDLE_STRING(
				"      // %lld:  vec4 %.*s\n"
				"      push_vec4_%s(arena, %.*s%.*s);\n"
			),
			line,
			field_name.size, field_name.original,

			endian_lexeme,
			parents.size, parents.data,
			field_name.size, field_name.original
		);

		stream_write_string(&unpacker_stream,
			BUNDLE_STRING(
				"      // %lld:  vec4 %.*s\n"
				"      %.*s%.*s = read_vec4_%s(stream);\n"
			),
			line,
			field_name.size, field_name.original,

			parents.size, parents.data,
			field_name.size, field_name.original,
			endian_lexeme
		);
	}
}

void emit_field_bytes(uptr line, Parent_Stack parent_stack, char* type_lexeme, char* endian_lexeme, String_Triplet field_name, Arena* arena)
{
	Arena_Snapshot snapshot = arena_take_snapshot(arena);
	DEFER_SCOPE(0, arena_apply_snapshot(snapshot))
	{
		// TODO(rhett): we can validate length in packer/unpacker
		stream_write_string(&main_struct_stream,
			BUNDLE_STRING(
				"  Buffer %.*s;\n"
			),
			field_name.size, field_name.original
		);

		Buffer parents = parent_stack_assemble(parent_stack, arena);
		stream_write_string(&packer_stream,
			BUNDLE_STRING(
				"      // %lld:  bytes:%s %.*s\n"
				"      push_%s_%s(arena, (%s)%.*s%.*s.size);\n"
				"      for (uptr %.*s_iter = 0; %.*s_iter < %.*s%.*s.size; %.*s_iter++)\n"
				"      {\n"
				"        push_u8_%s(arena, %.*s%.*s.data[%.*s_iter]);\n"
				"      }\n"
			),
			line,
			type_lexeme,
			field_name.size, field_name.original,

			type_lexeme, endian_lexeme,
			type_lexeme,
			parents.size, parents.data,
			field_name.size, field_name.original,

			field_name.size, field_name.original,
			field_name.size, field_name.original,
			parents.size, parents.data,
			field_name.size, field_name.original,
			field_name.size, field_name.original,

			//type_lexeme, endian_lexeme,
			endian_lexeme,
			parents.size, parents.data,
			field_name.size, field_name.original,
			field_name.size, field_name.original
		);

		stream_write_string(&unpacker_stream,
			BUNDLE_STRING(
				"      // %lld:  bytes:%s %.*s\n"
				"      %.*s%.*s.size = read_%s_%s(stream);\n"
				"			 %.*s%.*s.data = static_cast<char*>(arena_push_size(arena, %.*s%.*s.size));\n"
				"      for (uptr %.*s_iter = 0; %.*s_iter < %.*s%.*s.size; %.*s_iter++)\n"
				"      {\n"
				"        %.*s%.*s.data[%.*s_iter] = read_u8_%s(stream);\n"
				"      }\n"
			),
			line,
			type_lexeme,
			field_name.size, field_name.original,

			parents.size, parents.data,
			field_name.size, field_name.original,
			type_lexeme, endian_lexeme,

			parents.size, parents.data,
			field_name.size, field_name.original,
			parents.size, parents.data,
			field_name.size, field_name.original,

			field_name.size, field_name.original,
			field_name.size, field_name.original,
			parents.size, parents.data,
			field_name.size, field_name.original,
			field_name.size, field_name.original,

			parents.size, parents.data,
			field_name.size, field_name.original,
			field_name.size, field_name.original,
			//type_lexeme, endian_lexeme
			endian_lexeme
		);
	}
}

void emit_field_list_enter(uptr line, Parent_Stack parent_stack, char* type_lexeme, char* endian_lexeme, String_Triplet field_name, List_Kind list_kind, u64 count_value, Arena* arena)
{
	Arena_Snapshot snapshot = arena_take_snapshot(arena);
	DEFER_SCOPE(0, arena_apply_snapshot(snapshot))
	{
		stream_write_string(&main_struct_stream,
			BUNDLE_STRING(
				"  %.*s %.*s_count;\n"
				"  struct %.*s_s\n"
				"  {\n"
			),
			type_lexeme, type_lexeme,
			field_name.size, field_name.original,
			field_name.size, field_name.original
		);

		Buffer parents = parent_stack_assemble(parent_stack, arena);
		stream_write_string(&packer_stream,
			BUNDLE_STRING(
				"      // %lld:  list:%s %.*s\n"
			),
			line,
			type_lexeme,
			field_name.size, field_name.original
		);

		stream_write_string(&unpacker_stream,
			BUNDLE_STRING(
				"      // %lld:  list:%s %.*s\n"
			),
			line,
			type_lexeme,
			field_name.size, field_name.original
		);

		if (list_kind == List_Kind_Literal)
		{
			stream_write_string(&packer_stream,
				BUNDLE_STRING(
					"      %.*s%.*s_count = %llu;\n"
				),
				parents.size, parents.data,
				field_name.size, field_name.original,
				count_value
			);

			stream_write_string(&unpacker_stream,
				BUNDLE_STRING(
					"      %.*s%.*s_count = %llu;\n"
				),
				parents.size, parents.data,
				field_name.size, field_name.original,
				count_value
			);
		}
		else if (list_kind == List_Kind_Variable)
		{
			stream_write_string(&packer_stream,
				BUNDLE_STRING(
					"      push_%s_%s(arena, %.*s%.*s_count);\n"
				),
				type_lexeme, endian_lexeme,
				parents.size, parents.data,
				field_name.size, field_name.original
			);

			stream_write_string(&unpacker_stream,
				BUNDLE_STRING(
					"      %.*s%.*s_count = read_%s_%s(stream);\n"
				),
				parents.size, parents.data,
				field_name.size, field_name.original,
				type_lexeme, endian_lexeme
			);
		}
		else
		{
			return;
		}
		stream_write_string(&packer_stream,
			BUNDLE_STRING(
				"      for (uptr %.*s_iter = 0; %.*s_iter < %.*s%.*s_count; %.*s_iter++)\n"
				"      {\n"
			),
			field_name.size, field_name.original,
			field_name.size, field_name.original,
			parents.size, parents.data,
			field_name.size, field_name.original,
			field_name.size, field_name.original
		);

		stream_write_string(&unpacker_stream,
			BUNDLE_STRING(
				"      %.*s%.*s = static_cast<%.*s_s>(arena_push_size(arena, sizeof(%.*s_s) * %.*s%.*s_count));\n"
				"      for (uptr %.*s_iter = 0; %.*s_iter < %.*s%.*s_count; %.*s_iter++)\n"
				"      {\n"
			),
			parents.size, parents.data,
			field_name.size, field_name.original,
			field_name.size, field_name.original,
			field_name.size, field_name.original,
			parents.size, parents.data,
			field_name.size, field_name.original,

			field_name.size, field_name.original,
			field_name.size, field_name.original,
			parents.size, parents.data,
			field_name.size, field_name.original,
			field_name.size, field_name.original
		);
	}
}

void emit_field_list_exit(String_Triplet field_name)
{
	stream_write_string(&main_struct_stream,
		BUNDLE_STRING(
			"  }* %.*s;\n"
		),
		field_name.size, field_name.original
	);

	stream_write_string(&packer_stream,
		BUNDLE_STRING(
			"    } // %.*s\n"
		),
		field_name.size, field_name.original
	);

	stream_write_string(&unpacker_stream,
		BUNDLE_STRING(
			"    } // %.*s\n"
		),
		field_name.size, field_name.original
	);
}

void emit_field_switch_enter(uptr line, Parent_Stack parent_stack, char* type_lexeme, char* endian_lexeme, String_Triplet field_name, Arena* arena)
{
	Arena_Snapshot snapshot = arena_take_snapshot(arena);
	DEFER_SCOPE(0, arena_apply_snapshot(snapshot))
	{
		stream_write_string(&main_struct_stream,
			BUNDLE_STRING(
				"  %s %.*s_case;\n"
				"  union\n"
				"  {\n"
			),
			type_lexeme,
			field_name.size, field_name.original
		);

		Buffer parents = parent_stack_assemble(parent_stack, arena);
		stream_write_string(&packer_stream,
			BUNDLE_STRING(
				"      // %lld:  switch: %.*s\n"
				"      push_%s_%s(arena, %.*s%.*s_case);\n"
				"      switch(%.*s%.*s_case)\n"
				"      {\n"
			),
			line,
			field_name.size, field_name.original,

			type_lexeme,
			endian_lexeme,
			parents.size, parents.data,
			field_name.size, field_name.original,

			parents.size, parents.data,
			field_name.size, field_name.original
		);

		stream_write_string(&unpacker_stream,
			BUNDLE_STRING(
				"      // %lld:  switch: %.*s\n"
				"      %.*s%.*s_case = read_%s_%s(stream);\n"
				"      switch(%.*s%.*s_case)\n"
				"      {\n"
			),
			line,
			field_name.size, field_name.original,

			parents.size, parents.data,
			field_name.size, field_name.original,
			type_lexeme, endian_lexeme,

			parents.size, parents.data,
			field_name.size, field_name.original
		);
	}
}

void emit_field_switch_exit(String_Triplet field_name)
{
	stream_write_string(&main_struct_stream,
		BUNDLE_STRING(
			"  } %.*s;\n"
		),
		field_name.size, field_name.original
	);

	stream_write_string(&packer_stream,
		BUNDLE_STRING(
			"      } // %.*s\n"
		),
		field_name.size, field_name.original);

	stream_write_string(&unpacker_stream,
		BUNDLE_STRING(
			"      } // %.*s\n"
		),
		field_name.size, field_name.original);
}

void emit_field_case_enter(String_Triplet field_name, u64 case_value)
{
	stream_write_string(&main_struct_stream,
		BUNDLE_STRING(
			"  struct %.*s_s\n"
			"  {\n"
		),
		field_name.size, field_name.original
	);

	stream_write_string(&packer_stream,
		BUNDLE_STRING(
			"      case %llu:\n"
			"      {\n"
		),
		case_value);

	stream_write_string(&unpacker_stream,
		BUNDLE_STRING(
			"      case %llu:\n"
			"      {\n"
		),
		case_value);
}

void emit_field_case_exit(String_Triplet field_name)
{
	stream_write_string(&main_struct_stream,
		BUNDLE_STRING(
			"  } %.*s;\n"
		),
		field_name.size, field_name.original
	);

	stream_write_string(&packer_stream,
		BUNDLE_STRING(
			"      } break; // %.*s\n"
		),
		field_name.size, field_name.original
	);

	stream_write_string(&unpacker_stream,
		BUNDLE_STRING(
			"      } break; // %.*s\n"
		),
		field_name.size, field_name.original
	);
}

void emit_field_stream_enter(uptr line, Parent_Stack parent_stack, String_Triplet size_type, char* endian_type, String_Triplet field_name, Arena* arena)
{
	Arena_Snapshot snapshot = arena_take_snapshot(arena);
	DEFER_SCOPE(0, arena_apply_snapshot(snapshot))
	{
		stream_write_string(&main_struct_stream,
			BUNDLE_STRING(
				"  %.*s %.*s_size;\n"
				"  struct %.*s_s\n"
				"  {\n"
			),
			size_type.size, size_type.lower,
			field_name.size, field_name.original,
			field_name.size, field_name.original
		);

		// TODO(rhett): this looks like it can go weird, revise it
		Buffer parents = parent_stack_assemble(parent_stack, arena);
		stream_write_string(&packer_stream,
			BUNDLE_STRING(
				"      // %lld:  stream: %.*s\n"
				"      void* %.*s_size_ptr = (void*)((uptr)arena->memory + arena->cursor);\n"
				"      arena->cursor += sizeof(%.*s);\n"
				"      u32 will_pack_%.*s = %.*s%.*s_size == ~0 ? FALSE : TRUE;\n"
				"      if (!will_pack_%.*s)\n"
				"      {\n"
				"        write_%.*s_%s_at(%.*s_size_ptr, 0);\n"
				"      }\n"
				"      else\n"
				"      {\n"
			),
			line,
			field_name.size, field_name.original,

			field_name.size, field_name.original,

			size_type.size, size_type.lower,

			field_name.size, field_name.original,
			parents.size, parents.data,
			field_name.size, field_name.original,

			field_name.size, field_name.original,

			size_type.size, size_type.lower,
			endian_type,
			field_name.size, field_name.original
		);

		stream_write_string(&unpacker_stream,
			BUNDLE_STRING(
				"      // %lld:  stream: %.*s\n"
				//"      uptr %.*s_size_cursor = stream->cursor;\n"
				"      %.*s%.*s_size = read_%.*s_%s(stream);\n"
				"      if ((%.*s%.*s_size = %.*s%.*s_size ? %.*s%.*s_size : ~((%.*s)0)) != ~((%.*s)0))\n"
				"      {\n"
				//"      u32 will_pack_%.*s = %.*s%.*s_size == ~0 ? FALSE : TRUE;\n"
				//"      for (u32 %.*s_iter = FALSE; %.*s_iter < will_pack_%.*s; %.*s_iter++)\n"
				//"      {\n"
			),
			line,
			field_name.size, field_name.original,

			parents.size, parents.data,
			field_name.size, field_name.original,
			size_type.size, size_type.lower, endian_type,

			parents.size, parents.data,
			field_name.size, field_name.original,
			parents.size, parents.data,
			field_name.size, field_name.original,
			parents.size, parents.data,
			field_name.size, field_name.original,
			size_type.size, size_type.lower,
			size_type.size, size_type.lower,

			parents.size, parents.data,
			field_name.size, field_name.original

			//field_name.size, field_name.original,
			//parents.size, parents.data,
			//field_name.size, field_name.original,

			//field_name.size, field_name.original,
			//field_name.size, field_name.original,
			//field_name.size, field_name.original,
			//field_name.size, field_name.original
		);
	}
}

void emit_field_stream_exit(String_Triplet size_type, char* endian_type, String_Triplet field_name)
{
	stream_write_string(&main_struct_stream,
		BUNDLE_STRING(
			"  } %.*s;\n"
		),
		field_name.size, field_name.original
	);

	stream_write_string(&packer_stream,
		BUNDLE_STRING(
			"      write_%.*s_%s_at(%.*s_size_ptr, (%.*s)(((uptr)arena->memory + arena->cursor) - ((uptr)%.*s_size_ptr + sizeof(%.*s))));\n"
			"      } // %.*s\n"
		),
		size_type.size, size_type.lower,
		endian_type,
		field_name.size, field_name.original,
		size_type.size, size_type.lower,
		field_name.size, field_name.original,
		size_type.size, size_type.lower,

		field_name.size, field_name.original
	);

	stream_write_string(&unpacker_stream,
		BUNDLE_STRING(
			"      }\n"
		));
}

//------------------------------------------------------------------------------------------------------------------------

Token token_build_error(Tokenizer* tokenizer, Buffer message)
{
	Token result{};
	result.path = tokenizer->path;
	result.kind = Token_Kind_Error;
	result.lexeme = message;
	result.line = tokenizer->line;
	result.column_begin = tokenizer->column_begin;

	return result;
}

Token token_build(Tokenizer* tokenizer, Token_Kind kind)
{
	Token result{};
	result.path = tokenizer->path;
	result.kind = kind;
	result.lexeme.data = (char*)tokenizer->cursor_begin;
	result.lexeme.size = tokenizer->cursor - tokenizer->cursor_begin;
	result.line = tokenizer->line;
	result.column_begin = tokenizer->column_begin;

	return result;
}

char tokenizer_peek(Tokenizer* tokenizer)
{

	return *tokenizer->cursor;
}

u32 tokenizer_is_at_end(Tokenizer* tokenizer)
{
	return tokenizer_peek(tokenizer) == '\0';
}

char tokenizer_char_peek(Tokenizer* tokenizer)
{
	if (tokenizer_is_at_end(tokenizer))
	{
		return '\0';
	}
	return tokenizer->cursor[1];
}

char tokenizer_advance(Tokenizer* tokenizer)
{
	tokenizer->cursor++;
	tokenizer->column++;
	return tokenizer->cursor[-1];
}

void tokenizer_whitespace_skip(Tokenizer* tokenizer)
{
	for (;;)
	{
		char c = tokenizer_peek(tokenizer);
		switch (c)
		{
		case ' ': // fallthrough
		case '\t': // fallthrough
		case '\r': // fallthrough
		case '?':
		{
			tokenizer_advance(tokenizer);
		} break;

		case '\n':
		{
			tokenizer->line++;
			tokenizer->column = 0;
			tokenizer_advance(tokenizer);
		} break;

		case '/':
		{
			if (tokenizer_char_peek(tokenizer) == '/')
			{
				while (tokenizer_peek(tokenizer) != '\n' && !tokenizer_is_at_end(tokenizer))
				{
					tokenizer_advance(tokenizer);
				}
			}
			else
			{
				return;
			}
		} break;

		default:
		{
			return;
		}
		}
	}
}

u32 memory_match(void* data_1, void* data_2, uptr size)
{
	for (uptr i = 0; i < size; i++)
	{
		if (*((u8*)data_1 + i) != *((u8*)data_2 + i))
		{
			return FALSE;
		}
	}
	return TRUE;
}

Token_Kind tokenizer_keyword_check(Tokenizer* tokenizer,
	uptr start,
	uptr size,
	const char* rest,
	Token_Kind kind)
{
	if ((uptr)tokenizer->cursor - (uptr)tokenizer->cursor_begin == start + size && memory_match(tokenizer->cursor_begin + start, const_cast<char*>(rest), size))
	{
		return kind;
	}

	return Token_Kind_Identifier;
}

Token tokenizer_number(Tokenizer* tokenizer)
{
	while (is_char_digit(tokenizer_peek(tokenizer)))
	{
		tokenizer_advance(tokenizer);
	}

	return token_build(tokenizer, Token_Kind_Number);
}

Token tokenizer_hex_number(Tokenizer* tokenizer)
{
	while (is_char_hex_digit(tokenizer_peek(tokenizer)))
	{
		tokenizer_advance(tokenizer);
	}

	return token_build(tokenizer, Token_Kind_Hex_Number);
}

// TODO(rhett): could make this smaller with Stream and BUNDLE_STRING
Token_Kind tokenizer_identifier_kind(Tokenizer* tokenizer)
{
	switch (*tokenizer->cursor_begin)
	{
	case 'g': { return tokenizer_keyword_check(tokenizer, 1, 4, "roup", Token_Kind_Group); }
	case 'p': { return tokenizer_keyword_check(tokenizer, 1, 5, "acket", Token_Kind_Packet); }
	case 'e': { return tokenizer_keyword_check(tokenizer, 1, 5, "ndian", Token_Kind_Endian); }
	case 'o': { return tokenizer_keyword_check(tokenizer, 1, 5, "pcode", Token_Kind_Opcode); }
	case 'l':
	{
		if (tokenizer->cursor - tokenizer->cursor_begin > 1)
		{
			switch (tokenizer->cursor_begin[1])
			{
			case 'i':
			{
				if (tokenizer->cursor - tokenizer->cursor_begin > 2)
				{
					switch (tokenizer->cursor_begin[2])
					{
					case 's': { return tokenizer_keyword_check(tokenizer, 3, 1, "t", Token_Kind_List); }
					case 't': { return tokenizer_keyword_check(tokenizer, 3, 3, "tle", Token_Kind_Little); }
					}
				}
			}
			}
		}
	}
	case 'c': { return tokenizer_keyword_check(tokenizer, 1, 3, "ase", Token_Kind_Case); }
	case 's':
	{
		if (tokenizer->cursor - tokenizer->cursor_begin > 1)
		{
			switch (tokenizer->cursor_begin[1])
			{
			case 'w': { return tokenizer_keyword_check(tokenizer, 2, 4, "itch", Token_Kind_Switch); }
			case 't':
			{
				if (tokenizer->cursor - tokenizer->cursor_begin > 3)
				{
					switch (tokenizer->cursor_begin[3])
					{
					case 'i': { return tokenizer_keyword_check(tokenizer, 4, 2, "ng", Token_Kind_String); }
					case 'e': { return tokenizer_keyword_check(tokenizer, 4, 2, "am", Token_Kind_Stream); }
					}
				}
			}
			}
		}
	}
	case 'b':
	{
		if (tokenizer->cursor - tokenizer->cursor_begin > 1)
		{
			switch (tokenizer->cursor_begin[1])
			{
			case 'i': { return tokenizer_keyword_check(tokenizer, 2, 1, "g", Token_Kind_Big); }
			case 'y': { return tokenizer_keyword_check(tokenizer, 2, 3, "tes", Token_Kind_Bytes); }
			case '8': { return tokenizer_keyword_check(tokenizer, 2, 0, "", Token_Kind_B8); }
			case '1': { return tokenizer_keyword_check(tokenizer, 2, 1, "6", Token_Kind_B16); }
			case '3': { return tokenizer_keyword_check(tokenizer, 2, 1, "2", Token_Kind_B32); }
			case '6': { return tokenizer_keyword_check(tokenizer, 2, 1, "4", Token_Kind_B64); }
			}
		}
	}
	case 'i':
	{
		if (tokenizer->cursor - tokenizer->cursor_begin > 1)
		{
			switch (tokenizer->cursor_begin[1])
			{
			case '8': { return tokenizer_keyword_check(tokenizer, 2, 0, "", Token_Kind_I8); }
			case '1': { return tokenizer_keyword_check(tokenizer, 2, 1, "6", Token_Kind_I16); }
			case '3': { return tokenizer_keyword_check(tokenizer, 2, 1, "2", Token_Kind_I32); }
			case '6': { return tokenizer_keyword_check(tokenizer, 2, 1, "4", Token_Kind_I64); }
			}
		}
	}
	case 'u':
	{
		if (tokenizer->cursor - tokenizer->cursor_begin > 1)
		{
			switch (tokenizer->cursor_begin[1])
			{
			case 'i': { return tokenizer_keyword_check(tokenizer, 2, 4, "nt2b", Token_Kind_Uint2b); }
			case '8': { return tokenizer_keyword_check(tokenizer, 2, 0, "", Token_Kind_U8); }
			case '1': { return tokenizer_keyword_check(tokenizer, 2, 1, "6", Token_Kind_U16); }
			case '3': { return tokenizer_keyword_check(tokenizer, 2, 1, "2", Token_Kind_U32); }
			case '6': { return tokenizer_keyword_check(tokenizer, 2, 1, "4", Token_Kind_U64); }
			}
		}
	}
	case 'f':
	{
		if (tokenizer->cursor - tokenizer->cursor_begin > 1)
		{
			switch (tokenizer->cursor_begin[1])
			{
			case '1': { return tokenizer_keyword_check(tokenizer, 2, 1, "6", Token_Kind_F16); }
			case '3': { return tokenizer_keyword_check(tokenizer, 2, 1, "2", Token_Kind_F32); }
			case '6': { return tokenizer_keyword_check(tokenizer, 2, 1, "4", Token_Kind_F64); }
			}
		}
	}
	case 'v':
	{
		if (tokenizer->cursor - tokenizer->cursor_begin > 1)
		{
			switch (tokenizer->cursor_begin[1])
			{
			case 'e':
			{
				if (tokenizer->cursor - tokenizer->cursor_begin > 2)
				{
					switch (tokenizer->cursor_begin[2])
					{
					case 'c':
					{
						if (tokenizer->cursor - tokenizer->cursor_begin > 3)
						{
							switch (tokenizer->cursor_begin[3])
							{
							case '3': { return tokenizer_keyword_check(tokenizer, 4, 0, "", Token_Kind_Vec3); }
							case '4': { return tokenizer_keyword_check(tokenizer, 4, 0, "", Token_Kind_Vec4); }
							}
						}
					}
					}
				}
			}
			}
		}
	}
	}

	return Token_Kind_Identifier;
}

Token tokenizer_identifier(Tokenizer* tokenizer)
{
	while (is_char_alpha(tokenizer_peek(tokenizer)) || is_char_digit(tokenizer_peek(tokenizer)))
	{
		tokenizer_advance(tokenizer);
	}

	return token_build(tokenizer, tokenizer_identifier_kind(tokenizer));
}

Token tokenizer_get_token(Tokenizer* tokenizer)
{
	tokenizer_whitespace_skip(tokenizer);
	tokenizer->cursor_begin = tokenizer->cursor;
	tokenizer->column_begin = tokenizer->column;

	if (tokenizer_is_at_end(tokenizer))
	{
		return token_build(tokenizer, Token_Kind_End_Of_Stream);
	}

	char c = tokenizer_advance(tokenizer);
	if (is_char_alpha(c))
	{
		return tokenizer_identifier(tokenizer);
	}

	if (c == '0')
	{
		if (tokenizer_peek(tokenizer) == 'x')
		{
			tokenizer_advance(tokenizer);
			return tokenizer_hex_number(tokenizer);
		}
	}
	if (is_char_digit(c))
	{
		return tokenizer_number(tokenizer);
	}

	switch (c)
	{
	case '{': return token_build(tokenizer, Token_Kind_Open_Brace);
	case '}': return token_build(tokenizer, Token_Kind_Closed_Brace);
	case ':': return token_build(tokenizer, Token_Kind_Colon);
	}

	return token_build_error(tokenizer, BUNDLE_STRING("Unexpected character"));
}

//------------------------------------------------------------------------------------------------------------------------

u8 char_to_number(char c)
{
	switch (c)
	{
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	case 'a': case 'A': return 10;
	case 'b': case 'B': return 11;
	case 'c': case 'C': return 12;
	case 'd': case 'D': return 13;
	case 'e': case 'E': return 14;
	case 'f': case 'F': return 15;
	}
	return 0xff;
}

u64 number_token_to_u64(Token token)
{
	u8 base = 0;
	u64 result = 0;

	uptr number_offset = 0;
	if (token.kind == Token_Kind_Number)
	{
		base = 10;
	}
	else if (token.kind == Token_Kind_Hex_Number)
	{
		base = 16;
		number_offset += 2;
	}

	for (uptr i = number_offset; i < token.lexeme.size; i++)
	{
		result *= base;
		result += char_to_number(token.lexeme.data[i]);
	}

	return result;
}

typedef struct Parser Parser;
struct Parser
{
	Tokenizer* tokenizer;
	Arena* arena;
	Token current;
	Token previous;
	u32 had_error;
	u32 panic_mode;

	Token_Kind endian_type;
	Parent_Stack parent_stack;
};

//#define PARSER_PRINTF(...)  if (!parser->panic_mode) { printf(__VA_ARGS__); }
#define PARSER_PRINTF(...)  

void parser_error_at(Parser* parser, Token token, const char* message)
{
	if (parser->panic_mode)
	{
		return;
	}
	parser->panic_mode = TRUE;
	fprintf(stderr, "%s(%lld:%lld): ", token.path, token.line, token.column_begin);
	if (token.kind == Token_Kind_End_Of_Stream)
	{
		fprintf(stderr, "Error at end - ");
	}
	else
	{
		fprintf(stderr, "Error at '%.*s' - ", (int)token.lexeme.size, token.lexeme.data);
	}

	fprintf(stderr, "%s\n", message);
	parser->had_error = TRUE;
	DebugBreak();
}

void parser_error_at_current(Parser* parser, const char* message)
{
	parser_error_at(parser, parser->current, message);
}

void parser_error(Parser* parser, const char* message)
{
	parser_error_at(parser, parser->previous, message);
}

void parser_advance(Parser* parser)
{
	parser->previous = parser->current;

	for (;;)
	{
		parser->current = tokenizer_get_token(parser->tokenizer);
		if (parser->current.kind != Token_Kind_Error)
		{
			break;
		}
		parser_error_at_current(parser, (char*)parser->current.lexeme.data);
	}
}

void parser_consume(Parser* parser, Token_Kind kind, const char* message)
{
	if (parser->current.kind == kind)
	{
		parser_advance(parser);
		return;
	}

	//UNUSED(message);
	//u8* message_buffer[256]{};
	//sprintf((u8*)&message_buffer, "Expected %.*s", global_token_names[kind].size, global_token_names[kind].data);
	parser_error_at_current(parser, message);
}

u32 parser_is_endian_type(Parser* parser)
{
	return parser->current.kind == Token_Kind_Little || parser->current.kind == Token_Kind_Big;
}

void parser_optional_endian(Parser* parser)
{
	if (parser->current.kind == Token_Kind_Endian)
	{
		parser_advance(parser);
		if (parser_is_endian_type(parser))
		{
			parser_advance(parser);
			parser->endian_type = parser->previous.kind;
		}
		else
		{
			parser_error_at_current(parser, "Expected little or big");
		}
	}
}

u32 parser_is_integer_literal(Parser* parser)
{
	return parser->current.kind == Token_Kind_Number || parser->current.kind == Token_Kind_Hex_Number;
}

void parser_consume_integer_literal(Parser* parser, const char* message)
{
	if (parser_is_integer_literal(parser))
	{
		parser_advance(parser);
		return;
	}

	parser_error_at_current(parser, message);
}

u32 parser_is_integer_type(Parser* parser)
{
	// TODO(rhett): not so sure we'll EVER use signed
	switch (parser->current.kind)
	{
	case Token_Kind_U8: // fallthrough
	case Token_Kind_U16: // fallthrough
	case Token_Kind_U32: // fallthrough
	case Token_Kind_U64: // fallthrough
	case Token_Kind_I8: // fallthrough
	case Token_Kind_I16: // fallthrough
	case Token_Kind_I32: // fallthrough
	case Token_Kind_I64:
	{
		return TRUE;
	}

	default:
	{
		return FALSE;
	}
	}
}

void parser_consume_integer_type(Parser* parser, const char* message)
{
	if (parser_is_integer_type(parser))
	{
		parser_advance(parser);
		return;
	}

	parser_error_at_current(parser, message);
}

u32 parser_is_field_type(Parser* parser)
{
	switch (parser->current.kind)
	{
	case Token_Kind_List: // fallthrough
	case Token_Kind_String: // fallthrough
	case Token_Kind_Bytes: // fallthrough
	case Token_Kind_Stream: // fallthrough
	case Token_Kind_Uint2b: // fallthrough
	case Token_Kind_Vec3: // fallthrough
	case Token_Kind_Vec4: // fallthrough
	case Token_Kind_Switch: // fallthrough
	case Token_Kind_U8: // fallthrough
	case Token_Kind_U16: // fallthrough
	case Token_Kind_U32: // fallthrough
	case Token_Kind_U64: // fallthrough
	case Token_Kind_I8: // fallthrough
	case Token_Kind_I16: // fallthrough
	case Token_Kind_I32: // fallthrough
	case Token_Kind_I64: // fallthrough
	case Token_Kind_B8: // fallthrough
	case Token_Kind_B16: // fallthrough
	case Token_Kind_B32: // fallthrough
	case Token_Kind_B64: // fallthrough
	case Token_Kind_F16: // fallthrough
	case Token_Kind_F32: // fallthrough
	case Token_Kind_F64:
	{
		return TRUE;
	}

	default:
	{
		return FALSE;
	}
	}
}

void parser_consume_field_type(Parser* parser, const char* message)
{
	if (parser_is_field_type(parser))
	{
		parser_advance(parser);
		return;
	}

	parser_error_at_current(parser, message);
}

void parser_field(Parser* parser);

void parser_field_simple(Parser* parser)
{
	uptr line = parser->previous.line;
	Token_Kind field_type = parser->previous.kind;
	parser_consume(parser, Token_Kind_Identifier, "Expected identifier for field");

	String_Triplet field_lexeme = string_triplet_build(parser->previous.lexeme, parser->arena);
	char* type_lexeme = string_copy_as_lower_zt(global_token_names[field_type], parser->arena);
	char* endian_lexeme = string_copy_as_lower_zt(global_token_names[parser->endian_type], parser->arena);
	emit_field_simple(line, parser->parent_stack, type_lexeme, endian_lexeme, field_lexeme, parser->arena);

	PARSER_PRINTF("%s %.*s\n", type_lexeme, (int)parser->previous.lexeme.size, parser->previous.lexeme.data);
}

void parser_field_f16(Parser* parser)
{
	uptr line = parser->previous.line;
	Token_Kind field_type = parser->previous.kind;
	parser_consume(parser, Token_Kind_Identifier, "Expected identifier for field");

	String_Triplet field_lexeme = string_triplet_build(parser->previous.lexeme, parser->arena);
	char* type_lexeme = string_copy_as_lower_zt(global_token_names[field_type], parser->arena);
	char* endian_lexeme = string_copy_as_lower_zt(global_token_names[parser->endian_type], parser->arena);
	emit_field_f16(line, parser->parent_stack, type_lexeme, endian_lexeme, field_lexeme, parser->arena);

	PARSER_PRINTF("%s %.*s\n", type_lexeme, (int)parser->previous.lexeme.size, parser->previous.lexeme.data);
}

void parser_field_vec3(Parser* parser)
{
	uptr line = parser->previous.line;
	parser_consume(parser, Token_Kind_Identifier, "Expected identifier for field");

	String_Triplet field_lexeme = string_triplet_build(parser->previous.lexeme, parser->arena);
	char* endian_lexeme = string_copy_as_lower_zt(global_token_names[parser->endian_type], parser->arena);
	emit_field_vec3(line, parser->parent_stack, endian_lexeme, field_lexeme, parser->arena);

	PARSER_PRINTF("vec3 %.*s\n", (int)parser->previous.lexeme.size, parser->previous.lexeme.data);
}

void parser_field_vec4(Parser* parser)
{
	uptr line = parser->previous.line;
	parser_consume(parser, Token_Kind_Identifier, "Expected identifier for field");

	String_Triplet field_lexeme = string_triplet_build(parser->previous.lexeme, parser->arena);
	char* endian_lexeme = string_copy_as_lower_zt(global_token_names[parser->endian_type], parser->arena);
	emit_field_vec4(line, parser->parent_stack, endian_lexeme, field_lexeme, parser->arena);

	PARSER_PRINTF("vec4 %.*s\n", (int)parser->previous.lexeme.size, parser->previous.lexeme.data);
}

void parser_field_uint2b(Parser* parser)
{
	uptr line = parser->previous.line;
	parser_consume(parser, Token_Kind_Identifier, "Expected identifier for field");

	String_Triplet field_lexeme = string_triplet_build(parser->previous.lexeme, parser->arena);
	char* endian_lexeme = string_copy_as_lower_zt(global_token_names[parser->endian_type], parser->arena);
	emit_field_uint2b(line, parser->parent_stack, endian_lexeme, field_lexeme, parser->arena);

	PARSER_PRINTF("uint2b %.*s\n", (int)parser->previous.lexeme.size, parser->previous.lexeme.data);
}

void parser_field_string(Parser* parser)
{
	uptr line = parser->previous.line;
	Token_Kind string_size_type = Token_Kind_U32;
	if (parser->current.kind == Token_Kind_Colon)
	{
		parser_advance(parser);
		// TODO(rhett): give string a fixed size option?
		parser_consume_integer_type(parser, "Expected string size type");
		string_size_type = parser->previous.kind;
	}

	parser_consume(parser, Token_Kind_Identifier, "Expected identifier for string");

	String_Triplet field_lexeme = string_triplet_build(parser->previous.lexeme, parser->arena);
	char* type_lexeme = string_copy_as_lower_zt(global_token_names[string_size_type], parser->arena);
	char* endian_lexeme = string_copy_as_lower_zt(global_token_names[parser->endian_type], parser->arena);
	emit_field_string(line, parser->parent_stack, type_lexeme, endian_lexeme, field_lexeme, parser->arena);


	PARSER_PRINTF("string:%s %.*s\n", global_token_names[string_size_type].data, (int)parser->previous.lexeme.size, parser->previous.lexeme.data);
}

void parser_field_bytes(Parser* parser)
{
	uptr line = parser->previous.line;
	Token_Kind bytes_size_type = Token_Kind_U32;
	if (parser->current.kind == Token_Kind_Colon)
	{
		parser_advance(parser);
		// TODO(rhett): give bytes a fixed size option?
		parser_consume_integer_type(parser, "Expected bytes size type");
		bytes_size_type = parser->previous.kind;
	}

	parser_consume(parser, Token_Kind_Identifier, "Expected identifier for bytes");

	String_Triplet field_lexeme = string_triplet_build(parser->previous.lexeme, parser->arena);
	char* type_lexeme = string_copy_as_lower_zt(global_token_names[bytes_size_type], parser->arena);
	char* endian_lexeme = string_copy_as_lower_zt(global_token_names[parser->endian_type], parser->arena);
	emit_field_bytes(line, parser->parent_stack, type_lexeme, endian_lexeme, field_lexeme, parser->arena);

	PARSER_PRINTF("bytes:%s %.*s\n", global_token_names[bytes_size_type].data, (int)parser->previous.lexeme.size, parser->previous.lexeme.data);
}

void parser_field_case(Parser* parser)
{
	parser_consume_integer_literal(parser, "case requires integer literal");
	u64 case_number = number_token_to_u64(parser->previous);
	parser_consume(parser, Token_Kind_Identifier, "expected identifier for case");

	PARSER_PRINTF("case %#llx(%llu) %.*s\n", case_number, case_number, (int)parser->previous.lexeme.size, parser->previous.lexeme.data);

	String_Triplet name_lexeme = string_triplet_build(parser->previous.lexeme, parser->arena);
	emit_field_case_enter(name_lexeme, case_number);
	parent_stack_push(&parser->parent_stack, buffer_from_array(name_lexeme.size, name_lexeme.original), FALSE);

	// TODO(rhett): might need to maintain a stack of parents and whether they're pointers
	u32 has_a_field = FALSE;
	while (parser->current.kind != Token_Kind_Case && parser_is_field_type(parser))
	{
		has_a_field = TRUE;
		parser_field(parser);
	}
	if (!has_a_field)
	{
		parser_error_at_current(parser, "Generated c code requires at least one field");
	}

	emit_field_case_exit(name_lexeme);
	parent_stack_pop(&parser->parent_stack);
}

void parser_field_switch(Parser* parser)
{
	uptr line = parser->previous.line;
	// TODO(rhett): has colon?
	Token_Kind case_type = Token_Kind_U32;
	if (parser->current.kind == Token_Kind_Colon)
	{
		parser_advance(parser);
		parser_consume_integer_type(parser, "Expected integer type for switch case type");
		case_type = parser->previous.kind;
	}
	parser_consume(parser, Token_Kind_Identifier, "Expected identifier for switch");
	PARSER_PRINTF("switch:%s %.*s\n", global_token_names[case_type].data, (int)parser->previous.lexeme.size, parser->previous.lexeme.data);

	String_Triplet name_lexeme = string_triplet_build(parser->previous.lexeme, parser->arena);
	char* case_type_lexeme = string_copy_as_lower_zt(global_token_names[case_type], parser->arena);
	char* endian_lexeme = string_copy_as_lower_zt(global_token_names[parser->endian_type], parser->arena);
	emit_field_switch_enter(line, parser->parent_stack, case_type_lexeme, endian_lexeme, name_lexeme, parser->arena);
	parent_stack_push(&parser->parent_stack, buffer_from_array(name_lexeme.size, name_lexeme.original), FALSE);


	parser_consume(parser, Token_Kind_Open_Brace, "Expected open brace for switch");
	PARSER_PRINTF("{\n");

	// TODO(rhett): need at least one case
	parser_consume(parser, Token_Kind_Case, "At least one case definition required");
	parser_field_case(parser);

	while (parser->current.kind == Token_Kind_Case)
	{
		parser_advance(parser);
		parser_field_case(parser);
	}

	emit_field_switch_exit(name_lexeme);

	parser_consume(parser, Token_Kind_Closed_Brace, "Expected closing brace for switch");
	parent_stack_pop(&parser->parent_stack);

	PARSER_PRINTF("}\n");
}

void parser_field_list(Parser* parser)
{
	uptr line = parser->previous.line;
	List_Kind list_kind = List_Kind_Variable;
	Token_Kind count_type = Token_Kind_U32;
	u64 count_literal = 0;

	if (parser->current.kind == Token_Kind_Colon)
	{
		parser_advance(parser);
		// NOTE(rhett): can be an integer type or literal
		if (parser_is_integer_type(parser))
		{
			parser_advance(parser);
			count_type = parser->previous.kind;
		}
		else if (parser_is_integer_literal(parser))
		{
			parser_advance(parser);
			list_kind = List_Kind_Literal;
			count_literal = number_token_to_u64(parser->previous);
		}
		else
		{
			parser_error_at_current(parser, "Expected type or integer literal for list count");
		}
	}
	parser_consume(parser, Token_Kind_Identifier, "Expected identifier for list");
	String_Triplet name_lexeme = string_triplet_build(parser->previous.lexeme, parser->arena);

	PARSER_PRINTF("list(%s):", global_list_names[list_kind].data);
	if (list_kind == List_Kind_Variable)
	{
		PARSER_PRINTF("%s ", global_token_names[count_type].data);
	}
	else if (list_kind == List_Kind_Literal)
	{
		PARSER_PRINTF("%#llx(%lld) ", count_literal, count_literal);
	}

	PARSER_PRINTF("%.*s\n", (int)parser->previous.lexeme.size, parser->previous.lexeme.data);

	parser_consume(parser, Token_Kind_Open_Brace, "Expected opening brace for list");
	PARSER_PRINTF("{\n");

	//String_Triplet count_type_lexeme = string_triplet_build(global_token_names[count_type], parser->arena);
	//String_Triplet field_lexeme = string_triplet_build(parser->previous.lexeme, parser->arena);
	char* type_lexeme = string_copy_as_lower_zt(global_token_names[count_type], parser->arena);
	char* endian_lexeme = string_copy_as_lower_zt(global_token_names[parser->endian_type], parser->arena);
	emit_field_list_enter(line, parser->parent_stack, type_lexeme, endian_lexeme, name_lexeme, list_kind, count_literal, parser->arena);
	char parent_string_iter[128]{};
	uptr parent_string_iter_size = sprintf(parent_string_iter,
		"%.*s[%.*s_iter]",
		(int)name_lexeme.size, name_lexeme.original,
		(int)name_lexeme.size, name_lexeme.original);
	parent_stack_push(&parser->parent_stack, buffer_from_array(parent_string_iter_size, parent_string_iter), FALSE);

	//emit_field_list_enter(count_type_lexeme, name_lexeme);

	u32 has_a_field = FALSE;
	while (parser_is_field_type(parser))
	{
		has_a_field = TRUE;
		parser_field(parser);
	}
	if (!has_a_field)
	{
		parser_error_at_current(parser, "Generated c++ code requires at least one field");
	}

	parser_consume(parser, Token_Kind_Closed_Brace, "Expected closing brace for list");
	PARSER_PRINTF("}\n");

	emit_field_list_exit(name_lexeme);
	parent_stack_pop(&parser->parent_stack);
}

void parser_field_stream(Parser* parser)
{
	uptr line = parser->previous.line;
	Token_Kind size_type = Token_Kind_U32;

	if (parser->current.kind == Token_Kind_Colon)
	{
		parser_advance(parser);
		parser_consume_integer_type(parser, "Expected integer type for stream size");
		size_type = parser->previous.kind;
	}

	parser_consume(parser, Token_Kind_Identifier, "Expected identifier for stream");
	String_Triplet name_lexeme = string_triplet_build(parser->previous.lexeme, parser->arena);
	String_Triplet size_type_lexeme = string_triplet_build(global_token_names[size_type], parser->arena);
	char* endian_lexeme = string_copy_as_lower_zt(global_token_names[parser->endian_type], parser->arena);
	emit_field_stream_enter(line, parser->parent_stack, size_type_lexeme, endian_lexeme, name_lexeme, parser->arena);
	parent_stack_push(&parser->parent_stack, parser->previous.lexeme, FALSE);

	PARSER_PRINTF("stream:%s %.*s\n", global_token_names[size_type].data, (int)parser->previous.lexeme.size, parser->previous.lexeme.data);

	parser_consume(parser, Token_Kind_Open_Brace, "Expected opening brace for stream");
	PARSER_PRINTF("{\n");

	u32 has_a_field = FALSE;
	while (parser_is_field_type(parser))
	{
		has_a_field = TRUE;
		parser_field(parser);
	}
	if (!has_a_field)
	{
		parser_error_at_current(parser, "Generated c++ code requires at least one field");
	}

	parser_consume(parser, Token_Kind_Closed_Brace, "Expected closing brace for stream");
	PARSER_PRINTF("}\n");

	emit_field_stream_exit(size_type_lexeme, endian_lexeme, name_lexeme);
	parent_stack_pop(&parser->parent_stack);
}

void parser_field(Parser* parser)
{
	parser_advance(parser);
	switch (parser->previous.kind)
	{
	case Token_Kind_Switch: { parser_field_switch(parser); } break;
	case Token_Kind_String: { parser_field_string(parser); } break;
	case Token_Kind_Bytes: { parser_field_bytes(parser); } break;
	case Token_Kind_Stream: { parser_field_stream(parser); } break;
	case Token_Kind_Uint2b: { parser_field_uint2b(parser); } break;
	case Token_Kind_Vec3: { parser_field_vec3(parser); } break;
	case Token_Kind_Vec4: { parser_field_vec4(parser); } break;
	case Token_Kind_List: { parser_field_list(parser); } break;
	case Token_Kind_F16: { parser_field_f16(parser); } break;

	case Token_Kind_U8: // fallthrough
	case Token_Kind_U16: // fallthrough
	case Token_Kind_U32: // fallthrough
	case Token_Kind_U64: // fallthrough
	case Token_Kind_I8: // fallthrough
	case Token_Kind_I16: // fallthrough
	case Token_Kind_I32: // fallthrough
	case Token_Kind_I64: // fallthrough
	case Token_Kind_B8: // fallthrough
	case Token_Kind_B16: // fallthrough
	case Token_Kind_B32: // fallthrough
	case Token_Kind_B64: // fallthrough
	case Token_Kind_F32: // fallthrough
	case Token_Kind_F64:
	{
		parser_field_simple(parser);
	} break;

	default:
	{
	}
	}
}

void parser_packet(Parser* parser, String_Triplet group_lexeme)
{
	Token_Kind opcode_type = Token_Kind_U8;
	parser_consume_integer_literal(parser, "Expected integer literal");
	u64 opcode_value = number_token_to_u64(parser->previous);
	if (parser->current.kind == Token_Kind_Colon)
	{
		parser_advance(parser);
		parser_consume_integer_type(parser, "Expected integer type after colon");
		opcode_type = parser->previous.kind;
	}

	u32 has_subcode = FALSE;
	Token_Kind subcode_type = Token_Kind_U8;
	u64 subcode_value = 0;
	if (parser_is_integer_literal(parser))
	{
		parser_advance(parser);
		subcode_value = number_token_to_u64(parser->previous);
		has_subcode = TRUE;

		if (parser->current.kind == Token_Kind_Colon)
		{
			parser_advance(parser);
			parser_consume_integer_type(parser, "Expected integer type after colon");
			subcode_type = parser->previous.kind;
		}
	}

	parser_consume(parser, Token_Kind_Identifier, "Expected packet name");

	PARSER_PRINTF("packet %#llx(%llu):%s ",
		opcode_value,
		opcode_value,
		global_token_names[opcode_type].data);

	if (has_subcode)
	{
		PARSER_PRINTF("%#llx(%llu):%s ", subcode_value, subcode_value, global_token_names[subcode_type].data);
	}

	PARSER_PRINTF("%.*s\n", (int)parser->previous.lexeme.size, parser->previous.lexeme.data);

	u32 will_generate_struct = parser_is_field_type(parser);
	String_Triplet packet_lexeme = string_triplet_build(parser->previous.lexeme, parser->arena);
	emit_packet_enter(group_lexeme,
		packet_lexeme,
		opcode_value,
		opcode_type,
		has_subcode,
		subcode_value,
		subcode_type,
		parser->endian_type,
		will_generate_struct,
		parser->arena);

	while (parser_is_field_type(parser))
	{
		parser_field(parser);
	}

	emit_packet_exit(will_generate_struct);
}

void parser_group(Parser* parser)
{
	//#error check for option endian
	parser_consume(parser, Token_Kind_Group, "Expected group keyword");
	parser_consume(parser, Token_Kind_Identifier, "Expected group name");

	PARSER_PRINTF("group %.*s\n", (int)parser->previous.lexeme.size, parser->previous.lexeme.data);

	//String_Triplet group_lexeme = string_triplet_build(parser->previous.lexeme, parser->arena);

	String_Triplet group_lexeme = string_triplet_build(parser->previous.lexeme, parser->arena);
	emit_group_enter(group_lexeme);

	while (parser->current.kind == Token_Kind_Packet)
	{
		parser_advance(parser);
		parser_packet(parser, group_lexeme);
	}

	emit_group_exit(group_lexeme);
}

//------------------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	UNUSED(global_token_names);

	Buffer input_file_buffer{};
	input_file_buffer.size = MB(10);
	input_file_buffer.data = (char*)calloc(input_file_buffer.size, 1);
	uptr file_size = load_file_to_buffer(argv[1], input_file_buffer);
	UNUSED(file_size);

	Tokenizer tokenizer{};
	tokenizer.path = argv[1];
	tokenizer.cursor_begin = (u8*)input_file_buffer.data;
	tokenizer.cursor = (u8*)input_file_buffer.data;
	tokenizer.line = 1;
	tokenizer.column = 1;
	tokenizer.column_begin = 1;

	Stream initial_stream = stream_allocate(MB(1));
	kind_stream = stream_allocate(MB(1));
	registered_ids_stream = stream_allocate(MB(1));
	packer_stream = stream_allocate(MB(1));
	unpacker_stream = stream_allocate(MB(1));
	main_struct_stream = stream_allocate(MB(1));

	Parser parser{};
	parser.tokenizer = &tokenizer;
	parser.arena = arena_alloc(MB(1));
	// TODO(rhett): persist endian? make scope based?
	parser.endian_type = Token_Kind_Big;
	parser.parent_stack = parent_stack_allocate(128, parser.arena);
	parent_stack_push(&parser.parent_stack, BUNDLE_STRING("packet"), TRUE);

	parser_advance(&parser);
	parser_optional_endian(&parser);
	parser_group(&parser);
	parser_consume(&parser, Token_Kind_End_Of_Stream, "Expected end of file");

	// TODO(rhett): make sure f16 doesn't get sizeof'ed
	stream_write_string(&initial_stream,
		BUNDLE_STRING(
			"#include \"../../binary.hpp\"\n\n"
			"typedef f32 f16;\n"
			"typedef u32 uint2b;\n"
			"\n"
			"struct Read_Uint2b_Result\n"
			"{\n"
			"  uptr size;\n"
			"  u32 value;\n"
			"};\n\n"
			"Read_Uint2b_Result read_uint2b_little_at(void* memory)\n"
			"{\n"
			"  u32 raw = read_u8_at(memory);\n"
			"  uptr size = raw & 3;\n"
			"  for (uptr i = 0; i < size; ++i)\n"
			"  {\n"
			"    raw |= read_u8_at((void*)((uptr)memory + i + 1)) << ((i + 1) * 8);\n"
			"  }\n"
			"  raw >>= 2;\n"
			"\n"
			"  Read_Uint2b_Result result{};\n"
			"  result.size = size + 1;\n"
			"  result.value = raw;\n"
			"\n"
			"  return result;\n"
			"}\n"
			"\n"
			"uptr write_uint2b_little_at(void* memory, u32 value)\n"
			"{\n"
			"  value <<= 2;\n"
			"  uptr size = 0;\n"
			"  if (value > 0xffffff)\n"
			"  {\n"
			"    size = 3;\n"
			"  }\n"
			"  else if (value > 0xffff)\n"
			"  {\n"
			"    size = 2;\n"
			"  }\n"
			"  else if (value > 0xff)\n"
			"  {\n"
			"    size = 1;\n"
			"  }\n"
			"  value |= size;\n"
			"\n"
			"  write_u32_little_at(memory, value);\n"
			"  return size + 1;\n"
			"}\n"
			"\n"
			"u32 read_uint2b_little(Stream* stream)\n"
			"{\n"
			"  Read_Uint2b_Result result = read_uint2b_little_at(stream->buffer.data + stream->cursor);\n"
			"  stream->cursor += result.size;\n"
			"  return result.value;\n"
			"}\n"
			"\n"
			"void push_uint2b_little(Arena* arena, u32 value)\n"
			"{\n"
			"  void* at = arena_push_size(arena, 4);\n"
			"  uptr bytes_written = write_uint2b_little_at(at, value);\n"
			"  arena->cursor -= 4 - bytes_written;\n"
			"}\n\n"
		));

	FILE* generated_file = fopen(argv[2], "w");
	fwrite(initial_stream.buffer.data, 1, initial_stream.cursor, generated_file);
	fwrite(kind_stream.buffer.data, 1, kind_stream.cursor, generated_file);
	fwrite(registered_ids_stream.buffer.data, 1, registered_ids_stream.cursor, generated_file);
	fwrite(main_struct_stream.buffer.data, 1, main_struct_stream.cursor, generated_file);
	fwrite(packer_stream.buffer.data, 1, packer_stream.cursor, generated_file);
	fwrite(unpacker_stream.buffer.data, 1, unpacker_stream.cursor, generated_file);
	//fflush(generated_file);
	fclose(generated_file);

	//getchar();
	return 0;
}