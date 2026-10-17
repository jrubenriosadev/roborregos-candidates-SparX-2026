#ifndef DEF_SECTION2
#define DEF_SECTION2

#include "LevelsStates.hpp"
#include "../../container/RobotContainer.hpp"

namespace levels {

    // Seccion 2: campo de 2x4 casillas con lineas blancas (2 TCRT5000, piso verde).
    // Tres carriles (izquierda / centro / derecha, a 1/3 del ancho c/u). Va primero por el centro; si ve linea
    // retrocede un poco y prueba la derecha; si tambien hay linea, la izquierda. Al final se re-centra para
    // quedar dentro del checkpoint. No usa el TCS.
    class Section2 {
        public:
            explicit Section2(RobotContainer& container);

            void begin();
            void update();
            void abort();

            bool done() const { return _state == Section2State::DONE; }
            bool failed() const { return _state == Section2State::FAULT; }
            Section2State state() const { return _state; }
            Section2Error error() const { return _error; }
            float progress() const { return _progress; }
            int lane() const { return _lane; }

        private:
            static constexpr int kNoLane = 99;

            RobotContainer& _container;
            Section2State _state;
            Section2Error _error;

            int _lane;              // carril actual: -1 izquierda, 0 centro, +1 derecha (visto desde el frente de la pista)
            int _target;            // carril al que vamos
            uint8_t _tried;         // mascara de carriles bloqueados en esta posicion (bit = carril + 1)
            float _progress;        // avance (m) a lo largo de la pista
            float _segBase;         // avance al iniciar el tramo actual (el encoder se reinicia en cada tramo)
            float _blockAt;         // avance donde se vio la ultima linea
            float _sideYaw;
            bool _recentering;
            bool _armed;            // el TCRT solo se vigila despues de haber visto el piso verde (sobre el checkpoint rojo puede leer "blanco")
            unsigned long _t0;

            void fail(Section2Error e);
            bool lineBlocked() const;
            int nextLane() const;
            void startDrive();
            void startSideTurn();
    };

}

#endif
