#include"WordData.h"

WordInfo g_words[] = {
    // fruits
    {"apple", "fruit"}, {"banana", "fruit"}, {"orange", "fruit"},
    {"grape", "fruit"}, {"melon", "fruit"}, {"peach", "fruit"},
    {"cherry", "fruit"}, {"lemon", "fruit"}, {"mango", "fruit"},
    {"berry", "fruit"},

    // computer
    {"computer", "computer"}, {"keyboard", "computer"}, {"monitor", "computer"},
    {"mouse", "computer"}, {"printer", "computer"}, {"program", "computer"},
    {"language", "computer"}, {"function", "computer"}, {"variable", "computer"},
    {"pointer", "computer"},

    // school
    {"school", "school"}, {"teacher", "school"}, {"student", "school"},
    {"library", "school"}, {"science", "school"}, {"history", "school"},
    {"math", "school"}, {"english", "school"}, {"subject", "school"},
    {"lesson", "school"},

    // game
    {"game", "game"}, {"player", "game"}, {"score", "game"},
    {"level", "game"}, {"stage", "game"}, {"enemy", "game"},
    {"attack", "game"}, {"defense", "game"}, {"weapon", "game"},
    {"skill", "game"}
};

int g_nowWordCount = sizeof(g_words) / sizeof(WordInfo);
const int WORD_TOTAL_COUNT = sizeof(g_words) / sizeof(WordInfo);