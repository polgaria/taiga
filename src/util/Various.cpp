#include <cpr/cpr.h>
#include <date/date.h>

#include <aisaka/util/String.hpp>
#include <nlohmann/json.hpp>
#include <taiga/util/Math.hpp>
#include <taiga/util/String.hpp>
#include <taiga/util/Various.hpp>

std::string Taiga::Util::Various::format_bytes(uint64_t size) {
	if ((size > 1024ull) &&
		(size < 1024ull * 1024))  // over 1KB and up to 1MB show KB
	{
		return fmt::format("{:.3f}KB", static_cast<double>(size) / 1024);
	}
	if ((size > 1024ull * 1024) &&
		(size < 1024ull * 1024 * 1024))	 // over 1MB and up to 1GB show MB
	{
		return fmt::format("{:.3f}MB",
						   (static_cast<double>(size) / 1024) / 1024);
	}
	if (size > 1024ull * 1024 * 1024)  // over 1GB show GB
	{
		return fmt::format("{:.3f}GB",
						   ((static_cast<double>(size) / 1024) / 1024) / 1024);
	}
	return fmt::format("{}B", size);
}

float Taiga::Util::Various::conversion_rate(const std::string_view from,
											const std::string_view to,
											const std::string_view api_key,
											aegis::rest::rest_controller& rc) {
	if (api_key.empty()) {
		throw std::invalid_argument("Currency API key not set.");
	}
	auto currency_to_currency = fmt::format("{}_{}", from, to);
	Taiga::Util::String::to_upper(currency_to_currency);

	aegis::rest::request_params rp;
	rp.host = "free.currconv.com";
	rp.path = fmt::format("/api/v7/convert?q={}&compact=ultra&apiKey={}",
						  currency_to_currency, api_key);
	rp.method = aegis::rest::Get;
	rp.headers = {"User-Agent: taiga"};

	const auto& request =
		rc.execute2(std::forward<aegis::rest::request_params>(rp));

	switch (request.reply_code) {
		case aegis::rest::http_code::ok: {
			break;
		}
		case aegis::rest::http_code::too_many_requests: {
			throw std::runtime_error("Ratelimited.");
		}
		case aegis::rest::http_code::bad_request: {
			throw std::invalid_argument("Invalid API key.");
		}
		case aegis::rest::http_code::service_unavailable:
		case aegis::rest::http_code::internal_server_error:
		case aegis::rest::http_code::server_down: {
			throw std::runtime_error(
				"Currency Conversion API is down. Please try again later.");
		}
		default: {
			throw std::runtime_error("Unknown error.");
		}
	}

	auto json = nlohmann::json::parse(std::move(request.content));

	if (json.find(currency_to_currency) == json.end()) {
		throw std::invalid_argument("Invalid parameters.");
	}
	return json[currency_to_currency].get<float>();
}
