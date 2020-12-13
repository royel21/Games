#ifndef _LOGGER_H
#define _LOGGER_H

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Plutus
{

	class Logger
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

} // namespace Plutus

#define LOG_CORE_T(...) ::Plutus::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_CORE_E(...) ::Plutus::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CORE_I(...) ::Plutus::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_W(...) ::Plutus::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_F(...) ::Plutus::Logger::GetCoreLogger()->fatal(__VA_ARGS__)

#define LOG_T(...) ::Plutus::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_E(...) ::Plutus::Logger::GetClientLogger()->error(__VA_ARGS__)
#define LOG_I(...) ::Plutus::Logger::GetClientLogger()->info(__VA_ARGS__)
#define LOG_W(...) ::Plutus::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_F(...) ::Plutus::Logger::GetClientLogger()->fatal(__VA_ARGS__)

#endif