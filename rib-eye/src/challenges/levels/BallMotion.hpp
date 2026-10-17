#pragma once
#include "../maze/n/MotionController.hpp"

namespace levels {

    // Movimientos extra que necesita la pista Levels ademas de los del maze (reversa para
    // tomar la pelota con el intake de ATRAS y avance corto para salir con ella)
    class BallMotion : public maze::MotionController {
        public:
            virtual void startReverse(maze::Direction heading, float meters) = 0;
            virtual void startAdvance(maze::Direction heading, float meters) = 0;
    };

}
