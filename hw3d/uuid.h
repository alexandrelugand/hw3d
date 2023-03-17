#pragma once

#pragma comment(lib, "Rpcrt4.lib")

namespace Uuid
{
	UUID static New()
	{
		UUID uuid;
		UuidCreate(&uuid);
		return uuid;
	}

	UUID static ToGuid(const std::string& str)
	{
		GUID uuid;
		sscanf_s(str.c_str(),
		         "{%8x-%4hx-%4hx-%2hhx%2hhx-%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx}",
		         &uuid.Data1, &uuid.Data2, &uuid.Data3,
		         &uuid.Data4[0], &uuid.Data4[1], &uuid.Data4[2], &uuid.Data4[3],
		         &uuid.Data4[4], &uuid.Data4[5], &uuid.Data4[6], &uuid.Data4[7]);

		return uuid;
	}

	std::string static ToString(UUID uuid)
	{
		char guid_cstr[39];
		snprintf(guid_cstr, sizeof(guid_cstr),
		         "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
		         uuid.Data1, uuid.Data2, uuid.Data3,
		         uuid.Data4[0], uuid.Data4[1], uuid.Data4[2], uuid.Data4[3],
		         uuid.Data4[4], uuid.Data4[5], uuid.Data4[6], uuid.Data4[7]);

		return std::string(guid_cstr);
	}
}
