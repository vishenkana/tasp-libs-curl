#include "uri_impl.hpp"

#include <tasp/config.hpp>
#include <tasp/logging.hpp>

using std::shared_ptr;
using std::string;
using std::string_view;
using std::to_string;

namespace tasp::http
{
/*------------------------------------------------------------------------------
    UriImpl
------------------------------------------------------------------------------*/
UriImpl::UriImpl(string_view host,
                 int port,
                 string_view path,
                 shared_ptr<CURL> curl) noexcept
: curl_(std::move(curl))
, curl_url_(curl_url())
, path_(path)
{
    string uri{host};
    uri.append(":").append(to_string(port)).append(path);

    Init(uri);
}

//------------------------------------------------------------------------------
UriImpl::UriImpl(string_view config,
                 string_view path,
                 shared_ptr<CURL> curl) noexcept
: curl_(std::move(curl))
, curl_url_(curl_url())
, path_(path)
{
    string service{"services."};
    service.append(config.data()).append(".");

    const string format_pattern{"{}"};
    string uri{"{}://{}:{}"};

    auto &config_file = ConfigGlobal::Instance();

    auto replace = [&](string_view param, string_view default_value)
    {
        auto value = config_file.Get<string>(service + param.data(),
                                             default_value.data());
        auto pos = uri.find(format_pattern);
        if (pos != string::npos)
        {
            uri.replace(pos, format_pattern.length(), value);
        }
    };

    replace("schema", "http");
    replace("host", "127.0.0.1");
    replace("port", "80");

    prefix_ = config_file.Get<string>(service + "prefix", "/api/v1");

    Init(uri);
}

//------------------------------------------------------------------------------
UriImpl::~UriImpl() noexcept = default;

//------------------------------------------------------------------------------
void UriImpl::Init(string_view uri) noexcept
{
    const CURLUcode code = curl_url_set(
        curl_url_.get(), CURLUPART_URL, uri.data(), CURLU_DEFAULT_SCHEME);
    if (code != CURLUE_OK)
    {
        Logging::Error("Ошибка парсинга URI: {}", uri);
        return;
    }

    UriImpl::ChangePath(path_);
}

//------------------------------------------------------------------------------
const string &UriImpl::Url() const noexcept
{
    return url_;
}

//------------------------------------------------------------------------------
const string &UriImpl::Path() const noexcept
{
    return path_;
}

//------------------------------------------------------------------------------
void UriImpl::ChangePath(std::string_view path) noexcept
{
    path_ = prefix_ + path.data();
    curl_url_set(
        curl_url_.get(), CURLUPART_PATH, path_.c_str(), CURLU_DEFAULT_SCHEME);

    char *value{nullptr};

    const CURLUcode code =
        curl_url_get(curl_url_.get(), CURLUPART_URL, &value, 0);
    if (code == CURLUE_OK)
    {
        url_ = value;
    }
    curl_free(value);

    curl_easy_setopt(curl_.get(), CURLOPT_URL, url_.c_str());
}

//------------------------------------------------------------------------------
url::ParamValueVector UriImpl::ParamValues(
    [[maybe_unused]] string_view name) const noexcept
{
    return {};
}

//------------------------------------------------------------------------------
bool UriImpl::Match([[maybe_unused]] string_view expr) noexcept
{
    return false;
}

//------------------------------------------------------------------------------
const string &UriImpl::SubMatch([[maybe_unused]] size_t number) const noexcept
{
    static const string empty_value;
    return empty_value;
}

//------------------------------------------------------------------------------
std::string UriImpl::ToSQLCondition() const noexcept
{
    return {};
}

}  // namespace tasp::http
