#include "../../modules/config/config.h"
#include "../../modules/ldr/ldr.h"

namespace emojis {
    void changeEmoji(float soundLevel) {
        if (soundLevel < config::db_normal)
        {
            ldr::showImage("good.jpg");
        }
        else if (soundLevel > config::db_normal && soundLevel < config::db_angry)
        {
            ldr::showImage("normal.jpg");
        }
        else if (soundLevel > config::db_angry)
        {
            ldr::showImage("angry.jpg");
        }
    }
}