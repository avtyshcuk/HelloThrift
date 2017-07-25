#include <iostream>
#include <boost/make_shared.hpp>

#include <thrift/server/TSimpleServer.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "gen-cpp/Message.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::server;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

class HelloMsg : public MessageIf
{
    public:
    virtual void hello() {cout << "hello" << endl;}
};

class HelloMsgCloneFactory : virtual public MessageIfFactory
{
public:
    virtual MessageIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) 
    {
        return new HelloMsg;
    }
   
    virtual void releaseHandler(MessageIf* handler)
    {
        delete handler;
    }
};

int main()
{
    TSimpleServer server(
        boost::make_shared<MessageProcessorFactory>(boost::make_shared<HelloMsgCloneFactory>()),
        boost::make_shared<TServerSocket>(9090),
        boost::make_shared<TBufferedTransportFactory>(),
        boost::make_shared<TBinaryProtocolFactory>()
    );

    cout << "Start server..." << endl;
    server.serve();
    cout << "Done" << endl;
}
