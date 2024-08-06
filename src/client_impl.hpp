/**
 * @file
 * @brief Реализация интерфейсов для работы с HTTP-запросами.
 */
#ifndef TASP_CLIENT_IMPL_HPP_
#define TASP_CLIENT_IMPL_HPP_

#include <curl/curl.h>

#include <memory>
#include <sstream>

#include "http/request_impl.hpp"
#include "http/response_impl.hpp"

namespace tasp::http
{
/**
 * @brief Реализация интерфейса для работы с HTTP-запросами.
 */
class ClientImpl final
{
public:
    /**
     * @brief Конструктор.
     *
     * @param host Хост для отправки запроса
     * @param port Порт для отправки запроса
     * @param path Путь запроса
     * @param method Метод запроса
     */
    explicit ClientImpl(std::string_view host = {"127.0.0.1"},
                        int port = 80,
                        std::string_view path = {"/"},
                        Request::Method method = Request::Method::Get) noexcept;

    /**
     * @brief Конструктор с загрузкой данных из конфигурационного файла.
     *
     * @param config Путь в глобальном конфигурационном файле
     * @param path Путь запроса
     * @param method Метод запроса
     */
    explicit ClientImpl(std::string_view config,
                        std::string_view path = {"/"},
                        Request::Method method = Request::Method::Get) noexcept;

    /**
     * @brief Деструктор.
     */
    ~ClientImpl() noexcept;

    /**
     * @brief Получения данных запроса.
     *
     * @return Данные запроса
     */
    [[nodiscard]] std::shared_ptr<http::Request> Request() const noexcept;

    /**
     * @brief Выполнение запроса.
     *
     * @return Результат выполнения запроса
     */
    [[nodiscard]] std::shared_ptr<http::Response> Send() const noexcept;

    ClientImpl(const ClientImpl &) = delete;
    ClientImpl(ClientImpl &&) = delete;
    ClientImpl &operator=(const ClientImpl &) = delete;
    ClientImpl &operator=(ClientImpl &&) = delete;

private:
    /**
     * @brief Инициализация объекта.
     */
    void Init() noexcept;

    /**
     * @brief Указатель на главную структуру библиотеки CURL.
     */
    std::shared_ptr<CURL> curl_;

    /**
     * @brief Параметры запроса.
     */
    std::shared_ptr<http::Request> request_;
};

}  // namespace tasp::http

#endif  // TASP_CLIENT_IMPL_HPP_
