#include "Levels.hpp"

Levels::Levels(RobotContainer& container)
    : _container(container),
      _maze(),
      _motion(container),
      _sensors(container),
      _s1(_maze, _sensors, _motion),
      _s2(container),
      _s3(container),
      _state(levels::LevelsState::WAITING),
      _phase(levels::LevelsPhase::SECTION1),
      _startColor(ColorDetected::NONE),
      _runAll(false),
      _go(false) {}

void Levels::arm(levels::LevelsPhase first, ColorDetected startColor, bool runAll) {
    _phase = first;
    _startColor = startColor;
    _runAll = runAll;
    _go = false;
    _state = levels::LevelsState::WAITING;
}

void Levels::go() { _go = true; }

void Levels::abort() {
    _s1.abort();
    _s2.abort();
    _s3.abort();
    _container.getDrive().stop();
    _state = levels::LevelsState::FAULT;
}

void Levels::startPhase(levels::LevelsPhase p) {
    _phase = p;
    switch (p) {
        case levels::LevelsPhase::SECTION1: _s1.begin(); _state = levels::LevelsState::SECTION1; break;
        case levels::LevelsPhase::SECTION2: _s2.begin(); _state = levels::LevelsState::SECTION2; break;
        case levels::LevelsPhase::SECTION3: _s3.begin(); _state = levels::LevelsState::SECTION3; break;
    }
}

void Levels::nextPhase() {
    if (!_runAll) {
        _state = levels::LevelsState::DONE;
        return;
    }
    switch (_phase) {
        case levels::LevelsPhase::SECTION1: startPhase(levels::LevelsPhase::SECTION2); break;
        case levels::LevelsPhase::SECTION2: startPhase(levels::LevelsPhase::SECTION3); break;
        case levels::LevelsPhase::SECTION3: _state = levels::LevelsState::DONE; break;
    }
}

void Levels::update() {
    const ColorDetected color = _container.getColorDetected();

    switch (_state) {
        case levels::LevelsState::WAITING:
            _container.getDrive().stop();
            if (_go || (_startColor != ColorDetected::NONE && color == _startColor)) startPhase(_phase);
            break;

        case levels::LevelsState::SECTION1:
            _s1.update();
            if (_s1.failed()) { _container.getDrive().stop(); _state = levels::LevelsState::FAULT; }
            else if (_s1.done()) nextPhase();
            break;

        case levels::LevelsState::SECTION2:
            _s2.update();
            if (_s2.failed()) _state = levels::LevelsState::FAULT;
            else if (_s2.done()) nextPhase();
            break;

        case levels::LevelsState::SECTION3:
            _s3.update();
            if (_s3.failed()) _state = levels::LevelsState::FAULT;
            else if (_s3.done()) nextPhase();
            break;

        case levels::LevelsState::DONE:
        case levels::LevelsState::FAULT:
            _container.getDrive().stop();
            break;
    }
}
