#pragma once
#include <algorithm>
#include <string>
#include <sstream>
#include <string_view>

struct Uri {
    std::string_view Protocol, Host, Port, Path, QueryString;

    static Uri Parse(const std::string_view& uri) {
        Uri result;

        if (uri.empty()) return result;

        auto uriEnd = uri.end();

        auto queryStart = std::find(uri.begin(), uriEnd, '?');

        auto protocolStart = uri.begin();
        auto protocolEnd = std::find(protocolStart, uriEnd, ':');

        if (protocolEnd != uriEnd) {
            auto prot = std::string_view(&*(protocolEnd));
            if ((prot.length() > 3) && (prot.substr(0, 3) == "://")) {
                result.Protocol = std::string_view(uri.data(), std::distance(uri.begin(), protocolEnd));
                protocolEnd += 3;
            } else {
                protocolEnd = uri.begin();
            }
        } else {
            protocolEnd = uri.begin();
        }

        auto hostStart = protocolEnd;
        auto pathStart = std::find(hostStart, uriEnd, '/');

        auto hostEnd = std::find(protocolEnd, (pathStart != uriEnd) ? pathStart : queryStart, ':');

        result.Host = std::string_view(uri.data(), std::distance(hostStart, hostEnd));

        if ((hostEnd != uriEnd) && ((*hostEnd) == ':')) {
            ++hostEnd;
            auto portEnd = (pathStart != uriEnd) ? pathStart : queryStart;
            result.Port = std::string_view(uri.data(), std::distance(hostEnd, portEnd));
        }

        if (pathStart != uriEnd) result.Path = std::string_view(uri.data(), std::distance(pathStart, queryStart));

        if (queryStart != uriEnd) result.QueryString = std::string_view(uri.data(), std::distance(queryStart, uri.end()));

        return result;
    }

    static std::string CreateUri(std::string_view Protocol, std::string_view Host, std::string_view Port, std::string_view Path, std::string_view QueryString) {
        std::ostringstream str;
        if (!Protocol.empty()) {
            str << Protocol << "://";
        }
        str << Host;
        if (!Port.empty()) {
            str << ":" << Port;
        }
        if (!Path.empty()) {
            str << Path;
        }
        if (!QueryString.empty()) {
            str << QueryString;
        }
        return str.str();
    }
};
