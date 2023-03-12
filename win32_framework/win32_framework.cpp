#include "wapi.h"

____USE_NAMESPACE____


using namespace fox;

Button* btn;

void Create(WindowBase* win)
{
    btn = new Button();
    btn->SetLabel(L"Ngô Văn Thường");
    btn->SetSize(200, 24);
    btn->SetPosition(100, 100);
    btn->SetBorderRadius(2);
    btn->SetBorderWidth(1);
    btn->UseEffect(true);
    btn->Visible(true);

    win->AddControl(btn);

    btn = new Button();
    btn->SetLabel(L"Ngô Văn Thường");
    btn->SetSize(200, 24);
    btn->SetPosition(100, 124);
    btn->SetBorderRadius(2);
    btn->SetBorderWidth(1);
    btn->UseEffect(true);
    btn->Visible(true);

    win->AddControl(btn);

    btn = new Button();
    btn->SetLabel(L"Ngô Văn Thường");
    btn->SetSize(200, 24);
    btn->SetPosition(100, 150);
    btn->SetBorderRadius(2);
    btn->SetBorderWidth(1);
    btn->UseEffect(true);
    btn->Visible(true);

    win->AddControl(btn);

    Checkbox* chk = new Checkbox();

    chk->SetText(L"Check box 1");
    chk->SetPosition(100, 200);
    chk->SetSize(200, 24);
    chk->SetBackgroundColor(Color4(59, 91, 179));

    SubWindow* sub = new SubWindow();
    sub->SetTitle(L"sub window");
    sub->SetPosition(20, 20);
    sub->SetSize(20, 20);
    sub->Visible(true);

    win->AddSubWindow(sub);
    win->AddControl(chk);

}

void MouseButton(WindowBase* win)
{
    if (win->GetMouseButtonStatus(GLMouse::LeftButton))
    {

    }
}

void Keyboard(WindowBase* win)
{
    if (win->GetKeyboardStatus(GLKeyboard::Escapex))
    {
        //win->ExitFullScreen();
    }

    else if (win->GetKeyboardStatus(GLKeyboard::KeyA))
    {

    }
}


void Draw(WindowBase* win)
{
    int c = 10;
}

void Resize(WindowBase* win)
{

}

void Process(WindowBase* pWin)
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

    while (!window->closed())
    {
        window->process();
        window->draw();
        window->wait_event();
    }


    destroy_window(window);
}
