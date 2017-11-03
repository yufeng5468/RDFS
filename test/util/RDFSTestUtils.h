// Copyright 2017 Rice University, COMP 413 2017

#ifndef TEST_UTIL_RDFSTESTUTILS_H_
#define TEST_UTIL_RDFSTESTUTILS_H_

#include <string>

namespace RDFSTestUtils {

  static void initializeDatanodes(
      int startId,
      int numDatanodes,
      std::string serverName,
      int32_t xferPort,
      int32_t ipcPort) {
    int i = startId;
    for (; i < startId + numDatanodes; i++) {
      system(("truncate tfs" + std::to_string(i) + " -s 1000000000").c_str());
      std::string dnCliArgs = "-x " +
          std::to_string(xferPort + i) + " -p " + std::to_string(ipcPort + i)
          + " -b tfs" + std::to_string(i) + " &";
      std::string cmdLine =
          "bash -c \"exec -a " + serverName + std::to_string(i) +
              " /home/vagrant/rdfs/build/rice-datanode/datanode " +
              dnCliArgs + "\" & ";
      system(cmdLine.c_str());
      sleep(3);
    }
  }

  static hadoop::hdfs::CreateRequestProto getCreateRequestProto(
      const std::string &path
  ) {
    hadoop::hdfs::CreateRequestProto create_req;
    create_req.set_src(path);
    create_req.set_clientname("asdf");
    create_req.set_createparent(false);
    create_req.set_blocksize(1);
    create_req.set_replication(1);
    create_req.set_createflag(0);
    return create_req;
  }
}  // namespace RDFSTestUtils

#endif  // TEST_UTIL_RDFSTESTUTILS_H_
