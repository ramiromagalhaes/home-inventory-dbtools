#include <iostream>
#include <sstream>
#include <string>
#include <leveldb/db.h>
#include <leveldb/options.h>

using namespace std;



void print(leveldb::DB *db) {
    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        std::stringstream ss(it->value().ToString());
        int qtd;
        std::string nome;
        std::string classe;
        ss >> qtd;
        std::getline(ss, nome, '\n');
        ss >> classe;
        std::cout << it->key().ToString() << ": "  << qtd << "; " << nome << "; " << classe << std::endl;
    }
    assert(it->status().ok());  // Check for any errors found during the scan
    delete it;
}



int main(int argc, char *argv[])
{
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::DB *db;

    {
        leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);

        if ( !status.ok() ) {
            throw 2; //TODO fixme
        }
    }

    if (argc != 5) {
        print(db);
        return 0;
    }

    {
        std::string id = argv[1];
        std::string nome = argv[2];
        std::string classe = argv[3];
        std::string qtd = argv[4];
        std::stringstream ss;
        ss << qtd << nome << std::endl << classe;
        std::string reg = ss.str();

        leveldb::Slice key = id;
        leveldb::Status status = db->Put(leveldb::WriteOptions(), key, reg);
        if ( !status.ok() ) {
            throw 3; //TODO fixme
        }
    }

    return 0;
}
