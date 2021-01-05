
#pragma once

#include <rapidjson/prettywriter.h> // for stringify JSON
#include <unordered_map>

namespace Plutus
{
    using namespace rapidjson;

    class Serializer
    {
    public:
        Serializer() : sb(), writer(sb) {}
        template <typename T>
        inline void WriteMap(const char *name, std::unordered_map<std::string, T> map)
        {
            writer.String(name);
            writer.StartArray();
            for (auto item : map)
            {
                writer.String(item.first.c_str());
            }
            writer.EndArray();
        }
        PrettyWriter<StringBuffer> *getWriter() { return &writer; }
        const char *getString() { return sb.GetString(); };

    private:
        StringBuffer sb;
        PrettyWriter<StringBuffer> writer;
    };

} // namespace Plutus