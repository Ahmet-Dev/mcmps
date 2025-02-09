#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <jwt-cpp/jwt.h>
#include "virtual_ram.hpp"
#include "CodeManager.hpp"
#include "error_handler.h"

using boost::asio::ip::tcp;
namespace asio = boost::asio;

class ServerSession : public std::enable_shared_from_this<ServerSession>
{
public:
    // taskMode: "single" -> tek programın kodu, "all" -> tüm programların kod parçaları
    ServerSession(boost::asio::ssl::stream<tcp::socket> socket, const std::string &taskMode, CodeManager *codeMgr)
        : socket_(std::move(socket)), taskMode_(taskMode), codeManager_(codeMgr) {}

    void start() { doHandshake(); }

private:
    void doHandshake()
    {
        auto self(shared_from_this());
        socket_.async_handshake(boost::asio::ssl::stream_base::server,
                                [this, self](boost::system::error_code ec)
                                {
                                    if (!ec)
                                        doReadJWT();
                                    else
                                        std::cerr << "[SERVER] TLS handshake hatası: " << ec.message() << std::endl;
                                });
    }

    void doReadJWT()
    {
        auto self(shared_from_this());
        boost::asio::async_read_until(socket_, streambuf_, "\n",
                                      [this, self](boost::system::error_code ec, std::size_t)
                                      {
                                          if (!ec)
                                          {
                                              std::istream is(&streambuf_);
                                              std::string jwtToken;
                                              std::getline(is, jwtToken);
                                              std::cout << "[SERVER] Alınan JWT: " << jwtToken << std::endl;
                                              if (verifyJWT(jwtToken))
                                                  doSendTask();
                                              else
                                                  std::cerr << "[SERVER] JWT doğrulaması başarısız." << std::endl;
                                          }
                                          else
                                          {
                                              std::cerr << "[SERVER] JWT okuma hatası: " << ec.message() << std::endl;
                                          }
                                      });
    }

    bool verifyJWT(const std::string &token)
    {
        try
        {
            auto decoded = jwt::decode(token);
            auto verifier = jwt::verify()
                                .allow_algorithm(jwt::algorithm::hs256{"server_secret_key"})
                                .with_issuer("trusted_client");
            verifier.verify(decoded);
            std::cout << "[SERVER] JWT doğrulaması başarılı." << std::endl;
            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "[SERVER] JWT doğrulama hatası: " << e.what() << std::endl;
            return false;
        }
    }

    void doSendTask()
    {
        auto self(shared_from_this());
        std::string task;
        if (taskMode_ == "all")
        {
            task = codeManager_->getNextCodeSegment() + "\n" + codeManager_->getNextCodeSegment() + "\n";
        }
        else
        {
            task = codeManager_->getNextCodeSegment() + "\n";
        }
        boost::asio::async_write(socket_, boost::asio::buffer(task),
                                 [this, self](boost::system::error_code ec, std::size_t)
                                 {
                                     if (!ec)
                                         doReadResult();
                                     else
                                         std::cerr << "[SERVER] Görev gönderme hatası: " << ec.message() << std::endl;
                                 });
    }

    void doReadResult()
    {
        auto self(shared_from_this());
        boost::asio::async_read_until(socket_, streambuf_, "\n",
                                      [this, self](boost::system::error_code ec, std::size_t)
                                      {
                                          if (!ec)
                                          {
                                              std::istream is(&streambuf_);
                                              std::string result;
                                              std::getline(is, result);
                                              std::cout << "[SERVER] İstemciden alınan sonuç: " << result << std::endl;
                                          }
                                          else
                                          {
                                              std::cerr << "[SERVER] Sonuç okuma hatası: " << ec.message() << std::endl;
                                          }
                                      });
    }

    boost::asio::ssl::stream<tcp::socket> socket_;
    boost::asio::streambuf streambuf_;
    std::string taskMode_;
    CodeManager *codeManager_;
};

class Server
{
public:
    Server(boost::asio::io_context &io_context, int port, boost::asio::ssl::context &ssl_context, const std::string &taskMode, CodeManager *codeMgr)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
          ssl_context_(ssl_context), taskMode_(taskMode), codeManager_(codeMgr)
    {
        doAccept();
    }

private:
    void doAccept()
    {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket)
            {
                if (!ec)
                {
                    std::cout << "[SERVER] Yeni bağlantı kabul edildi." << std::endl;
                    std::make_shared<ServerSession>(
                        boost::asio::ssl::stream<tcp::socket>(std::move(socket), ssl_context_), taskMode_, codeManager_)
                        ->start();
                }
                else
                {
                    std::cerr << "[SERVER] Accept hatası: " << ec.message() << std::endl;
                }
                doAccept();
            });
    }

    tcp::acceptor acceptor_;
    boost::asio::ssl::context &ssl_context_;
    std::string taskMode_;
    CodeManager *codeManager_;
};

int main()
{
    try
    {
        boost::asio::io_context io_context;
        boost::asio::ssl::context ssl_context(boost::asio::ssl::context::sslv23);
        ssl_context.use_certificate_chain_file("server.crt");
        ssl_context.use_private_key_file("server.key", boost::asio::ssl::context::pem);

        core::VirtualRAM virtualRam(8);
        CodeManager codeManager(&virtualRam);

        std::string taskMode = "single"; // "single" veya "all"
        Server server(io_context, 1881, ssl_context, taskMode, &codeManager);
        io_context.run();
    }
    catch (std::exception &e)
    {
        std::cerr << "[SERVER] Exception: " << e.what() << std::endl;
    }
    return 0;
}
