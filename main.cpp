#include <iostream>
#include "sqlite_orm/sqlite_orm.h"

struct NormalTable {
    int id;
    std::string text;
    int otherValue;
};

struct SearchTable {
    int normal_table_id;
    std::string text;
};

inline auto initStorage(const std::string &databaseFilePath) {
    return sqlite_orm::make_storage(
        databaseFilePath,
        sqlite_orm::make_table("normal_table",
                               sqlite_orm::make_column("id", &NormalTable::id,
                                                       sqlite_orm::primary_key().autoincrement()),
                               sqlite_orm::make_column("path", &NormalTable::text),
                               sqlite_orm::make_column("other_value", &NormalTable::otherValue)),

        sqlite_orm::make_virtual_table(
            "search_table",
            sqlite_orm::using_fts5(
                sqlite_orm::make_column("text", &SearchTable::text),
                sqlite_orm::make_column("normal_table_id", &SearchTable::normal_table_id)))
    );
}

void goRaw(sqlite3 *rawDb) {
    sqlite3_stmt *statement = nullptr;
    int res = sqlite3_prepare_v2(rawDb,
                                 "SELECT * FROM normal_table where id in (SELECT normal_table_id FROM search_table WHERE text MATCH ?)",
                                 -1,
                                 &statement,
                                 nullptr);

    sqlite3_bind_text(statement, 1, "Some Text", -1, SQLITE_STATIC);


    assert(res == SQLITE_OK);

    while (SQLITE_ROW == (res = sqlite3_step(statement))) {
        for (int i = 0; i < sqlite3_column_count(statement); i++) {
            auto data = (const char *) sqlite3_column_text(statement, i); // text in ith column
            std::cout << " | " << std::setw(10) << (data ? data : "null");
        }
        std::cout << std::endl;
    }

    sqlite3_finalize(statement);
}

int main() {
    auto storage = initStorage("/Users/conrad/teststorage.dbs");

    storage.sync_schema();
    storage.open_forever();

    sqlite3 *rawDb = nullptr;
    storage.on_open = [&rawDb](auto *rawSQLite) {
        rawDb = rawSQLite;
    };

    storage.open_forever();

    auto rows = storage.iterate<NormalTable>(
        sqlite_orm::where(sqlite_orm::eq(&NormalTable::id,
                                         sqlite_orm::select(&SearchTable::normal_table_id,
                                                            sqlite_orm::where(
                                                                sqlite_orm::match<SearchTable>("Some Text"))))

        )


    );


    auto s = sqlite_orm::select(
        sqlite_orm::columns(&NormalTable::otherValue),

        sqlite_orm::where(sqlite_orm::eq(&NormalTable::id,
                                         sqlite_orm::select(&SearchTable::normal_table_id,
                                                            sqlite_orm::where(
                                                                sqlite_orm::match<SearchTable>("Some Text"))))

        )


    );
    std::string sql = storage.dump(s);

    std::cout << sql << std::endl;
    for (const auto &row: rows) {
    }

    return 0;
}
