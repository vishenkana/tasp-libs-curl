/**
 * @file
 * @brief Реализация интерфейсов работы с ответом HTTP.
 */
#ifndef TASP_HTTP_RESPONSE_IMPL_HPP_
#define TASP_HTTP_RESPONSE_IMPL_HPP_

#include <tasp/http/response.hpp>

#include "header_impl.hpp"

namespace tasp::http
{

/**
 * @brief Реализация интерфейса для работы с ответом HTTP.
 */
class ResponseImpl : public Response
{
public:
    /**
     * @brief Конструктор.
     *
     * @param curl Указатель на главную структуру библиотеки CURL
     */
    explicit ResponseImpl(const std::shared_ptr<CURL> &curl) noexcept;

    /**
     * @brief Деструктор.
     */
    ~ResponseImpl() noexcept override;

    /**
     * @brief Установка кода выполнения запроса.
     *
     * @param code Код результат выполнения запроса
     */
    void SetCode(Response::Code code) noexcept override;

    /**
     * @brief Запрос кода ответа.
     *
     * @return Код
     */
    [[nodiscard]] Response::Code GetCode() const noexcept override;

    /**
     * @brief Запрос заголовков ответа.
     *
     * @return Заголовок
     */
    [[nodiscard]] std::shared_ptr<http::Header> Header()
        const noexcept override;

    /**
     * @brief Запрос данных запроса в текстовом представлении.
     *
     * @return Данные
     */
    [[nodiscard]] std::shared_ptr<http::Data> Data() const noexcept override;

    /**
     * @brief Добавление в ответ сообщения об ошибке.
     *
     * @param code Код ответа
     * @param message Сообщение
     */
    void SetError(Code code, std::string_view message) noexcept override;

    /**
     * @brief Функция для чтения данных ответа, для передачи в библиотеку CURL.
     *
     * @param buffer Указатель на строку с данными
     * @param size Размер одного символа
     * @param nitems Количество символов в строке
     * @param userdata Указатель на объект с информаций ответа
     *
     * @return Количество прочитанных символов
     */
    static size_t WriteDataCallback(char *buffer,
                                    size_t size,
                                    size_t nitems,
                                    void *userdata) noexcept;

    ResponseImpl(const ResponseImpl &) = delete;
    ResponseImpl(ResponseImpl &&) = delete;
    ResponseImpl &operator=(const ResponseImpl &) = delete;
    ResponseImpl &operator=(ResponseImpl &&) = delete;

private:
    /**
     * @brief Код результат выполнения запроса.
     */
    Response::Code code_{Response::Code::NotFound};

    /**
     * @brief Заголовок ответа.
     */
    std::shared_ptr<http::Header> headers_;

    /**
     * @brief Данные ответа.
     */
    std::shared_ptr<http::Data> data_;
};

}  // namespace tasp::http

#endif  // TASP_HTTP_RESPONSE_IMPL_HPP_
