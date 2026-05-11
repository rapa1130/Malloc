#pragma once

struct WordInfo {
    const char* word;
    const char* category;
};

extern WordInfo g_words[];
extern int g_nowWordCount;
extern const int WORD_TOTAL_COUNT;