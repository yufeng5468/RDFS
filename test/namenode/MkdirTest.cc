// Copyright 2017 Rice University, COMP 413 2017

#include <limits>
#include "NameNodeTest.h"
#include <thread>


TEST_F(NamenodeTest, mkdirDepth1) {
    std::string src = "/testing/test_mkdir";
    hadoop::hdfs::MkdirsRequestProto mkdir_req;
    hadoop::hdfs::MkdirsResponseProto mkdir_resp;
    mkdir_req.set_createparent(true);
    mkdir_req.set_src(src);
    ASSERT_EQ(client->mkdir(mkdir_req, mkdir_resp),
              zkclient::ZkNnClient::MkdirResponse::Ok);
    ASSERT_TRUE(mkdir_resp.result());
    ASSERT_TRUE(client->file_exists(src));
    LOG(DEBUG) << "Finished all asserts";
}

TEST_F(NamenodeTest, mkdirDepth100) {
    std::string src;
    for (int i = 0; i < 100; ++i) {
        src.append("/testing/test_mkdir");
    }
    hadoop::hdfs::MkdirsRequestProto mkdir_req;
    hadoop::hdfs::MkdirsResponseProto mkdir_resp;
    mkdir_req.set_createparent(true);
    mkdir_req.set_src(src);
    ASSERT_EQ(client->mkdir(mkdir_req, mkdir_resp),
              zkclient::ZkNnClient::MkdirResponse::Ok);
    ASSERT_TRUE(mkdir_resp.result());
    ASSERT_TRUE(client->file_exists(src));
}

TEST_F(NamenodeTest, mkdirExistentDirectory) {
    std::string src = "/testing/test_mkdir";
    hadoop::hdfs::MkdirsRequestProto mkdir_req;
    hadoop::hdfs::MkdirsResponseProto mkdir_resp;
    mkdir_req.set_createparent(true);
    mkdir_req.set_src(src);
    ASSERT_EQ(client->mkdir(mkdir_req, mkdir_resp),
              zkclient::ZkNnClient::MkdirResponse::Ok);
    ASSERT_TRUE(mkdir_resp.result());
    ASSERT_TRUE(client->file_exists(src));

    // Now create again.
    // TODO(Yufeng): should really introduce a new response enum
    ASSERT_EQ(client->mkdir(mkdir_req, mkdir_resp),
              zkclient::ZkNnClient::MkdirResponse::Ok);
    ASSERT_TRUE(mkdir_resp.result());
}

TEST_F(NamenodeTest, mkdirExistentFile) {
    // Create a file.
    std::string src = "/testing/test_mkdir_file";
    hadoop::hdfs::CreateRequestProto create_req = getCreateRequestProto(src);
    hadoop::hdfs::CreateResponseProto create_resp;
    ASSERT_EQ(client->create_file(create_req, create_resp),
              zkclient::ZkNnClient::CreateResponse::Ok);

    hadoop::hdfs::HdfsFileStatusProto file_status = create_resp.fs();
    ASSERT_EQ(file_status.filetype(),
              hadoop::hdfs::HdfsFileStatusProto::IS_FILE);
    ASSERT_TRUE(client->file_exists(src));

    // Now create a directory with the same name.
    src.insert(0, "/");
    hadoop::hdfs::MkdirsRequestProto mkdir_req;
    hadoop::hdfs::MkdirsResponseProto mkdir_resp;
    mkdir_req.set_createparent(true);
    mkdir_req.set_src(src);
    ASSERT_EQ(client->mkdir(mkdir_req, mkdir_resp),
              zkclient::ZkNnClient::MkdirResponse::FailedZnodeCreation);
    ASSERT_FALSE(mkdir_resp.result());
}

TEST_F(NamenodeTest, mkdirProfHelper) {
    std::string root_src = "/testing";
    std::vector<int> depths = {1};
    std::vector<int> num_iters = {500};

    std::string curr_src;
    for (auto d : depths) {
        for (auto i : num_iters) {
            for (int j = 0; j < i; j++) {
                curr_src = root_src;
                for (int k = 0; k < d; k++) {
                    curr_src += "/test_mkdir_depth" + std::to_string(d) +
                                "_num" + std::to_string(i) + "_iter" + std::to_string(j);
                }
            }
        }
    }
    std::cerr << "Pollling for " << curr_src << "\n";
    while (!client->file_exists(curr_src)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now());
    auto epoch = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    std::cerr << "Polling ended at " << epoch.count() << "\n";
}

