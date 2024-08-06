/**
 * @file
 * @brief Реализация интерфейсов работы с заголовком HTTP.
 */
#ifndef TASP_HTTP_HEADER_IMPL_HPP_
#define TASP_HTTP_HEADER_IMPL_HPP_

#include <curl/curl.h>

#include <map>
#include <memory>

#include <tasp/http/header.hpp>

namespace tasp::http
{

/**
 * @brief Умный указатель с деструктором с заголовками запроса curl.
 */
using CurlSList = std::unique_ptr<curl_slist, decltype(&curl_slist_free_all)>;

/**
 * @brief Реализация интерфейса для работы с заголовком HTTP-запроса.
 */
class HeaderImpl : public Header
{
public:
    /**
     * @brief Конструктор.
     *
     * @param curl Указатель на главную структуру библиотеки CURL
     * @param type Тип заголовка
     */
    explicit HeaderImpl(std::shared_ptr<CURL> curl,
                        Header::Type type = Header::Type::Input) noexcept;

    /**
     * @brief Деструктор.
     */
    ~HeaderImpl() noexcept override;

    /**
     * @brief Запрос параметра заголовка.
     *
     * @param name Название параметра
     *
     * @return Значение
     */
    [[nodiscard]] const std::string &Get(
        std::string_view name) const noexcept override;

    /**
     * @brief Установка нового значения параметра заголовка.
     *
     * @param name Название параметра
     * @param value Новое значение
     */
    void Set(std::string_view name, std::string_view value) noexcept override;

    /**
     * @brief Установка нового параметра заголовка объединенного в одну строку,
     * с разделением символом :.
     *
     * @param header Параметр заголовка
     */
    void Set(std::string_view header) noexcept;

    /**
     * @brief Функция для установки значений заголовка ответа, для передачи в
     * библиотеку CURL.
     *
     * @param buffer Указатель на строку заголовка
     * @param size Размер одного символа
     * @param nitems Количество символов в строке
     * @param userdata Указатель на объект с информаций о заголовках для
     * внесения данных
     *
     * @return Количество прочитанных символов
     */
    static size_t Callback(char *buffer,
                           size_t size,
                           size_t nitems,
                           void *userdata) noexcept;

    HeaderImpl(const HeaderImpl &) = delete;
    HeaderImpl(HeaderImpl &&) = delete;
    HeaderImpl &operator=(const HeaderImpl &) = delete;
    HeaderImpl &operator=(HeaderImpl &&) = delete;

private:
    /**
     * @brief Указатель на главную структуру библиотеки CURL.
     */
    std::shared_ptr<CURL> curl_;

    /**
     * @brief Тип заголовка.
     */
    Header::Type type_;

    /**
     * @brief Заголовок в формате библиотеки CURL.
     */
    CurlSList curl_headers_{nullptr, curl_slist_free_all};

    /**
     * @brief Значения заголовка.
     */
    std::map<std::string, std::string> headers_;
};

}  // namespace tasp::http

#endif  // TASP_HTTP_HEADER_IMPL_HPP_
