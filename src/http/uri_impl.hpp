/**
 * @file
 * @brief Реализация интерфейсов работы с указателями на ресурс (URI).
 */
#ifndef TASP_HTTP_URI_IMPL_HPP_
#define TASP_HTTP_URI_IMPL_HPP_

#include <curl/curl.h>

#include <memory>
#include <unordered_map>

#include <tasp/http/uri.hpp>

namespace tasp::http
{

/**
 * @brief Деструктор для структуры URL библиотеки CURL.
 */
struct CurlURLDeleter
{
    /**
     * @brief Оператор удаления указателя.
     *
     * @param ptr Указатель на структуру URL библиотеки CURL.
     */
    void operator()(CURLU *ptr) const noexcept
    {
        curl_url_cleanup(ptr);
    }
};

/**
 * @brief Умный указатель с деструктором с URL библиотеки CURL.
 */
using CurlURL = std::unique_ptr<CURLU, CurlURLDeleter>;

/**
 * @brief Реализация интерфейса для работы с указателями на ресурс (URI).
 */
class UriImpl : public Uri
{
public:
    /**
     * @brief Конструктор.
     *
     * @param host Хост для отправки запроса
     * @param port Порт для отправки запроса
     * @param path Путь запроса
     * @param curl Указатель на главную структуру библиотеки CURL
     */
    explicit UriImpl(std::string_view host,
                     int port,
                     std::string_view path,
                     std::shared_ptr<CURL> curl) noexcept;

    /**
     * @brief Конструктор с загрузкой данных из конфигурационного файла.
     *
     * @param config Путь в глобальном конфигурационном файле
     * @param path Путь запроса
     * @param curl Указатель на главную структуру библиотеки CURL
     */
    explicit UriImpl(std::string_view config,
                     std::string_view path,
                     std::shared_ptr<CURL> curl) noexcept;

    /**
     * @brief Деструктор.
     */
    ~UriImpl() noexcept override;

    /**
     * @brief Запрос полного идентификатора ресурса.
     *
     * @return Идентификатор
     */
    [[nodiscard]] const std::string &Url() const noexcept override;

    /**
     * @brief Запрос URL-пути.
     *
     * @return Путь
     */
    [[nodiscard]] const std::string &Path() const noexcept override;

    /**
     * @brief Смена пути запроса.
     *
     * @param path Путь запроса
     */
    void ChangePath(std::string_view path) noexcept override;

    /**
     * @brief Запрос значений параметра.
     *
     * @param name Название параметра
     *
     * @return Массив значений, переданных в URL
     */
    [[nodiscard]] url::ParamValueVector ParamValues(
        std::string_view name) const noexcept override;

    /**
     * @brief Проверка соответствия URL-пути регулярному выражению и
     * формирование подгрупп пути.
     *
     * @param expr Регулярное выражение
     *
     * @return Результат сравнения
     */
    [[nodiscard]] bool Match(std::string_view expr) noexcept override;

    /**
     * @brief Запрос значения подгруппы пути. Если подгруппа отсутствует,
     * возвращается пустое значение.
     *
     * @param number Номер подгруппы
     *
     * @return Значение подгруппы
     */
    [[nodiscard]] const std::string &SubMatch(
        size_t number) const noexcept override;

    /**
     * @brief Преобразование параметров запроса в SQL-условие (для конкатенации
     * с 'WHERE').
     *
     * @return SQL-условие.
     */
    [[nodiscard]] std::string ToSQLCondition() const noexcept override;

    UriImpl(const UriImpl &) = delete;
    UriImpl(UriImpl &&) = delete;
    UriImpl &operator=(const UriImpl &) = delete;
    UriImpl &operator=(UriImpl &&) = delete;

private:
    /**
     * @brief Инициализация URI.
     *
     * @param uri Полный uri запроса
     */
    void Init(std::string_view uri) noexcept;

    /**
     * @brief Указатель на главную структуру библиотеки CURL.
     */
    std::shared_ptr<CURL> curl_;

    /**
     * @brief Указатель на структуру URL библиотеки CURL.
     */
    CurlURL curl_url_;

    /**
     * @brief Полный идентификатор ресурса.
     */
    std::string url_;

    /**
     * @brief Параметры запроса.
     */
    std::unordered_multimap<std::string, url::ParamValueVector> query_params_;

    /**
     * @brief Префикс пути запроса.
     */
    std::string prefix_;

    /**
     * @brief URL-путь запроса.
     */
    std::string path_{"/"};
};

}  // namespace tasp::http

#endif  // TASP_HTTP_URI_IMPL_HPP_
