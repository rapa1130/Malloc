#include "Hangman_ParkYiJun_ClassChange.h"
#include <iostream>
#include <random>

const char* RandomWord() { // 영단어 리스트 
    static const char* wordList[] = {
            "algorithm", "binary", "compiler", "debugger", "engine", "framework", "graphics", "hardware", "interface", "json",
            "kernel", "latency", "memory", "network", "object", "pipeline", "queue", "runtime", "shader", "texture",
            "unity", "vector", "window", "xbox", "yield", "zbuffer", "abstract", "boolean", "class", "dynamic",
            "entity", "function", "generic", "handle", "instance", "keyboard", "library", "module", "node", "overflow",
            "pointer", "query", "recursive", "stack", "template", "update", "virtual", "wizard", "yield", "zip",
            "warrior", "master", "dragon", "legend", "spirit", "sword", "blade", "temple", "palace", "shadow",
            "forest", "mountain", "clash", "honor", "loyalty", "justice", "power", "secret", "hidden", "flying",
            "tiger", "phoenix", "meditation", "energy", "nature", "battle", "empire", "kingdom", "ancient", "hero",
            "villain", "assassin", "guard", "monk", "sect", "manual", "technique", "style", "striking", "defense",
            "evade", "counter", "fist", "spear", "bow", "arrow", "armor", "helmet", "scroll", "journey",
            "adventure", "bridge", "camera", "desert", "emerald", "forest", "guitar", "harvest", "island", "jungle",
            "knight", "lantern", "market", "nebula", "ocean", "planet", "quartz", "river", "safari", "target",
            "universe", "volcano", "weather", "xylophone", "yacht", "zebra", "airport", "balloon", "castle", "diamond",
            "elephant", "fountain", "garden", "highway", "iceberg", "jacket", "kettle", "laptop", "mirror", "notebook",
            "orange", "pencil", "rocket", "silver", "ticket", "umbrella", "valley", "wallet", "yellow", "zodiac"
        };
    static std::random_device rd;
    static std::mt19937 gen(rd()); // 랜덤 숫자 계산하기
    int wordCount = sizeof(wordList) / sizeof(wordList[0]);
    std::uniform_int_distribution<int> dis(0, wordCount - 1); // 단어의 갯수만큼에서 랜덤을 돌리기
    return wordList[dis(gen)]; 
}

int main()
{
    srand(static_cast<unsigned int>(time(NULL)));  // 랜덤 숫자를 위한 준비
    const char* Word = RandomWord(); // 랜덤 단어 꺼내기

    HangmanGame Game(Word); // 행맨 게임 클래스 
    Game.Play(); // 게임 시작

    return 0;
}
