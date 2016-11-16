//
// Created by Prudhvi Boyapalli on 10/17/16.
//

#include "zk_queue.h"
#include <gtest/gtest.h>

#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

namespace {

	class ZKQueueTest : public ::testing::Test {
	protected:
		virtual void SetUp() {

			int error_code;
			// Code here will be called immediately after the constructor (right
			// before each test).

			zk = std::make_shared<ZKWrapper>("localhost:2181", error_code, "/testing");
			assert(error_code == 0); // Z_OK

			zk->create("/test_queue", ZKWrapper::EMPTY_VECTOR, error_code);
			assert(error_code == 0); // Z_OK
		}

		virtual void TearDown() {
			int error_code;
			zk->recursive_delete("/test_queue", error_code);
			assert(error_code == 0); // Z_OK
		}

		// Objects declared here can be used by all tests in the test case for Foo.
		std::shared_ptr <ZKWrapper> zk;
	};



	TEST_F(ZKQueueTest, testPushPeekPop) {
		int error_code;

		ASSERT_TRUE(push(zk, "/test_queue", ZKWrapper::EMPTY_VECTOR, error_code));
		std::string peeked_path;
		ASSERT_TRUE(peek(zk, "/test_queue", peeked_path, error_code));
		ASSERT_EQ("q-item-0000000000", peeked_path);

		auto popped_data = std::vector<std::uint8_t>();
		ASSERT_TRUE(pop(zk, "/test_queue", popped_data, error_code));
		ASSERT_TRUE(peek(zk, "/test_queue", peeked_path, error_code));
		ASSERT_EQ("/test_queue", peeked_path); // Since q is empty, the peeked path should be the same

		// TODO: test more rigorously, esp. ensure that pushed data is
		// correctly returned on pop
	}
}

int main(int argc, char **argv) {
	system("sudo ~/zookeeper/bin/zkServer.sh start");

	::testing::InitGoogleTest(&argc, argv);
	int res = RUN_ALL_TESTS();

	system("sudo ~/zookeeper/bin/zkCli.sh rmr /queue_test");
	system("sudo ~/zookeeper/bin/zkServer.sh stop");

	return res;
}