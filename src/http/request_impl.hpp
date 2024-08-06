/**
 * @file
 * @brief Реализация интерфейсов работы с запросом HTTP.
 */
#ifndef TASP_HTTP_REQUEST_IMPL_HPP_
#define TASP_HTTP_REQUEST_IMPL_HPP_

#include <curl/curl.h>

#include <tasp/http/data.hpp>
#include <tasp/http/header.hpp>
#include <tasp/http/request.hpp>
#include <tasp/http/uri.hpp>

namespace tasp::http
{

/**
 * @brief Реализация интерфейса для работы с запросом HTTP.
 */
class RequestImpl : public Request
{
public:
    /**
     * @brief Конструктор.
     *
     * @param host Хост для отправки запроса
     * @param port Порт для отправки запроса
     * @param path Путь запроса
     * @param method Метод запроса
     * @param curl Указатель на главную структуру библиотеки CURL
     */
    explicit RequestImpl(std::string_view host,
                         int port,
                         std::string_view path,
                         Request::Method method,
                         std::shared_ptr<CURL> curl) noexcept;

    /**
     * @brief Конструктор с загрузкой данных из конфигурационного файла.
     *
     * @param config Путь в глобальном конфигурационном файле
     * @param path Путь запроса
     * @param method Метод запроса
     * @param curl Указатель на главную структуру библиотеки CURL
     */
    explicit RequestImpl(std::string_view config,
                         std::string_view path,
                         Request::Method method,
                         std::shared_ptr<CURL> curl) noexcept;

    /**
     * @brief Деструктор.
     */
    ~RequestImpl() noexcept override;

    /**
     * @brief Запрос указателя на ресурс.
     *
     * @return Указатель на ресурс
     */
    [[nodiscard]] std::shared_ptr<http::Uri> Uri() const noexcept override;

    /**
     * @brief Запрос заголовков запроса.
     *
     * @return Заголовок
     */
    [[nodiscard]] std::shared_ptr<http::Header> Header()
        const noexcept override;

    /**
     * @brief Запрос метода запроса.
     *
     * @return Метод
     */
    [[nodiscard]] Request::Method GetMethod() const noexcept override;

    /**
     * @brief Запрос данных запроса в текстовом представлении.
     *
     * @return Данные
     */
    [[nodiscard]] std::shared_ptr<http::Data> Data() const noexcept override;

    /**
     * @brief Функция для записи данных запроса, для передачи в библиотеку CURL.
     *
     * @param buffer Указатель на строку с данными
     * @param size Размер одного символа
     * @param nitems Количество символов в строке
     * @param userdata Указатель на объект клиента
     *
     * @return Количество записанных символов
     */
    static size_t ReadDataCallback(char *buffer,
                                   size_t size,
                                   size_t nitems,
                                   void *userdata) noexcept;

    RequestImpl(const RequestImpl &) = delete;
    RequestImpl(RequestImpl &&) = delete;
    RequestImpl &operator=(const RequestImpl &) = delete;
    RequestImpl &operator=(RequestImpl &&) = delete;

private:
    /**
     * @brief Базовой конструктор.
     *
     * @param method Метод запроса
     * @param curl Указатель на главную структуру библиотеки CURL
     */
    explicit RequestImpl(Request::Method method,
                         std::shared_ptr<CURL> curl) noexcept;

    /**
     * @brief Указатель на главную структуру библиотеки CURL.
     */
    std::shared_ptr<CURL> curl_;

    /**
     * @brief Указатель на ресурс.
     */
    std::shared_ptr<http::Uri> uri_;

    /**
     * @brief Метод запроса.
     */
    http::Request::Method method_;

    /**
     * @brief Заголовок запроса.
     */
    std::shared_ptr<http::Header> headers_;

    /**
     * @brief Данные запроса.
     */
    std::shared_ptr<http::Data> data_;
};

}  // namespace tasp::http

#endif  // TASP_HTTP_REQUEST_IMPL_HPP_
