#include <set>
#include <stdio>

#include <boost/bind.hpp>

#include <bingo/base/Mutex.h>
#include <bingo/net/EventLoop.h>
#include <bingo/net/TcpServer.h>

using namespace bingo;
using namespace bingo::net;

class ChatSvr : boost::noncopyable
{
	private:
		std::set<TcpConnectionPtr> m_setTcpConnPtr;
		TcpServer m_tcpSvr;
		LengthHeaderCodec m_headCodec;

};
