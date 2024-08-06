#include "tasp/http/client.hpp"

#include "client_impl.hpp"

using std::make_unique;
using std::shared_ptr;
using std::string;
using std::string_view;

namespace tasp::http
{

/*------------------------------------------------------------------------------
    Client
------------------------------------------------------------------------------*/
Client::Client(string_view host,
               int port,
               string_view path,
               Request::Method method) noexcept
: impl_(make_unique<ClientImpl>(host, port, path, method))
{
}

//------------------------------------------------------------------------------
Client::Client(std::string_view config,
               std::string_view path,
               Request::Method method) noexcept
: impl_(make_unique<ClientImpl>(config, path, method))
{
}

//------------------------------------------------------------------------------
Client::~Client() noexcept = default;

//------------------------------------------------------------------------------
shared_ptr<http::Request> Client::Request() const noexcept
{
    return impl_->Request();
}

//------------------------------------------------------------------------------
shared_ptr<Response> Client::Send() const noexcept
{
    return impl_->Send();
}

}  // namespace tasp::http