TEST_F(NamenodeTest, mkdirPerformance) {
//  el::Loggers::setVerboseLevel(9);

  std::string root_src = "/testing";
  std::vector<int> warmup_depths = {128};
  std::vector<int> warmup_iters = {5};
  std::vector<int> depths = {1};
  std::vector<int> num_iters = {500};

  // Warmup
  for (auto d : warmup_depths) {
    for (auto i : warmup_iters) {
      for (int j = 0; j < i; j++) {
        std::string curr_src = root_src;
        for (int k = 0; k < d; k++) {
          curr_src += "/warmup_mkdir_depth" + std::to_string(d) +
                      "_num" + std::to_string(i) + "_iter" + std::to_string(j);
        }
        hadoop::hdfs::MkdirsRequestProto mkdir_req;
        hadoop::hdfs::MkdirsResponseProto mkdir_resp;
        mkdir_req.set_createparent(true);
        mkdir_req.set_src(curr_src);
        ASSERT_EQ(client->mkdir(mkdir_req, mkdir_resp),
                  zkclient::ZkNnClient::MkdirResponse::Ok);
        ASSERT_TRUE(mkdir_resp.result());
      }
    }
  }

  std::cerr << "Warmup done\n";

  // Benchmark
  for (auto d : depths) {
    for (auto i : num_iters) {
      for (int j = 0; j < i; j++) {
        std::string curr_src = root_src;
        for (int k = 0; k < d; k++) {
          curr_src += "/test_mkdir_depth" + std::to_string(d) +
              "_num" + std::to_string(i) + "_iter" + std::to_string(j);
        }
//        TIMED_SCOPE_IF(mkdirPerformanceTimer,
//                       "mkdir_depth" + std::to_string(d), VLOG_IS_ON(9));
        hadoop::hdfs::MkdirsRequestProto mkdir_req;
        hadoop::hdfs::MkdirsResponseProto mkdir_resp;
        mkdir_req.set_createparent(true);
        mkdir_req.set_src(curr_src);
        ASSERT_EQ(client->mkdir(mkdir_req, mkdir_resp),
                  zkclient::ZkNnClient::MkdirResponse::Ok);
        ASSERT_TRUE(mkdir_resp.result());
      }
    }
  }
  auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now());
  auto epoch = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
  std::cerr << "Master finished at " << epoch.count() << "\n";

//  std::cerr << "Round 2\n";
//
//  for (auto d : depths) {
//    for (auto i : num_iters) {
//      for (int j = 0; j < i; j++) {
//        std::string curr_src = root_src;
//        for (int k = 0; k < d; k++) {
//          curr_src += "/test_mkdir_r2_depth" + std::to_string(d) +
//                      "_num" + std::to_string(i) + "_iter" + std::to_string(j);
//        }
//        hadoop::hdfs::MkdirsRequestProto mkdir_req;
//        hadoop::hdfs::MkdirsResponseProto mkdir_resp;
//        mkdir_req.set_createparent(true);
//        mkdir_req.set_src(curr_src);
//        ASSERT_EQ(client->mkdir(mkdir_req, mkdir_resp),
//                  zkclient::ZkNnClient::MkdirResponse::Ok);
//        ASSERT_TRUE(mkdir_resp.result());
//      }
//    }
//  }
//
//  std::cerr << "Round 3\n";
//
//  for (auto d : depths) {
//    for (auto i : num_iters) {
//      for (int j = 0; j < i; j++) {
//        std::string curr_src = root_src;
//        for (int k = 0; k < d; k++) {
//          curr_src += "/test_mkdir_r3_depth" + std::to_string(d) +
//                      "_num" + std::to_string(i) + "_iter" + std::to_string(j);
//        }
//        hadoop::hdfs::MkdirsRequestProto mkdir_req;
//        hadoop::hdfs::MkdirsResponseProto mkdir_resp;
//        mkdir_req.set_createparent(true);
//        mkdir_req.set_src(curr_src);
//        ASSERT_EQ(client->mkdir(mkdir_req, mkdir_resp),
//                  zkclient::ZkNnClient::MkdirResponse::Ok);
//        ASSERT_TRUE(mkdir_resp.result());
//      }
//    }
//  }
//
//  std::cerr << "Round 4\n";
//
//  for (auto d : depths) {
//    for (auto i : num_iters) {
//      for (int j = 0; j < i; j++) {
//        std::string curr_src = root_src;
//        for (int k = 0; k < d; k++) {
//          curr_src += "/test_mkdir_r4_depth" + std::to_string(d) +
//                      "_num" + std::to_string(i) + "_iter" + std::to_string(j);
//        }
//        hadoop::hdfs::MkdirsRequestProto mkdir_req;
//        hadoop::hdfs::MkdirsResponseProto mkdir_resp;
//        mkdir_req.set_createparent(true);
//        mkdir_req.set_src(curr_src);
//        ASSERT_EQ(client->mkdir(mkdir_req, mkdir_resp),
//                  zkclient::ZkNnClient::MkdirResponse::Ok);
//        ASSERT_TRUE(mkdir_resp.result());
//      }
//    }
//  }
//
//  std::cerr << "Round 5\n";
//
//  for (auto d : depths) {
//    for (auto i : num_iters) {
//      for (int j = 0; j < i; j++) {
//        std::string curr_src = root_src;
//        for (int k = 0; k < d; k++) {
//          curr_src += "/test_mkdir_r5_depth" + std::to_string(d) +
//                      "_num" + std::to_string(i) + "_iter" + std::to_string(j);
//        }
//        hadoop::hdfs::MkdirsRequestProto mkdir_req;
//        hadoop::hdfs::MkdirsResponseProto mkdir_resp;
//        mkdir_req.set_createparent(true);
//        mkdir_req.set_src(curr_src);
//        ASSERT_EQ(client->mkdir(mkdir_req, mkdir_resp),
//                  zkclient::ZkNnClient::MkdirResponse::Ok);
//        ASSERT_TRUE(mkdir_resp.result());
//      }
//    }
//  }
}
