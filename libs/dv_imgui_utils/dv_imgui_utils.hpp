#pragma once

#include <dviglo/std_utils/string.hpp>


namespace dviglo
{

f32 calc_button_width(const StrUtf8& label);

// Возвращает true при нажатии кнопки "Открыть"
bool open_file_dialog(bool& visible, StrUtf8& path);

// Возвращает true при нажатии кнопки "Сохранить"
bool save_file_dialog(bool& visible, StrUtf8& path);

} // namespace dviglo
