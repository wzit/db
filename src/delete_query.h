#ifndef RJ_DB_DELETE_QUERY_H
#define RJ_DB_DELETE_QUERY_H

#include "modify_query.h"
#include "where_clause.h"

namespace rj
{
    namespace db
    {
        /*!
         * a query to delete from a table
         */
        class delete_query : public modify_query
        {
           public:
            using modify_query::modify_query;

            delete_query(const std::shared_ptr<rj::db::session> &session, const std::string &tableName);

            delete_query(const std::shared_ptr<schema> &schema);

            /* boilerplate */
            delete_query(const delete_query &other);
            delete_query(delete_query &&other);
            virtual ~delete_query();
            delete_query &operator=(const delete_query &other);
            delete_query &operator=(delete_query &&other);

            /*!
             * set the table to insert into
             * @see modify_query::table_name
             * @param  tableName the table name
             * @return           a reference to this instance
             */
            delete_query &from(const std::string &tableName);

            /*!
             * get the table name being inserted into
             * @see modify_query::table_name
             * @return the table name
             */
            std::string from() const;

            /*!
             * @return the string/sql representation of this query
             */
            std::string to_string() const;

            /*!
             * sets the where clause for the update query
             * @param value the where clause to set
             */
            delete_query &where(const where_clause &value);

            /*!
             * sets the where clause and binds a list of values
             * @param value the where clause to set
             * @param args a variadic list of indexed bind values
             * @return a reference to this instance
             */
            template <typename... List>
            delete_query &where(const where_clause &value, const List &... args)
            {
                where(value);
                bind_all(args...);
                return *this;
            }

            /*!
             * @param value the where sql/string to set
             */
            where_clause &where(const std::string &value);

            /*!
             * sets the where clause and binds a list of values
             * @param value the sql where clause string
             * @param args the variadic list of indexed bind values
             * @return a reference to this instance
             */
            template <typename... List>
            delete_query &where(const std::string &value, const List &... args)
            {
                where(value);
                bind_all(args...);
                return *this;
            }

            /*!
             * tests if this query is valid
             * @return true if valid
             */
            bool is_valid() const;

           private:
            where_clause where_;
            std::string tableName_;
        };
    }
}

#endif
