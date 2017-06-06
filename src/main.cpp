#include "application.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace std;
application app("Hello pbf!", 800, 600);

int main()
{

    if (!app.init()) {
        return -1;
    }
    app.event_loop();
    app.close();

    return 0;
}
