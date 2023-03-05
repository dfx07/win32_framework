#include "whandle.h"

____USE_NAMESPACE____


using namespace fox;



void Create(Window* win)
{


}

void MouseButton(Window* win)
{
    if (win->GetMouseButtonStatus(GLMouse::LeftButton))
    {

    }
}

void Keyboard(Window* win)
{
    if (win->GetKeyboardStatus(GLKeyboard::Escapex))
    {
        win->ExitFullScreen();
    }

    else if (win->GetKeyboardStatus(GLKeyboard::KeyA))
    {

    }
}


void Draw(Window* win)
{

}

void Resize(Window* win)
{

}

void Process(Window* pWin)
{
}

int main()
{
    Window *window = new Window(L"OpenGL", 100 ,100);
    window->SetOnDrawfunc(Draw);
    window->SetOnCreatedfunc(Create);
    window->SetProcessfunc(Process);
    window->SetOnKeyboardfunc(Keyboard);
    window->SetOnResizefunc(Resize);

    window = create_window(window);

    while (window->closed())
    {
        window->process();
        window->draw();
        window->poll_event();
    }

    destroy_window(window);
}
