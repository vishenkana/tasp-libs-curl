/**
 * @file
 * @brief Интерфейсы для работы с HTTP-запросами.
 */
#ifndef TASP_HTTP_CLIENT_HPP_
#define TASP_HTTP_CLIENT_HPP_

#include <memory>
#include <string_view>

#include <tasp/http/request.hpp>
#include <tasp/http/response.hpp>

namespace tasp::http
{

class ClientImpl;

/**
 * @brief Интерфейс для работы с HTTP-запросами.
 *
 * Класс скрывает от пользователя реализацию с помощью идиомы PIMPL
 * (Pointer to Implementation – указатель на реализацию).
 */
class [[gnu::visibility("default")]] Client final
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
    explicit Client(std::string_view host = {"127.0.0.1"},
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
    explicit Client(std::string_view config,
                    std::string_view path = {"/"},
                    Request::Method method = Request::Method::Get) noexcept;

    /**
     * @brief Деструктор.
     */
    ~Client() noexcept;

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
    [[nodiscard]] std::shared_ptr<Response> Send() const noexcept;

    Client(const Client &) = delete;
    Client(Client &&) = delete;
    Client &operator=(const Client &) = delete;
    Client &operator=(Client &&) = delete;

private:
    /**
     * @brief Указатель на реализацию.
     */
    std::unique_ptr<ClientImpl> impl_;
};

}  // namespace tasp::http

#endif  // TASP_HTTP_CLIENT_HPP_
