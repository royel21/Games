#pragma once

#include <rapidjson/prettywriter.h> // for stringify JSON

namespace Plutus
{
	using namespace rapidjson;

	class Serializer
	{
	public:
		StringBuffer sb;
		PrettyWriter<StringBuffer> writer;

		Serializer() : sb(), writer(sb) {}

		PrettyWriter<StringBuffer>* getWriter() { return &writer; }
	};
}