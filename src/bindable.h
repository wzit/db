/*!
 * @file bindable.h
 * An interface for data binding
 */
#ifndef RJ_DB_BINDABLE_H
#define RJ_DB_BINDABLE_H

#ifdef ENHANCED_PARAMETER_MAPPING
#include <regex>
#endif

#include <set>
#include <unordered_map>
#include <vector>

#include "exception.h"
#include "sql_value.h"

namespace rj
{
    namespace db
    {
        /*!
         * represents something that can have a sql value binded to it
         */
        class bindable
        {
           protected:
            /*!
             * bind_all override for one sql_value parameter
             * @param index the index of the binding
             * @param value the value to bind
             * @return a reference to this instance
             */
            template <typename T>
            bindable &bind_list(size_t index, const T &value)
            {
                return bind_value(index, value);
            }

            /*!
             * bind a list of a values, using the order of values as the index
             * @param index the initial index for the list
             * @param value the first value in the list
             * @param argv the remaining values
             * @return a reference to this instance
             */
            template <typename T, typename... List>
            bindable &bind_list(size_t index, const T &value, const List &... argv)
            {
                bind_value(index, value);
                bind_list(index + 1, argv...);
                return *this;
            }

           public:
#ifdef ENHANCED_PARAMETER_MAPPING
            static std::regex param_regex;
            static std::regex index_regex;
            static std::regex named_regex;
#endif
            template <typename T, typename... List>
            bindable &bind_all(const T &value, const List &... argv)
            {
                return bind_list(1, value, argv...);
            }

            /*!
             * Binds a sql_value using the other bind methods
             * @param index the index of the binding
             * @param value the value of the binding
             * @return a reference to this instance
             */
            bindable &bind_value(size_t index, const sql_value &value);

            /*!
             * Binds a vector of values by index
             * @param values the vector of values
             * @param start_index the starting index for the values
             * @return a reference to this instance
             */
            bindable &bind(const std::vector<sql_value> &values, size_t start_index = 1);

            /*!
             * Binds a map of named parameters
             * @param values the map of values
             * @return a reference to this instance
             */
            bindable &bind(const std::unordered_map<std::string, sql_value> &values);

            /*!
             * binds an integer value
             * @param  index the index of the binding
             * @param  value the value to bind
             * @return       a reference to this instance
             */
            virtual bindable &bind(size_t index, int value) = 0;

            /*!
             * binds an unsigned integer value
             * @param  index the index of the binding
             * @param  value the value to bind
             * @return       a reference to this instance
             */
            virtual bindable &bind(size_t index, unsigned value) = 0;

            /*!
             * binds a long long value
             * @param  index the index of the binding
             * @param  value the value to bind
             * @return       a reference to this instance
             */
            virtual bindable &bind(size_t index, long long value) = 0;

            /*!
             * binds an unsigned long long value
             * @param  index the index of the binding
             * @param  value the value to bind
             * @return       a reference to this instance
             */
            virtual bindable &bind(size_t index, unsigned long long value) = 0;

            /*!
             * binds a floating point value
             * @param  index the index of the binding
             * @param  value the value to bind
             * @return       a reference to this instance
             */
            virtual bindable &bind(size_t index, float value) = 0;

            /*!
             * binds a floating point value
             * @param  index the index of the binding
             * @param  value the value to bind
             * @return       a reference to this instance
             */
            virtual bindable &bind(size_t index, double value) = 0;

            /*!
             * binds a string value
             * @param  index the index of the binding
             * @param  value the value to bind
             * @param  len   the length of the binding (default: -1)
             * @return       a reference to this instance
             */
            virtual bindable &bind(size_t index, const std::string &value, int len = -1) = 0;

            /*!
             * binds a wide string value
             * @param  index the index of the binding
             * @param  value the value to bind
             * @param  len   the length of the binding (default: -1)
             * @return       a reference to this instance
             */
            virtual bindable &bind(size_t index, const std::wstring &value, int len = -1) = 0;

            /*!
             * binds a blob value
             * @param  index the index of the binding
             * @param  value the value to bind
             * @return       a reference to this instance
             */
            virtual bindable &bind(size_t index, const sql_blob &value) = 0;

            /*!
             * binds a null value
             * @param  index the index of the binding
             * @param  value the null value to bind
             * @return       a reference to this instance
             */
            virtual bindable &bind(size_t index, const sql_null_type &value) = 0;

            /*!
             * binds a timestamp
             * @param  index the index of the binding
             * @param  time  the value to bind
             * @return       a reference to this instance
             */
            virtual bindable &bind(size_t index, const sql_time &time) = 0;

            /*!
             * bind a named parameter
             * @param name the name of the parameter
             * @param value the value to bind
             * @return a reference to this instance
             */
            virtual bindable &bind(const std::string &name, const sql_value &value) = 0;
        };
    }
}

#endif
