#include "header_impl.hpp"

#include <string>

#include <tasp/logging.hpp>

using std::shared_ptr;
using std::string;
using std::string_view;

namespace tasp::http
{

/*------------------------------------------------------------------------------
    HeaderImpl
------------------------------------------------------------------------------*/
HeaderImpl::HeaderImpl(shared_ptr<CURL> curl, Header::Type type) noexcept
: curl_(std::move(curl))
, type_(type)
{
}

//------------------------------------------------------------------------------
HeaderImpl::~HeaderImpl() noexcept = default;

//------------------------------------------------------------------------------
const string &HeaderImpl::Get(string_view name) const noexcept
{
    auto param{headers_.find(name.data())};
    if (param != headers_.end())
    {
        return param->second;
    }

    static const string empty_value;
    return empty_value;
}

//------------------------------------------------------------------------------
void HeaderImpl::Set(string_view name, string_view value) noexcept
{
    headers_.insert_or_assign(name.data(), value.data());

    if (type_ == Header::Type::Output)
    {
        curl_headers_.reset(nullptr);

        for (auto &&header : headers_)
        {
            string field{header.first};
            field.append(": ").append(header.second);
            curl_headers_.reset(
                curl_slist_append(curl_headers_.release(), field.c_str()));
        }

        curl_easy_setopt(curl_.get(), CURLOPT_HTTPHEADER, curl_headers_.get());
    }
}

//------------------------------------------------------------------------------
void HeaderImpl::Set(string_view header) noexcept
{
    const auto pos = header.find_first_of(':');
    if (pos == string_view::npos)
    {
        return;
    }

    const auto value_start{pos + 2};
    auto value_end = header.find_last_not_of('\n');
    if (value_end == string_view::npos)
    {
        value_end = header.length();
    }

    const auto value = header.substr(value_start, value_end - value_start);

    Set(header.substr(0, pos), value);
}

//------------------------------------------------------------------------------
size_t HeaderImpl::Callback(char *buffer,
                            size_t size,
                            size_t nitems,
                            void *userdata) noexcept
{
    auto *header = static_cast<HeaderImpl *>(userdata);
    header->Set(string_view(buffer, nitems * size));

    return nitems * size;
}

}  // namespace tasp::http
