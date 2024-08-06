#include "response_impl.hpp"

#include <tasp/logging.hpp>

using std::make_shared;
using std::shared_ptr;
using std::string;
using std::string_view;

namespace tasp::http
{
/*------------------------------------------------------------------------------
    ResponseImpl
------------------------------------------------------------------------------*/
ResponseImpl::ResponseImpl(const shared_ptr<CURL> &curl) noexcept
: headers_(make_shared<HeaderImpl>(curl))
, data_(make_shared<http::Data>())
{
}

//------------------------------------------------------------------------------
ResponseImpl::~ResponseImpl() noexcept = default;

//------------------------------------------------------------------------------
void ResponseImpl::SetCode(Response::Code code) noexcept
{
    code_ = code;
}

//------------------------------------------------------------------------------
Response::Code ResponseImpl::GetCode() const noexcept
{
    return code_;
}

//------------------------------------------------------------------------------
shared_ptr<Header> ResponseImpl::Header() const noexcept
{
    return headers_;
}

//------------------------------------------------------------------------------
shared_ptr<http::Data> ResponseImpl::Data() const noexcept
{
    return data_;
}

//------------------------------------------------------------------------------
void ResponseImpl::SetError(Code code, string_view message) noexcept
{
    SetCode(code);
    Data()->Set(string(message.data(), message.size()));
}

//------------------------------------------------------------------------------
size_t ResponseImpl::WriteDataCallback(char *buffer,
                                       size_t size,
                                       size_t nitems,
                                       void *userdata) noexcept
{
    auto *response = static_cast<ResponseImpl *>(userdata);
    response->data_->Append(buffer, nitems * size);

    return nitems * size;
}

}  // namespace tasp::http
