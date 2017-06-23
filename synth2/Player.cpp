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

    // Trigger bassdrum
    if ((pos & 0x7FF) == 0) {
        bd.trigger();
    }
    
    // Trigger hihat
    if ((pos & 0xFF) == 0) {
        uint16_t vol = (((pos>>8)&0x7) == 4) ? 0x40 : 0x20;
        hh.trigger(vol, 0x80);
    }
	
    for (auto& iter : db) iter = 0;
    bd.render(db);
    hh.render(db);

    pos++;
}
