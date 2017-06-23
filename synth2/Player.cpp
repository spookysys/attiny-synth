#include "Player.hpp"
#include <cstdlib>
#include "myrand.hpp"
using namespace myrand;

Player::Player()
{
}

void Player::render(Buffer &db)
{
    myrand::rand32();


    if ((pos & 0xFF) == 0) {
        uint16_t vol = ((pos&0x7FF) == 0) ? 0x3FFF : 0x1FFF;
        hh.trigger(vol, 0x80);
    }
	
    for (auto& iter : db) iter = 0;
    hh.render(db);

    pos++;
}
