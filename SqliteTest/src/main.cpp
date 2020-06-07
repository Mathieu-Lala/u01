#include <chrono>
#include <iostream>
#include <functional>
#include <string_view>
#include <thread>

#include <sqlite3.h>

namespace my_sqlite3 {

class Connection {
public:

    Connection(const std::string_view path)
    {
        if (!!sqlite3_open(path.data(), &this->m_db)) {
            auto msg = sqlite3_errmsg(this->m_db);
            std::cout << "sqlite3: Can't open database: " << msg << "\n";
            sqlite3_free(static_cast<void *>(const_cast<char *>(msg)));
        }
    }

    auto is_valid() const noexcept -> bool { return !!m_db; }

    using QueryCallback = sqlite3_callback;

    auto execute(
        const std::string_view query,
        const QueryCallback &callback = nullptr,
        void *arguments = nullptr)
    {
        if (char *error; sqlite3_exec(this->m_db, query.data(), callback, arguments, &error)) {
            std::cout << "sqlite3: Query error: " << error << "\n";
            sqlite3_free(static_cast<void *>(error));
        }
    }

    ~Connection()
    {
        if (sqlite3_close(this->m_db) != SQLITE_OK)
            std::cout << "sqlite3: Error disconnecting\n";
    }

private:

    sqlite3 *m_db{ nullptr };

};

}; // namespace my_sqlite3

constexpr auto QUERY_CREATE_TABLE =
"CREATE TABLE COMPANY("  \
  "ID INT PRIMARY KEY     NOT NULL," \
  "NAME           TEXT    NOT NULL," \
  "AGE            INT     NOT NULL," \
  "ADDRESS        CHAR(50)," \
  "SALARY         REAL"   \
");";

constexpr auto QUERY_POPULATE_TABLE =
"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "     \
  "VALUES (1, 'Paul', 32, 'California', 20000.00 ); "   \
"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "     \
  "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "       \
"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"      \
  "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );"       \
"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"      \
  "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00"       \
");";

constexpr auto QUERY_1 =
"UPDATE COMPANY set SALARY = 25000.00 where ID=1 ; SELECT * from COMPANY";

constexpr auto QUERY_2 =
"DELETE from COMPANY where ID=2 ; SELECT * from COMPANY";

const auto dump_output = [](void *arguments, int argc, char **argv, char **colomnName)
{
    std::cout << "arguments: " << arguments << "\n";

    for (int i = 0; i < argc; i++)
        std::printf("%s = %s\n", colomnName[i], argv[i] ?: "NULL");
    std::printf("\n");
    return 0;
};

auto main(int ac, char **av) -> int
{
    my_sqlite3::Connection con(ac == 1 ? "build/default.db" : av[1]);

    if (!con.is_valid()) return 1;

    con.execute(QUERY_CREATE_TABLE);
    con.execute(QUERY_POPULATE_TABLE);
    con.execute(QUERY_1, dump_output);
    con.execute(QUERY_2, dump_output);

    return 0;
}
