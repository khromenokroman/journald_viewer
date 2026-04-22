#pragma once
#include <string_view>

/**
 * @brief Форматирует и печатает строки journalctl в человекочитаемом виде.
 *
 * Класс читает JSON-строки из стандартного ввода, извлекает поля записи журнала
 * и выводит их с цветовым оформлением.
 */
class JournalDViewer {
public:
    /**
     * @brief Запускает обработку входного потока journalctl.
     *
     * Конструктор начинает чтение строк из stdin и сразу выводит
     * отформатированный результат в stdout/stderr.
     */
    JournalDViewer();
    ~JournalDViewer() = default;

    JournalDViewer(JournalDViewer const &) = delete;
    JournalDViewer(JournalDViewer &&) = delete;
    JournalDViewer &operator=(JournalDViewer const &) = delete;
    JournalDViewer &operator=(JournalDViewer &&) = delete;


private:
    /**
    * @brief Возвращает ANSI-цвет для указанного приоритета syslog.
    *
    * @param p Приоритет сообщения журнала.
    * @return ANSI escape-последовательность цвета.
    */
    std::string_view color_for_priority(int p);
    /**
     * @brief Преобразует timestamp из journalctl в строку с датой и временем.
     *
     * Ожидается значение в микросекундах с начала эпохи Unix.
     *
     * @param value Строковое представление timestamp.
     * @return Отформатированная дата и время в виде `YYYY-MM-DD HH:MM:SS.ffffff`.
     * @throws std::runtime_error Если значение пустое или не может быть обработано.
     */
    std::string format_realtime_timestamp(std::string_view value);
};
