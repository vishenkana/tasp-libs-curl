#include "request_impl.hpp"

#include <tasp/logging.hpp>

#include "header_impl.hpp"
#include "uri_impl.hpp"

using std::make_shared;
using std::shared_ptr;
using std::string;
using std::string_view;

namespace tasp::http
{

/*------------------------------------------------------------------------------
    RequestImpl
------------------------------------------------------------------------------*/
RequestImpl::RequestImpl(string_view host,
                         int port,
                         string_view path,
                         Request::Method method,
                         shared_ptr<CURL> curl) noexcept
: RequestImpl(method, std::move(curl))
{
    uri_ = make_shared<UriImpl>(host, port, path, curl_);
}

//------------------------------------------------------------------------------
RequestImpl::RequestImpl(string_view config,
                         string_view path,
                         Request::Method method,
                         shared_ptr<CURL> curl) noexcept
: RequestImpl(method, std::move(curl))
{
    uri_ = make_shared<UriImpl>(config, path, curl_);
}

//------------------------------------------------------------------------------
RequestImpl::RequestImpl(Request::Method method, shared_ptr<CURL> curl) noexcept
: curl_(std::move(curl))
, method_(method)
, headers_(make_shared<HeaderImpl>(curl_, Header::Type::Output))
, data_(make_shared<http::Data>())
{
    curl_easy_setopt(
        curl_.get(), CURLOPT_CUSTOMREQUEST, MethodToString(method_));

    headers_->Set("Access-Control-Allow-Origin", "*");
}

//------------------------------------------------------------------------------
RequestImpl::~RequestImpl() noexcept = default;

//------------------------------------------------------------------------------
shared_ptr<Uri> RequestImpl::Uri() const noexcept
{
    return uri_;
}

//------------------------------------------------------------------------------
shared_ptr<Header> RequestImpl::Header() const noexcept
{
    return headers_;
}

//------------------------------------------------------------------------------
Request::Method RequestImpl::GetMethod() const noexcept
{
    return method_;
}

//------------------------------------------------------------------------------
shared_ptr<http::Data> RequestImpl::Data() const noexcept
{
    return data_;
}

//------------------------------------------------------------------------------
size_t RequestImpl::ReadDataCallback(char *buffer,
                                     size_t size,
                                     size_t nitems,
                                     void *userdata) noexcept
{
    auto *client = static_cast<RequestImpl *>(userdata);
    return client->data_->Read(buffer, nitems * size);
}

}  // namespace tasp::http
