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

    for (auto& iter : db)
    {
        iter = int8_t(myrand::rand8());
    }

}
