#pragma once

class AbstractProgramData {
    public:
        virtual bool Init() = 0;
        virtual void Update() = 0;
};