#include "client_impl.hpp"

#include <tasp/logging.hpp>

using std::make_shared;
using std::shared_ptr;
using std::string;
using std::string_view;

namespace tasp::http
{

/*------------------------------------------------------------------------------
    ClientImpl
------------------------------------------------------------------------------*/
ClientImpl::ClientImpl(string_view host,
                       int port,

                       string_view path,
                       Request::Method method) noexcept
: curl_(curl_easy_init(), curl_easy_cleanup)
, request_(make_shared<RequestImpl>(host, port, path, method, curl_))
{
    Init();
}

//------------------------------------------------------------------------------
ClientImpl::ClientImpl(string_view config,

                       string_view path,
                       Request::Method method) noexcept
: curl_(curl_easy_init(), curl_easy_cleanup)
, request_(make_shared<RequestImpl>(config, path, method, curl_))
{
    Init();
}

//------------------------------------------------------------------------------
void ClientImpl::Init() noexcept
{
    curl_easy_setopt(curl_.get(), CURLOPT_HEADERFUNCTION, HeaderImpl::Callback);
    curl_easy_setopt(
        curl_.get(), CURLOPT_WRITEFUNCTION, ResponseImpl::WriteDataCallback);

    curl_easy_setopt(
        curl_.get(), CURLOPT_READFUNCTION, RequestImpl::ReadDataCallback);
    curl_easy_setopt(curl_.get(), CURLOPT_READDATA, request_.get());
}

//------------------------------------------------------------------------------
ClientImpl::~ClientImpl() noexcept = default;

//------------------------------------------------------------------------------
shared_ptr<http::Request> ClientImpl::Request() const noexcept
{
    return request_;
}

//------------------------------------------------------------------------------
shared_ptr<Response> ClientImpl::Send() const noexcept
{
    request_->Header()->Set("Content-Type",
                            request_->Data()->GetType() + "; charset=UTF-8");

    const string method = Request::MethodToString(request_->GetMethod());
    const string &url = request_->Uri()->Url();

    auto response = make_shared<ResponseImpl>(curl_);

    curl_easy_setopt(curl_.get(), CURLOPT_UPLOAD, request_->Data()->Length());

    curl_easy_setopt(curl_.get(), CURLOPT_HEADERDATA, response->Header().get());
    curl_easy_setopt(curl_.get(), CURLOPT_WRITEDATA, response.get());

    const CURLcode result = curl_easy_perform(curl_.get());

    int64_t code{404};
    if (result == CURLcode::CURLE_OK)
    {
        curl_easy_getinfo(curl_.get(), CURLINFO_RESPONSE_CODE, &code);
    }
    else
    {
        Logging::Error("Ошибка выполнения HTTP-запроса: {}",
                       curl_easy_strerror(result));
    }

    Logging::Info("HTTP-запрос {} {} {}", method, url, code);

    response->SetCode(static_cast<Response::Code>(code));

    return response;
}

}  // namespace tasp::http
