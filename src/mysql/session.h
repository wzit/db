/*!
 * @file db.h
 * a mysql specific database
 */
#ifndef RJ_DB_MYSQL_SESSION_H
#define RJ_DB_MYSQL_SESSION_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_LIBMYSQLCLIENT

#include <mysql/mysql.h>
#include "../session.h"
#include "../session_factory.h"

namespace rj
{
    namespace db
    {
        struct sqldb;

        namespace mysql
        {
            class factory : public session_factory
            {
               public:
                std::shared_ptr<rj::db::session_impl> create(const uri &uri);
            };

            /*!
             * a mysql specific implementation of a database
             */
            class session : public rj::db::session_impl, public std::enable_shared_from_this<session>
            {
                friend sqldb;
                friend class resultset;
                friend class statement;
                friend class factory;

               protected:
                std::shared_ptr<MYSQL> db_;

               public:
                /*!
                 * default constructor takes a uri to connect to
                 * @param connInfo the uri connection info
                 */
                session(const uri &connInfo);

                /* boilerplate */
                session(const session &other) = delete;
                session(session &&other);
                session &operator=(const session &other) = delete;
                session &operator=(session &&other);
                virtual ~session();

                /* sqldb overrides */
                bool is_open() const;
                void open();
                void close();
                long long last_insert_id() const;
                int last_number_of_changes() const;
                std::string last_error() const;
                std::shared_ptr<resultset_impl> query(const std::string &sql);
                bool execute(const std::string &sql);
                std::shared_ptr<statement_type> create_statement();
                std::shared_ptr<transaction_impl> create_transaction() const;
                void query_schema(const std::string &dbName, const std::string &tablename, std::vector<column_definition> &columns);
            };
        }
    }
}

#endif

#endif
