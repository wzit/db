
#include "bindable.h"
#include "exception.h"
#include "log.h"

namespace rj
{
    namespace db
    {
#ifdef ENHANCED_PARAMETER_MAPPING
        // these ugly beasts will find parameters not in quotes
        std::regex bindable::param_regex("([@:]\\w+|\\$([0-9]+)|\\?)(?=(?:[^\"']|[\"'][^\"']*[\"'])*$)");
        std::regex bindable::index_regex("(\\$([0-9]+)|\\?)(?=(?:[^\"']|[\"'][^\"']*[\"'])*$)");
        std::regex bindable::named_regex("([@:]\\w+)(?=(?:[^\"']|[\"'][^\"']*[\"'])*$)");
#endif

        bindable &bindable::bind_value(size_t index, const sql_value &value)
        {
            switch (value.type()) {
                case variant::NULLTYPE:
                    bind(index, sql_null);
                    break;
                case variant::CHAR:
                case variant::WCHAR:
                case variant::BOOL:
                case variant::NUMBER:
                    if (value.size() <= sizeof(int)) {
                        bind(index, value.to_int());
                    } else {
                        bind(index, value.to_llong());
                    }
                    break;
                case variant::UNUMBER:
                    bind(index, value.to_llong());
                    break;
                case variant::REAL:
                    if (value.size() <= sizeof(float)) {
                        bind(index, value.to_float());
                    } else {
                        bind(index, value.to_double());
                    }
                    break;
                case variant::STRING:
                    bind(index, value.to_string());
                    break;
                case variant::WSTRING:
                    bind(index, value.to_wstring());
                    break;
                case variant::BINARY:
                    bind(index, value.to_binary());
                    break;
                case variant::COMPLEX:
                    if (value.is_time()) {
                        bind(index, value.to_time());
                    } else {
                        throw binding_error("unknown custom type in binding");
                    }
                    break;
            }
            return *this;
        }

        bindable &bindable::bind(const std::vector<sql_value> &values, size_t start_index)
        {
            size_t index = start_index;
            for (auto &value : values) {
                bind_value(index++, value);
            }
            return *this;
        }

        bindable &bindable::bind(const std::unordered_map<std::string, sql_value> &values)
        {
            for (auto &pair : values) {
                bind(pair.first, pair.second);
            }
            return *this;
        }
    }
}
