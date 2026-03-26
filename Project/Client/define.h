#pragma once

#define SHOW_EDITOR_DETAIL(Title) \
    static bool bShow##Title = true;              \
    if (DrawTitleUI(#Title))                      \
        bShow##Title = !bShow##Title;             \
    if (!bShow##Title)                            \
        return;                                   \