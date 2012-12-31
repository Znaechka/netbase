﻿#include "stdafx.h"
#include "core/smart_socket.h"
#include "core/logger.h"

using namespace core;
using namespace std::chrono;


int main(int argc, char **argv)
{
	std::locale::global(std::locale("rus"));
    LogService::ScopedGuard logGuard(&std::cout);

	try
	{
		auto io_service = std::make_shared<boost::asio::io_service>();

        SmartSocket socket(io_service, 0);
        socket.addObserver(std::make_shared<SocketStateLogger>());
        
        udp::resolver resolver(*io_service);
		udp::resolver::query serverQuery(udp::v4(), "localhost", "13999");
        udp::endpoint server = *resolver.resolve(serverQuery);

        auto conn = socket.getOrCreateConnection(server);

        std::thread io_thread([=]{ io_service->run(); });

        size_t maxTicks = argc > 1 ? atoi(argv[1]) : 10;
        for (size_t tick = 0; tick < maxTicks; ++tick)
        {
            if (conn->isDead())
                break;

            auto packet = std::make_shared<Packet>(1);
            conn->asyncSend(packet);

            cDebug << "tick";
            std::this_thread::sleep_for(milliseconds(50));
        }

        io_service->stop();
        io_thread.join();
	}
	catch (const std::exception& e)
	{
		cError << e.what();
	}

	return 0;
}

