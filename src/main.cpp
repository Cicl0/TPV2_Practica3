// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>

#include "game/Game.h"

int main(int, char**) {

    try {
        Game game;

        game.initGame();
        game.start();
    }
    catch (const std::string& e) {
        std::cerr << e << std::endl;
    }
    catch (const char* e) {
        std::cerr << e << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << e.what();
    }
    catch (...) {
        std::cerr << "Caught an exception of unknown type ...";
    }

    return 0;
}

