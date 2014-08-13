#ifndef BINGO_EXAMPLES_ASIO_CHAT_CODEC_H
#define BINGO_EXAMPLES_ASIO_CHAT_CODEC_H

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <bingo/base/Logging.h>
#include <bingo/net/Buffer.h>
#include <bingo/net/Endian.h>
#include <bingo/net/TcpConnection.h>

class LengthHeaderCodec : boost::noncopyable
{
	public:
		typedef boost::function<void (const bingo::net::TcpConnectionPtr&
						, const bingo::string &message
						, bingo::Timestamp)> StringMessageCallback;
	private:
		StringMessageCallback messageCallback_;
		const static size_t kHeaderLen = sizeof(int32_t);

	public:
		explicit LengthHeaderCodec(const StringMessageCallback &cb):messageCallback_(cb){}

		void onMessage(const bingo::net::TcpConnectionPtr &conn
						, bingo::net::Buffer *buf
						, bingo::Timestamp receiveTime){
			int32_t iLen = 0;
			while(buf->readableBytes() >= kHeaderLen){
				iLen = buf->peekInt32();

				if(65535 < iLen || 0 > iLen){
					LOG_ERROR << "Invalid length " << len;	

					conn->shutdown();
					break;
				}else if(buf->readableBytes() >= iLen + kHeaderLen){
					buf->retrieve(kHeaderLen);	
					bingo::string message(buf->peek(), iLen);
					messageCallback_(conn, message, receiveTime);
					buf->retrieve(iLen);
				}
			}	
		}

		void send(bingo::net::TcpConnection *conn
						, const bingo::StringPiece &message){
			bingo::net::Buffer buf;
			buf.append(message.data(), message.size());
			int32_t iLen = bingo::net::sockets::hostToNetwork32(static_cast<int32_t>(message.size()));
			buf.prepend(&iLen, sizeof(iLen));
			conn->send(buf);
		}
#endif
