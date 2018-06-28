https://helmundwalter.de/en/blog/riak-compact-eleveldb-tombstones-and-reclaim-disk-space/

In short, there is an c++ function in leveldb that is used to compact the underlying storage. The function is called `CompactRange`.

In particular, deleted and overwritten versions are discarded, and the data is rearranged to reduce the cost of operations needed to access the data.

This function does not exists in the erlang code that uses this c++ library. This means that we needed to build an standalone tool that calls this library function on all leveldb files in Riak. Drawback of this is, that your Riak server has to be offline while running such an 3rd party tool

    root@knxwork:/home/dwalter/Projects/RiakToolsCxx/build# ./src/riakcompact /var/lib/riak/leveldb/
    "/var/lib/riak/leveldb/91343852333181432387730302044767688728495783936" [directory]
    compacting...
    done
    "/var/lib/riak/leveldb/685078892498860742907977265335757665463718379520" [directory]
    compacting...
    done
