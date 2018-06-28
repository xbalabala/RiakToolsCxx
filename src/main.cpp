#include <iostream>
#include <sstream>

#include <leveldb/db.h>
#include <leveldb/filter_policy.h>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

int main(int argc, char* argv[]) {
    //basho, see https://github.com/hw-dwalter/leveldb/blob/develop/README#L42
    leveldb::Options options;

    options.filter_policy = leveldb::NewBloomFilterPolicy(16);
    options.write_buffer_size = 62914560;  // 60Mbytes
    // options.total_leveldb_mem = 2684354560; // 2.5Gbytes (details below)
    // options.env = leveldb::Env::Default();

    fs::path p(fs::current_path());
    if (argc > 1) {
        p = fs::system_complete(argv[1]);
    } else {
        std::cout << "usage:   " << argv[0] << " [path]" << std::endl;
        return 0;
    }

    fs::directory_iterator end_iter;
    for (fs::directory_iterator dir_itr(p);
         dir_itr != end_iter;
         ++dir_itr) {
        if (fs::is_directory(dir_itr->status())) {
            std::cout << dir_itr->path() << " [directory]\n";

            leveldb::DB *db;

            // Open the database with the specified "name".
            // Stores a pointer to a heap-allocated database in *dbptr and returns
            // OK on success.
            // Stores NULL in *dbptr and returns a non-OK status on error.
            // Caller should delete *dbptr when it is no longer needed.
            leveldb::Status status = leveldb::DB::Open(options, dir_itr->path().c_str(), &db);
            if (status.ok()) {
                std::cout << "compacting..." << std::endl;

                // Compact the underlying storage for the key range [*begin,*end].
                // In particular, deleted and overwritten versions are discarded,
                // and the data is rearranged to reduce the cost of operations
                // needed to access the data.  This operation should typically only
                // be invoked by users who understand the underlying implementation.
                //
                // begin==NULL is treated as a key before all keys in the database.
                // end==NULL is treated as a key after all keys in the database.
                // Therefore the following call will compact the entire database:
                //    db->CompactRange(NULL, NULL);
                db->CompactRange(NULL, NULL);
                delete db;

                std::cout << "done" << std::endl;
            } else {
                std::stringstream msg;
                msg << "Unable to open " << dir_itr->path() << ", error: " << status.ToString();
                throw std::runtime_error(msg.str());
            }
        }
    }

    return 0;
}
