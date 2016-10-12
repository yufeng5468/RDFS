#ifndef RDFS_ZKCLIENTCOMMON_H
#define RDFS_ZKCLIENTCOMMON_H

#include <zkwrapper.h>

#include <boost/shared_ptr.hpp>

namespace zkclient {

    class ZkClientCommon {
    public:
        ZkClientCommon();

        void init();

    private:
        std::shared_ptr <ZKWrapper> zk;
	//ZKWrapper zk;
    };
}

#endif //RDFS_ZKCLIENTCOMMON_H
