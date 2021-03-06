
#include <bandit/bandit.h>
#include "../db.test.h"
#include "postgres/statement.h"

#ifdef HAVE_LIBPQ

using namespace bandit;

using namespace std;

using namespace rj::db;

go_bandit([]() {

    describe("postgres statement", []() {
        before_each([]() { setup_current_session(); });

        after_each([]() { teardown_current_session(); });


        it("is movable", []() {
            postgres::statement stmt(dynamic_pointer_cast<postgres::session>(current_session->impl()));

            stmt.prepare("select * from users");

            AssertThat(stmt.is_valid(), IsTrue());

            postgres::statement s2(std::move(stmt));

            AssertThat(s2.is_valid(), IsTrue());

            AssertThat(stmt.is_valid(), IsFalse());

            postgres::statement s3(dynamic_pointer_cast<postgres::session>(current_session->impl()));

            AssertThat(s3.is_valid(), IsFalse());

            s3 = std::move(s2);

            AssertThat(s3.is_valid(), IsTrue());

            AssertThat(s2.is_valid(), IsFalse());
        });

        it("throws exceptions", []() {
            auto session = sqldb::create_session("postgres://ssssss/tttttt");

            postgres::statement stmt(dynamic_pointer_cast<postgres::session>(session->impl()));

            AssertThrows(database_exception, stmt.prepare("select * from users"));
        });

        it("can reset", []() {
            postgres::statement stmt(dynamic_pointer_cast<postgres::session>(current_session->impl()));

            stmt.prepare("select * from users");

            AssertThat(stmt.is_valid(), IsTrue());

            stmt.reset();

            stmt.prepare("select id, first_name from users where id > 0");

            AssertThat(stmt.is_valid(), IsTrue());
        });

    });

});

#endif
