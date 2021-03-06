#include <bandit/bandit.h>
#include "db.test.h"
#include "resultset.h"

using namespace bandit;

using namespace std;

using namespace rj::db;

go_bandit([]() {

    describe("resultset", []() {

        before_each([&]() {
            setup_current_session();

            user user1;
            user user2;

            user1.set("first_name", "Bryan");
            user1.set("last_name", "Jenkins");

            user1.save();

            user2.set("first_name", "Mark");
            user2.set("last_name", "Smith");

            user2.save();
        });

        after_each([]() { teardown_current_session(); });

        it("is movable", []() {
            auto rs = current_session->query("select * from users");

            Assert::That(rs.is_valid(), Equals(true));

            resultset rs2(std::move(rs));

            Assert::That(rs2.is_valid(), Equals(true));

            Assert::That(rs.is_valid(), Equals(false));
        });

        it("has a current row", []() {
            select_query q(current_session);

            auto rs = q.from("users").execute();

            auto i = rs.begin();

            auto row = rs.current_row();

            Assert::That(row.is_valid(), Equals(true));

            auto rowCol = row[0].to_value().to_string();

            Assert::That(rowCol.empty(), Equals(false));

            Assert::That(rowCol, Equals(i->column(0).to_value().to_string()));

        });

        it("can use for each", []() {
            select_query q(current_session);

            auto rs = q.from("users").execute();

            rs.for_each([](const row& row) {
                AssertThat(row.is_valid(), IsTrue());

                row.for_each([](const column& c) { AssertThat(c.is_valid(), IsTrue()); });
            });
        });

        it("can be reset", []() {
            select_query q(current_session);

            auto rs = q.from("users").execute();

            Assert::That(rs.size() == 2, IsTrue());

            auto i = rs.begin();

            Assert::That(i != rs.end(), IsTrue());

            Assert::That(i->column(1).to_value(), Equals("Bryan"));

            ++i;

            Assert::That(i != rs.end(), IsTrue());

            Assert::That(i->column(1).to_value(), Equals("Mark"));

            rs.reset();

            if (rs.next()) Assert::That(rs.current_row().column(1).to_value(), Equals("Bryan"));

        });

        it("can construct iterators", []() {
            select_query q(current_session);

            auto rs = q.from("users").execute();

            auto i = rs.begin();

            auto i2(i);

            Assert::That(i2 != rs.end(), Equals(true));
            Assert::That(i != rs.end(), Equals(true));

            Assert::That(i == i2, Equals(true));

            auto i3(std::move(i));

            Assert::That(i == rs.end(), Equals(true));

            Assert::That(i3 == i2, Equals(true));

        });

        it("can operate on iterators", []() {
            select_query q(current_session);

            auto rs = q.from("users").execute();

            auto i = rs.end();

            i++;

            Assert::That(i == rs.end(), Equals(true));

            i = rs.begin();

            ++i;

            Assert::That(i != rs.end(), Equals(true));

            auto j = i + 1;

            Assert::That(j == rs.end(), Equals(true));

            Assert::That(i < j, Equals(true));

            Assert::That(j > i, Equals(true));

            Assert::That(i <= j, Equals(true));

            Assert::That(j >= i, Equals(true));

            i += 1;

            Assert::That(i == rs.end(), Equals(true));

            Assert::That(i <= j, Equals(true));

            Assert::That(j >= i, Equals(true));
        });
    });

});
