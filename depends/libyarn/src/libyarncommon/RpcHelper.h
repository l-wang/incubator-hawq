/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef _HDFS_LIBHDFS3_SERVER_RPCHELPER_H_
#define _HDFS_LIBHDFS3_SERVER_RPCHELPER_H_

#include "Exception.h"
#include "ExceptionInternal.h"

#include "StackPrinter.h"

#include "YARNSecurity.pb.h"

#include <algorithm>
#include <cassert>

using namespace google::protobuf;

namespace Yarn {
namespace Internal {

class Nothing {
};

template < typename T1 = Nothing, typename T2 = Nothing, typename T3 = Nothing,
         typename T4 = Nothing, typename T5 = Nothing, typename T6 = Nothing,
         typename T7 = Nothing, typename T8 = Nothing, typename T9 = Nothing,
         typename T10 = Nothing, typename T11 = Nothing  >
class UnWrapper: public UnWrapper<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, Nothing> {
private:
    typedef UnWrapper<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, Nothing> BaseType;

public:
    UnWrapper(const YarnRpcServerException & e) :
        BaseType(e), e(e) {
    }

    void ATTRIBUTE_NORETURN ATTRIBUTE_NOINLINE unwrap(const char * file,
            int line) {
        if (e.getErrClass() == T1::ReflexName) {
#ifdef NEED_BOOST
            boost::throw_exception(T1(e.getErrMsg(), SkipPathPrefix(file), line, PrintStack(1, STACK_DEPTH).c_str()));
#else
            throw T1(e.getErrMsg(), SkipPathPrefix(file), line, PrintStack(1, STACK_DEPTH).c_str());
#endif
        } else {
            BaseType::unwrap(file, line);
        }
    }
private:
    const YarnRpcServerException & e;
};

template<>
class UnWrapper < Nothing, Nothing, Nothing, Nothing, Nothing, Nothing, Nothing,
        Nothing, Nothing, Nothing, Nothing > {
public:
    UnWrapper(const YarnRpcServerException & e) :
        e(e) {
    }
    void ATTRIBUTE_NORETURN ATTRIBUTE_NOINLINE unwrap(const char * file,
            int line) {
        THROW(YarnIOException,
              "Unexpected exception: when unwrap the rpc remote exception \"%s\", %s in %s: %d",
              e.getErrClass().c_str(), e.getErrMsg().c_str(), file, line);
    }
private:
    const YarnRpcServerException & e;
};



static inline void Convert(Token & token,
                           const hadoop::common::TokenProto & proto) {
    token.setIdentifier(proto.identifier());
    token.setKind(proto.kind());
    token.setPassword(proto.password());
    token.setService(proto.service());
}






static inline Token Convert(const hadoop::common::TokenProto & proto) {
    Token retval;
    retval.setIdentifier(proto.identifier());
    retval.setKind(proto.kind());
    retval.setPassword(proto.password());
    return retval;
}

static inline void Build(const Token & token,
                         hadoop::common::TokenProto * proto) {
    proto->set_identifier(token.getIdentifier());
    proto->set_kind(token.getKind());
    proto->set_password(token.getPassword());
    proto->set_service(token.getService());
}


static inline void Build(const std::vector<std::string> & srcs,
                         RepeatedPtrField<std::string> * proto) {
    for (size_t i = 0; i < srcs.size(); ++i) {
        proto->Add()->assign(srcs[i]);
    }
}


}
}

#endif /* _HDFS_LIBHDFS3_SERVER_RPCHELPER_H_ */
