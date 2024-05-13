#include "State.h"

class Menu_state : public State
{
public:
    void Init() override;
    void Clean() override;

    void HandleEvent(SDL_Event& event) override;
    void Update() override;
    void Render() override;

private:
    bool WithinBox(int x, int y, int box_x, int box_y, int box_w, int box_h);
};
